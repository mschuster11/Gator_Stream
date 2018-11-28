/* --COPYRIGHT--,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/

/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
/* GrLib Includes */
#include "grlib.h"
#include "button.h"
#include "imageButton.h"
#include "radioButton.h"
#include "checkbox.h"
#include "touch_P401R.h"
#include "images/images.h"
#include "uart.h"
#include "personal/headers/animations.h"
#include "personal/headers/general.h"
#include "personal/headers/uart_utils.h"
#include "personal/headers/main_menu_ui.h"
#include "personal/headers/bluetooth_ui.h"
#include "personal/headers/audio_ui.h"
#include "personal/headers/queue.h"

  /* The following constants specifies the debug UART's baud rate.     */

extern touch_context g_sTouchContext;


//Touch screen context

extern Graphics_ImageButton audioOptionsButton;
extern Graphics_ImageButton btConnectivityButton;
// Graphic library context

extern queue* rxUartCmdQueue;
//Flag to know if a demo was run
bool g_ranDemo = false;

void initializeDemoButtons(void);

void runPrimitivesDemo(void);
void runImagesDemo(void);
void openBTMenu(void);
void drawRestarDemo(void);


void main(void) {

  sysInit();
  Remote_EnableUART(HRDWCFG_DEBUG_UART_MODULE, HRDWCFG_DEBUG_UART_INT_NUM, DEBUG_UART_BAUD_RATE);

  initMainMenuButtons();
  initAudioMenuButtons();
  initBTMenuButtons();
  drawMainMenu();
  rxUartCmdQueue = queue_InitQueue();
  // Loop to detect touch
  for(;;) {
    touch_updateCurrentTouch(&g_sTouchContext);

    if(g_sTouchContext.touch) {
      if(Graphics_isImageButtonSelected(&audioOptionsButton, g_sTouchContext.x,  g_sTouchContext.y)){
        Graphics_drawSelectedImageButton(&g_sContext,&audioOptionsButton);
        openAudioMenu();
      } else if(Graphics_isImageButtonSelected(&btConnectivityButton, g_sTouchContext.x,  g_sTouchContext.y)){
        Graphics_drawSelectedImageButton(&g_sContext,&btConnectivityButton);
        openBTMenu();
      }

       if(g_ranDemo == true) {

        g_ranDemo = false;

        drawMainMenu();
       }
    }
  }

}


void runPrimitivesDemo(void) {
  Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
  Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
  Graphics_clearDisplay(&g_sContext);
  Graphics_setForegroundColor(&g_sContext, ClrLawnGreen);
  Graphics_setBackgroundColor(&g_sContext, ClrBlack);
  Graphics_drawStringCentered(&g_sContext, "Rainbow of Colored Lines", AUTO_STRING_LENGTH, 159, 15, TRANSPARENT_TEXT);
  drawLoadingAnimation(10);
  g_ranDemo = true;
}
