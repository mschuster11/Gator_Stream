//###########################################################################
//
// FILE:   Main.c
//
// TITLE:  McBSP Loopback
//
//! \addtogroup cpu01_example_list
//! <h1>McBSP Loopback (mcbsp_loopback)</h1>
//!
//!  Three different serial word sizes can be tested.  Before compiling
//!  this project, select the serial word size of 8, 16 or 32 by using
//!  the \#define statements at the beginning of the code.
//!
//!  This example does not use interrupts.  Instead, a polling
//!  method is used to check the receive data.  The incoming
//!  data is checked for accuracy.  If an error is found the error()
//!  function is called and execution stops.
//!
//!  This program will execute until terminated by the user.
//!
//! \b 8-bit \b word \b example: \n
//!      The sent data looks like this:\n
//!      00 01 02 03 04 05 06 07 .... FE FF\n
//!
//! \b 16-bit \b word \b example: \n
//!      The sent data looks like this:\n
//!      0000 0001 0002 0003 0004 0005 0006 0007 .... FFFE FFFF\n
//!
//! \b 32-bit \b word \b example: \n
//!      The sent data looks like this: \n
//!      FFFF0000 FFFE0001 FFFD0002 .... 0000FFFF \n
//!
//! \b Watch \b Variables: \n
//! - sdata1 - Sent data word:     8 or 16-bit or low half of 32-bit
//! - sdata2 - Sent data word:     upper half of 32-bit
//! - rdata1 - Received data word: 8 or 16-bit or low half of 32-bit
//! - rdata2 - Received data word: upper half of 32-bit
//! - rdata1_point - Tracks last position in receive stream 1 for error
//!                  checking
//! - rdata2_point - Tracks last position in receive stream 2 for error
//!                  checking
//!
//! \note sdata2 and rdata2 are not used for 8-bit or 16-bit word size
//!
//
//###########################################################################
// $TI Release: F2837xD Support Library v3.05.00.00 $
// $Release Date: Tue Jun 26 03:15:23 CDT 2018 $
// $Copyright:
// Copyright (C) 2013-2018 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//###########################################################################

//
// Included Files
//
#include "F28x_Project.h"

//
// Defines
//
// Choose a word size.  Uncomment one of the following lines
//
//#define WORD_SIZE    8    // Run a loopback test in 8-bit mode
// #define WORD_SIZE 16      // Run a loopback test in 16-bit mode
 #define WORD_SIZE 32      // Run a loopback test in 32-bit mode

//
// Globals
//
Uint16 sdata1 = 0x000;    // Sent Data
Uint16 rdata1 = 0x000;    // Received Data
Uint16 sdata2 = 0x000;    // Sent Data
Uint16 rdata2 = 0x000;    // Received Data
Uint16 rdata1_point;
Uint16 rdata2_point;

Uint16 sinLut[512] = {
    0x8000,0x8192,0x8324,0x84b6,0x8647,0x87d9,0x896a,0x8afb,0x8c8b,0x8e1b,0x8fab,0x9139,0x92c7,0x9455,0x95e1,0x976d,
    0x98f8,0x9a82,0x9c0b,0x9d93,0x9f19,0xa09f,0xa223,0xa3a6,0xa527,0xa6a7,0xa826,0xa9a3,0xab1f,0xac98,0xae10,0xaf87,
    0xb0fb,0xb26e,0xb3de,0xb54d,0xb6b9,0xb824,0xb98c,0xbaf2,0xbc56,0xbdb7,0xbf17,0xc073,0xc1cd,0xc325,0xc47a,0xc5cc,
    0xc71c,0xc869,0xc9b3,0xcafb,0xcc3f,0xcd81,0xcebf,0xcffb,0xd133,0xd268,0xd39a,0xd4c9,0xd5f5,0xd71d,0xd842,0xd964,
    0xda82,0xdb9c,0xdcb3,0xddc7,0xded7,0xdfe3,0xe0eb,0xe1f0,0xe2f1,0xe3ee,0xe4e8,0xe5dd,0xe6cf,0xe7bc,0xe8a6,0xe98b,
    0xea6d,0xeb4a,0xec23,0xecf8,0xedc9,0xee96,0xef5e,0xf022,0xf0e2,0xf19d,0xf254,0xf307,0xf3b5,0xf45f,0xf504,0xf5a5,
    0xf641,0xf6d8,0xf76b,0xf7fa,0xf884,0xf909,0xf989,0xfa05,0xfa7c,0xfaee,0xfb5c,0xfbc5,0xfc29,0xfc88,0xfce3,0xfd39,
    0xfd89,0xfdd5,0xfe1d,0xfe5f,0xfe9c,0xfed5,0xff09,0xff37,0xff61,0xff86,0xffa6,0xffc1,0xffd8,0xffe9,0xfff5,0xfffd,
    0xffff,0xfffd,0xfff5,0xffe9,0xffd8,0xffc1,0xffa6,0xff86,0xff61,0xff37,0xff09,0xfed5,0xfe9c,0xfe5f,0xfe1d,0xfdd5,
    0xfd89,0xfd39,0xfce3,0xfc88,0xfc29,0xfbc5,0xfb5c,0xfaee,0xfa7c,0xfa05,0xf989,0xf909,0xf884,0xf7fa,0xf76b,0xf6d8,
    0xf641,0xf5a5,0xf504,0xf45f,0xf3b5,0xf307,0xf254,0xf19d,0xf0e2,0xf022,0xef5e,0xee96,0xedc9,0xecf8,0xec23,0xeb4a,
    0xea6d,0xe98b,0xe8a6,0xe7bc,0xe6cf,0xe5dd,0xe4e8,0xe3ee,0xe2f1,0xe1f0,0xe0eb,0xdfe3,0xded7,0xddc7,0xdcb3,0xdb9c,
    0xda82,0xd964,0xd842,0xd71d,0xd5f5,0xd4c9,0xd39a,0xd268,0xd133,0xcffb,0xcebf,0xcd81,0xcc3f,0xcafb,0xc9b3,0xc869,
    0xc71c,0xc5cc,0xc47a,0xc325,0xc1cd,0xc073,0xbf17,0xbdb7,0xbc56,0xbaf2,0xb98c,0xb824,0xb6b9,0xb54d,0xb3de,0xb26e,
    0xb0fb,0xaf87,0xae10,0xac98,0xab1f,0xa9a3,0xa826,0xa6a7,0xa527,0xa3a6,0xa223,0xa09f,0x9f19,0x9d93,0x9c0b,0x9a82,
    0x98f8,0x976d,0x95e1,0x9455,0x92c7,0x9139,0x8fab,0x8e1b,0x8c8b,0x8afb,0x896a,0x87d9,0x8647,0x84b6,0x8324,0x8192,
    0x8000,0x7e6d,0x7cdb,0x7b49,0x79b8,0x7826,0x7695,0x7504,0x7374,0x71e4,0x7054,0x6ec6,0x6d38,0x6baa,0x6a1e,0x6892,
    0x6707,0x657d,0x63f4,0x626c,0x60e6,0x5f60,0x5ddc,0x5c59,0x5ad8,0x5958,0x57d9,0x565c,0x54e0,0x5367,0x51ef,0x5078,
    0x4f04,0x4d91,0x4c21,0x4ab2,0x4946,0x47db,0x4673,0x450d,0x43a9,0x4248,0x40e8,0x3f8c,0x3e32,0x3cda,0x3b85,0x3a33,
    0x38e3,0x3796,0x364c,0x3504,0x33c0,0x327e,0x3140,0x3004,0x2ecc,0x2d97,0x2c65,0x2b36,0x2a0a,0x28e2,0x27bd,0x269b,
    0x257d,0x2463,0x234c,0x2238,0x2128,0x201c,0x1f14,0x1e0f,0x1d0e,0x1c11,0x1b17,0x1a22,0x1930,0x1843,0x1759,0x1674,
    0x1592,0x14b5,0x13dc,0x1307,0x1236,0x1169,0x10a1,0xfdd,0xf1d,0xe62,0xdab,0xcf8,0xc4a,0xba0,0xafb,0xa5a,
    0x9be,0x927,0x894,0x805,0x77b,0x6f6,0x676,0x5fa,0x583,0x511,0x4a3,0x43a,0x3d6,0x377,0x31c,0x2c6,
    0x276,0x22a,0x1e2,0x1a0,0x163,0x12a,0xf6,0xc8,0x9e,0x79,0x59,0x3e,0x27,0x16,0xa,0x2,
    0x0,0x2,0xa,0x16,0x27,0x3e,0x59,0x79,0x9e,0xc8,0xf6,0x12a,0x163,0x1a0,0x1e2,0x22a,
    0x276,0x2c6,0x31c,0x377,0x3d6,0x43a,0x4a3,0x511,0x583,0x5fa,0x676,0x6f6,0x77b,0x805,0x894,0x927,
    0x9be,0xa5a,0xafb,0xba0,0xc4a,0xcf8,0xdab,0xe62,0xf1d,0xfdd,0x10a1,0x1169,0x1236,0x1307,0x13dc,0x14b5,
    0x1592,0x1674,0x1759,0x1843,0x1930,0x1a22,0x1b17,0x1c11,0x1d0e,0x1e0f,0x1f14,0x201c,0x2128,0x2238,0x234c,0x2463,
    0x257d,0x269b,0x27bd,0x28e2,0x2a0a,0x2b36,0x2c65,0x2d97,0x2ecc,0x3004,0x3140,0x327e,0x33c0,0x3504,0x364c,0x3796,
    0x38e3,0x3a33,0x3b85,0x3cda,0x3e32,0x3f8c,0x40e8,0x4248,0x43a9,0x450d,0x4673,0x47db,0x4946,0x4ab2,0x4c21,0x4d91,
    0x4f04,0x5078,0x51ef,0x5367,0x54e0,0x565c,0x57d9,0x5958,0x5ad8,0x5c59,0x5ddc,0x5f60,0x60e6,0x626c,0x63f4,0x657d,
    0x6707,0x6892,0x6a1e,0x6baa,0x6d38,0x6ec6,0x7054,0x71e4,0x7374,0x7504,0x7695,0x7826,0x79b8,0x7b49,0x7cdb,0x7e6d
};
Uint16 i = 0x00;
Uint16 j = 0x00;
//
// Function Prototypes
//
void mcbsp_init(void);
void mcbsp_xmit(int a, int b);
void error(void);
interrupt void audio_ISR(void);

enum side{
    LEFT,
    RIGHT
}channel;
//
// Main
//
void main(void)
{
//
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the F2837xD_SysCtrl.c file.
//
   InitSysCtrl();

//
// Step 2. Initialize GPIO:
// This example function is found in the F2837xD_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// For this example, only enable the GPIO for McBSP-A
//
   InitMcbspbGpio();

//
// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
//
   DINT;

//
// Initialize PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the F2837xD_PieCtrl.c file.
//
   InitPieCtrl();

//
// Disable CPU interrupts and clear all CPU interrupt flags:
//
   IER = 0x0000;
   IFR = 0x0000;

//
// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in F2837xD_DefaultIsr.c.
// This function is found in F2837xD_PieVect.c.
//
   InitPieVectTable();

//
// Initialize and release peripheral (McBSP) from Reset.
//
   mcbsp_init();
   EALLOW;
   PieVectTable.MCBSPB_TX_INT = audio_ISR; //link it to my interrupt
   PieCtrlRegs.PIEIER6.bit.INTx8 = 1;
   IER |= M_INT6;
   EnableInterrupts();
   McbspbRegs.SPCR2.bit.XRST = 0;
   McbspbRegs.SPCR2.bit.XRST = 1;
   EDIS;
//
// Step 4. User specific code, enable interrupts:
//
    for(;;);
}

//
// error - Function to indicate error and halt debugger
//
void error(void)
{
   __asm("     ESTOP0");  // test failed!! Stop!
   for (;;);
}

//
// mcbsp_init - Initialize McBSP with DLB enabled and setup for specific
//                  loopback test bit mode
//
void mcbsp_init()
{
    EALLOW;
        McbspbRegs.SPCR2.all = 0; // Reset FS generator, sample rate generator & transmitter
        McbspbRegs.SPCR1.all = 0; // Reset Receiver, Right justify word
        McbspbRegs.SPCR1.bit.RJUST = 2; // left-justify word in DRR and zero-fill LSBs
        McbspbRegs.MFFINT.all=0x0; // Disable all interrupts
        McbspbRegs.SPCR1.bit.RINTM = 0; // McBSP interrupt flag - RRDY
        McbspbRegs.SPCR2.bit.XINTM = 0; // McBSP interrupt flag - XRDY
        // Clear Receive Control Registers
        McbspbRegs.RCR2.all = 0x0;
        McbspbRegs.RCR1.all = 0x0;
        // Clear Transmit Control Registers
        McbspbRegs.XCR2.all = 0x0;
        McbspbRegs.XCR1.all = 0x0;
        // Set Receive/Transmit to 32-bit operation
        McbspbRegs.RCR2.bit.RWDLEN2 = 2;
        McbspbRegs.RCR1.bit.RWDLEN1 = 2;
        McbspbRegs.XCR2.bit.XWDLEN2 = 2;
        McbspbRegs.XCR1.bit.XWDLEN1 = 2;
        McbspbRegs.RCR2.bit.RPHASE = 1; // Dual-phase frame for receive
        McbspbRegs.RCR1.bit.RFRLEN1 = 0; // Receive frame length = 1 word in phase 1
        McbspbRegs.RCR2.bit.RFRLEN2 = 0; // Receive frame length = 1 word in phase 2
        McbspbRegs.XCR2.bit.XPHASE = 1; // Dual-phase frame for transmit

        McbspbRegs.XCR1.bit.XFRLEN1 = 0; // Transmit frame length = 1 word in phase 1
        McbspbRegs.XCR2.bit.XFRLEN2 = 0; // Transmit frame length = 1 word in phase 2
        // I2S mode: R/XDATDLY = 1 always
        McbspbRegs.RCR2.bit.RDATDLY = 1;
        McbspbRegs.XCR2.bit.XDATDLY = 1;
        // Frame Width = 1 CLKG period, CLKGDV must be 1 as slave
        McbspbRegs.SRGR1.all = 0x0001;
        McbspbRegs.PCR.all = 0x0000;
        // Transmit frame synchronization is supplied by an external source via the FSX pin
        McbspbRegs.PCR.bit.FSXM = 0;
        // Receive frame synchronization is supplied by an external source via the FSR pin
        McbspbRegs.PCR.bit.FSRM = 0;
        // Select sample rate generator to be signal on MCLKR pin
        McbspbRegs.PCR.bit.SCLKME = 1;
        McbspbRegs.SRGR2.bit.CLKSM = 0;
        // Receive frame-synchronization pulses are active low - (L-channel first)
        McbspbRegs.PCR.bit.FSRP = 1;
        // Transmit frame-synchronization pulses are active low - (L-channel first)
        McbspbRegs.PCR.bit.FSXP = 1;
        // Receive data is sampled on the rising edge of MCLKR
        McbspbRegs.PCR.bit.CLKRP = 1;
        // Transmit data is sampled on the falling edge of CLKX
        McbspbRegs.PCR.bit.CLKXP = 0;
        // The transmitter gets its clock signal from MCLKX
        McbspbRegs.PCR.bit.CLKXM = 0;
        // The receiver gets its clock signal from MCLKR
        McbspbRegs.PCR.bit.CLKRM = 0;
        // Enable Receive Interrupt
        McbspbRegs.MFFINT.bit.XINT = 1;
        // Ignore unexpected frame sync
        //McbspbRegs.XCR2.bit.XFIG = 1;
        McbspbRegs.SPCR2.all |=0x00C0; // Frame sync & sample rate generators pulled out of reset
        delay_loop();
        McbspbRegs.SPCR2.bit.XRST=1; // Enable Transmitter
        McbspbRegs.SPCR1.bit.RRST=1; // Enable Receiver
        EDIS;
}

//
// mcbsp_xmit - Transmit data via McBPP
//
void mcbsp_xmit(int a, int b)
{
    McbspbRegs.DXR2.all=b;
    McbspbRegs.DXR1.all=a;
}


interrupt void audio_ISR(void)
{
    if (channel == LEFT)
    {
        McbspbRegs.DXR1.all = sinLut[i > 511 ? i=0 : i++];
    }
    else if (channel == RIGHT)
    {
        McbspbRegs.DXR1.all = sinLut[j > 511 ? j=0 : j++];
    }
    channel ^= 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;
}

//
// End of file
//
