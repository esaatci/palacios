/* 
 * V3 Control utility
 * (c) Jack lange, 2010
 */


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <sys/ioctl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <string.h>
#include <dirent.h> 

#include "v3_ctrl.h"

#define SYS_PATH "/sys/devices/system/memory/"

#define BUF_SIZE 128




static int offline_memory(unsigned long long mem_size_bytes,
			  unsigned long long mem_min_start,
			  int limit32, 
			  unsigned long long *num_bytes, 
			  unsigned long long *base_addr);

int main(int argc, char * argv[]) {
    unsigned long long mem_size_bytes = 0;
    unsigned long long mem_min_start = 0;
    int v3_fd = -1;
    int request = 0;
    int limit32 = 0;
    int node = -1;
    int c;
    unsigned long long num_bytes, base_addr;
    struct v3_mem_region mem;

    if (argc<2 || argc>5) {
	printf("usage: v3_mem [-r] [-l] [-n k] <memory size (MB)> [min_start (MB)]\n\n"
	       "Allocate memory for use by Palacios.\n\n"
	       "With    -r    this requests in-kernel allocation.\n"
	       "Without -r    this attempts to offline memory via hot\n"
	       "              remove.\n"
	       "With    -l    the request or offlining is limited to first 4 GB\n"
	       "Without -l    the request or offlining has no limits\n\n"
	       "With    -n k  the request is for numa node k\n"
	       "Without -n k  the request can be on any numa node\n\n"
	       "For offlining, min_start is the minimum allowable starting address.\n"
               "This is zero by default\n\n");  
        return -1;
    }

    while ((c=getopt(argc,argv,"rln:"))!=-1) {
	switch (c) {
	    case 'r':
		request=1;
		break;
	    case 'l':
		limit32=1;
		break;
	    case 'n':
		node = atoi(optarg);
		break;
	    case '?':
		if (optopt=='n') { 
		    printf("-n requires the numa node...\n");
		} else {
		    printf("Unknown option %c\n",optopt);
		}
		break;
	    default:
		printf("Unknown option %c\n",optopt);
		break;
	}
    }
	

    mem_size_bytes = atoll(argv[optind]) * (1024 * 1024);

    if ((optind+1) < argc) { 
        mem_min_start = atoll(argv[optind+1]) * (1024 * 1024);
    }

    v3_fd = open(v3_dev, O_RDONLY);
    
    if (v3_fd == -1) {
	printf("Error opening V3Vee control device\n");
	return -1;
    }

    if (!request) { 
	printf("Trying to offline memory (size=%llu, min_start=%llu, limit32=%d)\n",mem_size_bytes,mem_min_start,limit32);
	if (offline_memory(mem_size_bytes,mem_min_start,limit32, &num_bytes, &base_addr)) { 
	    printf("Could not offline memory\n");
	    return -1;
	}
	mem.type=PREALLOCATED;
	mem.node=0;
	mem.base_addr=base_addr;
	mem.num_pages=num_bytes/4096;
    } else {
	printf("Generating memory allocation request (size=%llu, limit32=%d)\n", mem_size_bytes, limit32);
	mem.type = limit32 ? REQUESTED32 : REQUESTED;
	mem.node = node;
	num_bytes = mem_size_bytes;
	base_addr = 0;
    }
    
    if (ioctl(v3_fd, V3_ADD_MEMORY, &mem)<0) { 
	printf("Request rejected by Palacios\n");
	close(v3_fd);
	return -1;
    } else {
	printf("Request accepted by Palacios\n");
	close(v3_fd); 	
	return 0;
    }
} 


static int dir_filter(const struct dirent * dir) {
    if (strncmp("memory", dir->d_name, 6) == 0) {
	return 1;
    }

    return 0;
}


static int dir_cmp(const struct dirent **dir1, const struct dirent ** dir2) {
    int num1 = atoi((*dir1)->d_name + 6);
    int num2 = atoi((*dir2)->d_name + 6);

    return num1 - num2;
}


static int offline_memory(unsigned long long mem_size_bytes,
			  unsigned long long mem_min_start,
			  int limit32, 
			  unsigned long long *num_bytes, 
			  unsigned long long *base_addr)
{

    unsigned int block_size_bytes = 0;
    int bitmap_entries = 0;
    unsigned char * bitmap = NULL;
    int num_blocks = 0;    
    int reg_start = 0;
    int mem_ready = 0;
    

    printf("Trying to find %dMB (%d bytes) of memory above %llu with limit32=%d\n", mem_size_bytes/(1024*1024), mem_size_bytes, mem_min_start, limit32);
	
    /* Figure out the block size */
    {
	int tmp_fd = 0;
	char tmp_buf[BUF_SIZE];
	
	tmp_fd = open(SYS_PATH "block_size_bytes", O_RDONLY);
	
	if (tmp_fd == -1) {
	    perror("Could not open block size file: " SYS_PATH "block_size_bytes");
	    return -1;
	}
    
	if (read(tmp_fd, tmp_buf, BUF_SIZE) <= 0) {
	    perror("Could not read block size file: " SYS_PATH "block_size_bytes");
	    return -1;
	}
	
	close(tmp_fd);
	
	block_size_bytes = strtoll(tmp_buf, NULL, 16);
	
	printf("Memory block size is %dMB (%d bytes)\n", block_size_bytes / (1024 * 1024), block_size_bytes);
	
    }
    
    
    num_blocks =  mem_size_bytes / block_size_bytes;
    if (mem_size_bytes % block_size_bytes) num_blocks++;
    
    mem_min_start = block_size_bytes * 
	((mem_min_start / block_size_bytes) + (!!(mem_min_start % block_size_bytes)));
    
    printf("Looking for %d blocks of memory starting at %p (block %llu) with limit32=%d\n", num_blocks, (void*)mem_min_start, mem_min_start/block_size_bytes,limit32);
	
    
    // We now need to find <num_blocks> consecutive offlinable memory blocks
    
    /* Scan the memory directories */
    {
	struct dirent ** namelist = NULL;
	int size = 0;
	int i = 0;
	int j = 0;
	int last_block = 0;
	int first_block = mem_min_start/block_size_bytes;
	int limit_block = 0xffffffff / block_size_bytes; // for 32 bit limiting
	
	last_block = scandir(SYS_PATH, &namelist, dir_filter, dir_cmp);
	bitmap_entries = atoi(namelist[last_block - 1]->d_name + 6) + 1;
	    
	size = bitmap_entries / 8;
	if (bitmap_entries % 8) size++;
	    
	bitmap = malloc(size);
	if (!bitmap) {
	    printf("ERROR: could not allocate space for bitmap\n");
	    return -1;
	}
	
	memset(bitmap, 0, size);
	
	for (i = 0 ; j < bitmap_entries - 1; i++) {
	    struct dirent * tmp_dir = namelist[i];
	    int block_fd = 0;	    
	    char status_str[BUF_SIZE];
	    char fname[BUF_SIZE];
		
	    memset(status_str, 0, BUF_SIZE);
	    memset(fname, 0, BUF_SIZE);
		
	    snprintf(fname, BUF_SIZE, "%s%s/removable", SYS_PATH, tmp_dir->d_name);
		
	    j = atoi(tmp_dir->d_name + 6);
	    int major = j / 8;
	    int minor = j % 8;
		
		
	    if (i<first_block) { 
		printf("Skipping %s due to minimum start constraint\n",fname);
		continue;
	    } 

	    if (limit32 && i>limit_block) { 
		printf("Skipping %s due to 32 bit constraint\n",fname);
		continue;
	    } 
		
		
	    printf("Checking %s...", fname);
		
	    block_fd = open(fname, O_RDONLY);
		
	    if (block_fd == -1) {
		printf("Hotpluggable memory not supported...\n");
		return -1;
	    }
		
	    if (read(block_fd, status_str, BUF_SIZE) <= 0) {
		perror("Could not read block status");
		return -1;
	    }
		
	    close(block_fd);
		
	    if (atoi(status_str) == 1) {
		printf("Removable\n");
		bitmap[major] |= (0x1 << minor);
	    } else {
		printf("Not removable\n");
	    }
	}
	
    }
    
    while (!mem_ready) {
	
	
	/* Scan bitmap for enough consecutive space */
	{
	    // num_blocks: The number of blocks we need to find
	    // bitmap: bitmap of blocks (1 == allocatable)
	    // bitmap_entries: number of blocks in the system/number of bits in bitmap
	    // reg_start: The block index where our allocation will start
		
	    int i = 0;
	    int run_len = 0;
	    
	    for (i = 0; i < bitmap_entries; i++) {
		int i_major = i / 8;
		int i_minor = i % 8;
		
		if (!(bitmap[i_major] & (0x1 << i_minor))) {
		    reg_start = i + 1; // skip the region start to next entry
		    run_len = 0;
		    continue;
		}
		
		run_len++;
		    
		if (run_len >= num_blocks) {
		    break;
		}
	    }
	    
	    
	    if (run_len < num_blocks) {
		fprintf(stderr, "Could not find enough consecutive memory blocks... (found %d)\n", run_len);
		return -1;
	    }
	}
	
	
	/* Offline memory blocks starting at reg_start */
	{
	    int i = 0;
	    
	    for (i = 0; i < num_blocks; i++) {	
		FILE * block_file = NULL;
		char fname[256];
		
		memset(fname, 0, 256);
		
		snprintf(fname, 256, "%smemory%d/state", SYS_PATH, i + reg_start);
		
		block_file = fopen(fname, "r+");
		
		if (block_file == NULL) {
		    perror("Could not open block file");
		    return -1;
		}
		
		    
		printf("Offlining block %d (%s)\n", i + reg_start, fname);
		fprintf(block_file, "offline\n");
		
		fclose(block_file);
	    }
	}
	
	
	/*  We asked to offline set of blocks, but Linux could have lied. 
	 *  To be safe, check whether blocks were offlined and start again if not 
	 */
	
	{
	    int i = 0;
	    
	    mem_ready = 1; // Hopefully we are ok...
	    
	    
	    for (i = 0; i < num_blocks; i++) {
		int block_fd = 0;
		char fname[BUF_SIZE];
		char status_buf[BUF_SIZE];
		
		
		memset(fname, 0, BUF_SIZE);
		memset(status_buf, 0, BUF_SIZE);
		
		snprintf(fname, BUF_SIZE, "%smemory%d/state", SYS_PATH, i + reg_start);
		
		
		block_fd = open(fname, O_RDONLY);
		
		if (block_fd == -1) {
		    perror("Could not open block file");
		    return -1;
		}
		
		if (read(block_fd, status_buf, BUF_SIZE) <= 0) {
		    perror("Could not read block status");
		    return -1;
		}
		
		printf("Checking offlined block %d (%s)...", i + reg_start, fname);
		
		int ret = strncmp(status_buf, "offline", strlen("offline"));
		
		if (ret != 0) {
		    int j = 0;
		    int major = (i + reg_start) / 8;
		    int minor = (i + reg_start) % 8;

		    bitmap[major] &= ~(0x1 << minor); // mark the block as not removable in bitmap
		    
		    mem_ready = 0; // Keep searching
		    
		    printf("ERROR (%d)\n", ret);
		    
		    for (j = 0; j < i; j++) {
			FILE * block_file = NULL;
			char fname[256];
			
			memset(fname, 0, 256);
			
			snprintf(fname, 256, "%smemory%d/state", SYS_PATH, j + reg_start);
			
			block_file = fopen(fname, "r+");
			
			if (block_file == NULL) {
			    perror("Could not open block file");
			    return -1;
			}
			
			fprintf(block_file, "online\n");
			
			fclose(block_file);
		    }
		    
		    break;
		} 
		
		printf("OK\n");
		
	    }
	    
	    
	}
    }
    
    free(bitmap);
    
    /* Memory is offlined. Calculate size and phys start addr to send to Palacios */
    *num_bytes = (unsigned long long)(num_blocks) * (unsigned long long)(block_size_bytes);
    *base_addr = (unsigned long long)(reg_start) * (unsigned long long)(block_size_bytes);

    return 0;
}

