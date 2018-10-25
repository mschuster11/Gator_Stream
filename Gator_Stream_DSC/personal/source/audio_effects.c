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
#include "personal/headers/audio_effects.h"

/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-Enums~-~-~-~--~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */

enum side{
  LEFT,
  RIGHT
}channel;


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~Globals~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */

Uint16 leftVals[128];
Uint16 rightVals[128];
Uint16 leftIndex = 0x00;
Uint16 rightIndex = 0x00;
Uint16 sdRdy = 0;
Uint16 i = 0x00;
Uint16 j = 0x00;


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~Function Definitions-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */

interrupt void audio_ISR(void) {
  if (channel == LEFT) {
    McbspbRegs.DXR1.all = McbspbRegs.DRR1.all;
  }
  else if (channel == RIGHT) {
    McbspbRegs.DXR1.all = McbspbRegs.DRR1.all;
  }
  channel ^= 1;
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;
}


///* -~-~-~-~-~-~-~-~-~-~-~-~-~-~Function Definitions-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
///* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
//
//interrupt void audio_ISR(void) {
//  if (channel == LEFT) {
////      leftVals[leftIndex++] = McbspbRegs.DRR1.all;
//      McbspbRegs.DXR1.all = McbspbRegs.DRR1.all;
////      if(leftIndex >= 128)
////          leftIndex=0;
////      McbspbRegs.DXR1.all = (signed int)(((float)(signed int)McbspbRegs.DRR1.all/32768.0 * ((float)sinLut[i > 511 ? i=0 : i]/65535.0 + 16383.0))*32768);
////    j >= 86 ? j = 0, i++ : j++;
//  }
//  else if (channel == RIGHT) {
////      rightVals[rightIndex++] = McbspbRegs.DRR1.all;
//      McbspbRegs.DXR1.all = McbspbRegs.DRR1.all;
////      if(rightIndex >= 128) {
////          rightIndex=0;
////          sdRdy=1;
////      }
////      McbspbRegs.DXR1.all = (signed int)(((float)(signed int)McbspbRegs.DRR1.all/32768.0 * ((float)sinLut[i > 511 ? i=0 : i]/65535.0 + 16383.0))*32768);
////      j >= 86 ? j = 0, i++ : j++;
//  }
//  channel ^= 1;
//  PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;
//}
