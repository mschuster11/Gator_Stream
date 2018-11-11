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
#include <math.h>
/* GrLib Includes */
#include "grlib.h"
#include "button.h"
#include "imageButton.h"
#include "radioButton.h"
#include "checkbox.h"
#include "LcdDriver/kitronix320x240x16_ssd2119_spi.h"
#include "images/images.h"
#include "touch_P401R.h"
#include "uart.h"

  /* The following constants specifies the debug UART's baud rate.     */
#define DEBUG_UART_BAUD_RATE        9600

  /* The following constants define interrupt priorities used by this  */
  /* module. Note that the there are 8 bits used to set the priority   */
  /* level, but only the upper 3 bits are currently used by MSP432     */
  /* MCUs, please see the MSP432 Technical Reference Manual and the    */
  /* MSP432 Driver Library's User's Guide for more information.        */
#define PRIORITY_HIGH               (0)
#define PRIORITY_NORMAL             (1 << 5)
#define HRDWCFG_DEBUG_UART_MODULE         EUSCI_A3_BASE
#define HRDWCFG_DEBUG_UART_INT_NUM        INT_EUSCIA3

#define HRDWCFG_DEBUG_UART_TX_PORT_NUM    GPIO_PORT_P9
#define HRDWCFG_DEBUG_UART_TX_PIN_NUM     GPIO_PIN7

#define HRDWCFG_DEBUG_UART_RX_PORT_NUM    GPIO_PORT_P9
#define HRDWCFG_DEBUG_UART_RX_PIN_NUM     GPIO_PIN6
#define UART_RX_CMD_BUF_SIZE    100
//Touch screen context
touch_context g_sTouchContext;
Graphics_ImageButton audioOptionsButton;
Graphics_ImageButton btConnectivityButton;
Graphics_Button yesButton;
Graphics_Button noButton;
Graphics_Button backButton;
Graphics_Button choiceButton1;
Graphics_Button choiceButton2;
Graphics_Button choiceButton3;
Graphics_Button disconnectButton;

// Graphic library context
Graphics_Context g_sContext;
uint16_t bufIndex = 0;
//Flag to know if a demo was run
bool g_ranDemo = false;
char uartRxCmdBuf[UART_RX_CMD_BUF_SIZE];
void Delay(uint16_t msec);
void boardInit(void);
void clockInit(void);
void initializeDemoButtons(void);
void drawMainMenu(void);
void runPrimitivesDemo(void);
void runImagesDemo(void);
void openBTMenu(void);
void drawRestarDemo(void);
void Remote_EnableUART(uint32_t ModuleInstance, uint32_t InterruptNumber, unsigned long BaudRate);
void UART_rxIsr(void);
void drawLoadingAnimation(uint16_t seconds);
void UART_transmitString(char* s);

void main(void) {
//  int16_t ulIdx;
  WDT_A_hold(__WDT_A_BASE__);

  /* Initialize the demo. */
  boardInit();
  clockInit();
  initializeDemoButtons();

  /* Globally enable interrupts. */
  __enable_interrupt();

  // LCD setup using Graphics Library API calls
  Kitronix320x240x16_SSD2119Init();
  Graphics_initContext(&g_sContext, &g_sKitronix320x240x16_SSD2119);
  Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
  Graphics_setFont(&g_sContext, &g_sFontCmss20b);
  Graphics_clearDisplay(&g_sContext);

  touch_initInterface();

  drawMainMenu();
  GPIO_setAsPeripheralModuleFunctionOutputPin(HRDWCFG_DEBUG_UART_TX_PORT_NUM, HRDWCFG_DEBUG_UART_TX_PIN_NUM, GPIO_PRIMARY_MODULE_FUNCTION);
  GPIO_setAsPeripheralModuleFunctionInputPin(HRDWCFG_DEBUG_UART_RX_PORT_NUM, HRDWCFG_DEBUG_UART_RX_PIN_NUM, GPIO_PRIMARY_MODULE_FUNCTION);
  Remote_EnableUART(HRDWCFG_DEBUG_UART_MODULE, HRDWCFG_DEBUG_UART_INT_NUM, DEBUG_UART_BAUD_RATE);
  Interrupt_setPriority(HRDWCFG_DEBUG_UART_INT_NUM, PRIORITY_NORMAL);

  // Loop to detect touch
  for(;;) {
    touch_updateCurrentTouch(&g_sTouchContext);

    if(g_sTouchContext.touch) {
      if(Graphics_isImageButtonSelected(&audioOptionsButton, g_sTouchContext.x,  g_sTouchContext.y)){
        Graphics_drawSelectedImageButton(&g_sContext,&audioOptionsButton);
        runPrimitivesDemo();
      } else if(Graphics_isImageButtonSelected(&btConnectivityButton, g_sTouchContext.x,  g_sTouchContext.y)){
        Graphics_drawSelectedImageButton(&g_sContext,&btConnectivityButton);
        openBTMenu();
        UART_transmitString("This is a test");
      }

       if(g_ranDemo == true) {

        g_ranDemo = false;

        UART_transmitString("This is a test");
        drawMainMenu();
       }
    }
  }

}

void initializeDemoButtons(void) {
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

  yesButton.xMin = 80;
  yesButton.xMax = 150;
  yesButton.yMin = 100;
  yesButton.yMax = 140;
  yesButton.borderWidth = 1;
  yesButton.selected = false;
  yesButton.fillColor = GRAPHICS_COLOR_RED;
  yesButton.borderColor = GRAPHICS_COLOR_RED;
  yesButton.selectedColor = GRAPHICS_COLOR_BLACK;
  yesButton.textColor = GRAPHICS_COLOR_BLACK;
  yesButton.selectedTextColor = GRAPHICS_COLOR_RED;
  yesButton.textXPos = 100;
  yesButton.textYPos = 110;
  yesButton.text = "YES";
  yesButton.font = &g_sFontCm18;

  noButton.xMin = 180;
  noButton.xMax = 250;
  noButton.yMin = 100;
  noButton.yMax = 140;
  noButton.borderWidth = 1;
  noButton.selected = false;
  noButton.fillColor = GRAPHICS_COLOR_RED;
  noButton.borderColor = GRAPHICS_COLOR_RED;
  noButton.selectedColor = GRAPHICS_COLOR_BLACK;
  noButton.textColor = GRAPHICS_COLOR_BLACK;
  noButton.selectedTextColor = GRAPHICS_COLOR_RED;
  noButton.textXPos = 200;
  noButton.textYPos = 110;
  noButton.text = "NO";
  noButton.font = &g_sFontCm18;

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
  choiceButton1.textXPos = 160;
  choiceButton1.textYPos = 75;
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
  choiceButton2.textXPos = 160;
  choiceButton2.textYPos = 120;
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
  choiceButton3.textXPos = 160;
  choiceButton3.textYPos = 180;
  choiceButton3.text = "3";
  choiceButton3.font = &g_sFontCm18;

  disconnectButton.xMin = 0;
  disconnectButton.xMax = 70;
  disconnectButton.yMin = 0;
  disconnectButton.yMax = 40;
  disconnectButton.borderWidth = 1;
  disconnectButton.selected = false;
  disconnectButton.fillColor = GRAPHICS_COLOR_RED;
  disconnectButton.borderColor = GRAPHICS_COLOR_RED;
  disconnectButton.selectedColor = GRAPHICS_COLOR_BLACK;
  disconnectButton.textColor = GRAPHICS_COLOR_BLACK;
  disconnectButton.selectedTextColor = GRAPHICS_COLOR_RED;
  disconnectButton.textXPos = 10;
  disconnectButton.textYPos = 10;
  disconnectButton.text = "Disconnect";
  disconnectButton.font = &g_sFontCm18;

}

void drawMainMenu(void) {
  Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_ORANGE);
  Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
  Graphics_clearDisplay(&g_sContext);
  Graphics_drawStringCentered(&g_sContext, "Gator Stream",
    AUTO_STRING_LENGTH,
    159,
    30,
    TRANSPARENT_TEXT);

  // Draw TI banner at the bottom of screnn
  // Graphics_drawImage(&g_sContext,
  //     &TI_platform_bar_red4BPP_UNCOMP,
  //     0,
  //     Graphics_getDisplayHeight(&g_sContext) - TI_platform_bar_red4BPP_UNCOMP.ySize);

  // Draw Primitives image button
  Graphics_drawImageButton(&g_sContext, &audioOptionsButton);

  // Draw Images image button
  Graphics_drawImageButton(&g_sContext, &btConnectivityButton);
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

//  drawRestarDemo();

}



void runImagesDemo(void) {
  drawLoadingAnimation(2);
  Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
  Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
  Graphics_clearDisplay(&g_sContext);
  // Graphics_drawStringCentered(&g_sContext, "Draw Uncompressed Image", AUTO_STRING_LENGTH, 159, 200, TRANSPARENT_TEXT);
  // Delay(2000);
  // Draw Image on the display
  Graphics_drawImage(&g_sContext, &maserati8BPP_UNCOMP, 0, 0);
  Delay(100);
  Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
  Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
  Graphics_clearDisplay(&g_sContext);
  Graphics_drawStringCentered(&g_sContext, "Draw RLE4 compressed Image", AUTO_STRING_LENGTH, 159, 200, TRANSPARENT_TEXT);
  Delay(100);
  Graphics_drawImage(&g_sContext, &TI_logo_150x1501BPP_COMP_RLE4, 85, 45);
  Delay(100);

  g_ranDemo = true;

//  drawRestarDemo();
}
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


  for(;;){
    touch_updateCurrentTouch(&g_sTouchContext);
    if(g_sTouchContext.touch) {
      if(Graphics_isButtonSelected(&backButton, g_sTouchContext.x,  g_sTouchContext.y)){
        Graphics_drawSelectedButton(&g_sContext, &yesButton);
        break;
      } else if(Graphics_isButtonSelected(&disconnectButton, g_sTouchContext.x,  g_sTouchContext.y)) {
        Graphics_drawSelectedButton(&g_sContext,&disconnectButton);
        UART_transmitString("CS");
        Graphics_drawButton(&g_sContext,&disconnectButton);
      } else if(Graphics_isButtonSelected(&choiceButton1, g_sTouchContext.x,  g_sTouchContext.y)) {
        Graphics_drawSelectedButton(&g_sContext,&choiceButton1);
        UART_transmitString("OS 1");
        Graphics_drawButton(&g_sContext,&choiceButton1);
      } else if(Graphics_isButtonSelected(&choiceButton2, g_sTouchContext.x,  g_sTouchContext.y)) {
        Graphics_drawSelectedButton(&g_sContext,&choiceButton2);
        UART_transmitString("OS 2");
        Graphics_drawButton(&g_sContext,&choiceButton2);
      } else if(Graphics_isButtonSelected(&choiceButton3, g_sTouchContext.x,  g_sTouchContext.y)) {
        Graphics_drawSelectedButton(&g_sContext,&choiceButton3);
        UART_transmitString("OS 3");
        Graphics_drawButton(&g_sContext,&choiceButton3);
      }
    } 
  }
  Graphics_clearDisplay(&g_sContext);
  drawMainMenu();
}

void drawRestarDemo(void) {
  g_ranDemo = false;
  Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
  Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
  Graphics_clearDisplay(&g_sContext);
  Graphics_drawStringCentered(&g_sContext, "Would you like to go back",
    AUTO_STRING_LENGTH,
    159,
    45,
    TRANSPARENT_TEXT);
  Graphics_drawStringCentered(&g_sContext, "to the main menu?",
    AUTO_STRING_LENGTH,
    159,
    65,
    TRANSPARENT_TEXT);
  Graphics_drawStringCentered(&g_sContext, (int8_t*)uartRxCmdBuf,
    AUTO_STRING_LENGTH,
    159,
    85,
    TRANSPARENT_TEXT);

  // Draw Primitives image button
  Graphics_drawButton(&g_sContext, &yesButton);

  // Draw Images image button
  Graphics_drawButton(&g_sContext, &noButton);

  do{
    touch_updateCurrentTouch(&g_sTouchContext);
    if(Graphics_isButtonSelected(&noButton, g_sTouchContext.x,  g_sTouchContext.y)) {
      Graphics_drawSelectedButton(&g_sContext, &noButton);
      g_ranDemo = true;
    }else{
      if(g_ranDemo) {
        Graphics_drawReleasedButton(&g_sContext, &noButton);
        g_ranDemo = false;
      }
    }
  }while(!Graphics_isButtonSelected(&yesButton, g_sTouchContext.x,  g_sTouchContext.y));

  Graphics_drawSelectedButton(&g_sContext, &yesButton);

  g_ranDemo = true;
  Delay(1000);

}


void boardInit() {
  FPU_enableModule();
}

void clockInit(void) {
  /* 2 flash wait states, VCORE = 1, running off DC-DC, 48 MHz */
  FlashCtl_setWaitState( FLASH_BANK0, 2);
  FlashCtl_setWaitState( FLASH_BANK1, 2);
  PCM_setPowerState( PCM_AM_DCDC_VCORE1 );
  CS_setDCOCenteredFrequency( CS_DCO_FREQUENCY_48 );
  CS_setDCOFrequency(48000000);
  CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, 1);
  CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, 1);
  CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, 1);

  return;
}


void Delay(uint16_t msec){
  volatile uint32_t i=0;
  uint32_t time=(msec/1000)*(SYSTEM_CLOCK_SPEED/15);

  for(i=0;i<time;i++);
}

/* The following function enables a UART and configures it to the    */
/* specified baud rate.  Note that this function enables the UART Rx */
/* interrupt, but it is up to the caller to enable the UART Tx       */
/* interrupt.                                                        */
void Remote_EnableUART(uint32_t ModuleInstance, uint32_t InterruptNumber, unsigned long BaudRate) {
/* Set the UART configuration parameters.                            */
eUSCI_UART_Config UARTConfig =
{
  EUSCI_A_UART_CLOCKSOURCE_SMCLK,                 /* SMCLK Source    */
  312,                                            /* BRDIV = 312     */
  8,                                              /* UCxBRF = 8      */
  0,                                              /* UCxBRS = 0      */
  EUSCI_A_UART_NO_PARITY,                         /* No Parity       */
  EUSCI_A_UART_LSB_FIRST,                         /* MSB First       */
  EUSCI_A_UART_ONE_STOP_BIT,                      /* One stop bit    */
  EUSCI_A_UART_MODE,                              /* UART mode       */
  EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION   /* Low Frequency   */
};

/* Configure the UART using the procedure described in "22.3.1       */
/* eUSCI_A Initialization and Reset" of the MSP432P4xx Family        */
/* Technical Reference Manual.                                       */
UART_disableModule(ModuleInstance);

/* Initialize the UART.                                              */
UART_initModule(ModuleInstance, &UARTConfig);

/* Enable the UART.                                                  */
UART_enableModule(ModuleInstance);

/* Enable the receive interrupt.                                     */
UART_enableInterrupt(ModuleInstance, EUSCI_A_UART_RECEIVE_INTERRUPT);

/* Enable the eUSCI interrupt.                                       */
Interrupt_enableInterrupt(InterruptNumber);
}

/* The following function disables a UART.                           */
void HAL_DisableUART(uint32_t ModuleInstance, uint32_t InterruptNumber) {
Interrupt_disableInterrupt(InterruptNumber);
UART_disableModule(ModuleInstance);
}

void UART_rxIsr(void) {
  uartRxCmdBuf[bufIndex] = UART_receiveData(EUSCI_A3_BASE);
  if(bufIndex >= UART_RX_CMD_BUF_SIZE || uartRxCmdBuf[bufIndex] == 0x00) {
   bufIndex = 0;
  } else {
   bufIndex++;
  }
}

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
      Delay(100);
    }
    for(ulIdx = 256; ulIdx >= 1; ulIdx--) {
      // Red Color
      * ((uint16_t*) (&color)+1)  = (ulIdx/2 * 255) >> 7;
      // Blue and Green Colors
      * ((uint16_t*) (&color))  = 255 << ClrBlueShift;

      Graphics_setForegroundColor(&g_sContext, color);
      Graphics_drawLine(&g_sContext, 160, 120, (160 + (int16_t) (12.5f * cosf((float)ulIdx * ((M_PI * 2.0f) / 256.0f)))), (120 + (int16_t) (12.5f * sinf((float)ulIdx * ((M_PI * 2.0f) / 256.0f)))));
      Delay(100);
    }
    // Clear Red Color
    * ((uint16_t*) (&color)+1)  = 0;
    for(ulIdx = 256; ulIdx >= 1; ulIdx--) {
      // Blue and Green Colors
      * ((uint16_t*) (&color))  = ((((128 - ulIdx/2) * 255) >> 7) << ClrGreenShift) | (255 << ClrBlueShift);

      Graphics_setForegroundColor(&g_sContext, color);
      Graphics_drawLine(&g_sContext, 160, 120, (160 + (int16_t) (12.5f * cosf((float)ulIdx * ((M_PI * 2.0f) / 256.0f)))), (120 + (int16_t) (12.5f * sinf((float)ulIdx * ((M_PI * 2.0f) / 256.0f)))));
      Delay(100);
    }
    for(ulIdx = 256; ulIdx >= 1; ulIdx--) {
      // Blue and Green Colors
      * ((uint16_t*) (&color))  = (255 << ClrGreenShift) | (((ulIdx/2 * 255) >> 7) << ClrBlueShift);

      Graphics_setForegroundColor(&g_sContext, color);
      Graphics_drawLine(&g_sContext, 160, 120, (160 + (int16_t) (12.5f * cosf((float)ulIdx * ((M_PI * 2.0f) / 256.0f)))), (120 + (int16_t) (12.5f * sinf((float)ulIdx * ((M_PI * 2.0f) / 256.0f)))));
      Delay(100);
    }
  }
}


void UART_transmitString(char* s) {
  uint16_t i;
  for(i = 0;s[i]!=0;i++)
    UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, s[i]);
  UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, 0x00);
}
