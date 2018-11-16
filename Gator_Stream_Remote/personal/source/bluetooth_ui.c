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
#include "button.h"
#include "imageButton.h"
#include "images/images.h"
#include "personal/headers/animations.h"
#include "personal/headers/general.h"
#include "touch_P401R.h"
#include "personal/headers/uart_utils.h"
#include "personal/headers/main_menu_ui.h"
#include "personal/headers/bluetooth_ui.h"


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Enums */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
enum streamState playPauseFlag = PLAY;


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Externs */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
extern touch_context g_sTouchContext;


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Globals */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
Graphics_Button backButton;
Graphics_Button choiceButton1;
Graphics_Button choiceButton2;
Graphics_Button choiceButton3;
Graphics_Button disconnectButton;
Graphics_Button playButton;
Graphics_Button pauseButton;


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Function Definitions */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
void openBTMenu(void) {
  drawLoadingAnimation(2);
  Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE_SMOKE);
  Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE_SMOKE);
  Graphics_clearDisplay(&g_sContext);
  Graphics_drawButton(&g_sContext,&backButton);
  Graphics_drawButton(&g_sContext,&disconnectButton);
  Graphics_drawButton(&g_sContext,&choiceButton1);
  Graphics_drawButton(&g_sContext,&choiceButton2);
  Graphics_drawButton(&g_sContext,&choiceButton3);
  Graphics_drawButton(&g_sContext,&playButton);


  for(;;){
    touch_updateCurrentTouch(&g_sTouchContext);
    if(g_sTouchContext.touch) {
      if(Graphics_isButtonSelected(&backButton, g_sTouchContext.x,  g_sTouchContext.y)){
        Graphics_drawSelectedButton(&g_sContext, &backButton);
        break;
      } else if(Graphics_isButtonSelected(&disconnectButton, g_sTouchContext.x,  g_sTouchContext.y)) {
        Graphics_drawSelectedButton(&g_sContext,&disconnectButton);
        UART_transmitString("CS  \n\r");
        Delay(10000);
        Graphics_drawButton(&g_sContext,&disconnectButton);
      } else if(Graphics_isButtonSelected(&choiceButton1, g_sTouchContext.x,  g_sTouchContext.y)) {
        Graphics_drawSelectedButton(&g_sContext,&choiceButton1);
        UART_transmitString("OS 1\n\r");
        Delay(10000);
        Graphics_drawButton(&g_sContext,&choiceButton1);
      } else if(Graphics_isButtonSelected(&choiceButton2, g_sTouchContext.x,  g_sTouchContext.y)) {
        Graphics_drawSelectedButton(&g_sContext,&choiceButton2);
        UART_transmitString("OS 2\n\r");
        Delay(10000);
        Graphics_drawButton(&g_sContext,&choiceButton2);
      } else if(Graphics_isButtonSelected(&choiceButton3, g_sTouchContext.x,  g_sTouchContext.y)) {
        Graphics_drawSelectedButton(&g_sContext,&choiceButton3);
        UART_transmitString("OS 3\n\r");
        Delay(10000);
        Graphics_drawButton(&g_sContext,&choiceButton3);
      } else if(Graphics_isButtonSelected(&playButton, g_sTouchContext.x,  g_sTouchContext.y) && (playPauseFlag == PLAY)) {
        Graphics_drawSelectedButton(&g_sContext,&playButton);
        UART_transmitString("PL\n\r");
        playPauseFlag = PAUSE;
        Delay(10000);
        Graphics_drawButton(&g_sContext,&pauseButton);
      } else if(Graphics_isButtonSelected(&pauseButton, g_sTouchContext.x,  g_sTouchContext.y) && (playPauseFlag == PAUSE)) {
        Graphics_drawSelectedButton(&g_sContext,&pauseButton);
        UART_transmitString("PA\n\r");
        playPauseFlag = PLAY;
        Delay(10000);
        Graphics_drawButton(&g_sContext,&playButton);
      }
    } 
  }
  Graphics_clearDisplay(&g_sContext);
  drawMainMenu();
}


void initBTMenuButtons(void) {
  backButton.xMin = 250;
  backButton.xMax = 320;
  backButton.yMin = 0;
  backButton.yMax = 40;
  backButton.borderWidth = 1;
  backButton.selected = false;
  backButton.fillColor = GRAPHICS_COLOR_RED;
  backButton.borderColor = GRAPHICS_COLOR_RED;
  backButton.selectedColor = GRAPHICS_COLOR_BLACK;
  backButton.textColor = GRAPHICS_COLOR_BLACK;
  backButton.selectedTextColor = GRAPHICS_COLOR_RED;
  backButton.textXPos = 270;
  backButton.textYPos = 10;
  backButton.text = "BACK";
  backButton.font = &g_sFontCm18;

  choiceButton1.xMin = 145;
  choiceButton1.xMax = 175;
  choiceButton1.yMin = 55;
  choiceButton1.yMax = 95;
  choiceButton1.borderWidth = 1;
  choiceButton1.selected = false;
  choiceButton1.fillColor = GRAPHICS_COLOR_ORANGE;
  choiceButton1.borderColor = GRAPHICS_COLOR_BLUE;
  choiceButton1.selectedColor = GRAPHICS_COLOR_BLACK;
  choiceButton1.textColor = GRAPHICS_COLOR_BLACK;
  choiceButton1.selectedTextColor = GRAPHICS_COLOR_RED;
  choiceButton1.textXPos = 157;
  choiceButton1.textYPos = 70;
  choiceButton1.text = "1";
  choiceButton1.font = &g_sFontCm18;

  choiceButton2.xMin = 145;
  choiceButton2.xMax = 175;
  choiceButton2.yMin = 100;
  choiceButton2.yMax = 140;
  choiceButton2.borderWidth = 1;
  choiceButton2.selected = false;
  choiceButton2.fillColor = GRAPHICS_COLOR_BLUE;
  choiceButton2.borderColor = GRAPHICS_COLOR_ORANGE;
  choiceButton2.selectedColor = GRAPHICS_COLOR_BLACK;
  choiceButton2.textColor = GRAPHICS_COLOR_BLACK;
  choiceButton2.selectedTextColor = GRAPHICS_COLOR_RED;
  choiceButton2.textXPos = 157;
  choiceButton2.textYPos = 115;
  choiceButton2.text = "2";
  choiceButton2.font = &g_sFontCm18;

  choiceButton3.xMin = 145;
  choiceButton3.xMax = 175;
  choiceButton3.yMin = 145;
  choiceButton3.yMax = 185;
  choiceButton3.borderWidth = 1;
  choiceButton3.selected = false;
  choiceButton3.fillColor = GRAPHICS_COLOR_ORANGE;
  choiceButton3.borderColor = GRAPHICS_COLOR_BLUE;
  choiceButton3.selectedColor = GRAPHICS_COLOR_BLACK;
  choiceButton3.textColor = GRAPHICS_COLOR_BLACK;
  choiceButton3.selectedTextColor = GRAPHICS_COLOR_RED;
  choiceButton3.textXPos = 157;
  choiceButton3.textYPos = 160;
  choiceButton3.text = "3";
  choiceButton3.font = &g_sFontCm18;

  disconnectButton.xMin = 0;
  disconnectButton.xMax = 90;
  disconnectButton.yMin = 0;
  disconnectButton.yMax = 40;
  disconnectButton.borderWidth = 1;
  disconnectButton.selected = false;
  disconnectButton.fillColor = GRAPHICS_COLOR_RED;
  disconnectButton.borderColor = GRAPHICS_COLOR_RED;
  disconnectButton.selectedColor = GRAPHICS_COLOR_BLACK;
  disconnectButton.textColor = GRAPHICS_COLOR_BLACK;
  disconnectButton.selectedTextColor = GRAPHICS_COLOR_RED;
  disconnectButton.textXPos = 5;
  disconnectButton.textYPos = 10;
  disconnectButton.text = "Disconnect";
  disconnectButton.font = &g_sFontCm18;

  playButton.xMin = 140;
  playButton.xMax = 180;
  playButton.yMin = 0;
  playButton.yMax = 40;
  playButton.borderWidth = 1;
  playButton.selected = false;
  playButton.fillColor = GRAPHICS_COLOR_BLUE;
  playButton.borderColor = GRAPHICS_COLOR_ORANGE;
  playButton.selectedColor = GRAPHICS_COLOR_BLACK;
  playButton.textColor = GRAPHICS_COLOR_BLACK;
  playButton.selectedTextColor = GRAPHICS_COLOR_GREEN;
  playButton.textXPos = 150;
  playButton.textYPos = 10;
  playButton.text = "Play";
  playButton.font = &g_sFontCm18;

  pauseButton.xMin = 140;
  pauseButton.xMax = 180;
  pauseButton.yMin = 0;
  pauseButton.yMax = 40;
  pauseButton.borderWidth = 1;
  pauseButton.selected = false;
  pauseButton.fillColor = GRAPHICS_COLOR_BLUE;
  pauseButton.borderColor = GRAPHICS_COLOR_ORANGE;
  pauseButton.selectedColor = GRAPHICS_COLOR_BLACK;
  pauseButton.textColor = GRAPHICS_COLOR_BLACK;
  pauseButton.selectedTextColor = GRAPHICS_COLOR_GREEN;
  pauseButton.textXPos = 145;
  pauseButton.textYPos = 10;
  pauseButton.text = "Pause";
  pauseButton.font = &g_sFontCm18;
}
