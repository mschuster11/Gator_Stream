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
#include <stdint.h>
#include "grlib.h"
#include <math.h>
#include "personal/headers/general.h"


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Externs */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
extern Graphics_Context g_sContext;


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Function Definitions */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
void drawLoadingAnimation(uint16_t seconds) {
  int16_t ulIdx;
  uint32_t color;
  for(uint16_t i=0; i<seconds; i++) {
    for(ulIdx = 256; ulIdx >= 1; ulIdx--) {
      // Red Color
      * ((uint16_t*) (&color)+1)  = 255;
      // Blue and Green Colors
      * ((uint16_t*) (&color))  = ((((128 - ulIdx/2) * 255) >> 7) << ClrBlueShift);

      Graphics_setForegroundColor(&g_sContext, color);
      Graphics_drawLine(&g_sContext, 160, 120, (160 + (int16_t) (12.5f * cosf((float)ulIdx * ((M_PI * 2.0f) / 256.0f)))), (120 + (int16_t) (12.5f * sinf((float)ulIdx * ((M_PI * 2.0f) / 256.0f)))));
      Delay(20);
    }
    for(ulIdx = 256; ulIdx >= 1; ulIdx--) {
      // Red Color
      * ((uint16_t*) (&color)+1)  = (ulIdx/2 * 255) >> 7;
      // Blue and Green Colors
      * ((uint16_t*) (&color))  = 255 << ClrBlueShift;

      Graphics_setForegroundColor(&g_sContext, color);
      Graphics_drawLine(&g_sContext, 160, 120, (160 + (int16_t) (12.5f * cosf((float)ulIdx * ((M_PI * 2.0f) / 256.0f)))), (120 + (int16_t) (12.5f * sinf((float)ulIdx * ((M_PI * 2.0f) / 256.0f)))));
      Delay(20);
    }
    // Clear Red Color
    * ((uint16_t*) (&color)+1)  = 0;
    for(ulIdx = 256; ulIdx >= 1; ulIdx--) {
      // Blue and Green Colors
      * ((uint16_t*) (&color))  = ((((128 - ulIdx/2) * 255) >> 7) << ClrGreenShift) | (255 << ClrBlueShift);

      Graphics_setForegroundColor(&g_sContext, color);
      Graphics_drawLine(&g_sContext, 160, 120, (160 + (int16_t) (12.5f * cosf((float)ulIdx * ((M_PI * 2.0f) / 256.0f)))), (120 + (int16_t) (12.5f * sinf((float)ulIdx * ((M_PI * 2.0f) / 256.0f)))));
      Delay(20);
    }
    for(ulIdx = 256; ulIdx >= 1; ulIdx--) {
      // Blue and Green Colors
      * ((uint16_t*) (&color))  = (255 << ClrGreenShift) | (((ulIdx/2 * 255) >> 7) << ClrBlueShift);

      Graphics_setForegroundColor(&g_sContext, color);
      Graphics_drawLine(&g_sContext, 160, 120, (160 + (int16_t) (12.5f * cosf((float)ulIdx * ((M_PI * 2.0f) / 256.0f)))), (120 + (int16_t) (12.5f * sinf((float)ulIdx * ((M_PI * 2.0f) / 256.0f)))));
      Delay(20);
    }
    for(ulIdx = 256; ulIdx >= 1; ulIdx--) {
      // Red color
       * ((uint16_t*) (&color)+1)  = (((128 - ulIdx/2) * 255) >> 7);
      // Blue and Green Colors
      * ((uint16_t*) (&color))  = (((127 + ulIdx/2) * 255) >> 7) << ClrGreenShift;

      Graphics_setForegroundColor(&g_sContext, color);
      Graphics_drawLine(&g_sContext, 160, 120, (160 + (int16_t) (12.5f * cosf((float)ulIdx * ((M_PI * 2.0f) / 256.0f)))), (120 + (int16_t) (12.5f * sinf((float)ulIdx * ((M_PI * 2.0f) / 256.0f)))));
      Delay(20);
    }
  }
}
