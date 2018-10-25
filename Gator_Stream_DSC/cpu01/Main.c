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
Uint16 sinLut1[100] = {
                       0x0000,
                       0x0809, 0x100a, 0x17fb, 0x1fd4, 0x278d, 0x2f1e, 0x367f, 0x3da9, 0x4495, 0x4b3b,
                       0x5196, 0x579e, 0x5d4e, 0x629f, 0x678d, 0x6c12, 0x7029, 0x73d0, 0x7701, 0x79bb,
                       0x7bf9, 0x7dba, 0x7efc, 0x7fbe, 0x7fff, 0x7fbe, 0x7efc, 0x7dba, 0x7bf9, 0x79bb,
                       0x7701, 0x73d0, 0x7029, 0x6c12, 0x678d, 0x629f, 0x5d4e, 0x579e, 0x5196, 0x4b3b,
                       0x4495, 0x3da9, 0x367f, 0x2f1e, 0x278d, 0x1fd4, 0x17fb, 0x100a, 0x0809, 0x0000,
                       0xf7f7, 0xeff6, 0xe805, 0xe02c, 0xd873, 0xd0e2, 0xc981, 0xc257, 0xbb6b, 0xb4c5,
                       0xae6a, 0xa862, 0xa2b2, 0x9d61, 0x9873, 0x93ee, 0x8fd7, 0x8c30, 0x88ff, 0x8645,
                       0x8407, 0x8246, 0x8104, 0x8042, 0x8001, 0x8042, 0x8104, 0x8246, 0x8407, 0x8645,
                       0x88ff, 0x8c30, 0x8fd7, 0x93ee, 0x9873, 0x9d61, 0xa2b2, 0xa862, 0xae6a, 0xb4c5,
                       0xbb6b, 0xc257, 0xc981, 0xd0e2, 0xd873, 0xe02c, 0xe805, 0xeff6, 0xf7f7
};
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

  // Enable and assign ISRs.
  init_ints();

  // Initialize and release peripheral (McBSP) from Reset.
  InitMcbspb();

  // Initialize the SD card interface and associated peripherals.
  initMMC();
  init_scib();
  init_scic();

  f_unlink("/New_Song.wav");
  WaveFile* wf = wave_open("/New_Song.wav", "w");
  Uint16* buf[2];
  buf[0] = sinLut1;
  buf[1] = sinLut1;
  for(uint16_t j=0; j<4410; j++) {
    wave_write((void **)buf, 100, wf);
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
  PieVectTable.MCBSPB_RX_INT = audio_ISR;
  PieCtrlRegs.PIEIER6.bit.INTx7 = 1;

  // Enable the McBsp Tx (Audio Transmission) interrupt and point to its ISR (PIE: 6.1).
//  PieVectTable.MCBSPB_TX_INT = audio_ISR;
//  PieCtrlRegs.PIEIER6.bit.INTx8 = 1;
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
