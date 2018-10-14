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

/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~Includes-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */

#include "F28x_Project.h"
#include "F28x_Project.h"
#include "personal/headers/audio_effects.h"
#include "personal/headers/sd_utils.h"
#include "personal/headers/wav.h"
#include "personal/headers/wav_priv.h"

#define SYSCTL_PERIPH_SCI2              0x00010007
#define SYSCTL_PERIPH_SCI3              0x00020007
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~Globals~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
Uint16 sinLut1[512] = {
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
char g_uartRemoteRxBuf[100];
bool_t newRemoteCmd = FALSE;
static uint16_t uartRemoteRxBufIndex = 0;

char g_uartMspRxBuf[100];
bool_t newMspCmd = FALSE;
static uint16_t uartMspRxBufIndex = 0;
WaveFile* wf;

/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~Externs~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */

extern char g_cCwdBuf[CMD_BUF_SIZE];
extern char g_cCmdBuf[CMD_BUF_SIZE];


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-Function Prototypes-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */

interrupt void audio_ISR(void);
interrupt void remoteUartRx_ISR(void);
interrupt void mspUartRx_ISR(void);
void init_scib(void);
void init_scic(void);
void init_ints(void);
void scib_txChar(int a);
void scic_txChar(int a);


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~Function Definitions-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */

int main (void) {
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

  // Initialize and release peripheral (McBSP) from Reset.
  InitMcbspb();

  // Initialize the SD card interface and associated peripherals.
  initMMC();
  init_scib();
  init_scic();


  WaveFile* wf = wave_open("/New_Song.wav", "w");
  Uint16* buf[2];
  buf[0] = sinLut1;
  buf[1] = sinLut1;
  for(uint16_t j=0; j<86; j++) {
//      for(uint16_t i=0; i<512; i++, buf[0]++, buf[1]++)
          wave_write((void **)buf, 512, wf);
//      buf[0] = sinLut1;
//      buf[1] = sinLut1;
  }
  wave_close(wf);
  while(1) {
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


void init_ints (void) {
  EALLOW;

  // Enable the McBsp Tx (Audio Transmission) interrupt and point to its ISR (PIE: 6.1).
  PieVectTable.MCBSPB_TX_INT = audio_ISR;      
  PieCtrlRegs.PIEIER6.bit.INTx8 = 1;
  IER |= M_INT6;

  // Enable the SCI-B (MSP Comms) interrupt and point to its ISR (PIE: 8.5).
  PieVectTable.SCIC_RX_INT = mspUartRx_ISR;
  PieCtrlRegs.PIEIER8.bit.INTx5 = 1;
  IER |= M_INT8;

  // Enable the SCI-B (Remote Comms) interrupt and point to its ISR (PIE: 9.3).
  PieVectTable.SCIB_RX_INT = remoteUartRx_ISR;
  PieCtrlRegs.PIEIER9.bit.INTx3 = 1;
  IER |= M_INT9;

  EnableInterrupts();
  McbspbRegs.SPCR2.bit.XRST = 0;
  McbspbRegs.SPCR2.bit.XRST = 1;
  EDIS;
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
