//###########################################################################
// FILE:   Gator_Stream_DSP_CPU1.c
//###########################################################################
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
//###########################################################################

//
// Included Files
//
#include "F28x_Project.h"
#include "F2837xD_Ipc_drivers.h"
#include "personal/headers/sd_utils.h"
#include "personal/headers/wav.h"
#include "personal/headers/wav_priv.h"
#include "personal/headers/cpu1_ipc.h"
#include "personal/headers/sci_utils.h"
#include "stdlib.h"
// #include "string.h"

// Defines
#define CPU01TOCPU02_PASSMSG  0x0003FFF4     // CPU01 to CPU02 MSG RAM offsets
#define CPU02TOCPU01_PASSMSG  0x0003FBF4     // CPU02 to CPU01 MSG RAM offsets

/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~Globals~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
// At least 1 volatile global tIpcController instance is required when using
// IPC API Drivers.
volatile tIpcController g_sIpcController1;
volatile tIpcController g_sIpcController2;
extern int16 leftSamplesBuffer1[MMC_SAMPLE_PING_PONG_BUFF_SIZE];
extern int16 rightSamplesBuffer1[MMC_SAMPLE_PING_PONG_BUFF_SIZE];
extern int16 leftSamplesBuffer2[MMC_SAMPLE_PING_PONG_BUFF_SIZE];
extern int16 rightSamplesBuffer2[MMC_SAMPLE_PING_PONG_BUFF_SIZE];
extern int16* mmcSampleBuffer[2];
extern bool_t mmcReady;
extern bool_t fileReady;
extern char uartRemoteRxBuf[100];
extern bool_t newRemoteCmd;
extern char uartMspRxBuf[100];
extern bool_t newMspCmd;

//
// Global variables used in this example to read/write data passed between
// CPU01 and CPU02
//
uint16_t leftSample;
uint16_t rightSample;
uint16_t activeEffect;
uint16_t count;
extern uint16_t uartMspRxBufIndex ;


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~Externs~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
extern char g_cCwdBuf[CMD_BUF_SIZE];
extern char g_cCmdBuf[CMD_BUF_SIZE];
extern node* currentMspUartCmd;
extern node* newMspUartCmd;

/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-Prototypes~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
void init_ints(void);
void printString(char* s);
void sendMspString(char* s);
static bool_t simple_strcmp(char* s1, char* s2, uint16_t n);
//
// Main
//
void main(void) {
  uint32_t *crossCoreMemory;

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

  // Enable the SCI-B (Remote Comms) interrupt and point to its ISR (PIE: 9.3).
  PieVectTable.SCIB_RX_INT = remoteUartRx_ISR;
  PieCtrlRegs.PIEIER9.bit.INTx3 = 1;
  IER |= M_INT9;

#ifndef CUSTOM_HW
  // Enable the SCI-C (MSP Comms) interrupt and point to its ISR (PIE: 8.5).
  PieVectTable.SCIC_RX_INT = mspUartRx_ISR;
  PieCtrlRegs.PIEIER8.bit.INTx5 = 1;
  IER |= M_INT8;
#else
  // Enable the SCI-C (MSP Comms) interrupt and point to its ISR (PIE: 8.5).
  PieVectTable.SCIA_RX_INT = mspUartRx_ISR;
  PieCtrlRegs.PIEIER9.bit.INTx1 = 1;
#endif
  EDIS;    // This is needed to disable write to EALLOW protected registers


#ifdef _STANDALONE
  //  Send boot command to allow the CPU02 application to begin execution
  IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_FLASH);
#endif

  // Step 4. Initialize the Device Peripherals:
  IPCInitialize(&g_sIpcController1, IPC_INT0, IPC_INT0);
  IPCInitialize(&g_sIpcController2, IPC_INT1, IPC_INT1);

  // Enable CPU INT1 which is connected to Upper PIE IPC INT0-3:
  IER |= M_INT1;

  // Enable CPU2 to CPU1 IPC INTn in the PIE: Group 1 interrupts
  PieCtrlRegs.PIEIER1.bit.INTx13 = 1;    // CPU2 to CPU1 INT0
  PieCtrlRegs.PIEIER1.bit.INTx14 = 1;    // CPU2 to CPU1 INT1

  // Enable global Interrupts and higher priority real-time debug events:
  EINT;   // Enable Global interrupt INTM
  ERTM;   // Enable Global realtime interrupt DBGM

  // Initialize local variables
  crossCoreMemory = (void *)CPU01TOCPU02_PASSMSG;
  crossCoreMemory[0] = (uint32_t)&leftSample;
  crossCoreMemory[1] = (uint32_t)&rightSample;
  crossCoreMemory[2] = (uint32_t)&activeEffect;
  count = 0;
#ifdef CUSTOM_HW
  // Assign GPIO19 to CPU-1 as the RX of SCI-B.
  GPIO_SetupPinMux(75, GPIO_MUX_CPU1, 0);
  GPIO_SetupPinOptions(75, GPIO_OUTPUT, GPIO_OUTPUT);
  GPIO_WritePin(75, 0);
  GPIO_WritePin(75, 1);
#endif
  initMMC();
  init_scib();
#ifndef CUSTOM_HW
  init_scic();
#else
  init_scia();  
#endif
  IpcRegs.IPCSET.bit.IPC17 = 1;
  IpcRegs.IPCSET.bit.IPC11 = 1;
  f_unlink("/New_Song.wav");
  WaveFile* wf = wave_open("/New_Song.wav", "w");
  mmcSampleBuffer[0] = leftSamplesBuffer2;
  mmcSampleBuffer[1] = rightSamplesBuffer2;
  for(;;) {
    if(fileReady && mmcReady) {
      if(count < 200){
        mmcReady = FALSE;
        wave_write((void **)mmcSampleBuffer, MMC_SAMPLE_PING_PONG_BUFF_SIZE, wf);
        count++;
      } else {
        fileReady = FALSE;
        wave_close(wf);
      }
    }

    if(newRemoteCmd == TRUE) {
      // UARTprintf("%s\n", uartRemoteRxBuf);
      printString(uartRemoteRxBuf);
      if(uartRemoteRxBuf[0] == 'C' && uartRemoteRxBuf[1] == 'S'){
       sendMspString("CS\r\n");
      } else if(uartRemoteRxBuf[0] == 'O' && uartRemoteRxBuf[1] == 'S'){
       sendMspString(uartRemoteRxBuf);
      } else if(simple_strcmp(uartRemoteRxBuf, "EFFECT: ", 8)) {
        IPCLtoRDataWrite(&g_sIpcController1, crossCoreMemory[2],uartRemoteRxBuf[8], IPC_LENGTH_16_BITS, ENABLE_BLOCKING,NO_FLAG);
      } else if(simple_strcmp(uartRemoteRxBuf, "AUDIO: ", 7)) {
        sendMspString((uartRemoteRxBuf+7));
      }
      newRemoteCmd = FALSE;
    }

    while(currentMspUartCmd != NULL){
      node* temp = currentMspUartCmd;
      printString(currentMspUartCmd->uartString);
      currentMspUartCmd = currentMspUartCmd->next;
      free(temp);
    }
  }
}



#ifndef CUSTOM_HW
void printString(char* s) {
  for(uint16_t i=0;s[i]!=NULL;i++)
    scia_txChar(s[i]);
}

void sendMspString(char* s) {
  for(uint16_t i=0;s[i]!=NULL;i++)
    scic_txChar(s[i]);
  scic_txChar('\0');
}
#else
void printString(char* s) {
  for(uint16_t i=0;s[i]!=NULL;i++)
    scic_txChar(s[i]);
}

void sendMspString(char* s) {
  for(uint16_t i=0;s[i]!=NULL;i++)
    scia_txChar(s[i]);
  scia_txChar('\0');
}
#endif

static bool_t simple_strcmp(char* s1, char* s2, uint16_t n) {
  for(uint16_t i=0;i<n;i++)
    if(s1[i] != s2[i])
      return FALSE;
  return TRUE;
}
