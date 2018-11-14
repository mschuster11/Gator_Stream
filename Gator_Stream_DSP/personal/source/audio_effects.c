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
#include "personal/headers/audio_effects.h"
#include "math.h"

/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-Enums~-~-~-~--~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
enum side channel = LEFT;
enum buffer bufferNum = FIRST;
int16 effectLeftBuff[EFFECT_BUFFER_SIZE];
int16 effectRightBuff[EFFECT_BUFFER_SIZE];
uint16_t effectLeftBuffIndex = 0;
uint16_t effectRightBuffIndex = 0;
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~Globals~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
int16 currentLeftSample;
int16 currentRightSample;
extern uint32_t *crossCoreMemory;
extern uint16_t activeEffect;
extern volatile tIpcController g_sIpcController1;


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~Function Definitions-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */

interrupt void audio_ISR(void) {
  if (channel == LEFT) {
    currentLeftSample = McbspbRegs.DRR1.all;
    effectLeftBuff[effectLeftBuffIndex++] = McbspbRegs.DRR1.all;
    switch(activeEffect) {
      case NO_AUDIO_EFFECT:
        break;
    }

    // If the IPC flag is set, send left sample to CPU1 to be written to the MMC.
    if(IpcRegs.IPCSTS.bit.IPC11 == 1)
      IPCLtoRDataWrite(&g_sIpcController1, crossCoreMemory[0],(uint32_t)currentLeftSample, IPC_LENGTH_16_BITS, ENABLE_BLOCKING,NO_FLAG);
    
    // Reset left effect buffer index upon overflow.
    if(effectLeftBuffIndex > EFFECT_BUFFER_SIZE)
      effectLeftBuffIndex = 0;
    McbspbRegs.DXR1.all = currentLeftSample;
  } else if (channel == RIGHT) {
    currentRightSample = McbspbRegs.DRR1.all;
    effectRightBuff[effectRightBuffIndex++] = McbspbRegs.DRR1.all;
    switch(activeEffect) {
      case NO_AUDIO_EFFECT:
        break; 
    }

    // If the IPC flag is set, send right sample to CPU1 to be written to the MMC.
    if(IpcRegs.IPCSTS.bit.IPC11 == 1)
      IPCLtoRDataWrite(&g_sIpcController1, crossCoreMemory[1],(uint32_t)currentRightSample, IPC_LENGTH_16_BITS, ENABLE_BLOCKING,NO_FLAG);
    
    // Reset right effect buffer index upon overflow.
    if(effectRightBuffIndex > EFFECT_BUFFER_SIZE)
      effectRightBuffIndex = 0;
    McbspbRegs.DXR1.all = currentRightSample;
  }

  channel ^= 1;
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;
}

int16 flangerEffect(float sample, enum side channel) {
  int16 modifiedSample;
  static Uint16 oscillationIndex = 0;
  Uint16 oscillationDelay = (Uint16)ceilf(fabsf(sinf(2.0f * (float)M_PI * (float)oscillationIndex / 44100.0f)));
  if(oscillationIndex >= 44100)
    oscillationIndex = 0;
  else
    oscillationIndex++;

  if(channel == LEFT) {
    Uint16 delayIndex = (effectLeftBuffIndex > oscillationDelay ? effectLeftBuffIndex - oscillationDelay : (EFFECT_BUFFER_SIZE - (oscillationDelay - effectLeftBuffIndex)));
    modifiedSample = (0.7 * sample) + (0.7 * effectLeftBuff[delayIndex]);
  }
  else {
    Uint16 delayIndex = (effectRightBuffIndex > oscillationDelay ? effectRightBuffIndex - oscillationDelay : (EFFECT_BUFFER_SIZE - (oscillationDelay - effectRightBuffIndex)));
    modifiedSample = (0.7 * sample) + (0.7 * effectRightBuff[delayIndex]);
  }
  return modifiedSample;
}
