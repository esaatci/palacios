/* 
 * This file is part of the Palacios Virtual Machine Monitor developed
 * by the V3VEE Project with funding from the United States National 
 * Science Foundation and the Department of Energy.  
 *
 * The V3VEE Project is a joint project between Northwestern University
 * and the University of New Mexico.  You can find out more at 
 * http://www.v3vee.org
 *
 * Copyright (c) 2008, Peter Dinda <pdinda@northwestern.edu> 
 * Copyright (c) 2008, The V3VEE Project <http://www.v3vee.org> 
 * All rights reserved.
 *
 * Author: Peter Dinda <pdinda@northwestern.edu>
 *
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "V3VEE_LICENSE".
 */

#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <palacios/vm_dev.h>

//
// The underlying driver needs to call this on each key that
// it wants to inject into the VMM for delivery to a VM
//
void deliver_key_to_vmm(uchar_t status, uchar_t scancode);
// And call this one each streaming mouse event
// that the VMM should deliver
void deliver_mouse_to_vmm(uchar_t mouse_packet[3]);

struct vm_device *create_keyboard();

#endif
