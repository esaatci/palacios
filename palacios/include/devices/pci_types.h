/* 
 * This file is part of the Palacios Virtual Machine Monitor developed
 * by the V3VEE Project with funding from the United States National 
 * Science Foundation and the Department of Energy.  
 *
 * The V3VEE Project is a joint project between Northwestern University
 * and the University of New Mexico.  You can find out more at 
 * http://www.v3vee.org
 *
 * Copyright (c) 2009, Jack Lange <jarusl@cs.northwestern.edu>
 * Copyright (c) 2009, The V3VEE Project <http://www.v3vee.org> 
 * All rights reserved.
 *
 * Author:  Jack Lange 
 *
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "V3VEE_LICENSE".
 */

#ifndef __DEVICES_PCI_TYPES_H__
#define __DEVICES_PCI_TYPES_H__


#include <palacios/vmm_types.h>

// struct pci_device_config
struct pci_config_header {
    union {
	uint32_t reg_00;
	struct {
	    uint16_t   vendor_id;
	    uint16_t   device_id;
	} __attribute__((packed));
    } __attribute__((packed));

    union {
	uint32_t reg_04;
	struct {
	    uint16_t   command;
	    uint16_t   status;
	} __attribute__((packed));
    } __attribute__((packed));
    
	
    union {
	uint32_t reg_08;
	struct {
	    uint16_t    revision;
	    uint8_t     subclass;
	    uint8_t     class;
	} __attribute__((packed));
    } __attribute__((packed));

    union {
	uint32_t reg_0c;
	struct {
	    uint8_t    cache_line_size;
	    uint8_t    latency_time;
	    uint8_t    header_type; // bits 6-0: 00: other, 01: pci-pci bridge, 02: pci-cardbus; bit 7: 1=multifunction
	    uint8_t    BIST;  
	} __attribute__((packed));
    } __attribute__((packed));
    

    union {
	uint32_t reg_10;
	uint32_t   BAR0;
    } __attribute__((packed));

    union {
	uint32_t reg_14;
	uint32_t   BAR1;
    } __attribute__((packed));

    union {
	uint32_t reg_18;
	uint32_t   BAR2;
    } __attribute__((packed));

    union {
	uint32_t reg_1c;
	uint32_t   BAR3;
    } __attribute__((packed));

    union {
	uint32_t reg_20;
	uint32_t   BAR4;
    } __attribute__((packed));

    union {
	uint32_t reg_24;
	uint32_t   BAR5;
    } __attribute__((packed));


    union {
	uint32_t reg_28;
	uint32_t   cardbus_cis_pointer;
    } __attribute__((packed));

    union {
	uint32_t reg_2c;
	struct {
	    uint16_t   subsystem_vendor_id;
	    uint16_t   subsystem_id;
	} __attribute__((packed));
    } __attribute__((packed));

    union {
	uint32_t reg_30;
	uint32_t expansion_rom_address;
    } __attribute__((packed));;

    union {
	uint32_t reg_34;
	struct {
	    uint8_t cap_ptr;  // capabilities list offset in config space
	    uint8_t rsvd1[3];
	} __attribute__((packed));
    } __attribute__((packed));

    union {
	uint32_t reg_38;
	uint32_t rsvd2;
    } __attribute__((packed));


    union {
	uint32_t reg_3c;
	struct {
	    uint8_t    intr_line; // 00=none, 01=IRQ1, etc.
	    uint8_t    intr_pin;  // 00=none, otherwise INTA# to INTD#
	    uint8_t    min_grant; // min busmaster time - units of 250ns
	    uint8_t    max_latency; // units of 250ns - busmasters
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));


/*
  struct pci_class_desc {
  uint16_t class;
  const char * desc;
  };
  
  static struct pci_class_desc pci_class_descriptions[] = {
  { 0x0100, "SCSI controller"},
  { 0x0101, "IDE controller"},
  { 0x0102, "Floppy controller"},
  { 0x0103, "IPI controller"},
  { 0x0104, "RAID controller"},
  { 0x0106, "SATA controller"},
  { 0x0107, "SAS controller"},
  { 0x0180, "Storage controller"},
  { 0x0200, "Ethernet controller"},
  { 0x0201, "Token Ring controller"},
  { 0x0202, "FDDI controller"},
  { 0x0203, "ATM controller"},
  { 0x0280, "Network controller"},
  { 0x0300, "VGA controller"},
  { 0x0301, "XGA controller"},
  { 0x0302, "3D controller"},
  { 0x0380, "Display controller"},
  { 0x0400, "Video controller"},
  { 0x0401, "Audio controller"},
  { 0x0402, "Phone"},
  { 0x0480, "Multimedia controller"},
  { 0x0500, "RAM controller"},
  { 0x0501, "Flash controller"},
  { 0x0580, "Memory controller"},
  { 0x0600, "Host bridge"},
  { 0x0601, "ISA bridge"},
  { 0x0602, "EISA bridge"},
  { 0x0603, "MC bridge"},
  { 0x0604, "PCI bridge"},
  { 0x0605, "PCMCIA bridge"},
  { 0x0606, "NUBUS bridge"},
  { 0x0607, "CARDBUS bridge"},
  { 0x0608, "RACEWAY bridge"},
  { 0x0680, "Bridge"},
  { 0x0c03, "USB controller"},
  { 0, NULL}
  };
*/

#endif
