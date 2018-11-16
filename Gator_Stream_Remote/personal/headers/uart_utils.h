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

#ifndef _UART_UTILS_H_
#define _UART_UTILS_H_

/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Defines */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
#define DEBUG_UART_BAUD_RATE              9600
#define PRIORITY_NORMAL                   (1 << 5)
#define HRDWCFG_DEBUG_UART_MODULE         EUSCI_A3_BASE
#define HRDWCFG_DEBUG_UART_INT_NUM        INT_EUSCIA3

#define HRDWCFG_DEBUG_UART_TX_PORT_NUM    GPIO_PORT_P9
#define HRDWCFG_DEBUG_UART_TX_PIN_NUM     GPIO_PIN7

#define HRDWCFG_DEBUG_UART_RX_PORT_NUM    GPIO_PORT_P9
#define HRDWCFG_DEBUG_UART_RX_PIN_NUM     GPIO_PIN6
#define UART_RX_CMD_BUF_SIZE              100


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Function Prototypes */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */

void Remote_EnableUART(uint32_t ModuleInstance, uint32_t InterruptNumber, unsigned long BaudRate);
void UART_rxIsr(void);
void UART_transmitString(char* s);

#endif
