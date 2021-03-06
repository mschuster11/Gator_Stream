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
#include "driverlib/sysctl.h"
#include "personal/headers/sci_utils.h"
#include "fatfs/src/integer.h"
#include "stdlib.h"
#include "personal/headers/queue.h"

/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-Externs~-~-~-~--~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */



/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~Globals~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
queue* mspUartCmdQueue;
#ifdef REMOTE_QUEUE
queue* remoteUartCmdQueue;
#else
char* remoteUartCmdBuf[128];
uint16_t uartRemoteRxBufIndex = 0;
bool_t newRemoteCmd = FALSE;
#endif
char uartRemoteRxBuf[100];
 uint16_t uartMspRxBufIndex = 0;

/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~Function Definitions-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
#ifndef CUSTOM_HW
interrupt void mspUartRx_ISR (void) {
  while(ScicRegs.SCIFFRX.bit.RXFFST > 0) {
    if(queue_tail(mspUartCmdQueue) == NULL || queue_tail(mspUartCmdQueue)->isComplete){
      node* newMspUartCmd = malloc(sizeof(node));
      newMspUartCmd->length = 0;
      newMspUartCmd->isComplete = FALSE;
      newMspUartCmd->isConsumed = FALSE;
      queue_push(mspUartCmdQueue, newMspUartCmd);
    }
    node* mspUartCmd = queue_tail(mspUartCmdQueue);
    mspUartCmd->uartString[mspUartCmd->length++] = ScicRegs.SCIRXBUF.all;
    if(mspUartCmd->uartString[mspUartCmd->length-2] == '\r' && mspUartCmd->uartString[mspUartCmd->length-1] == '\n'){
      mspUartCmd->uartString[mspUartCmd->length] = '\0';
      mspUartCmd->isComplete = TRUE;
    }
  }
  if(queue_tail(mspUartCmdQueue)->length >= 100) 
      queue_tail(mspUartCmdQueue)->length = 0;
  ScicRegs.SCIFFRX.bit.RXFFINTCLR = 1;
}
#else
interrupt void mspUartRx_ISR (void) {
  while(SciaRegs.SCIFFRX.bit.RXFFST > 0) {
    if(queue_tail(mspUartCmdQueue) == NULL || queue_tail(mspUartCmdQueue)->isComplete){
      node* newMspUartCmd = malloc(sizeof(node));
      newMspUartCmd->length = 0;
      newMspUartCmd->isComplete = FALSE;
      newMspUartCmd->isConsumed = FALSE;
      queue_push(mspUartCmdQueue, newMspUartCmd);
    }
    node* mspUartCmd = queue_tail(mspUartCmdQueue);
    mspUartCmd->uartString[mspUartCmd->length++] = SciaRegs.SCIRXBUF.all;
    if(mspUartCmd->uartString[mspUartCmd->length-2] == '\r' && mspUartCmd->uartString[mspUartCmd->length-1] == '\n'){
      mspUartCmd->uartString[mspUartCmd->length] = '\0';
      mspUartCmd->isComplete = TRUE;
    }
  }
  if(queue_tail(mspUartCmdQueue)->length >= 100) 
    queue_tail(mspUartCmdQueue)->length = 0;
  SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;
}
#endif

#ifdef REMOTE_QUEUE
interrupt void remoteUartRx_ISR (void) {
  while(ScibRegs.SCIFFRX.bit.RXFFST > 0) {
    if(queue_tail(remoteUartCmdQueue) == NULL || queue_tail(remoteUartCmdQueue)->isComplete){
      node* newRemoteUartCmd = malloc(sizeof(node));
      newRemoteUartCmd->length = 0;
      newRemoteUartCmd->isComplete = FALSE;
      newRemoteUartCmd->isConsumed = FALSE;
      queue_push(remoteUartCmdQueue, newRemoteUartCmd);
    }
    node* remoteUartCmd = queue_tail(remoteUartCmdQueue);
    remoteUartCmd->uartString[remoteUartCmd->length++] = ScibRegs.SCIRXBUF.all;
    if(remoteUartCmd->uartString[remoteUartCmd->length-3] == '\r' && remoteUartCmd->uartString[remoteUartCmd->length-2] == '\n' && remoteUartCmd->uartString[remoteUartCmd->length-1] == '\0'){
      remoteUartCmd->uartString[remoteUartCmd->length] = '\0';
      remoteUartCmd->isComplete = TRUE;
    }
  }
  if(queue_tail(remoteUartCmdQueue)->length >= 100) 
    queue_tail(remoteUartCmdQueue)->length = 0;
  ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;
}
#else
interrupt void remoteUartRx_ISR (void) {
    while(ScibRegs.SCIFFRX.bit.RXFFST > 0) {
        remoteUartCmdBuf[uartRemoteRxBufIndex++] = ScibRegs.SCIRXBUF.all;
      if(remoteUartCmdBuf[uartRemoteRxBufIndex-3] == '\r' && remoteUartCmdBuf[uartRemoteRxBufIndex-2] == '\n' && remoteUartCmdBuf[uartRemoteRxBufIndex-1] == '\0'){
          newRemoteCmd = TRUE;
          uartRemoteRxBufIndex = 0;
      }
    }
    if(uartRemoteRxBufIndex >= 128)
        uartRemoteRxBufIndex = 0;
    ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;
}
#endif
void scia_txChar (char c) {
  while (SciaRegs.SCICTL2.bit.TXRDY != 1);
  SciaRegs.SCITXBUF.all =c;
}

void scib_txChar (char c) {
  while (ScibRegs.SCICTL2.bit.TXRDY != 1);
  ScibRegs.SCITXBUF.all = c;
}

void scic_txChar (char c) {
  while (ScicRegs.SCICTL2.bit.TXRDY != 1);
  ScicRegs.SCITXBUF.all = c;
}


void init_scia () {

  // Assign GPIO43 to CPU-1 as the RX of SCI-A.
  GPIO_SetupPinMux(43, GPIO_MUX_CPU1, 15);
  GPIO_SetupPinOptions(43, GPIO_INPUT, GPIO_PUSHPULL);

  // Assign GPIO42 to CPU-1 as the TX of SCI-A.
  GPIO_SetupPinMux(42, GPIO_MUX_CPU1, 15);
  GPIO_SetupPinOptions(42, GPIO_OUTPUT, GPIO_ASYNC);

  SciaRegs.SCICCR.all = 0x0007;         // 1 stop bit,  No loopback
                                        // No parity,8 char bits,
                                        // async mode, idle-line protocol
  SciaRegs.SCICTL1.all = 0x0003;        // enable TX, RX, internal SCICLK,
                                        // Disable RX ERR, SLEEP, TXWAKE
  SciaRegs.SCICTL2.all = 0x0003;        // Enable the interrupts for both TX and RX. 

  // SCIC at 9600 baud
  SciaRegs.SCIHBAUD.all = 0x0002;
  SciaRegs.SCILBAUD.all = 0x008B;
  SciaRegs.SCIFFRX.all  = 0x2044;       // Relinquish RX FIFO from Reset,
                                        // enable the interrupt with priority 4.
  SciaRegs.SCIFFTX.bit.SCIFFENA = 1;    // Enable the FIFO.
  SciaRegs.SCIFFRX.bit.RXFFIENA = 1;    // Enable the RX FIFO's interrup.

  // Relinquish SCI from Reset
  SciaRegs.SCICTL1.all = 0x0023;  

  // Enable SCI-A's peripheral clock.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_SCI1);
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

  // SCIB at 9600 baud
  ScibRegs.SCIHBAUD.all = 0x0002;
  ScibRegs.SCILBAUD.all = 0x008B;
  ScibRegs.SCIFFRX.all  = 0x2044;       // Relinquish RX FIFO from Reset,
                                        // enable the interrupt with priority 4.
  ScibRegs.SCIFFTX.bit.SCIFFENA = 1;    // Enable the FIFO.
  ScibRegs.SCIFFRX.bit.RXFFIENA = 1;    // Enable the RX FIFO's interrup.

  // Relinquish SCI from Reset
  ScibRegs.SCICTL1.all = 0x0023;  

  // Enable SCI-C's peripheral clock.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_SCI2);
}


void init_scic () {

#ifndef CUSTOM_HW
  // Launchpad HW
  // Assign GPIO139 to CPU-1 as the RX of SCI-C.
  GPIO_SetupPinMux(139, GPIO_MUX_CPU1, 6);
  GPIO_SetupPinOptions(139, GPIO_INPUT, GPIO_PUSHPULL);

  // Assign GPIO56 to CPU-1 as the TX of SCI-C.
  GPIO_SetupPinMux(56, GPIO_MUX_CPU1, 6);
  GPIO_SetupPinOptions(56, GPIO_OUTPUT, GPIO_ASYNC);

#else
  // Custom HW
  // Assign GPIO43 to CPU-1 as the RX of SCI-C.
  GPIO_SetupPinMux(43, GPIO_MUX_CPU1, 15);
  GPIO_SetupPinOptions(43, GPIO_INPUT, GPIO_PUSHPULL);

  // Assign GPIO42 to CPU-1 as the TX of SCI-C.
  GPIO_SetupPinMux(42, GPIO_MUX_CPU1, 15);
  GPIO_SetupPinOptions(42, GPIO_OUTPUT, GPIO_ASYNC);
#endif


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
  SysCtlPeripheralEnable(SYSCTL_PERIPH_SCI3);
}


void init_messageQueues(void) {
  mspUartCmdQueue = queue_InitQueue();
  remoteUartCmdQueue = queue_InitQueue();
}
