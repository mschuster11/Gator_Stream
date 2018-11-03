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

#define CPU01TOCPU02_PASSMSG  0x0003FFF4     // CPU01 to CPU02 MSG RAM offsets
#define CPU02TOCPU01_PASSMSG  0x0003FBF4     // CPU02 to CPU01 MSG RAM offsets
//
// Globals
//

//
// At least 1 volatile global tIpcController instance is required when using
// IPC API Drivers.
//
volatile tIpcController g_sIpcController1;
volatile tIpcController g_sIpcController2;
enum buffer{
  FIRST,
  SECOND
}bufferNum = FIRST;
int16 leftVals1[2048];
int16 rightVals1[2048];
int16 leftVals2[2048];
int16 rightVals2[2048];
int16* buf[2];
Uint16 bufIndexL = 0x00;
Uint16 bufIndexR = 0x00;
bool_t sdRdy = FALSE;
bool_t fRdy = TRUE;
//
// Global variables used in this example to read/write data passed between
// CPU01 and CPU02
//
uint16_t leftSample;
uint16_t rightSample;
uint16_t activeEffect;
uint16_t count;

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
  uint32_t *pulMsgRam;

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
  pulMsgRam = (void *)CPU01TOCPU02_PASSMSG;
  pulMsgRam[0] = (uint32_t)&leftSample;
  pulMsgRam[1] = (uint32_t)&rightSample;
  pulMsgRam[2] = (uint32_t)&activeEffect;
  count = 0;

  initMMC();
  init_scib();
  init_scic();
  IpcRegs.IPCSET.bit.IPC17 = 1;
  f_unlink("/New_Song.wav");
  WaveFile* wf = wave_open("/New_Song.wav", "w");
  buf[0] = leftVals2;
  buf[1] = rightVals2;
  for(;;) {
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
// CPU02toCPU01IPC0IntHandler - Handles writes into CPU01 addresses as a
//                              result of read commands to the CPU02.
//
interrupt void CPU02toCPU01IPC0IntHandler (void) {
  tIpcMessage sMessage;

  //
  // Continue processing messages as long as CPU01 to CPU02
  // GetBuffer1 is full
  //
  while(IpcGet(&g_sIpcController1, &sMessage, DISABLE_BLOCKING) != STATUS_FAIL) {
    switch (sMessage.ulcommand) {
      case IPC_DATA_WRITE:
        if(sMessage.uladdress == (uint32_t)&leftSample){
          if (bufferNum == FIRST) {
            leftVals1[bufIndexL++] = sMessage.uldataw2;
          } else {
            leftVals2[bufIndexL++] = sMessage.uldataw2;
          }
        } else if(sMessage.uladdress == (uint32_t)&rightSample){
          if (bufferNum == FIRST) {
            rightVals1[bufIndexR++] = sMessage.uldataw2;
          } else {
            rightVals2[bufIndexR++] = sMessage.uldataw2;
          }
        }
        if (bufIndexR > 2047 && bufIndexL > 2047) {
          bufIndexL = 0;
          bufIndexR = 0;
          sdRdy = 1;
          if(bufferNum == FIRST) {
            buf[0] = leftVals1;
            buf[1] = rightVals1;
            bufferNum = SECOND;
          } else {
            buf[0] = leftVals2;
            buf[1] = rightVals2;
            bufferNum = FIRST;
          }
        } 
        break;
      default:
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
void scib_txChar (int a) {
  while (ScibRegs.SCICTL2.bit.TXRDY != 1);
  ScibRegs.SCITXBUF.all =a;
}


void scic_txChar (int a) {
  while (ScicRegs.SCICTL2.bit.TXRDY != 1);
  ScicRegs.SCITXBUF.all =a;
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

