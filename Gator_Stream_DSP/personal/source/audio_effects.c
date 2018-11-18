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
#include "stdlib.h"

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
    effectLeftBuff[effectLeftBuffIndex++] = currentLeftSample;
    switch(activeEffect) {
      case NO_AUDIO_EFFECT:
          currentLeftSample = bassBoostEffect((float)currentLeftSample, channel);
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
    effectRightBuff[effectRightBuffIndex++] = currentRightSample;
    switch(activeEffect) {
      case NO_AUDIO_EFFECT:
          currentRightSample = bassBoostEffect((float)currentRightSample, channel);
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
  Uint16 oscillationDelay = (Uint16)ceilf(132.0f * fabsf(sinf(2.0f * (float)M_PI * (float)oscillationIndex / FS_FLOAT)));
  if(oscillationIndex >= (FS_INT-1))
    oscillationIndex = 0;
  else
    oscillationIndex++;

  if(channel == LEFT) {
    Uint16 delayIndex = ((effectLeftBuffIndex-1) > oscillationDelay ? (effectLeftBuffIndex-1) - oscillationDelay : (EFFECT_BUFFER_SIZE - (oscillationDelay - (effectLeftBuffIndex-1))));
    modifiedSample = (0.5 * sample) + (0.5 * effectLeftBuff[delayIndex]);
  }
  else {
    Uint16 delayIndex = ((effectRightBuffIndex-1) > oscillationDelay ? (effectRightBuffIndex-1) - oscillationDelay : (EFFECT_BUFFER_SIZE - (oscillationDelay - (effectRightBuffIndex-1))));
    modifiedSample = (0.5 * sample) + (0.5 * effectRightBuff[delayIndex]);
  }
  return modifiedSample;
}


int16 overDriveEffect(float sample, enum side channel) {
  float modifiedSample;
  float normalizedSample = sample / MAX_VAL;
  int16 sign = normalizedSample / fabsf(normalizedSample);
  modifiedSample = MAX_VAL * (sign * (1 - expf(20.0f * (sign * normalizedSample))));
  modifiedSample = 0.1f * modifiedSample + (0.9) * sample;
  return (int16)modifiedSample;
}



int16 vibratoEffect(float sample, enum side channel) {
  float modifiedSample;
  static Uint16 oscillationIndex = 0;
  float oscillationDelay = 0.5f * sinf(2.0f * (float)M_PI * (float)oscillationIndex * 5.0f / FS_FLOAT);

  if(oscillationIndex >= (FS_INT - 1))
    oscillationIndex = 0;
  else
    oscillationIndex++;

  float zeiger = 1 + 309.0f + 309.0f * oscillationDelay;
  Uint16 oscillationDelayOffset = (Uint16)roundf(309.0f * (0.5f + 0.5f * oscillationDelay));
  float frac = zeiger - floorf(zeiger);
  if(channel == LEFT) {
    Uint16 delayIndex = ((effectLeftBuffIndex-1) > oscillationDelayOffset ? (effectLeftBuffIndex-1) - oscillationDelayOffset : (EFFECT_BUFFER_SIZE - (oscillationDelayOffset - (effectLeftBuffIndex-1))));
    modifiedSample = frac * (effectLeftBuff[delayIndex]) + (1 - frac) * (effectLeftBuff[(delayIndex == 0 ? EFFECT_BUFFER_SIZE -1 : delayIndex)]);
  }
  else {
    Uint16 delayIndex = ((effectRightBuffIndex-1) > oscillationDelayOffset ? (effectRightBuffIndex-1) - oscillationDelayOffset : (EFFECT_BUFFER_SIZE - (oscillationDelayOffset - (effectRightBuffIndex-1))));
    modifiedSample = frac * (effectRightBuff[delayIndex]) + (1 - frac) * (effectRightBuff[(delayIndex == 0 ? EFFECT_BUFFER_SIZE -1 : delayIndex)]);
  }

  return (int16) modifiedSample;
}


int16 wahwahEffect(float sample, enum side channel) {
  static float yhL[2], ybL[2], ylL[2];
  static float yhR[2], ybR[2], ylR[2];
  static Uint32 cutoffFrequencyIndex = 0;
  static Uint16 firstIndexL = 0;
  static Uint16 secondIndexL = 1;
  static Uint16 firstIndexR = 0;
  static Uint16 secondIndexR = 1;
  float modifiedSample; 
  float cutoffFrequency = 0.02f + (0.1f * fabsf(sinf(2.0f * (float)M_PI * (float)cutoffFrequencyIndex / (16.0f * FS_FLOAT))));
  if(cutoffFrequencyIndex >= (FS_INT * 16) - 1)
    cutoffFrequencyIndex = 0;
  else
    cutoffFrequencyIndex++;

  float fl = cutoffFrequency;
  float ql = 2.0f * 0.05f;
  float normalizedSample = sample / MAX_VAL;

  if(channel == LEFT) {
    yhL[firstIndexL] = normalizedSample - ylL[secondIndexL] - ql * ybL[secondIndexL];
    ybL[firstIndexL] = fl * yhL[firstIndexL] + ybL[secondIndexL];
    ylL[firstIndexL] = fl * ybL[firstIndexL] + ylL[secondIndexL];
    modifiedSample =  MAX_VAL * ybL[firstIndexL];
    firstIndexL ^= 1;
    secondIndexL ^= 1;
  }
  else {
    yhR[firstIndexR] = normalizedSample - ylR[secondIndexR] - ql * ybR[secondIndexR];
    ybR[firstIndexR] = fl * yhR[firstIndexR] + ybR[secondIndexR];
    ylR[firstIndexR] = fl * ybR[firstIndexR] + ylR[secondIndexR];
    modifiedSample =  MAX_VAL * ybR[firstIndexR];
    firstIndexR ^= 1;
    secondIndexR ^= 1;
  }
  return (int16)modifiedSample;
}


int16 ringModulationEffect(float sample, enum side channel) {
  float modifiedSample;
  static Uint16 oscillationIndex = 0;
  float modulation = sinf(2.0f * (float)M_PI * (float)oscillationIndex * 440.0f / FS_FLOAT);
  if(oscillationIndex >= (FS_INT-1))
    oscillationIndex = 0;
  else
    oscillationIndex++;

  modifiedSample = ((sample / MAX_VAL) * modulation);
  return (int16)(MAX_VAL * modifiedSample);
}


int16 chorusEffect(float sample, enum side channel) {
  float modifiedSample;
  static Uint16 oscillationIndex = 0;
  Uint16 voice1Delay = (Uint16)(132.0f * sinf((2.0f * (float)M_PI * (float)oscillationIndex / (75.0f *FS_FLOAT)) + (float)M_PI / 3.0f) + 1323.3f);
  Uint16 voice2Delay = (Uint16)(132.0f * sinf((2.0f * (float)M_PI * (float)oscillationIndex / (100.0f *FS_FLOAT)) + (float)M_PI / 6.0f) + 970.2f);
  Uint16 voice3Delay = (Uint16)(132.0f * sinf((2.0f * (float)M_PI * (float)oscillationIndex / (50.0f *FS_FLOAT)) + (float)M_PI / 4.0f) + 1146.75f);
  oscillationIndex++;

  if(channel == LEFT) {
    Uint16 voice1DelayIndex = ((effectLeftBuffIndex-1) > voice1Delay ? (effectLeftBuffIndex-1) - voice1Delay : (EFFECT_BUFFER_SIZE - (voice1Delay - (effectLeftBuffIndex-1))));
    Uint16 voice2DelayIndex = ((effectLeftBuffIndex-1) > voice2Delay ? (effectLeftBuffIndex-1) - voice2Delay : (EFFECT_BUFFER_SIZE - (voice2Delay - (effectLeftBuffIndex-1))));
    Uint16 voice3DelayIndex = ((effectLeftBuffIndex-1) > voice3Delay ? (effectLeftBuffIndex-1) - voice3Delay : (EFFECT_BUFFER_SIZE - (voice3Delay - (effectLeftBuffIndex-1))));
    // modifiedSample = 0.6f * sample + 0.3f * effectLeftBuff[voice1DelayIndex] + 0.1f * effectLeftBuff[voice2DelayIndex];
    modifiedSample = 0.6f * sample + 0.25f * effectLeftBuff[voice1DelayIndex] + 0.1f * effectLeftBuff[voice2DelayIndex] + 0.05f * effectLeftBuff[voice3DelayIndex];
  } else {
    Uint16 voice1DelayIndex = ((effectRightBuffIndex-1) > voice1Delay ? (effectRightBuffIndex-1) - voice1Delay : (EFFECT_BUFFER_SIZE - (voice1Delay - (effectRightBuffIndex-1))));
    Uint16 voice2DelayIndex = ((effectRightBuffIndex-1) > voice2Delay ? (effectRightBuffIndex-1) - voice2Delay : (EFFECT_BUFFER_SIZE - (voice2Delay - (effectRightBuffIndex-1))));
    Uint16 voice3DelayIndex = ((effectRightBuffIndex-1) > voice3Delay ? (effectRightBuffIndex-1) - voice3Delay : (EFFECT_BUFFER_SIZE - (voice3Delay - (effectRightBuffIndex-1))));
    // modifiedSample = 0.6f * sample + 0.3f * effectRightBuff[voice1DelayIndex] + 0.1f * effectRightBuff[voice2DelayIndex];
    modifiedSample = 0.6f * sample + 0.25f * effectRightBuff[voice1DelayIndex] + 0.1f * effectRightBuff[voice2DelayIndex] + 0.05f * effectRightBuff[voice3DelayIndex];
  }
  return (int16) modifiedSample;
}


const float a[3] = { 1.000000000000000,  -1.982859743037343,   0.984922370590001 };
const float b[3] = { 1.004379783748927,  -1.981582232862108,   0.981820097016308 };
float yL[3] = {0,0,0};
float yR[3] = {0,0,0};
float xL[3] = {0,0,0};
float xR[3] = {0,0,0};
int16 bassBoostEffect(float sample, enum side channel) {
  float modifiedSample;
  Uint16 x1DelayIndex, x2DelayIndex;
  if(channel == LEFT) {
    // x1DelayIndex = (effectLeftBuffIndex-2 < EFFECT_BUFFER_SIZE ? effectLeftBuffIndex-2 : EFFECT_BUFFER_SIZE-2);
    // x2DelayIndex = (effectLeftBuffIndex-3 < EFFECT_BUFFER_SIZE ? effectLeftBuffIndex-3 : EFFECT_BUFFER_SIZE-3);
    yL[0] = (b[0]*xL[0]) + (b[1]*xL[1]) + (b[2]*xL[2]) - (a[1]*yL[1]) - (a[2]*yL[2]);
    yL[2] = yL[1];
    yL[1] = yL[0];
    modifiedSample = yL[0];
    xL[2] = xL[1];
    xL[1] = xL[0];
    xL[0] = sample/MAX_VAL;
  } else {
    // x1DelayIndex = (effectRightBuffIndex-2 < EFFECT_BUFFER_SIZE ? effectRightBuffIndex-2 : EFFECT_BUFFER_SIZE-2);
    // x2DelayIndex = (effectRightBuffIndex-3 < EFFECT_BUFFER_SIZE ? effectRightBuffIndex-3 : EFFECT_BUFFER_SIZE-3);
    yR[0] = (b[0]*xR[0]) + (b[1]*xR[1]) + (b[2]*xR[2])- (a[1]*yR[1]) - (a[2]*yR[2]);
    yR[2] = yR[1];
    yR[1] = yR[0];
    modifiedSample = yR[0];
    xR[2] = xR[1];
    xR[1] = xR[0];
    xR[0] = sample/MAX_VAL;
  }
  return (int16)(modifiedSample * MAX_VAL);
}

