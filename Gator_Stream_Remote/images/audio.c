/* Copyright (c) 2012, Texas Instruments Incorporated
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

*  Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

*  Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

*  Neither the name of Texas Instruments Incorporated nor the names of
   its contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#include "grlib.h"

static const uint8_t pixel_audio4BPP_UNCOMP[] =
{
0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x2e, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe2, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 
0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 
0x22, 0x22, 0x22, 0x22, 0x22, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0x22, 0x22, 0x22, 0x22, 0x22, 
0x22, 0x22, 0x22, 0x22, 0xee, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x22, 0x22, 0x22, 0x22, 
0x22, 0x22, 0x22, 0x25, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe2, 0x22, 0x22, 0x22, 
0x22, 0x22, 0x22, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x36, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xde, 0x5e, 0x5e, 0x5e, 0x5e, 0x22, 0x22, 0x22, 
0x22, 0x22, 0x25, 0xe5, 0xe5, 0xe5, 0xe5, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0xf5, 0xe5, 0xe5, 0xe5, 0xe2, 0x22, 0x22, 
0x22, 0x22, 0x5e, 0x5e, 0x5e, 0x5e, 0xf6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x22, 0x22, 
0x22, 0x2e, 0xe5, 0xe5, 0xee, 0xe6, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x63, 0xe5, 0xe5, 0xee, 0xe2, 0x22, 
0x22, 0x2e, 0x5e, 0x5e, 0x5e, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0xce, 0x5e, 0x5e, 0x52, 0x22, 
0x22, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x63, 0xe5, 0xe5, 0xe5, 0x22, 
0x22, 0x5e, 0x5e, 0x5e, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x22, 
0x25, 0xe5, 0xee, 0xef, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x65, 0xe5, 0xe5, 0xe2, 
0x2e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x52, 
0x25, 0xe5, 0xe5, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x60, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0xe5, 0xe5, 0xe2, 
0xee, 0x5e, 0x5e, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x69, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0x8e, 0x5e, 0x5e, 
0xee, 0xe5, 0xe3, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0x06, 0x66, 0x66, 0x66, 0x66, 0x66, 0x6e, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x6e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x50, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x6e, 0x5e, 0x5e, 
0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x68, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x69, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 
0xe5, 0xe5, 0xe6, 0x66, 0x66, 0x6c, 0x66, 0x65, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe9, 0x66, 0x6c, 0x66, 0x66, 0x66, 0xe5, 0xee, 
0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x0e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 
0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0x6c, 0x66, 0xb5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xcd, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0xc6, 0x64, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0xf6, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 
0xee, 0xe5, 0xe6, 0x66, 0x66, 0x66, 0x66, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0x66, 0x66, 0x66, 0x66, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe6, 0x66, 0x66, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xe6, 0x66, 0x66, 0x66, 0x66, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5c, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0xfe, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 
0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xac, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xec, 0x6c, 0x66, 0x65, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5d, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0xc6, 0x66, 0xcc, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 
0xe5, 0xe5, 0xe6, 0x66, 0x66, 0x6c, 0x66, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0x96, 0x66, 0x6c, 0x66, 0x66, 0x66, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0x66, 0x6c, 0x66, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe6, 0x6c, 0x66, 0x66, 0x66, 0xe5, 0xee, 
0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0xc6, 0x66, 0xc6, 0x96, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0x1e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 
0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x6b, 0xec, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x65, 0xe5, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0xc6, 0x66, 0xc6, 0x8e, 0x56, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x59, 0x66, 0xc6, 0x69, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 
0xee, 0xe5, 0xe6, 0x66, 0x66, 0x66, 0x66, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe8, 0x66, 0x66, 0x66, 0xe5, 0xe6, 0x66, 0x66, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0x66, 0x66, 0x68, 0xee, 0xe5, 0xe5, 0xe5, 0xe6, 0x66, 0x66, 0x66, 0x66, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0xb6, 0x66, 0xc6, 0x6e, 0x5e, 0x56, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x06, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 
0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xed, 0x66, 0x6c, 0x66, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe6, 0x6c, 0x66, 0xe5, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x6e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xce, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 
0xe5, 0xe5, 0xe6, 0x66, 0x66, 0x6c, 0x66, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0x66, 0x6c, 0x66, 0xd5, 0xe5, 0xe5, 0xe6, 0x66, 0x66, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xed, 0x66, 0x66, 0x65, 0xe5, 0xee, 0xe5, 0xe6, 0x6c, 0x66, 0x66, 0x66, 0xe5, 0xee, 
0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x64, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x66, 0xc6, 0x6e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 
0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0x66, 0x6c, 0x66, 0x65, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe0, 0x6c, 0x66, 0xb5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0x6c, 0x66, 0x6b, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x58, 0xc6, 0x66, 0xce, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x46, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 
0xee, 0xe5, 0xe6, 0x66, 0x66, 0x66, 0x66, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xf6, 0x66, 0x66, 0x65, 0xe5, 0xe5, 0xee, 0xe5, 0xe6, 0x66, 0x66, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0x66, 0x66, 0x64, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe6, 0x66, 0x66, 0xe5, 0xe5, 0xe5, 0xe6, 0x66, 0x66, 0x66, 0x66, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x06, 0x8c, 0xc6, 0x8c, 0x86, 0x8c, 0xc6, 0x8c, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0xc6, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 
0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 0xe6, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x65, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x15, 0xe5, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x05, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0xf6, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0x9e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0xce, 0x5e, 0x5e, 0x5e, 0x5e, 0x5c, 0x66, 0xc6, 0x6e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 
0xe5, 0xe5, 0xe6, 0x66, 0x66, 0x6c, 0x66, 0xe5, 0xe5, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x6b, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xe6, 0x66, 0x66, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xe5, 0x66, 0x66, 0x65, 0xee, 0xe5, 0xe5, 0xe5, 0xe5, 0x66, 0x66, 0x65, 0xee, 0xe5, 0xe6, 0x6c, 0x66, 0x66, 0x66, 0xe5, 0xee, 
0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0x9e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0xc6, 0x66, 0xc3, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0xc6, 0x66, 0xce, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 
0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 0x66, 0x6c, 0x65, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xc6, 0x6c, 0x66, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0x66, 0x6c, 0x65, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x66, 0xc6, 0x69, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x66, 0xc6, 0x6e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 
0xee, 0xe5, 0xe6, 0x66, 0x66, 0x66, 0x66, 0xe5, 0xee, 0x66, 0x66, 0x65, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe6, 0x66, 0x66, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe6, 0x66, 0x66, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0x66, 0x66, 0x6d, 0xe5, 0xe5, 0xe6, 0x66, 0x66, 0x66, 0x66, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 0xc6, 0x66, 0xce, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x3e, 0x5e, 0x5e, 0x5e, 0x5e, 0xa6, 0x66, 0xcc, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 
0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 0x66, 0x6c, 0x65, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xeb, 0x6c, 0x66, 0x65, 0xe5, 0xe5, 0xe5, 0xe5, 0xb6, 0x6c, 0x6c, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x66, 0xc6, 0x69, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x59, 0xc6, 0x66, 0xce, 0x5e, 0x5e, 0x5e, 0x5e, 0xd6, 0xc6, 0x66, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 
0xe5, 0xe5, 0xe6, 0x66, 0x66, 0x6c, 0x66, 0xe5, 0xe5, 0x6c, 0x66, 0x65, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xe6, 0x66, 0x66, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0x66, 0x6c, 0x6e, 0xe5, 0xe5, 0xe5, 0xee, 0x96, 0x66, 0x6c, 0xee, 0xe5, 0xe6, 0x6c, 0x66, 0x66, 0x66, 0xe5, 0xee, 
0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 0xc6, 0x66, 0xce, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x66, 0xc6, 0x6e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 
0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 0x66, 0x6c, 0x65, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0x6c, 0x66, 0x65, 0xe5, 0xe5, 0xe5, 0xe5, 0xe6, 0x6c, 0x66, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x66, 0xc6, 0x69, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0xc6, 0x66, 0xce, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 
0xee, 0xe5, 0xe6, 0x66, 0x66, 0x66, 0x66, 0xe5, 0xee, 0x66, 0x66, 0x65, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe6, 0x66, 0x66, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0x66, 0x66, 0x65, 0xe5, 0xee, 0xe5, 0xe5, 0xe6, 0x66, 0x66, 0xe5, 0xe5, 0xe6, 0x66, 0x66, 0x66, 0x66, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 0xc6, 0x66, 0xce, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x66, 0xc6, 0x6e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 
0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 0x66, 0x6c, 0x65, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0x6c, 0x66, 0x65, 0xe5, 0xe5, 0xe5, 0xe5, 0xe6, 0x6c, 0x66, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x66, 0xc6, 0x69, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0xc6, 0x66, 0xce, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 
0xe5, 0xe5, 0xe6, 0x66, 0x66, 0x6c, 0x66, 0xe5, 0xe5, 0x6c, 0x66, 0x65, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xe6, 0x66, 0x66, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0x66, 0x6c, 0x6e, 0xe5, 0xe5, 0xe5, 0xee, 0xe6, 0x66, 0x6c, 0xee, 0xe5, 0xe6, 0x6c, 0x66, 0x66, 0x66, 0xe5, 0xee, 
0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 0xc6, 0x66, 0xce, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x66, 0xc6, 0x6e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 
0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 0x66, 0x6c, 0x65, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0x6c, 0x66, 0x65, 0xe5, 0xe5, 0xe5, 0xe5, 0xd6, 0x6c, 0x66, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x66, 0xc6, 0x69, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x59, 0xc6, 0x66, 0xce, 0x5e, 0x5e, 0x5e, 0x5e, 0x96, 0xc6, 0x66, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 
0xee, 0xe5, 0xe6, 0x66, 0x66, 0x66, 0x66, 0xe5, 0xee, 0x66, 0x66, 0x65, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe6, 0x66, 0x66, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xeb, 0x66, 0x66, 0x65, 0xe5, 0xee, 0xe5, 0xe5, 0x36, 0x66, 0x6c, 0xe5, 0xe5, 0xe6, 0x66, 0x66, 0x66, 0x66, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 0xc6, 0x66, 0xce, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x3e, 0x5e, 0x5e, 0x5e, 0x5e, 0xa6, 0x66, 0xcc, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 
0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 0x66, 0x6c, 0x65, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe6, 0x6c, 0x66, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0x66, 0x6c, 0x61, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x0e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x66, 0xc6, 0x6e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 
0xe5, 0xe5, 0xe6, 0x66, 0x66, 0x6c, 0x66, 0xe5, 0xe5, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x6b, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xe6, 0x66, 0x66, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xc6, 0x66, 0x6c, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0x66, 0x66, 0x65, 0xee, 0xe5, 0xe6, 0x6c, 0x66, 0x66, 0x66, 0xe5, 0xee, 
0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 0xf6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x9e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0xc6, 0x66, 0xc3, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0xc6, 0x66, 0xce, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 
0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 0xe6, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x65, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0x66, 0x6c, 0x65, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0x66, 0x6c, 0x65, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x0c, 0x8c, 0x8c, 0x8c, 0x8c, 0x8c, 0x8c, 0x8c, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0xce, 0x5e, 0x5e, 0x5e, 0x5e, 0x5c, 0x66, 0xc6, 0x6e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 
0xee, 0xe5, 0xe6, 0x66, 0x66, 0x66, 0x66, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xf6, 0x66, 0x66, 0x65, 0xe5, 0xe5, 0xee, 0xe5, 0xe6, 0x66, 0x66, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe6, 0x66, 0x66, 0x15, 0xe5, 0xe5, 0xee, 0xe5, 0xe6, 0x66, 0x66, 0x05, 0xe5, 0xe5, 0xe6, 0x66, 0x66, 0x66, 0x66, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0xc6, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 
0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0x66, 0x6c, 0x66, 0x65, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0x6c, 0x66, 0x6a, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xca, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x58, 0xc6, 0x66, 0xce, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0xb6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 
0xe5, 0xe5, 0xe6, 0x66, 0x66, 0x6c, 0x66, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0x66, 0x6c, 0x66, 0x15, 0xe5, 0xe5, 0xe6, 0x66, 0x66, 0xe5, 0xee, 0xe5, 0xe5, 0xe0, 0x66, 0x66, 0xa5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0x66, 0x66, 0x6b, 0xe5, 0xee, 0xe5, 0xe6, 0x6c, 0x66, 0x66, 0x66, 0xe5, 0xee, 
0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xce, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x66, 0xc6, 0x6e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 
0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xed, 0x66, 0x6c, 0x66, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xd5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xed, 0x6c, 0x66, 0x65, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0xa6, 0xc6, 0x66, 0xce, 0x5e, 0x56, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xce, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 
0xee, 0xe5, 0xe6, 0x66, 0x66, 0x66, 0x66, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe8, 0x66, 0x66, 0x66, 0xe5, 0xe6, 0x66, 0x66, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe6, 0x66, 0x66, 0x9e, 0xe5, 0xe5, 0xe5, 0xe6, 0x66, 0x66, 0x66, 0x66, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x66, 0xc6, 0x66, 0x8e, 0x56, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0xd6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 
0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x6a, 0xec, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0x66, 0x6c, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x66, 0xc6, 0x66, 0xd6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x66, 0xc6, 0x69, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 
0xe5, 0xe5, 0xe6, 0x66, 0x66, 0x6c, 0x66, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0x96, 0x66, 0x6c, 0x66, 0x66, 0x66, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe8, 0x6c, 0x66, 0x65, 0xe5, 0xe5, 0xee, 0xe5, 0xe6, 0x6c, 0x66, 0x66, 0x66, 0xe5, 0xee, 
0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5d, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0x0e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 
0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xac, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xfc, 0x66, 0x6c, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5c, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0xc6, 0x66, 0xc8, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0xf6, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 
0xee, 0xe5, 0xe6, 0x66, 0x66, 0x66, 0x66, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0x66, 0x66, 0x66, 0x66, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe8, 0x66, 0x66, 0x65, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0x66, 0x66, 0x66, 0x66, 0x66, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0xce, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x61, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x8e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 
0xe5, 0xe5, 0xec, 0x66, 0x6c, 0x66, 0x6c, 0x65, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0x6c, 0x66, 0xb5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe6, 0x6c, 0x66, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe8, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0xe5, 0xe5, 
0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0xc6, 0x64, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 
0xe5, 0xe5, 0xe6, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x05, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe9, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x65, 0xe5, 0xee, 
0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x60, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x56, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x6e, 0x5e, 0x5e, 
0xe5, 0xe5, 0xed, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x65, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x65, 0xe5, 0xee, 
0x2e, 0x5e, 0x5e, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0x9e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x86, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xde, 0x5e, 0x52, 
0x2e, 0xe5, 0xe5, 0xf6, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x6f, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xef, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0xee, 0xe5, 0xe2, 
0x2e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xce, 0x5e, 0x5e, 0x52, 
0x22, 0xe5, 0xe5, 0xe5, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x65, 0xe5, 0xe5, 0x22, 
0x22, 0x5e, 0x5e, 0x5e, 0x06, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x22, 
0x22, 0x25, 0xee, 0xe5, 0xec, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x6e, 0xe5, 0xe5, 0xe2, 0x22, 
0x22, 0x2e, 0x5e, 0x5e, 0x5e, 0x06, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0xe2, 0x22, 
0x22, 0x22, 0xe5, 0xe5, 0xe5, 0xe5, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x66, 0x6c, 0x65, 0xe5, 0xe5, 0xe5, 0x22, 0x22, 
0x22, 0x22, 0x2e, 0x5e, 0x5e, 0x5e, 0x56, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xce, 0x5e, 0x5e, 0x5e, 0x52, 0x22, 0x22, 
0x22, 0x22, 0x22, 0xe5, 0xee, 0xe5, 0xe5, 0xe8, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x61, 0xee, 0xe5, 0xe5, 0xe5, 0x22, 0x22, 0x22, 
0x22, 0x22, 0x22, 0x2e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x86, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x66, 0xc6, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x52, 0x22, 0x22, 0x22, 
0x22, 0x22, 0x22, 0x22, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0xee, 0x22, 0x22, 0x22, 0x22, 
0x22, 0x22, 0x22, 0x22, 0x22, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x22, 0x22, 0x22, 0x22, 0x22, 
0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0xe5, 0xee, 0xe5, 0xe5, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 
0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x2e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0xe2, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22
};

static const uint32_t palette_audio4BPP_UNCOMP[]=
{
	0xf5bb85, 	0xf4b479, 	0x000000, 	0xf7c493, 
	0xf9d3af, 	0xf08f36, 	0xffffff, 	0xfbe3cb, 
	0xfcecda, 	0xf29d52, 	0xfadbbd, 	0xf8cba0, 
	0xfdf3ea, 	0xf3ac6c, 	0xef8833, 	0xfbe4cf
};

Graphics_Image audio4BPP_UNCOMP=
{
	IMAGE_FMT_4BPP_UNCOMP,
	110,
	110,
	16,
	palette_audio4BPP_UNCOMP,
	pixel_audio4BPP_UNCOMP,
};

