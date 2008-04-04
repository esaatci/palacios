#ifndef __VMM_EMULATE_H
#define __VMM_EMULATE_H
#include <geekos/vm_guest.h>


/*
 * This is where we do the hideous X86 instruction parsing among other things
 * We can parse out the instruction prefixes, as well as decode the operands 
 *
 * Before we begin I'd just like to say a few words to those that made this possible...
 *
 *
 *				     _____
 *				    ||	 ||
 *				    |\___/|
 *				    |	  |
 *				    |	  |
 *				    |	  |
 *				    |	  |
 *				    |	  |
 *				    |	  |
 *			       _____|<--->|_____
 *			   ___/     |	  |	 \
 *			 /    |     |	  |	| \
 *			 |    |     |	  |	|  |
 *			 |    |     |	  |	|  |
 *			 |			|  |
 *			 |			|  |
 *			 |    Fuck You Intel!	  /
 *			 |			 /
 *			  \		       /
 *			   \		      /
 *			    |		      |
 *			    |		      |
 *
 * That is all.
 *
 */


/* JRL: Some of this was taken from the Xen sources... 
 *
 */

#define PACKED __attribute__((packed))

#define MODRM_MOD(x) ((x >> 6) & 0x3)
#define MODRM_REG(x) ((x >> 3) & 0x7)
#define MODRM_RM(x)  (x & 0x7)

struct modrm_byte {
  uint_t rm   :   3 PACKED;
  uint_t reg  :   3 PACKED;
  uint_t mod  :   2 PACKED;
};


#define SIB_BASE(x) ((x >> 6) & 0x3)
#define SIB_INDEX(x) ((x >> 3) & 0x7)
#define SIB_SCALE(x) (x & 0x7)

struct sib_byte {
  uint_t base     :   3 PACKED;
  uint_t index    :   3 PACKED;
  uint_t scale    :   2 PACKED;
};



#define MAKE_INSTR(nm, ...) static const uchar_t OPCODE_##nm[] = { __VA_ARGS__ }

/* 
 * Here's how it works:
 * First byte: Length. 
 * Following bytes: Opcode bytes. 
 * Special case: Last byte, if zero, doesn't need to match. 
 */
MAKE_INSTR(INVD,   2, 0x0f, 0x08);
MAKE_INSTR(CPUID,  2, 0x0f, 0xa2);
MAKE_INSTR(RDMSR,  2, 0x0f, 0x32);
MAKE_INSTR(WRMSR,  2, 0x0f, 0x30);
MAKE_INSTR(RDTSC,  2, 0x0f, 0x31);
MAKE_INSTR(RDTSCP, 3, 0x0f, 0x01, 0xf9);
MAKE_INSTR(CLI,    1, 0xfa);
MAKE_INSTR(STI,    1, 0xfb);
MAKE_INSTR(RDPMC,  2, 0x0f, 0x33);
MAKE_INSTR(CLGI,   3, 0x0f, 0x01, 0xdd);
MAKE_INSTR(STGI,   3, 0x0f, 0x01, 0xdc);
MAKE_INSTR(VMRUN,  3, 0x0f, 0x01, 0xd8);
MAKE_INSTR(VMLOAD, 3, 0x0f, 0x01, 0xda);
MAKE_INSTR(VMSAVE, 3, 0x0f, 0x01, 0xdb);
MAKE_INSTR(VMCALL, 3, 0x0f, 0x01, 0xd9);
MAKE_INSTR(PAUSE,  2, 0xf3, 0x90);
MAKE_INSTR(SKINIT, 3, 0x0f, 0x01, 0xde);
MAKE_INSTR(MOV2CR, 3, 0x0f, 0x22, 0x00);
MAKE_INSTR(MOVCR2, 3, 0x0f, 0x20, 0x00);
MAKE_INSTR(MOV2DR, 3, 0x0f, 0x23, 0x00);
MAKE_INSTR(MOVDR2, 3, 0x0f, 0x21, 0x00);
MAKE_INSTR(PUSHF,  1, 0x9c);
MAKE_INSTR(POPF,   1, 0x9d);
MAKE_INSTR(RSM,    2, 0x0f, 0xaa);
MAKE_INSTR(INVLPG, 3, 0x0f, 0x01, 0x00);
MAKE_INSTR(INVLPGA,3, 0x0f, 0x01, 0xdf);
MAKE_INSTR(HLT,    1, 0xf4);
MAKE_INSTR(CLTS,   2, 0x0f, 0x06);
MAKE_INSTR(LMSW,   3, 0x0f, 0x01, 0x00);
MAKE_INSTR(SMSW,   3, 0x0f, 0x01, 0x00);


static const uchar_t PREFIX_LOCK = 0xF0;
static const uchar_t PREFIX_REPNE = 0xF2;
static const uchar_t PREFIX_REPNZ = 0xF2;
static const uchar_t PREFIX_REP = 0xF3;
static const uchar_t PREFIX_REPE = 0xF3;
static const uchar_t PREFIX_REPZ = 0xF3;
static const uchar_t PREFIX_CS_OVERRIDE = 0x2E;
static const uchar_t PREFIX_SS_OVERRIDE = 0x36;
static const uchar_t PREFIX_DS_OVERRIDE = 0x3E;
static const uchar_t PREFIX_ES_OVERRIDE = 0x26;
static const uchar_t PREFIX_FS_OVERRIDE = 0x64;
static const uchar_t PREFIX_GS_OVERRIDE = 0x65;
static const uchar_t PREFIX_BR_NOT_TAKEN = 0x2E;
static const uchar_t PREFIX_BR_TAKEN = 0x3E;
static const uchar_t PREFIX_OP_SIZE = 0x66;
static const uchar_t PREFIX_ADDR_SIZE = 0x67;


static inline int is_prefix_byte(char byte) {
  switch (byte) {
  case 0xF0:      // lock
  case 0xF2:      // REPNE/REPNZ
  case 0xF3:      // REP or REPE/REPZ
  case 0x2E:      // CS override or Branch hint not taken (with Jcc instrs)
  case 0x36:      // SS override
  case 0x3E:      // DS override or Branch hint taken (with Jcc instrs)
  case 0x26:      // ES override
  case 0x64:      // FS override
  case 0x65:      // GS override
    //case 0x2E:      // branch not taken hint
    //  case 0x3E:      // branch taken hint
  case 0x66:      // operand size override
  case 0x67:      // address size override
    return 1;
    break;
  default:
    return 0;
    break;
  }
}



typedef enum {INVALID_ADDR_TYPE, REG, DISP0, DISP8, DISP16, DISP32} modrm_mode_t;
typedef enum {INVALID_REG_SIZE, REG64, REG32, REG16, REG8} reg_size_t;
typedef enum {INVALID_OPERAND, REG_OPERAND, MEM_OPERAND} operand_type_t;

struct guest_gprs;

static inline addr_t decode_register(struct guest_gprs * gprs, char reg_code, reg_size_t reg_size) {
  addr_t reg_addr;

  switch (reg_code) {
  case 0:
    reg_addr = (addr_t)&(gprs->rax);
    break;
  case 1:
    reg_addr = (addr_t)&(gprs->rcx);
    break;
  case 2:
    reg_addr = (addr_t)&(gprs->rdx);
    break;
  case 3:
    reg_addr = (addr_t)&(gprs->rbx);
    break;
  case 4:
    if (reg_size == REG8) {
      reg_addr = (addr_t)&(gprs->rax) + 1;
    } else {
      reg_addr = (addr_t)&(gprs->rsp);
    }
    break;
  case 5:
    if (reg_size == REG8) {
      reg_addr = (addr_t)&(gprs->rcx) + 1;
    } else {
      reg_addr = (addr_t)&(gprs->rbp);
    }
    break;
  case 6:
    if (reg_size == REG8) {
      reg_addr = (addr_t)&(gprs->rdx) + 1;
    } else {
      reg_addr = (addr_t)&(gprs->rsi);
    }
    break;
  case 7:
    if (reg_size == REG8) {
      reg_addr = (addr_t)&(gprs->rbx) + 1;
    } else {
      reg_addr = (addr_t)&(gprs->rdi);
    }
    break;
  default:
    reg_addr = 0;
    break;
  }

  return reg_addr;
}



static inline operand_type_t decode_operands16(struct guest_gprs * gprs, // input/output
					       char * modrm_instr,       // input
					       int * offset,             // output
					       addr_t * first_operand,   // output
					       addr_t * second_operand,  // output
					       reg_size_t reg_size) {    // input
  
  struct modrm_byte * modrm = (struct modrm_byte *)modrm_instr;
  addr_t base_addr = 0;
  modrm_mode_t mod_mode = 0;
  operand_type_t addr_type = INVALID_OPERAND;
  char * instr_cursor = modrm_instr;

  PrintDebug("ModRM mod=%d\n", modrm->mod);

  instr_cursor += 1;

  if (modrm->mod == 3) {
    mod_mode = REG;
    addr_type = REG_OPERAND;
    PrintDebug("first operand = Register (RM=%d)\n",modrm->rm);

    *first_operand = decode_register(gprs, modrm->rm, reg_size);

  } else {

    addr_type = MEM_OPERAND;

    if (modrm->mod == 0) {
      mod_mode = DISP0;
    } else if (modrm->mod == 1) {
      mod_mode = DISP8;
    } else if (modrm->mod == 2) {
      mod_mode = DISP16;
    }

    switch (modrm->rm) {
    case 0:
      base_addr = gprs->rbx + gprs->rsi;
      break;
    case 1:
      base_addr = gprs->rbx + gprs->rdi;
      break;
    case 2:
      base_addr = gprs->rbp + gprs->rsi;
      break;
    case 3:
      base_addr = gprs->rbp + gprs->rdi;
      break;
    case 4:
      base_addr = gprs->rsi;
      break;
    case 5:
      base_addr = gprs->rdi;
      break;
    case 6:
      if (modrm->mod == 0) {
	base_addr = 0;
	mod_mode = DISP16;
      } else {
	base_addr = gprs->rbp;
      }
      break;
    case 7:
      base_addr = gprs->rbx;
      break;
    }



    if (mod_mode == DISP8) {
      base_addr += (uchar_t)*(instr_cursor);
      instr_cursor += 1;
    } else if (mod_mode == DISP16) {
      base_addr += (ushort_t)*(instr_cursor);
      instr_cursor += 2;
    }
    
    *first_operand = base_addr;
  }

  *offset +=  (instr_cursor - modrm_instr);
  *second_operand = decode_register(gprs, modrm->reg, reg_size);

  return addr_type;
}



static inline operand_type_t decode_operands32(struct guest_gprs * gprs, // input/output
					       char * modrm_instr,       // input
					       int * offset,             // output
					       addr_t * first_operand,   // output
					       addr_t * second_operand,  // output
					       reg_size_t reg_size) {    // input
  
  char * instr_cursor = modrm_instr;
  struct modrm_byte * modrm = (struct modrm_byte *)modrm_instr;
  addr_t base_addr = 0;
  modrm_mode_t mod_mode = 0;
  uint_t has_sib_byte = 0;
  operand_type_t addr_type = INVALID_OPERAND;



  instr_cursor += 1;

  if (modrm->mod == 3) {
    mod_mode = REG;
    addr_type = REG_OPERAND;
    
    PrintDebug("first operand = Register (RM=%d)\n",modrm->rm);

    *first_operand = decode_register(gprs, modrm->rm, reg_size);

  } else {

    addr_type = MEM_OPERAND;

    if (modrm->mod == 0) {
      mod_mode = DISP0;
    } else if (modrm->mod == 1) {
      mod_mode = DISP8;
    } else if (modrm->mod == 2) {
      mod_mode = DISP32;
    }
    
    switch (modrm->rm) {
    case 0:
      base_addr = gprs->rax;
      break;
    case 1:
      base_addr = gprs->rcx;
      break;
    case 2:
      base_addr = gprs->rdx;
      break;
    case 3:
      base_addr = gprs->rbx;
      break;
    case 4:
      has_sib_byte = 1;
      break;
    case 5:
      if (modrm->mod == 0) {
	base_addr = 0;
	mod_mode = DISP32;
      } else {
	base_addr = gprs->rbp;
      }
      break;
    case 6:
      base_addr = gprs->rsi;
      break;
    case 7:
      base_addr = gprs->rdi;
      break;
    }

    if (has_sib_byte) {
      instr_cursor += 1;
      struct sib_byte * sib = (struct sib_byte *)(instr_cursor);
      int scale = 1;

      instr_cursor += 1;


      if (sib->scale == 1) {
	scale = 2;
      } else if (sib->scale == 2) {
	scale = 4;
      } else if (sib->scale == 3) {
	scale = 8;
      }


      switch (sib->index) {
      case 0:
	base_addr = gprs->rax;
	break;
      case 1:
	base_addr = gprs->rcx;
	break;
      case 2:
	base_addr = gprs->rdx;
	break;
      case 3:
	base_addr = gprs->rbx;
	break;
      case 4:
	base_addr = 0;
	break;
      case 5:
	base_addr = gprs->rbp;
	break;
      case 6:
	base_addr = gprs->rsi;
	break;
      case 7:
	base_addr = gprs->rdi;
	break;
      }

      base_addr *= scale;


      switch (sib->base) {
      case 0:
	base_addr += gprs->rax;
	break;
      case 1:
	base_addr += gprs->rcx;
	break;
      case 2:
	base_addr += gprs->rdx;
	break;
      case 3:
	base_addr += gprs->rbx;
	break;
      case 4:
	base_addr += gprs->rsp;
	break;
      case 5:
	if (modrm->mod != 0) {
	  base_addr += gprs->rbp;
	}
	break;
      case 6:
	base_addr += gprs->rsi;
	break;
      case 7:
	base_addr += gprs->rdi;
	break;
      }

    } 


    if (mod_mode == DISP8) {
      base_addr += (uchar_t)*(instr_cursor);
      instr_cursor += 1;
    } else if (mod_mode == DISP32) {
      base_addr += (uint_t)*(instr_cursor);
      instr_cursor += 4;
    }
    

    *first_operand = base_addr;
  }

  *offset += (instr_cursor - modrm_instr);

  *second_operand = decode_register(gprs, modrm->reg, reg_size);

  return addr_type;
}




#endif
