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
// test
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~Includes-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */

#include "F28x_Project.h"
#include "personal/headers/audio_effects.h"
#include "personal/headers/sd_utils.h"
#include "personal/headers/wav.h"


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~Globals~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */

char g_uartRemoteRxBuf[100];
bool_t newRemoteCmd = FALSE;
static uint16_t uartRemoteRxBufIndex = 0;

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
void scia_txChar(int a);
char scia_rxChar(void);


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~Function Definitions-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */

int main(void) {
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the F2837xD_SysCtrl.c file.
  InitSysCtrl();

// Step 2. Initialize GPIO:
// This example function is found in the F2837xD_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// For this example, only enable the GPIO for McBSP-A.
  InitMcbspbGpio();

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
  DINT;

// Initialize PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the F2837xD_PieCtrl.c file.
  InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
  IER = 0x0000;
  IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in F2837xD_DefaultIsr.c.
// This function is found in F2837xD_PieVect.c.
  InitPieVectTable();


// Initialize and release peripheral (McBSP) from Reset.
  InitMcbspb();
  EALLOW;
  PieVectTable.MCBSPB_TX_INT = audio_ISR;      //link it to my interrupt
  PieCtrlRegs.PIEIER6.bit.INTx8 = 1;
  IER |= M_INT6;

  PieVectTable.SCIB_RX_INT = remoteUartRx_ISR; //link it to my interrupt
  PieCtrlRegs.PIEIER9.bit.INTx3 = 1;
  IER |= M_INT9;

  EnableInterrupts();
  McbspbRegs.SPCR2.bit.XRST = 0;
  McbspbRegs.SPCR2.bit.XRST = 1;
  EDIS;

  initMMC();

//  SysCtlPeripheralEnable(SYSCTL_PERIPH_SCI2);

  GPIO_SetupPinMux(19, GPIO_MUX_CPU1, 2);
  GPIO_SetupPinOptions(19, GPIO_INPUT, GPIO_PUSHPULL);
  GPIO_SetupPinMux(18, GPIO_MUX_CPU1, 2);
  GPIO_SetupPinOptions(18, GPIO_OUTPUT, GPIO_ASYNC);

  ScibRegs.SCICCR.all = 0x0007;   // 1 stop bit,  No loopback
                                  // No parity,8 char bits,
                                  // async mode, idle-line protocol
  ScibRegs.SCICTL1.all = 0x0003;  // enable TX, RX, internal SCICLK,
                                  // Disable RX ERR, SLEEP, TXWAKE
  ScibRegs.SCICTL2.all = 0x0003;
  ScibRegs.SCICTL2.bit.TXINTENA = 1;
  ScibRegs.SCICTL2.bit.RXBKINTENA = 1;

  //
  // SCIA at 9600 baud
  // @LSPCLK = 50 MHz (200 MHz SYSCLK) HBAUD = 0x02 and LBAUD = 0x8B.
  // @LSPCLK = 30 MHz (120 MHz SYSCLK) HBAUD = 0x01 and LBAUD = 0x86.
  //
  ScibRegs.SCIHBAUD.all = 0x0002;
  ScibRegs.SCILBAUD.all = 0x008B;
  ScibRegs.SCIFFRX.all  = 0x2044;
  ScibRegs.SCIFFTX.bit.SCIFFENA = 1;
  ScibRegs.SCIFFRX.bit.RXFFIENA = 1;
  ScibRegs.SCIFFRX.bit.RXFFIL = 0b0001;
//      SciaRegs.SCIFFCT.all = 0x0;

  ScibRegs.SCICTL1.all = 0x0023;  // Relinquish SCI from Reset

  GPIO_SetupPinMux(139, GPIO_MUX_CPU1, 6);
  GPIO_SetupPinOptions(139, GPIO_INPUT, GPIO_PUSHPULL);
  GPIO_SetupPinMux(56, GPIO_MUX_CPU1, 2);
  GPIO_SetupPinOptions(56, GPIO_OUTPUT, GPIO_ASYNC);

  ScidRegs.SCICCR.all = 0x0007;   // 1 stop bit,  No loopback
                                  // No parity,8 char bits,
                                  // async mode, idle-line protocol
  ScidRegs.SCICTL1.all = 0x0003;  // enable TX, RX, internal SCICLK,
                                  // Disable RX ERR, SLEEP, TXWAKE
  ScidRegs.SCICTL2.all = 0x0003;
  ScidRegs.SCICTL2.bit.TXINTENA = 1;
  ScidRegs.SCICTL2.bit.RXBKINTENA = 1;

//
// SCIA at 9600 baud
// @LSPCLK = 50 MHz (200 MHz SYSCLK) HBAUD = 0x02 and LBAUD = 0x8B.
// @LSPCLK = 30 MHz (120 MHz SYSCLK) HBAUD = 0x01 and LBAUD = 0x86.
//
  ScidRegs.SCIHBAUD.all = 0x0002;
  ScidRegs.SCILBAUD.all = 0x008B;
  ScidRegs.SCIFFRX.all  = 0x2044;
//      SciaRegs.SCIFFCT.all = 0x0;

  ScidRegs.SCICTL1.all = 0x0023;  // Relinquish SCI from Reset



  WaveFile* wf = wave_open("/New_Song.wav", "wb+x");
  wave_close(wf);
  // Enter an (almost) infinite loop for reading and processing commands from
  // the user.
  int nStatus;
  while(1) {
    // Print a prompt to the console.  Show the CWD.
//    UARTprintf("\n%s> ", g_cCwdBuf);

    // Get a line of text from the user.
    UARTgets(g_cCmdBuf, sizeof(g_cCmdBuf));
    
    for(Uint16 i=0; i<CMD_BUF_SIZE; i++){
        if(g_cCmdBuf[i]!='\0')
            scia_txChar(g_cCmdBuf[i]);
        else
            break;
    }
    scia_txChar(0x00);
//    for(Uint16 i=0; i<CMD_BUF_SIZE; i++){
//        g_cCmdBuf[i] = scia_rxChar();
//        if(g_cCmdBuf[i]==0x00)
//            break;
//        else
//            continue;
//    }
    if(newRemoteCmd == TRUE) {
      UARTprintf("%s\n", g_uartRemoteRxBuf);
      newRemoteCmd = FALSE;
    }


    // Pass the line from the user to the command processor.
    // It will be parsed and valid commands executed.
//    nStatus = CmdLineProcess(g_cCmdBuf);

    // Handle the case of bad command.
//    if(nStatus == CMDLINE_BAD_CMD) {
//      UARTprintf("Bad command!\n");
//    }
//    // Handle the case of too many arguments.
//    else if(nStatus == CMDLINE_TOO_MANY_ARGS) {
//      UARTprintf("Too many arguments for command processor!\n");
//    }
//
//    // Otherwise the command was executed.  Print the error
//    // code if one was returned.
//    else if(nStatus != 0) {
//      UARTprintf("Command returned error code %s\n",
//           StringFromFresult((FRESULT)nStatus));
//    }
  }
}


interrupt void remoteUartRx_ISR(void)
{

//  uartRemoteRxBufIndex = 0;
//  for(uint16_t i=0; i<100; i++)
//      g_uartRemoteRxBuf[i]=0;
  while(ScibRegs.SCIFFRX.bit.RXFFST > 0) {
    g_uartRemoteRxBuf[uartRemoteRxBufIndex++] = ScibRegs.SCIRXBUF.all;
    if(g_uartRemoteRxBuf[uartRemoteRxBufIndex-1] == 0x00){
        newRemoteCmd = TRUE;
    }

  }
  if(newRemoteCmd || uartRemoteRxBufIndex >= 100) {
      uartRemoteRxBufIndex =0;
  }

  ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;
}


void scia_txChar(int a)
{
    while (ScibRegs.SCICTL2.bit.TXRDY != 1) {}
    ScibRegs.SCITXBUF.all =a;
}

char scia_rxChar(void)
{
    while(ScibRegs.SCIRXST.bit.RXRDY != 1) { }
    return ScibRegs.SCIRXBUF.all;
}
