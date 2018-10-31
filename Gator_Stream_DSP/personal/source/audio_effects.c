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
}channel = LEFT;

enum buffer{
  FIRST,
  SECOND
}bufferNum = FIRST;
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~Globals~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */

int16 leftVals1[100];
int16 rightVals1[100];
int16 leftVals2[100];
int16 rightVals2[100];
int16* buf[2];

Uint16 bufIndexL = 0x00;
Uint16 bufIndexR = 0x00;
bool_t sdRdy = 0;
Uint16 i = 0x00;
Uint16 j = 0x00;

/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~Function Definitions-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */

interrupt void audio_ISR(void) {
  if (channel == LEFT) {
    if (bufferNum == FIRST) {
      leftVals1[bufIndexL++] = McbspbRegs.DRR1.all;
    } else {
      leftVals2[bufIndexL++] = McbspbRegs.DRR1.all;
    }
    McbspbRegs.DXR1.all = McbspbRegs.DRR1.all;
  } else if (channel == RIGHT) {
    if (bufferNum == FIRST) {
      rightVals1[bufIndexR++] = McbspbRegs.DRR1.all;
    } else {
      rightVals2[bufIndexR++] = McbspbRegs.DRR1.all;
    }
    McbspbRegs.DXR1.all = McbspbRegs.DRR1.all;
  }

  if (bufIndexR > 2047 && bufIndexL > 2047) {
    bufIndexL = 0;
    bufIndexR = 0;
    sdRdy = 1;
    if(bufferNum == FIRST) {
      buf[0] = leftVals1;
      buf[1] = rightVals1;
      bufferNum = SECOND;
    } else {
      buf[0] = leftVals2;
      buf[1] = rightVals2;
      bufferNum = FIRST;
    }
  } 
  channel ^= 1;
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;
}
