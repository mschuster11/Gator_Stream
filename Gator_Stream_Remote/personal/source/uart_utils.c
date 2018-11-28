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
/* Includes */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
#include "driverlib.h"
#include "stdlib.h"
#include "personal/headers/general.h"
#include "personal/headers/uart_utils.h"
#include "personal/headers/queue.h"

/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Globals */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
queue* rxUartCmdQueue;

/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Function Definitions */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* The following function enables a UART and configures it to the    */
/* specified baud rate.  Note that this function enables the UART Rx */
/* interrupt, but it is up to the caller to enable the UART Tx       */
/* interrupt.                                                        */
void Remote_EnableUART(uint32_t ModuleInstance, uint32_t InterruptNumber, unsigned long BaudRate) {
  GPIO_setAsPeripheralModuleFunctionOutputPin(HRDWCFG_DEBUG_UART_TX_PORT_NUM, HRDWCFG_DEBUG_UART_TX_PIN_NUM, GPIO_PRIMARY_MODULE_FUNCTION);
  GPIO_setAsPeripheralModuleFunctionInputPin(HRDWCFG_DEBUG_UART_RX_PORT_NUM, HRDWCFG_DEBUG_UART_RX_PIN_NUM, GPIO_PRIMARY_MODULE_FUNCTION);

  /* Set the UART configuration parameters.                            */
  eUSCI_UART_Config UARTConfig = {
    EUSCI_A_UART_CLOCKSOURCE_SMCLK,                 /* SMCLK Source    */
    312,                                            /* BRDIV = 312     */
    8,                                              /* UCxBRF = 8      */
    0,                                              /* UCxBRS = 0      */
    EUSCI_A_UART_NO_PARITY,                         /* No Parity       */
    EUSCI_A_UART_LSB_FIRST,                         /* MSB First       */
    EUSCI_A_UART_ONE_STOP_BIT,                      /* One stop bit    */
    EUSCI_A_UART_MODE,                              /* UART mode       */
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION   /* Low Frequency   */
  };

  /* Configure the UART using the procedure described in "22.3.1       */
  /* eUSCI_A Initialization and Reset" of the MSP432P4xx Family        */
  /* Technical Reference Manual.                                       */
  UART_disableModule(ModuleInstance);

  /* Initialize the UART.                                              */
  UART_initModule(ModuleInstance, &UARTConfig);

  /* Enable the UART.                                                  */
  UART_enableModule(ModuleInstance);

  /* Enable the receive interrupt.                                     */
  UART_enableInterrupt(ModuleInstance, EUSCI_A_UART_RECEIVE_INTERRUPT);
  UART_registerInterrupt(ModuleInstance, UART_rxIsr);
  /* Enable the eUSCI interrupt.                                       */
  Interrupt_enableInterrupt(InterruptNumber);
  Interrupt_setPriority(HRDWCFG_DEBUG_UART_INT_NUM, PRIORITY_NORMAL);

}

/* The following function disables a UART.                           */
void HAL_DisableUART(uint32_t ModuleInstance, uint32_t InterruptNumber) {
  Interrupt_disableInterrupt(InterruptNumber);
  UART_disableModule(ModuleInstance);
}

void UART_rxIsr(void) {
  if(queue_tail(rxUartCmdQueue) == NULL || queue_tail(rxUartCmdQueue)->isComplete){
    node* newRxUartCmd = malloc(sizeof(node));
    newRxUartCmd->length = 0;
    newRxUartCmd->isComplete = false;
    newRxUartCmd->isConsumed = false;
    queue_push(rxUartCmdQueue, newRxUartCmd);
  }
  node* rxUartCmd = queue_tail(rxUartCmdQueue);
  rxUartCmd->uartString[rxUartCmd->length++] = UART_receiveData(EUSCI_A3_BASE);
  if(rxUartCmd->uartString[rxUartCmd->length-3] == '\r' && rxUartCmd->uartString[rxUartCmd->length-2] == '\n' && rxUartCmd->uartString[rxUartCmd->length-1] == '\0'){
//    rxUartCmd->uartString[rxUartCmd->length] = '\0';
    rxUartCmd->isComplete = true;
  }
  if(queue_tail(rxUartCmdQueue)->length >= 100) 
    queue_tail(rxUartCmdQueue)->length = 0;
}


void UART_transmitChar(char c) {
  UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, c);
}


void UART_transmitString(char* s) {
  uint16_t i;
  for(i = 0;s[i]!=0;i++)
    UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, s[i]);
}


void UART_transmitStringNullTerm(char* s) {
  uint16_t i;
  for(i = 0;s[i]!=0;i++)
    UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, s[i]);
  UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, '\0');
}


void UART_initQueue(void) {
  rxUartCmdQueue = queue_InitQueue();
}
