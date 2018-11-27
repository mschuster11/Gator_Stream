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
#include <stdlib.h>
#include "grlib.h"
#include "button.h"
#include "imageButton.h"
#include "images/images.h"
#include "personal/headers/animations.h"
#include "personal/headers/general.h"
#include "touch_P401R.h"
#include "personal/headers/uart_utils.h"
#include "personal/headers/main_menu_ui.h"
#include "personal/headers/audio_ui.h"
#include "personal/headers/bluetooth_ui.h"


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Externs */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
extern touch_context g_sTouchContext;
extern enum streamState playPauseFlag;

/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Globals */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
static Graphics_Button backButton;
static Graphics_Button effectButtons[NUMBER_OF_EFFECTS];
static Graphics_Button playButton;
static Graphics_Button pauseButton;
static Graphics_Button scrollUpButton;
static Graphics_Button scrollDownButton;
static const Graphics_Rectangle bottomCover = {
  65,
  215,
  255,
  240
};
static int8_t* effectNames[NUMBER_OF_EFFECTS] = {
  "Flanger",
  "Overdrive",
  "Vibrato",
  "WahWah",
  "Ring Modulation",
  "Chorus",
  "Bass Boost",
  "Treb Boost"
};
static uint8_t scrollOffset;

/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Function Definitions */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
void openAudioMenu(void) {
  scrollOffset = 0;
  drawLoadingAnimation(2);
  initAudioMenuButtons();
  Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE_SMOKE);
  Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE_SMOKE);
  Graphics_clearDisplay(&g_sContext);
  Graphics_drawButton(&g_sContext,&backButton);
  drawEffectButtons();
  Graphics_drawButton(&g_sContext,&playButton);
  Graphics_drawButton(&g_sContext,&scrollUpButton);
  Graphics_drawButton(&g_sContext,&scrollDownButton);


  for(;;){
    touch_updateCurrentTouch(&g_sTouchContext);
    if(g_sTouchContext.touch) {
      if(Graphics_isButtonSelected(&scrollUpButton, g_sTouchContext.x,  g_sTouchContext.y)) {
        Graphics_drawSelectedButton(&g_sContext,&scrollUpButton);
        if(scrollUpEffectButtons()){
          drawEffectButtons();
        }
        Graphics_drawButton(&g_sContext,&scrollUpButton);
      } else if(Graphics_isButtonSelected(&scrollDownButton, g_sTouchContext.x,  g_sTouchContext.y)) {
        Graphics_drawSelectedButton(&g_sContext,&scrollDownButton);
        if(scrollDownEffectButtons()){
          drawEffectButtons();
        }
        Graphics_drawButton(&g_sContext,&scrollDownButton);
      } else if(Graphics_isButtonSelected(&backButton, g_sTouchContext.x,  g_sTouchContext.y)){
        Graphics_drawSelectedButton(&g_sContext, &backButton);
        break;
      } else {
        for(uint16_t i = 0;i<NUMBER_OF_EFFECTS;i++) {
          if((int16_t)effectButtons[i].yMin >= 55 && (int16_t)effectButtons[i].yMax <= 215 && Graphics_isButtonSelected(&effectButtons[i], g_sTouchContext.x,  g_sTouchContext.y)) {
            Graphics_drawSelectedButton(&g_sContext, &effectButtons[i]);
            UART_transmitString("EFFECT: ");
            UART_transmitChar((char)i+1);
            UART_transmitStringNullTerm("\r\n");
            Delay(10000);
            Graphics_drawButton(&g_sContext,&effectButtons[i]);
          }
        }
        if(Graphics_isButtonSelected(&playButton, g_sTouchContext.x,  g_sTouchContext.y) && (playPauseFlag == PLAY)) {
          Graphics_drawSelectedButton(&g_sContext,&playButton);
          UART_transmitStringNullTerm("AUDIO: PL\r\n");
          playPauseFlag = PAUSE;
          Delay(10000);
          Graphics_drawButton(&g_sContext,&pauseButton);
        } else if(Graphics_isButtonSelected(&pauseButton, g_sTouchContext.x,  g_sTouchContext.y) && (playPauseFlag == PAUSE)) {
          Graphics_drawSelectedButton(&g_sContext,&pauseButton);
          UART_transmitStringNullTerm("AUDIO: PA\r\n");
          playPauseFlag = PLAY;
          Delay(10000);
          Graphics_drawButton(&g_sContext,&playButton);
        }
      }
    } 
  }
  Graphics_clearDisplay(&g_sContext);
  drawMainMenu();
}


void initAudioMenuButtons(void) {
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

  for(uint16_t i = 0; i<NUMBER_OF_EFFECTS; i++) {
    effectButtons[i].xMin = 65;
    effectButtons[i].xMax = 255;
    effectButtons[i].yMin = (55 + 40 * i) + 1;
    effectButtons[i].yMax = (95 + 40 * i) - 1;
    effectButtons[i].borderWidth = 1;
    effectButtons[i].selected = false;
    if(i & 0x0001) {
    effectButtons[i].fillColor = GRAPHICS_COLOR_ORANGE;
    effectButtons[i].borderColor = GRAPHICS_COLOR_BLUE;
    effectButtons[i].selectedColor = GRAPHICS_COLOR_BLACK;
    effectButtons[i].textColor = GRAPHICS_COLOR_BLACK;
    effectButtons[i].selectedTextColor = GRAPHICS_COLOR_RED;
    } else {
      effectButtons[i].fillColor = GRAPHICS_COLOR_BLUE;
      effectButtons[i].borderColor = GRAPHICS_COLOR_ORANGE;
      effectButtons[i].selectedColor = GRAPHICS_COLOR_BLACK;
      effectButtons[i].textColor = GRAPHICS_COLOR_BLACK;
      effectButtons[i].selectedTextColor = GRAPHICS_COLOR_RED;
    }
    effectButtons[i].textXPos = 137;
    effectButtons[i].textYPos = 70 + 40 * i;
    effectButtons[i].text = effectNames[i];
    effectButtons[i].font = &g_sFontCm18;
  }


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

  scrollUpButton.xMin = 280;
  scrollUpButton.xMax = 320;
  scrollUpButton.yMin = 60;
  scrollUpButton.yMax = 120;
  scrollUpButton.borderWidth = 1;
  scrollUpButton.selected = false;
  scrollUpButton.fillColor = GRAPHICS_COLOR_BLUE;
  scrollUpButton.borderColor = GRAPHICS_COLOR_ORANGE;
  scrollUpButton.selectedColor = GRAPHICS_COLOR_BLACK;
  scrollUpButton.textColor = GRAPHICS_COLOR_BLACK;
  scrollUpButton.selectedTextColor = GRAPHICS_COLOR_GREEN;
  scrollUpButton.textXPos = 285;
  scrollUpButton.textYPos = 90;
  scrollUpButton.text = "UP";
  scrollUpButton.font = &g_sFontCm18;

  scrollDownButton.xMin = 280;
  scrollDownButton.xMax = 320;
  scrollDownButton.yMin = 120;
  scrollDownButton.yMax = 180;
  scrollDownButton.borderWidth = 1;
  scrollDownButton.selected = false;
  scrollDownButton.fillColor = GRAPHICS_COLOR_BLUE;
  scrollDownButton.borderColor = GRAPHICS_COLOR_ORANGE;
  scrollDownButton.selectedColor = GRAPHICS_COLOR_BLACK;
  scrollDownButton.textColor = GRAPHICS_COLOR_BLACK;
  scrollDownButton.selectedTextColor = GRAPHICS_COLOR_GREEN;
  scrollDownButton.textXPos = 285;
  scrollDownButton.textYPos = 150;
  scrollDownButton.text = "DOWN";
  scrollDownButton.font = &g_sFontCm18;
}


void drawEffectButtons(void) {
  for(uint16_t i = 0;i<NUMBER_OF_EFFECTS;i++){
    if((int16_t)effectButtons[i].yMin >= 55 && (int16_t)effectButtons[i].yMax <= 215)
      Graphics_drawButton(&g_sContext,&effectButtons[i]);
  }
}


uint8_t scrollUpEffectButtons(void) {
  if(scrollOffset < 4 ) {
    for(uint16_t i = 0;i<NUMBER_OF_EFFECTS;i++){
      effectButtons[i].yMin -= 40;
      effectButtons[i].yMax -= 40;
      effectButtons[i].textYPos -= 40;
    }
    scrollOffset++;
    return 1;
  }
  return 0;
}


uint8_t scrollDownEffectButtons(void) {
  if(scrollOffset > 0) {
    for(uint16_t i = 0;i<NUMBER_OF_EFFECTS;i++){
      effectButtons[i].yMin += 40;
      effectButtons[i].yMax += 40;
      effectButtons[i].textYPos += 40;
    }
    scrollOffset--;
    return 1;
  }
  return 0;
}
