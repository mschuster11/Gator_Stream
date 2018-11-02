//###########################################################################
//
// FILE:   Gator_Stream_DSP_CPU1.c
//
// TITLE:  CPU01 to CPU02 IPC Driver TestExample
//
//! \addtogroup dual_example_list
//! <h1> CPU01 to CPU02 IPC Driver </h1>
//!
//! This example tests all of the basic read/write CPU01 to CPU02 IPC Driver
//! functions available in F2837xD_Ipc_Driver.c.
//! The CPU01 project sends commands to the CPU02 project, which then processes
//! the commands.
//! The CPU02 project responds to the commands sent from the CPU01 project.
//! Note that IPC INT0 and IPC INT1 are used for this example to process IPC
//! commands.
//!
//! \b  Watch \b Variables \b for \b CPU01 : \n
//!  - ErrorCount - Counts # of errors
//!  - pusCPU01BufferPt - Stores 256 16-bit words block to write to CPU02
//!  - pusCPU02BufferPt - Points to beginning of 256 word block received
//!                       back from CPU02
//!  - usWWord16 - 16-bit word to write to CPU02
//!  - ulWWord32 - 32-bit word to write to CPU02
//!  - usRWord16 - 16-bit word to read from CPU02
//!  - ulRWord32 - 32-bit word to read from CPU02
//!
//! \b  Watch \b Variables \b for \b CPU02 : \n
//!  - ErrorFlag  - Indicates an unrecognized command was sent from
//!                 CPU01 to CPU02.
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
#include "F2837xD_Ipc_drivers.h"
#include "personal/headers/sd_utils.h"
#include "personal/headers/wav.h"
#include "personal/headers/wav_priv.h"
//
// Defines
//
#define CPU02TOCPU01_PASSMSG  0x0003FBF4     // CPU02 to CPU01 MSG RAM offsets
                                             // for passing address
#define SETMASK_16BIT         0xFF00         // Mask for setting bits of
                                             // 16-bit word
#define CLEARMASK_16BIT       0xA5A5         // Mask for clearing bits of
                                             // 16-bit word
#define SETMASK_32BIT         0xFFFF0000     // Mask for setting bits of
                                             // 32-bit word
#define CLEARMASK_32BIT       0xA5A5A5A5     // Mask for clearing bits of
                                             // 32-bit word
#define GS0SARAM_START        0xC000         // Start of GS0 SARAM

//
// Globals
//

//
// At least 1 volatile global tIpcController instance is required when using
// IPC API Drivers.
//
volatile tIpcController g_sIpcController1;
volatile tIpcController g_sIpcController2;

volatile uint16_t ErrorFlag;
volatile uint16_t ErrorCount;
int16 leftVals1[2048];
int16 rightVals1[2048];
int16 leftVals2[2048];
int16 rightVals2[2048];
int16* buf[2];
//
// Global variables used in this example to read/write data passed between
// CPU01 and CPU02
//
uint16_t usWWord16;
uint32_t ulWWord32;
uint16_t usRWord16;
uint32_t ulRWord32;
uint16_t usCPU01Buffer[256];
char g_uartRemoteRxBuf[100];
bool_t newRemoteCmd = FALSE;
static uint16_t uartRemoteRxBufIndex = 0;

char g_uartMspRxBuf[100];
bool_t newMspCmd = FALSE;
static uint16_t uartMspRxBufIndex = 0;

/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~Externs~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */

extern char g_cCwdBuf[CMD_BUF_SIZE];
extern char g_cCmdBuf[CMD_BUF_SIZE];
//
// Function Prototypes
//
void Error(void);
interrupt void CPU02toCPU01IPC0IntHandler(void);
interrupt void CPU02toCPU01IPC1IntHandler(void);
interrupt void remoteUartRx_ISR(void);
interrupt void mspUartRx_ISR(void);
void init_scib(void);
void init_scic(void);
void init_ints(void);
void scib_txChar(int a);
void scic_txChar(int a);


//
// Main
//
void main(void) {
  uint16_t counter;
  uint16_t *pusCPU01BufferPt;
  uint16_t *pusCPU02BufferPt;
  uint32_t *pulMsgRam ;

  // Initialize System Control.
  InitSysCtrl();

  // Initialize McBsp GPIO:
  InitMcbspbGpio();

  // Disable CPU interrupts.
  DINT;

  // Initialize PIE control registers to their default state.
  InitPieCtrl();

  // Disable CPU interrupts and clear all CPU interrupt flags:
  IER = 0x0000;
  IFR = 0x0000;

  // Initialize the PIE vector table with pointers to the shell Interrupt
  // Service Routines (ISR).
  InitPieVectTable();

  // Interrupts that are used in this example are re-mapped to
  // ISR functions found within this file.
  EALLOW;  // This is needed to write to EALLOW protected registers
  PieVectTable.IPC0_INT = &CPU02toCPU01IPC0IntHandler;
  PieVectTable.IPC1_INT = &CPU02toCPU01IPC1IntHandler;

  // Enable the SCI-B (MSP Comms) interrupt and point to its ISR (PIE: 8.5).
  PieVectTable.SCIC_RX_INT = mspUartRx_ISR;
  PieCtrlRegs.PIEIER8.bit.INTx5 = 1;
  IER |= M_INT8;

  // Enable the SCI-B (Remote Comms) interrupt and point to its ISR (PIE: 9.3).
  PieVectTable.SCIB_RX_INT = remoteUartRx_ISR;
  PieCtrlRegs.PIEIER9.bit.INTx3 = 1;
  IER |= M_INT9;
  EDIS;    // This is needed to disable write to EALLOW protected registers

#ifdef _STANDALONE
  //  Send boot command to allow the CPU02 application to begin execution
  IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_FLASH);
#endif

  // Step 4. Initialize the Device Peripherals:
  ErrorFlag = 0;

  IPCInitialize (&g_sIpcController1, IPC_INT0, IPC_INT0);
  IPCInitialize (&g_sIpcController2, IPC_INT1, IPC_INT1);

  // Enable CPU INT1 which is connected to Upper PIE IPC INT0-3:
  IER |= M_INT1;

  // Enable CPU2 to CPU1 IPC INTn in the PIE: Group 1 interrupts
  PieCtrlRegs.PIEIER1.bit.INTx13 = 1;    // CPU2 to CPU1 INT0
  PieCtrlRegs.PIEIER1.bit.INTx14 = 1;    // CPU2 to CPU1 INT1

  // Enable global Interrupts and higher priority real-time debug events:
  EINT;   // Enable Global interrupt INTM
  ERTM;   // Enable Global realtime interrupt DBGM

  // Initialize local variables
  pulMsgRam = (void *)CPU02TOCPU01_PASSMSG;
  pusCPU01BufferPt = (void *)GS0SARAM_START;
  pusCPU02BufferPt = (void *)(GS0SARAM_START + 256);
  ErrorCount = 0;

  // Initialize all variables used in example.
  for(counter = 0; counter < 256; counter++) {
    usCPU01Buffer[counter] = ((counter<<8)+(~counter));
  }

  usWWord16 = 0x1234;
  ulWWord32 = 0xABCD5678;
  usRWord16 = 0;
  ulRWord32 = 0;

  // Spin here until CPU02 has written variable addresses to pulMsgRam
  IpcRegs.IPCSET.bit.IPC17 = 1;
  initMMC();
  init_scib();
  init_scic();

  f_unlink("/New_Song.wav");
  WaveFile* wf = wave_open("/New_Song.wav", "w");
  while(1) {
    if(fRdy && sdRdy) {
      if(count < 3024){
        sdRdy = FALSE;
        wave_write((void **)buf, 2048, wf);
        count++;
      } else {
        fRdy = FALSE;
        wave_close(wf);
      }
    }
    if(newRemoteCmd == TRUE) {
      UARTprintf("%s\n", g_uartRemoteRxBuf);
      scic_txChar('i');
      scic_txChar('n');
      scic_txChar('q');
      scic_txChar('u');
      scic_txChar('i');
      scic_txChar('r');
      scic_txChar('y');
      scic_txChar('\r');
      scic_txChar('\n');
      newRemoteCmd = FALSE;
    }
    if(newMspCmd == TRUE) {
      UARTprintf("%s\n", g_uartMspRxBuf);
      newMspCmd = FALSE;
    }
  }
}

//
// 16 and 32-bit Data Writes
// Write 16-bit word to CPU02 16-bit write word variable.
//
    IPCLtoRDataWrite(&g_sIpcController1, pulMsgRam[0],(uint32_t)usWWord16,
                     IPC_LENGTH_16_BITS, ENABLE_BLOCKING,NO_FLAG);

//
// Read 16-bit word from CPU02 16-bit write word variable. Use IPC Flag 17 to
// check when read data is ready.
//
    IPCLtoRDataRead(&g_sIpcController1, pulMsgRam[0], &usRWord16,
                    IPC_LENGTH_16_BITS, ENABLE_BLOCKING,
                    IPC_FLAG17);

//
// Write 32-bit word to CPU02 32-bit write word variable.
//
    IPCLtoRDataWrite(&g_sIpcController1, pulMsgRam[1],ulWWord32,
                     IPC_LENGTH_32_BITS, ENABLE_BLOCKING,
                     NO_FLAG);

//
// Read 32-bit word from CPU02 32-bit write word variable. Use IPC Flag 18 to
// check when read data is ready.
//
    IPCLtoRDataRead(&g_sIpcController1, pulMsgRam[1], &ulRWord32,
                    IPC_LENGTH_32_BITS, ENABLE_BLOCKING,
                    IPC_FLAG18);

//
// Wait until read variables are ready (by checking IPC Response Flag is
// cleared). Then check Read var = Write var
//
    while(IpcRegs.IPCFLG.bit.IPC17) {
    }

    if(usWWord16 != usRWord16) {
        ErrorCount++;
    }

    while(IpcRegs.IPCFLG.bit.IPC18) {
    }

    if(ulWWord32 != ulRWord32) {
        ErrorCount++;
    }

//
// 16 and 32-bit Data Set Bits
// Set upper 8 bits in 16-bit write word variable location.
//
    IPCLtoRSetBits(&g_sIpcController1, pulMsgRam[0],(uint32_t)SETMASK_16BIT,
                   IPC_LENGTH_16_BITS,ENABLE_BLOCKING);

//
// Read 16-bit word from CPU02 16-bit write word variable. Use IPC Flag 17 to
// check when read data is ready.
//
    IPCLtoRDataRead(&g_sIpcController1, pulMsgRam[0], &usRWord16,
                    IPC_LENGTH_16_BITS, ENABLE_BLOCKING,IPC_FLAG17);

//
// Set upper 16 bits in 32-bit write word variable location.
//
    IPCLtoRSetBits(&g_sIpcController1, pulMsgRam[1], SETMASK_32BIT,
                   IPC_LENGTH_32_BITS,ENABLE_BLOCKING);

//
// Read 32-bit word from CPU02 32-bit write word variable. Use IPC Flag 18 to
// check when read data is ready.
//
    IPCLtoRDataRead(&g_sIpcController1, pulMsgRam[1], &ulRWord32,
                    IPC_LENGTH_32_BITS, ENABLE_BLOCKING,IPC_FLAG18);

//
// Wait until read variables are ready (by checking IPC Response Flag is
// cleared). Then check correct bits are set.
//
    while(IpcRegs.IPCFLG.bit.IPC17) {
    }

    if(usRWord16 != (usWWord16 | SETMASK_16BIT)) {
        ErrorCount++;
    }

    while(IpcRegs.IPCFLG.bit.IPC18) {
    }

    if(ulRWord32 != (ulWWord32 | SETMASK_32BIT)) {
        ErrorCount++;
    }

//
// 16 and 32-bit Data Clear Bits
// Clear alternating bits in 16-bit write word variable location
//
    IPCLtoRClearBits(&g_sIpcController1, pulMsgRam[0],(uint32_t)CLEARMASK_16BIT,
                     IPC_LENGTH_16_BITS,ENABLE_BLOCKING);

//
// Read 16-bit word from CPU02 16-bit write word variable. Use IPC Flag 17 to
// check when read data is ready.
//
    IPCLtoRDataRead(&g_sIpcController1, pulMsgRam[0], &usRWord16,
                    IPC_LENGTH_16_BITS, ENABLE_BLOCKING,IPC_FLAG17);

//
// Clear alternating bits in 32-bit write word variable location
//
    IPCLtoRClearBits(&g_sIpcController1, pulMsgRam[1],(uint32_t)CLEARMASK_32BIT,
                     IPC_LENGTH_32_BITS,ENABLE_BLOCKING);

//
// Read 16-bit word from CPU02 32-bit write word variable. Use IPC Flag 18 to
// check when read data is ready.
//
    IPCLtoRDataRead(&g_sIpcController1, pulMsgRam[1], &ulRWord32,
                    IPC_LENGTH_32_BITS, ENABLE_BLOCKING,IPC_FLAG18);

//
// Wait until read variables are ready (by checking IPC Response Flag is
// cleared). Then check correct bits are clear.
//
    while(IpcRegs.IPCFLG.bit.IPC17) {
    }

    if(usRWord16 != ((usWWord16 | SETMASK_16BIT) & (~CLEARMASK_16BIT))) {
        ErrorCount++;
    }

    while(IpcRegs.IPCFLG.bit.IPC18) {
    }

    if(ulRWord32  != ((ulWWord32 | SETMASK_32BIT) & (~CLEARMASK_32BIT))) {
        ErrorCount++;
    }

//
// Data Block Writes
//

//
// Request Memory Access to GS0 SARAM for CPU01
// Clear bits to let CPU01 own GS0
//
    if((MemCfgRegs.GSxMSEL.bit.MSEL_GS0) == 1) {
        EALLOW;
        MemCfgRegs.GSxMSEL.bit.MSEL_GS0 = 0;
        EDIS;
    }

//
// Write a block of data from CPU01 to GS0 shared RAM which is then written to
// an CPU02 address.
//
    for(counter = 0; counter < 256; counter++) {
        pusCPU01BufferPt[counter] = usCPU01Buffer[counter];
    }

    IPCLtoRBlockWrite(&g_sIpcController2, pulMsgRam[2],
                      (uint32_t)pusCPU01BufferPt, 256,
                      IPC_LENGTH_16_BITS,ENABLE_BLOCKING);

//
// Give Memory Access to GS0 SARAM to CPU02
//
    while(!(MemCfgRegs.GSxMSEL.bit.MSEL_GS0)) {
        EALLOW;
        MemCfgRegs.GSxMSEL.bit.MSEL_GS0 = 1;
        EDIS;
    }

//
// Read data back from CPU02.
//
    IPCLtoRBlockRead(&g_sIpcController2, pulMsgRam[2],
                     (uint32_t)pusCPU02BufferPt, 256,
                     ENABLE_BLOCKING,IPC_FLAG17);

//
// Wait until read data is ready (by checking IPC Response Flag is cleared).
// Then check for correct data.
//
    while(IpcRegs.IPCFLG.bit.IPC17) {
    }

    for(counter = 0; counter <256; counter++) {
        if(usCPU01Buffer[counter] != pusCPU01BufferPt[counter]) {
            ErrorFlag = 1;
        }
    }

    if (ErrorFlag == 1) {
        ErrorCount++;
    }

//
// Check Function Call Function
//

//
// Call FunctionCall() function on CPU02 with a dummy parameter of "0"(i.e. no
// parameter).
//
    IPCLtoRFunctionCall(&g_sIpcController1, pulMsgRam[3], 0, ENABLE_BLOCKING);

//
// Read status variable to check if function was entered. Use IPC Flag 17 to
// check when read data is ready.
//
    IPCLtoRDataRead(&g_sIpcController1, pulMsgRam[5], &usRWord16,
                    IPC_LENGTH_16_BITS, ENABLE_BLOCKING,
                    IPC_FLAG17);

//
// Call FunctionCall() function on CPU02 with a parameter of "0x12345678".
//
    IPCLtoRFunctionCall(&g_sIpcController1, pulMsgRam[4], 0x12345678,
                        ENABLE_BLOCKING);

//
// Read status variable to check if function was entered. Use IPC Flag 18 to
// check when read data is ready.
//
    IPCLtoRDataRead(&g_sIpcController1, pulMsgRam[5], &ulRWord32,
                    IPC_LENGTH_32_BITS, ENABLE_BLOCKING,
                    IPC_FLAG18);

//
// Wait until read data is ready (by checking IPC Response Flag is cleared).
// Then check status variables to see if function was entered.
//
    while(IpcRegs.IPCFLG.bit.IPC17) {
    }

    if(usRWord16 != 1) {
        ErrorCount++;
    }

    while(IpcRegs.IPCFLG.bit.IPC18) {
    }

    if(ulRWord32 != 0x12345678) {
        ErrorCount++;
    }

    if(ErrorCount != 0) {
        ESTOP0;
    }

    for(;;) {
        //
        // When Complete, Loop Forever here.
        //
    }
}

//
// CPU02toCPU01IPC0IntHandler - Handles writes into CPU01 addresses as a
//                              result of read commands to the CPU02.
//
interrupt void CPU02toCPU01IPC0IntHandler (void) {
    tIpcMessage sMessage;

    //
    // Continue processing messages as long as CPU01 to CPU02
    // GetBuffer1 is full
    //
    while(IpcGet(&g_sIpcController1, &sMessage,
                 DISABLE_BLOCKING) != STATUS_FAIL) {
        switch (sMessage.ulcommand) {
            case IPC_DATA_WRITE:
                IPCRtoLDataWrite(&sMessage);
                break;
            default:
                ErrorFlag = 1;
                break;
        }
    }

    //
    // Acknowledge IPC INT0 Flag and PIE to receive more interrupts
    //
    IpcRegs.IPCACK.bit.IPC0 = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

//
// CPU02toCPU01IPC1IntHandler - Should never reach this ISR. This is an
//                              optional placeholder for g_sIpcController2.
//
interrupt void CPU02toCPU01IPC1IntHandler (void) {
    //
    // Should never reach here - Placeholder for Debug
    //
    // Acknowledge IPC INT1 Flag and PIE to receive more interrupts
    //
    IpcRegs.IPCACK.bit.IPC1 = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

//
// End of file
//
interrupt void remoteUartRx_ISR (void) {
  while(ScibRegs.SCIFFRX.bit.RXFFST > 0) {
    g_uartRemoteRxBuf[uartRemoteRxBufIndex++] = ScibRegs.SCIRXBUF.all;
    if(g_uartRemoteRxBuf[uartRemoteRxBufIndex-1] == 0x00){
        newRemoteCmd = TRUE;
    }
  }
  if(newRemoteCmd || uartRemoteRxBufIndex >= 100) {
      uartRemoteRxBufIndex = 0;
  }
  ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;
}

interrupt void mspUartRx_ISR (void) {
  while(ScicRegs.SCIFFRX.bit.RXFFST > 0) {
    g_uartMspRxBuf[uartMspRxBufIndex++] = ScicRegs.SCIRXBUF.all;
    if(g_uartMspRxBuf[uartMspRxBufIndex-2] == '\r' && g_uartMspRxBuf[uartMspRxBufIndex-1] == '\n'){
        newMspCmd = TRUE;
    }
  }
  if(newMspCmd || uartMspRxBufIndex >= 100) {
      uartMspRxBufIndex =0;
  }
  ScicRegs.SCIFFRX.bit.RXFFINTCLR = 1;
}


void init_scib (void) {
  // Assign GPIO19 to CPU-1 as the RX of SCI-B.
  GPIO_SetupPinMux(19, GPIO_MUX_CPU1, 2);
  GPIO_SetupPinOptions(19, GPIO_INPUT, GPIO_PUSHPULL);

  // Assign GPIO18 to CPU-1 as the TX of SCI-B.
  GPIO_SetupPinMux(18, GPIO_MUX_CPU1, 2);
  GPIO_SetupPinOptions(18, GPIO_OUTPUT, GPIO_ASYNC);

  ScibRegs.SCICCR.all = 0x0007;         // 1 stop bit,  No loopback
                                        // No parity,8 char bits,
                                        // async mode, idle-line protocol
  ScibRegs.SCICTL1.all = 0x0003;        // enable TX, RX, internal SCICLK,
                                        // Disable RX ERR, SLEEP, TXWAKE
  ScibRegs.SCICTL2.all = 0x0003;        // Enable the interrupts for both TX and RX. 

  // SCIA at 9600 baud
  ScibRegs.SCIHBAUD.all = 0x0002;
  ScibRegs.SCILBAUD.all = 0x008B;
  ScibRegs.SCIFFRX.all  = 0x2044;       // Relinquish RX FIFO from Reset,
                                        // enable the interrupt with priority 4.
  ScibRegs.SCIFFTX.bit.SCIFFENA = 1;    // Enable the FIFO.
  ScibRegs.SCIFFRX.bit.RXFFIENA = 1;    // Enable the RX FIFO's interrup.

  // Relinquish SCI from Reset
  ScibRegs.SCICTL1.all = 0x0023;  

  // Enable SCI-C's peripheral clock.
//  SysCtlPeripheralEnable(SYSCTL_PERIPH_SCI2);
}


void init_scic () {
  // Assign GPIO139 to CPU-1 as the RX of SCI-C.
  GPIO_SetupPinMux(139, GPIO_MUX_CPU1, 6);
  GPIO_SetupPinOptions(139, GPIO_INPUT, GPIO_PUSHPULL);

  // Assign GPIO56 to CPU-1 as the TX of SCI-C.
  GPIO_SetupPinMux(56, GPIO_MUX_CPU1, 6);
  GPIO_SetupPinOptions(56, GPIO_OUTPUT, GPIO_ASYNC);

  ScicRegs.SCICCR.all = 0x0007;         // 1 stop bit,  No loopback
                                        // No parity,8 char bits,
                                        // async mode, idle-line protocol
  ScicRegs.SCICTL1.all = 0x0003;        // enable TX, RX, internal SCICLK,
                                        // Disable RX ERR, SLEEP, TXWAKE
  ScicRegs.SCICTL2.all = 0x0003;        // Enable the interrupts for both TX and RX. 

  // SCIC at 9600 baud
  ScicRegs.SCIHBAUD.all = 0x0002;
  ScicRegs.SCILBAUD.all = 0x008B;
  ScicRegs.SCIFFRX.all  = 0x2044;       // Relinquish RX FIFO from Reset,
                                        // enable the interrupt with priority 4.
  ScicRegs.SCIFFTX.bit.SCIFFENA = 1;    // Enable the FIFO.
  ScicRegs.SCIFFRX.bit.RXFFIENA = 1;    // Enable the RX FIFO's interrup.

  // Relinquish SCI from Reset
  ScicRegs.SCICTL1.all = 0x0023;  

  // Enable SCI-C's peripheral clock.
//  SysCtlPeripheralEnable(SYSCTL_PERIPH_SCI3);
}

