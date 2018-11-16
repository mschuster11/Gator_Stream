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
#include "grlib.h"
#include "imageButton.h"
#include "images/images.h"


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Globals */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
Graphics_ImageButton audioOptionsButton;
Graphics_ImageButton btConnectivityButton;


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Function Definitions */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
void drawMainMenu(void) {
  Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_ORANGE);
  Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
  Graphics_clearDisplay(&g_sContext);
  Graphics_drawStringCentered(&g_sContext, "Gator Stream",
    AUTO_STRING_LENGTH,
    159,
    30,
    TRANSPARENT_TEXT);

  // Draw Primitives image button
  Graphics_drawImageButton(&g_sContext, &audioOptionsButton);

  // Draw Images image button
  Graphics_drawImageButton(&g_sContext, &btConnectivityButton);
}


void initMainMenuButtons(void) {
  // Initiliaze primitives Demo Button
  audioOptionsButton.xPosition=25;
  audioOptionsButton.yPosition=50;
  audioOptionsButton.borderWidth=1;
  audioOptionsButton.selected=false;
  audioOptionsButton.imageWidth=audio4BPP_UNCOMP.xSize;
  audioOptionsButton.imageHeight=audio4BPP_UNCOMP.ySize;
  audioOptionsButton.borderColor=GRAPHICS_COLOR_BLACK;
  audioOptionsButton.selectedColor=GRAPHICS_COLOR_ORANGE;
  audioOptionsButton.image=&audio4BPP_UNCOMP;


  // Initiliaze images Demo Button
  btConnectivityButton.xPosition=185;
  btConnectivityButton.yPosition=50;
  btConnectivityButton.borderWidth=1;
  btConnectivityButton.selected=false;
  btConnectivityButton.imageWidth=bt_logo4BPP_UNCOMP.xSize;
  btConnectivityButton.imageHeight=bt_logo4BPP_UNCOMP.ySize;
  btConnectivityButton.borderColor=GRAPHICS_COLOR_BLACK;
  btConnectivityButton.selectedColor=GRAPHICS_COLOR_BLUE;
  btConnectivityButton.image=&bt_logo4BPP_UNCOMP;
}
