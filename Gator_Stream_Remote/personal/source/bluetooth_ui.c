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
Graphics_Button bt_backButton;
Graphics_Button bt_choiceButton1;
Graphics_Button bt_choiceButton2;
Graphics_Button bt_choiceButton3;
Graphics_Button bt_disconnectButton;
Graphics_Button bt_playButton;
Graphics_Button bt_pauseButton;


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Function Definitions */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
void openBTMenu(void) {
  drawLoadingAnimation(2);
  Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE_SMOKE);
  Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE_SMOKE);
  Graphics_clearDisplay(&g_sContext);
  Graphics_drawButton(&g_sContext,&bt_backButton);
  Graphics_drawButton(&g_sContext,&bt_disconnectButton);
  Graphics_drawButton(&g_sContext,&bt_choiceButton1);
  Graphics_drawButton(&g_sContext,&bt_choiceButton2);
  Graphics_drawButton(&g_sContext,&bt_choiceButton3);
  Graphics_drawButton(&g_sContext,&bt_playButton);


  for(;;){
    touch_updateCurrentTouch(&g_sTouchContext);
    if(g_sTouchContext.touch) {
      if(Graphics_isButtonSelected(&bt_backButton, g_sTouchContext.x,  g_sTouchContext.y)){
        Graphics_drawSelectedButton(&g_sContext, &bt_backButton);
        break;
      } else if(Graphics_isButtonSelected(&bt_disconnectButton, g_sTouchContext.x,  g_sTouchContext.y)) {
        Graphics_drawSelectedButton(&g_sContext,&bt_disconnectButton);
        UART_transmitStringNullTerm("CS  \r\n");
        Delay(10000);
        Graphics_drawButton(&g_sContext,&bt_disconnectButton);
      } else if(Graphics_isButtonSelected(&bt_choiceButton1, g_sTouchContext.x,  g_sTouchContext.y)) {
        Graphics_drawSelectedButton(&g_sContext,&bt_choiceButton1);
        UART_transmitStringNullTerm("OS 1\r\n");
        Delay(10000);
        Graphics_drawButton(&g_sContext,&bt_choiceButton1);
      } else if(Graphics_isButtonSelected(&bt_choiceButton2, g_sTouchContext.x,  g_sTouchContext.y)) {
        Graphics_drawSelectedButton(&g_sContext,&bt_choiceButton2);
        UART_transmitStringNullTerm("OS 2\r\n");
        Delay(10000);
        Graphics_drawButton(&g_sContext,&bt_choiceButton2);
      } else if(Graphics_isButtonSelected(&bt_choiceButton3, g_sTouchContext.x,  g_sTouchContext.y)) {
        Graphics_drawSelectedButton(&g_sContext,&bt_choiceButton3);
        UART_transmitStringNullTerm("OS 3\r\n");
        Delay(10000);
        Graphics_drawButton(&g_sContext,&bt_choiceButton3);
      } else if(Graphics_isButtonSelected(&bt_playButton, g_sTouchContext.x,  g_sTouchContext.y) && (playPauseFlag == PLAY)) {
        Graphics_drawSelectedButton(&g_sContext,&bt_playButton);
        UART_transmitStringNullTerm("PL\r\n");
        playPauseFlag = PAUSE;
        Delay(10000);
        Graphics_drawButton(&g_sContext,&bt_pauseButton);
      } else if(Graphics_isButtonSelected(&bt_pauseButton, g_sTouchContext.x,  g_sTouchContext.y) && (playPauseFlag == PAUSE)) {
        Graphics_drawSelectedButton(&g_sContext,&bt_pauseButton);
        UART_transmitStringNullTerm("PA\r\n");
        playPauseFlag = PLAY;
        Delay(10000);
        Graphics_drawButton(&g_sContext,&bt_playButton);
      }
    } 
  }
  Graphics_clearDisplay(&g_sContext);
  drawMainMenu();
}


void initBTMenuButtons(void) {
  bt_backButton.xMin = 250;
  bt_backButton.xMax = 320;
  bt_backButton.yMin = 0;
  bt_backButton.yMax = 40;
  bt_backButton.borderWidth = 1;
  bt_backButton.selected = false;
  bt_backButton.fillColor = GRAPHICS_COLOR_RED;
  bt_backButton.borderColor = GRAPHICS_COLOR_RED;
  bt_backButton.selectedColor = GRAPHICS_COLOR_BLACK;
  bt_backButton.textColor = GRAPHICS_COLOR_BLACK;
  bt_backButton.selectedTextColor = GRAPHICS_COLOR_RED;
  bt_backButton.textXPos = 270;
  bt_backButton.textYPos = 10;
  bt_backButton.text = "BACK";
  bt_backButton.font = &g_sFontCm18;

  bt_choiceButton1.xMin = 145;
  bt_choiceButton1.xMax = 175;
  bt_choiceButton1.yMin = 55;
  bt_choiceButton1.yMax = 95;
  bt_choiceButton1.borderWidth = 1;
  bt_choiceButton1.selected = false;
  bt_choiceButton1.fillColor = GRAPHICS_COLOR_ORANGE;
  bt_choiceButton1.borderColor = GRAPHICS_COLOR_BLUE;
  bt_choiceButton1.selectedColor = GRAPHICS_COLOR_BLACK;
  bt_choiceButton1.textColor = GRAPHICS_COLOR_BLACK;
  bt_choiceButton1.selectedTextColor = GRAPHICS_COLOR_RED;
  bt_choiceButton1.textXPos = 157;
  bt_choiceButton1.textYPos = 70;
  bt_choiceButton1.text = "1";
  bt_choiceButton1.font = &g_sFontCm18;

  bt_choiceButton2.xMin = 145;
  bt_choiceButton2.xMax = 175;
  bt_choiceButton2.yMin = 100;
  bt_choiceButton2.yMax = 140;
  bt_choiceButton2.borderWidth = 1;
  bt_choiceButton2.selected = false;
  bt_choiceButton2.fillColor = GRAPHICS_COLOR_BLUE;
  bt_choiceButton2.borderColor = GRAPHICS_COLOR_ORANGE;
  bt_choiceButton2.selectedColor = GRAPHICS_COLOR_BLACK;
  bt_choiceButton2.textColor = GRAPHICS_COLOR_BLACK;
  bt_choiceButton2.selectedTextColor = GRAPHICS_COLOR_RED;
  bt_choiceButton2.textXPos = 157;
  bt_choiceButton2.textYPos = 115;
  bt_choiceButton2.text = "2";
  bt_choiceButton2.font = &g_sFontCm18;

  bt_choiceButton3.xMin = 145;
  bt_choiceButton3.xMax = 175;
  bt_choiceButton3.yMin = 145;
  bt_choiceButton3.yMax = 185;
  bt_choiceButton3.borderWidth = 1;
  bt_choiceButton3.selected = false;
  bt_choiceButton3.fillColor = GRAPHICS_COLOR_ORANGE;
  bt_choiceButton3.borderColor = GRAPHICS_COLOR_BLUE;
  bt_choiceButton3.selectedColor = GRAPHICS_COLOR_BLACK;
  bt_choiceButton3.textColor = GRAPHICS_COLOR_BLACK;
  bt_choiceButton3.selectedTextColor = GRAPHICS_COLOR_RED;
  bt_choiceButton3.textXPos = 157;
  bt_choiceButton3.textYPos = 160;
  bt_choiceButton3.text = "3";
  bt_choiceButton3.font = &g_sFontCm18;

  bt_disconnectButton.xMin = 0;
  bt_disconnectButton.xMax = 90;
  bt_disconnectButton.yMin = 0;
  bt_disconnectButton.yMax = 40;
  bt_disconnectButton.borderWidth = 1;
  bt_disconnectButton.selected = false;
  bt_disconnectButton.fillColor = GRAPHICS_COLOR_RED;
  bt_disconnectButton.borderColor = GRAPHICS_COLOR_RED;
  bt_disconnectButton.selectedColor = GRAPHICS_COLOR_BLACK;
  bt_disconnectButton.textColor = GRAPHICS_COLOR_BLACK;
  bt_disconnectButton.selectedTextColor = GRAPHICS_COLOR_RED;
  bt_disconnectButton.textXPos = 5;
  bt_disconnectButton.textYPos = 10;
  bt_disconnectButton.text = "Disconnect";
  bt_disconnectButton.font = &g_sFontCm18;

  bt_playButton.xMin = 140;
  bt_playButton.xMax = 180;
  bt_playButton.yMin = 0;
  bt_playButton.yMax = 40;
  bt_playButton.borderWidth = 1;
  bt_playButton.selected = false;
  bt_playButton.fillColor = GRAPHICS_COLOR_BLUE;
  bt_playButton.borderColor = GRAPHICS_COLOR_ORANGE;
  bt_playButton.selectedColor = GRAPHICS_COLOR_BLACK;
  bt_playButton.textColor = GRAPHICS_COLOR_BLACK;
  bt_playButton.selectedTextColor = GRAPHICS_COLOR_GREEN;
  bt_playButton.textXPos = 150;
  bt_playButton.textYPos = 10;
  bt_playButton.text = "Play";
  bt_playButton.font = &g_sFontCm18;

  bt_pauseButton.xMin = 140;
  bt_pauseButton.xMax = 180;
  bt_pauseButton.yMin = 0;
  bt_pauseButton.yMax = 40;
  bt_pauseButton.borderWidth = 1;
  bt_pauseButton.selected = false;
  bt_pauseButton.fillColor = GRAPHICS_COLOR_BLUE;
  bt_pauseButton.borderColor = GRAPHICS_COLOR_ORANGE;
  bt_pauseButton.selectedColor = GRAPHICS_COLOR_BLACK;
  bt_pauseButton.textColor = GRAPHICS_COLOR_BLACK;
  bt_pauseButton.selectedTextColor = GRAPHICS_COLOR_GREEN;
  bt_pauseButton.textXPos = 145;
  bt_pauseButton.textYPos = 10;
  bt_pauseButton.text = "Pause";
  bt_pauseButton.font = &g_sFontCm18;
}
