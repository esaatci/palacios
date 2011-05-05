/* 
 * This file is part of the Palacios Virtual Machine Monitor developed
 * by the V3VEE Project with funding from the United States National 
 * Science Foundation and the Department of Energy.  
 *
 * The V3VEE Project is a joint project between Northwestern University
 * and the University of New Mexico.  You can find out more at 
 * http://www.v3vee.org
 *
 * Copyright (c) 2011, Lei Xia <lxia@northwestern.edu> 
 * Copyright (c) 2011, The V3VEE Project <http://www.v3vee.org> 
 * All rights reserved.
 *
 * Author: Lei Xia <lxia@northwestern.edu>
 *
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "V3VEE_LICENSE".
 */
 
#include <vnet/vnet_host.h>
#include <vnet/vnet.h>

struct vnet_host_hooks * host_hooks;


int vnet_lock_init(vnet_lock_t * lock) {
    *lock = (addr_t)(host_hooks->mutex_alloc());

    if (!(*lock)) {
	return -1;
    }

    return 0;
}


struct vnet_thread * vnet_start_thread(int (*func)(void *), void *arg, char * name){
    struct vnet_thread * thread;

    thread = Vnet_Malloc(sizeof(struct vnet_thread));
	
    thread->host_thread = host_hooks->thread_start(func, arg, name);

    if(thread->host_thread == NULL){
	Vnet_Free(thread);
	return NULL;
    }

    return thread;
}



struct vnet_timer * vnet_create_timer(unsigned long interval, 
	void (* timer_fun)(void * priv_data), void * priv_data){
    struct vnet_timer * timer = NULL; 

    if((host_hooks) && host_hooks->timer_create){
	timer = Vnet_Malloc(sizeof(struct vnet_timer));

	timer->host_timer = host_hooks->timer_create(interval, timer_fun, priv_data);
    }

    return timer;
 }


void init_vnet(struct vnet_host_hooks * hooks){
    host_hooks = hooks;
    v3_init_vnet();
}

