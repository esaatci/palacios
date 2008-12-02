/*BEGIN_LEGAL 
Intel Open Source License 

Copyright (c) 2002-2007 Intel Corporation 
All rights reserved. 
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.  Redistributions
in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.  Neither the name of
the Intel Corporation nor the names of its contributors may be used to
endorse or promote products derived from this software without
specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE INTEL OR
ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
END_LEGAL */
/// @file xed-operand-bitvec.h
/// @author Mark Charney <mark.charney@intel.com>

// This file was automatically generated.
// Do not edit this file.

#if !defined(_XED_OPERAND_BITVEC_H_)
# define _XED_OPERAND_BITVEC_H_
#include <xed/xed-types.h>
typedef union {
   xed_uint32_t i[4];
   struct {
     xed_uint32_t  x_AGEN : 1; /* 00:00 */
     xed_uint32_t  x_AMODE : 1; /* 00:01 */
     xed_uint32_t  x_ASZ : 1; /* 00:02 */
     xed_uint32_t  x_BASE0 : 1; /* 00:03 */
     xed_uint32_t  x_BASE1 : 1; /* 00:04 */
     xed_uint32_t  x_BRDISP_WIDTH : 1; /* 00:05 */
     xed_uint32_t  x_BRDISP0 : 1; /* 00:06 */
     xed_uint32_t  x_BRDISP1 : 1; /* 00:07 */
     xed_uint32_t  x_DEFAULT_SEG : 1; /* 00:08 */
     xed_uint32_t  x_DF64 : 1; /* 00:09 */
     xed_uint32_t  x_DISP_WIDTH : 1; /* 00:10 */
     xed_uint32_t  x_DISP0 : 1; /* 00:11 */
     xed_uint32_t  x_DISP1 : 1; /* 00:12 */
     xed_uint32_t  x_DISP2 : 1; /* 00:13 */
     xed_uint32_t  x_DISP3 : 1; /* 00:14 */
     xed_uint32_t  x_EASZ : 1; /* 00:15 */
     xed_uint32_t  x_ENCODER_PREFERRED : 1; /* 00:16 */
     xed_uint32_t  x_EOSZ : 1; /* 00:17 */
     xed_uint32_t  x_ERROR : 1; /* 00:18 */
     xed_uint32_t  x_HINT_TAKEN : 1; /* 00:19 */
     xed_uint32_t  x_HINT_NOT_TAKEN : 1; /* 00:20 */
     xed_uint32_t  x_ICLASS : 1; /* 00:21 */
     xed_uint32_t  x_IMM_WIDTH : 1; /* 00:22 */
     xed_uint32_t  x_IMM0 : 1; /* 00:23 */
     xed_uint32_t  x_IMM0SIGNED : 1; /* 00:24 */
     xed_uint32_t  x_IMM1 : 1; /* 00:25 */
     xed_uint32_t  x_INDEX : 1; /* 00:26 */
     xed_uint32_t  x_LOCK : 1; /* 00:27 */
     xed_uint32_t  x_LOCKABLE : 1; /* 00:28 */
     xed_uint32_t  x_MEM_WIDTH : 1; /* 00:29 */
     xed_uint32_t  x_MEM0 : 1; /* 00:30 */
     xed_uint32_t  x_MEM1 : 1; /* 00:31 */
     xed_uint32_t  x_MOD : 1; /* 01:00 */
     xed_uint32_t  x_MODE : 1; /* 01:01 */
     xed_uint32_t  x_MODRM : 1; /* 01:02 */
     xed_uint32_t  x_NOREX : 1; /* 01:03 */
     xed_uint32_t  x_OSZ : 1; /* 01:04 */
     xed_uint32_t  x_OUTREG : 1; /* 01:05 */
     xed_uint32_t  x_PTR : 1; /* 01:06 */
     xed_uint32_t  x_REFINING : 1; /* 01:07 */
     xed_uint32_t  x_REG : 1; /* 01:08 */
     xed_uint32_t  x_REG0 : 1; /* 01:09 */
     xed_uint32_t  x_REG1 : 1; /* 01:10 */
     xed_uint32_t  x_REG2 : 1; /* 01:11 */
     xed_uint32_t  x_REG3 : 1; /* 01:12 */
     xed_uint32_t  x_REG4 : 1; /* 01:13 */
     xed_uint32_t  x_REG5 : 1; /* 01:14 */
     xed_uint32_t  x_REG6 : 1; /* 01:15 */
     xed_uint32_t  x_REG7 : 1; /* 01:16 */
     xed_uint32_t  x_REG8 : 1; /* 01:17 */
     xed_uint32_t  x_REG9 : 1; /* 01:18 */
     xed_uint32_t  x_REG10 : 1; /* 01:19 */
     xed_uint32_t  x_REG11 : 1; /* 01:20 */
     xed_uint32_t  x_REG12 : 1; /* 01:21 */
     xed_uint32_t  x_REG13 : 1; /* 01:22 */
     xed_uint32_t  x_REG14 : 1; /* 01:23 */
     xed_uint32_t  x_REG15 : 1; /* 01:24 */
     xed_uint32_t  x_RELBR : 1; /* 01:25 */
     xed_uint32_t  x_REP : 1; /* 01:26 */
     xed_uint32_t  x_REP_ABLE : 1; /* 01:27 */
     xed_uint32_t  x_REX : 1; /* 01:28 */
     xed_uint32_t  x_REXB : 1; /* 01:29 */
     xed_uint32_t  x_REXR : 1; /* 01:30 */
     xed_uint32_t  x_REXW : 1; /* 01:31 */
     xed_uint32_t  x_REXX : 1; /* 02:00 */
     xed_uint32_t  x_RM : 1; /* 02:01 */
     xed_uint32_t  x_SCALE : 1; /* 02:02 */
     xed_uint32_t  x_SEG_OVD : 1; /* 02:03 */
     xed_uint32_t  x_SEG0 : 1; /* 02:04 */
     xed_uint32_t  x_SEG1 : 1; /* 02:05 */
     xed_uint32_t  x_SIB : 1; /* 02:06 */
     xed_uint32_t  x_SIBBASE : 1; /* 02:07 */
     xed_uint32_t  x_SIBINDEX : 1; /* 02:08 */
     xed_uint32_t  x_SIBSCALE : 1; /* 02:09 */
     xed_uint32_t  x_SMODE : 1; /* 02:10 */
     xed_uint32_t  x_UIMM00 : 1; /* 02:11 */
     xed_uint32_t  x_UIMM1 : 1; /* 02:12 */
     xed_uint32_t  x_UIMM01 : 1; /* 02:13 */
     xed_uint32_t  x_UIMM02 : 1; /* 02:14 */
     xed_uint32_t  x_UIMM03 : 1; /* 02:15 */
     xed_uint32_t  x_USING_DEFAULT_SEGMENT0 : 1; /* 02:16 */
     xed_uint32_t  x_USING_DEFAULT_SEGMENT1 : 1; /* 02:17 */
   } s;
} xed_operand_bitvec_t;
#endif
