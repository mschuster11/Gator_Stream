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
#include "stdlib.h"
#include "string.h"
#include "button.h"
#include "imageButton.h"
#include "images/images.h"
#include "personal/headers/animations.h"
#include "personal/headers/general.h"
#include "touch_P401R.h"
#include "personal/headers/uart_utils.h"
#include "personal/headers/main_menu_ui.h"
#include "personal/headers/bluetooth_ui.h"
#include "personal/headers/queue.h"


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Enums */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
enum streamState playPauseFlag = PLAY;


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Externs */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
extern touch_context g_sTouchContext;
extern queue* rxUartCmdQueue;

/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Globals */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
static Graphics_Button bt_backButton;
static Graphics_Button availableDeviceButtons[15];
static Graphics_Button bt_scrollUpButton;
static Graphics_Button bt_scrollDownButton;
static Graphics_Button bt_disconnectButton;
static Graphics_Button bt_playButton;
static Graphics_Button bt_pauseButton;
static uint8_t bt_scrollOffset;
static char bt_deviceNames[15][100]; 
static uint8_t bt_ActiceDevices = 0;
static uint8_t bt_CurrentReceivedDevice = 0;
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Function Definitions */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
void openBTMenu(void) {
  // drawLoadingAnimation(2);
  Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE_SMOKE);
  Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE_SMOKE);
  Graphics_clearDisplay(&g_sContext);
  Graphics_drawButton(&g_sContext,&bt_backButton);
  Graphics_drawButton(&g_sContext,&bt_disconnectButton);
  drawAvailableDeviceButtons();
  Graphics_drawButton(&g_sContext,&bt_playButton);
  Graphics_drawButton(&g_sContext,&bt_scrollUpButton);
  Graphics_drawButton(&g_sContext,&bt_scrollDownButton);

  for(;;) {
    touch_updateCurrentTouch(&g_sTouchContext);
    if(g_sTouchContext.touch) {
      if(Graphics_isButtonSelected(&bt_scrollUpButton, g_sTouchContext.x,  g_sTouchContext.y)) {
        Graphics_drawSelectedButton(&g_sContext, &bt_scrollUpButton);
        if(scrollDownAvailableDeviceButtons()){
          drawAvailableDeviceButtons();
        }
        Graphics_drawButton(&g_sContext, &bt_scrollUpButton);
      } else if(Graphics_isButtonSelected(&bt_scrollDownButton, g_sTouchContext.x,  g_sTouchContext.y)) {
        Graphics_drawSelectedButton(&g_sContext, &bt_scrollDownButton);
        if(scrollUpAvailableDeviceButtons()){
          drawAvailableDeviceButtons();
        }
        Graphics_drawButton(&g_sContext, &bt_scrollDownButton);
      } else if(Graphics_isButtonSelected(&bt_backButton, g_sTouchContext.x,  g_sTouchContext.y)){
        Graphics_drawSelectedButton(&g_sContext, &bt_backButton);
        break;
      } else if(Graphics_isButtonSelected(&bt_disconnectButton, g_sTouchContext.x,  g_sTouchContext.y)) {
        Graphics_drawSelectedButton(&g_sContext,&bt_disconnectButton);
        UART_transmitStringNullTerm("BT: CS\r\n");
        Delay(10000);
        Graphics_drawButton(&g_sContext,&bt_disconnectButton);
      }else if(Graphics_isButtonSelected(&bt_playButton, g_sTouchContext.x,  g_sTouchContext.y) && (playPauseFlag == PLAY)) {
        Graphics_drawSelectedButton(&g_sContext,&bt_playButton);
        UART_transmitStringNullTerm("AUDIO: PL\r\n");
        playPauseFlag = PAUSE;
        Delay(10000);
        Graphics_drawButton(&g_sContext,&bt_pauseButton);
      } else if(Graphics_isButtonSelected(&bt_pauseButton, g_sTouchContext.x,  g_sTouchContext.y) && (playPauseFlag == PAUSE)) {
        Graphics_drawSelectedButton(&g_sContext,&bt_pauseButton);
        UART_transmitStringNullTerm("AUDIO: PA\r\n");
        playPauseFlag = PLAY;
        Delay(10000);
        Graphics_drawButton(&g_sContext,&bt_playButton);
      } else {
        for(uint16_t i = 0;i<NUMBER_OF_DEVICES;i++) {
          if((int16_t)availableDeviceButtons[i].yMin >= 55 && (int16_t)availableDeviceButtons[i].yMax <= 215 && Graphics_isButtonSelected(&availableDeviceButtons[i], g_sTouchContext.x,  g_sTouchContext.y)) {
            Graphics_drawSelectedButton(&g_sContext, &availableDeviceButtons[i]);
            UART_transmitString("BT: OS ");
            char temp[3];
            ltoa((i+1), temp); 
            UART_transmitString(temp);
            UART_transmitStringNullTerm("\r\n");
            Delay(10000);
            Graphics_drawButton(&g_sContext,&availableDeviceButtons[i]);
          }
        }
      }
    }
    if(queue_head(rxUartCmdQueue) != NULL && queue_head(rxUartCmdQueue)->isComplete) {
      node* currentRxUartCmd = queue_head(rxUartCmdQueue);
      if(!currentRxUartCmd->isConsumed){
        if(simple_strcmp(currentRxUartCmd->uartString, "DEVICE LIST: #", 14))
          bt_ActiceDevices = *(currentRxUartCmd->uartString+14);
        else if(simple_strcmp(currentRxUartCmd->uartString, "DL: ", 4)) {
          if(bt_CurrentReceivedDevice < bt_ActiceDevices)
            strcpy(bt_deviceNames[bt_CurrentReceivedDevice++], currentRxUartCmd->uartString);
          if(bt_CurrentReceivedDevice >= bt_ActiceDevices) {
            bt_CurrentReceivedDevice = 0;
          }
        }
        currentRxUartCmd->isConsumed = true;
      } else
        queue_pop(rxUartCmdQueue);
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

  for(uint16_t i = 0; i<NUMBER_OF_DEVICES; i++) {
    availableDeviceButtons[i].xMin = 65;
    availableDeviceButtons[i].xMax = 255;
    availableDeviceButtons[i].yMin = (55 + 40 * i) + 1;
    availableDeviceButtons[i].yMax = (95 + 40 * i) - 1;
    availableDeviceButtons[i].borderWidth = 1;
    availableDeviceButtons[i].selected = false;
    if(i & 0x0001) {
    availableDeviceButtons[i].fillColor = GRAPHICS_COLOR_WHITE;
    availableDeviceButtons[i].borderColor = GRAPHICS_COLOR_BLUE;
    availableDeviceButtons[i].selectedColor = GRAPHICS_COLOR_BLACK;
    availableDeviceButtons[i].textColor = GRAPHICS_COLOR_BLACK;
    availableDeviceButtons[i].selectedTextColor = GRAPHICS_COLOR_RED;
    } else {
      availableDeviceButtons[i].fillColor = GRAPHICS_COLOR_BLUE;
      availableDeviceButtons[i].borderColor = GRAPHICS_COLOR_WHITE;
      availableDeviceButtons[i].selectedColor = GRAPHICS_COLOR_BLACK;
      availableDeviceButtons[i].textColor = GRAPHICS_COLOR_BLACK;
      availableDeviceButtons[i].selectedTextColor = GRAPHICS_COLOR_RED;
    }
    availableDeviceButtons[i].textXPos = 137;
    availableDeviceButtons[i].textYPos = 70 + 40 * i;
    ltoa((i+1), bt_deviceNames[i]);
    availableDeviceButtons[i].text = bt_deviceNames[i];
    availableDeviceButtons[i].font = &g_sFontCm18;

  }

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

  bt_scrollUpButton.xMin = 280;
  bt_scrollUpButton.xMax = 320;
  bt_scrollUpButton.yMin = 60;
  bt_scrollUpButton.yMax = 120;
  bt_scrollUpButton.borderWidth = 1;
  bt_scrollUpButton.selected = false;
  bt_scrollUpButton.fillColor = GRAPHICS_COLOR_WHITE;
  bt_scrollUpButton.borderColor = GRAPHICS_COLOR_BLUE;
  bt_scrollUpButton.selectedColor = GRAPHICS_COLOR_BLACK;
  bt_scrollUpButton.textColor = GRAPHICS_COLOR_BLACK;
  bt_scrollUpButton.selectedTextColor = GRAPHICS_COLOR_GREEN;
  bt_scrollUpButton.textXPos = 285;
  bt_scrollUpButton.textYPos = 90;
  bt_scrollUpButton.text = "UP";
  bt_scrollUpButton.font = &g_sFontCm18;

  bt_scrollDownButton.xMin = 280;
  bt_scrollDownButton.xMax = 320;
  bt_scrollDownButton.yMin = 120;
  bt_scrollDownButton.yMax = 180;
  bt_scrollDownButton.borderWidth = 1;
  bt_scrollDownButton.selected = false;
  bt_scrollDownButton.fillColor = GRAPHICS_COLOR_WHITE;
  bt_scrollDownButton.borderColor = GRAPHICS_COLOR_BLUE;
  bt_scrollDownButton.selectedColor = GRAPHICS_COLOR_BLACK;
  bt_scrollDownButton.textColor = GRAPHICS_COLOR_BLACK;
  bt_scrollDownButton.selectedTextColor = GRAPHICS_COLOR_GREEN;
  bt_scrollDownButton.textXPos = 285;
  bt_scrollDownButton.textYPos = 150;
  bt_scrollDownButton.text = "DOWN";
  bt_scrollDownButton.font = &g_sFontCm18;
}


void drawAvailableDeviceButtons(void) {
  for(uint16_t i = 0;i<NUMBER_OF_DEVICES;i++){
    if((int16_t)availableDeviceButtons[i].yMin >= 55 && (int16_t)availableDeviceButtons[i].yMax <= 215)
      Graphics_drawButton(&g_sContext,&availableDeviceButtons[i]);
  }
}


uint8_t scrollUpAvailableDeviceButtons(void) {
  if(bt_scrollOffset < 12 ) {
    for(uint16_t i = 0;i<NUMBER_OF_DEVICES;i++){
      availableDeviceButtons[i].yMin -= 40;
      availableDeviceButtons[i].yMax -= 40;
      availableDeviceButtons[i].textYPos -= 40;
    }
    bt_scrollOffset++;
    return 1;
  }
  return 0;
}


uint8_t scrollDownAvailableDeviceButtons(void) {
  if(bt_scrollOffset > 0) {
    for(uint16_t i = 0;i<NUMBER_OF_DEVICES;i++){
      availableDeviceButtons[i].yMin += 40;
      availableDeviceButtons[i].yMax += 40;
      availableDeviceButtons[i].textYPos += 40;
    }
    bt_scrollOffset--;
    return 1;
  }
  return 0;
}
