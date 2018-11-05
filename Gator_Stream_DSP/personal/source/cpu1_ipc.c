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
#include "F2837xD_Ipc_drivers.h"
#include "personal/headers/cpu1_ipc.h"
#include "personal/headers/audio_effects.h"
#include "fatfs/src/integer.h"

/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-Externs~-~-~-~--~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
extern enum side channel = LEFT;
extern enum buffer bufferNum = FIRST;
extern uint16_t leftSample;
extern uint16_t rightSample;
extern volatile tIpcController g_sIpcController1;
extern volatile tIpcController g_sIpcController2;


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~Globals~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
int16 leftSamplesBuffer1[MMC_SAMPLE_PING_PONG_BUFF_SIZE];
int16 rightSamplesBuffer1[MMC_SAMPLE_PING_PONG_BUFF_SIZE];
int16 leftSamplesBuffer2[MMC_SAMPLE_PING_PONG_BUFF_SIZE];
int16 rightSamplesBuffer2[MMC_SAMPLE_PING_PONG_BUFF_SIZE];
int16* mmcSampleBuffer[2];
Uint16 leftMMCSamplesBufferIndex = 0x00;
Uint16 RightMMCSamplesBufferIndex = 0x00;
bool_t mmcReady = FALSE;
bool_t fileReady = TRUE;

/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~Function Definitions-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
// CPU02toCPU01IPC0IntHandler - Handles writes into CPU01 addresses as a
//                              result of read commands to the CPU02.
interrupt void CPU02toCPU01IPC0IntHandler (void) {
  tIpcMessage sMessage;

  // Continue processing messages as long as CPU01 to CPU02
  // GetBuffer1 is full
  while(IpcGet(&g_sIpcController1, &sMessage, DISABLE_BLOCKING) != STATUS_FAIL) {
    switch (sMessage.ulcommand) {
      case IPC_DATA_WRITE:
        if(sMessage.uladdress == (uint32_t)&leftSample){
          if (bufferNum == FIRST) {
            leftSamplesBuffer1[leftMMCSamplesBufferIndex++] = sMessage.uldataw2;
          } else {
            leftSamplesBuffer2[leftMMCSamplesBufferIndex++] = sMessage.uldataw2;
          }
        } else if(sMessage.uladdress == (uint32_t)&rightSample){
          if (bufferNum == FIRST) {
            rightSamplesBuffer1[RightMMCSamplesBufferIndex++] = sMessage.uldataw2;
          } else {
            rightSamplesBuffer2[RightMMCSamplesBufferIndex++] = sMessage.uldataw2;
          }
        }
        if ((RightMMCSamplesBufferIndex > MMC_SAMPLE_PING_PONG_BUFF_SIZE - 1) &&
            (leftMMCSamplesBufferIndex > MMC_SAMPLE_PING_PONG_BUFF_SIZE - 1)) {
          leftMMCSamplesBufferIndex = 0x00;
          RightMMCSamplesBufferIndex = 0x00;
          mmcReady = 1;
          if(bufferNum == FIRST) {
            mmcSampleBuffer[0] = leftSamplesBuffer1;
            mmcSampleBuffer[1] = rightSamplesBuffer1;
            bufferNum = SECOND;
          } else {
            mmcSampleBuffer[0] = leftSamplesBuffer2;
            mmcSampleBuffer[1] = rightSamplesBuffer2;
            bufferNum = FIRST;
          }
        } 
        break;
      default:
        break;
    }
  }

  // Acknowledge IPC INT0 Flag and PIE to receive more interrupts
  IpcRegs.IPCACK.bit.IPC0 = 1;
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}


// CPU02toCPU01IPC1IntHandler - Should never reach this ISR. This is an
//                              optional placeholder for g_sIpcController2.
interrupt void CPU02toCPU01IPC1IntHandler (void) {
    // Acknowledge IPC INT1 Flag and PIE to receive more interrupts
    IpcRegs.IPCACK.bit.IPC1 = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
