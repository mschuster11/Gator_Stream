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
Graphics_ImageButton primitiveButton;
Graphics_ImageButton imageButton;
Graphics_Button yesButton;
Graphics_Button noButton;

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
void drawRestarDemo(void);
void Remote_EnableUART(uint32_t ModuleInstance, uint32_t InterruptNumber, unsigned long BaudRate);
void UART_rxIsr(void);

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
	while(1) {
		touch_updateCurrentTouch(&g_sTouchContext);

		if(g_sTouchContext.touch) {
			if(Graphics_isImageButtonSelected(&primitiveButton, g_sTouchContext.x,  g_sTouchContext.y)){
				Graphics_drawSelectedImageButton(&g_sContext,&primitiveButton);
				runPrimitivesDemo();
			} else if(Graphics_isImageButtonSelected(&imageButton, g_sTouchContext.x,  g_sTouchContext.y)){
				Graphics_drawSelectedImageButton(&g_sContext,&imageButton);
				runImagesDemo();
			}

			if(g_ranDemo == true) {

				g_ranDemo = false;
//                for(uint16_t i = 0; i < UART_RX_CMD_BUF_SIZE ; i++){
//                    if(uartRxCmdBuf[i] == 0x00){
//                        break;
//                    }
//                    UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, uartRxCmdBuf[i]);
//                }
				UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, 'T');
				UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, 'h');
				UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, 'i');
				UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, 's');
				UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, ' ');
				UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, 'i');
				UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, 's');
				UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, ' ');
				UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, 'a');
				UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, ' ');
				UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, 't');
				UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, 'e');
				UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, 's');
				UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, 't');
				UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, '\b');
				UART_transmitData(HRDWCFG_DEBUG_UART_MODULE, 0x00);
				drawMainMenu();
			}
		}
	}

}

void initializeDemoButtons(void) {
	// Initiliaze primitives Demo Button
	primitiveButton.xPosition=20;
	primitiveButton.yPosition=50;
	primitiveButton.borderWidth=5;
	primitiveButton.selected=false;
	primitiveButton.imageWidth=Primitives_Button4BPP_UNCOMP.xSize;
	primitiveButton.imageHeight=Primitives_Button4BPP_UNCOMP.ySize;
	primitiveButton.borderColor=GRAPHICS_COLOR_WHITE;
	primitiveButton.selectedColor=GRAPHICS_COLOR_RED;
	primitiveButton.image=&Primitives_Button4BPP_UNCOMP;


	// Initiliaze images Demo Button
	imageButton.xPosition=180;
	imageButton.yPosition=50;
	imageButton.borderWidth=1;
	imageButton.selected=false;
	imageButton.imageWidth=bt_logo4BPP_UNCOMP.xSize;
	imageButton.imageHeight=bt_logo4BPP_UNCOMP.ySize;
	imageButton.borderColor=GRAPHICS_COLOR_BLACK;
	imageButton.selectedColor=GRAPHICS_COLOR_BLUE;
	imageButton.image=&bt_logo4BPP_UNCOMP;



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
	Graphics_drawImageButton(&g_sContext, &primitiveButton);

	// Draw Images image button
	Graphics_drawImageButton(&g_sContext, &imageButton);
}

void runPrimitivesDemo(void) {
	int16_t ulIdx;
	uint32_t color;

	Graphics_Rectangle myRectangle1 = { 10, 50, 155, 120};
	Graphics_Rectangle myRectangle2 = { 150, 100, 300, 200};
	Graphics_Rectangle myRectangle3 = { 0, 0, 319, 239};

	Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
	Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
	Graphics_clearDisplay(&g_sContext);
	Graphics_drawString(&g_sContext, "Draw Pixels & Lines", AUTO_STRING_LENGTH, 60, 5, TRANSPARENT_TEXT);
	Graphics_drawPixel(&g_sContext, 45, 45);
	Graphics_drawPixel(&g_sContext, 45, 50);
	Graphics_drawPixel(&g_sContext, 50, 50);
	Graphics_drawPixel(&g_sContext, 50, 45);
	Graphics_drawLine(&g_sContext, 60, 60, 200, 200);
	Graphics_drawLine(&g_sContext, 30, 200, 200, 60);
	Graphics_drawLine(&g_sContext, 0, Graphics_getDisplayHeight(&g_sContext) - 1,
			Graphics_getDisplayWidth(&g_sContext) - 1,
			Graphics_getDisplayHeight(&g_sContext) - 1);
	Delay(2000);
	Graphics_clearDisplay(&g_sContext);
	Graphics_drawStringCentered(&g_sContext, "Draw Rectangles", AUTO_STRING_LENGTH, 159, 15, TRANSPARENT_TEXT);
	Graphics_drawRectangle(&g_sContext, &myRectangle1);
	Graphics_fillRectangle(&g_sContext, &myRectangle2);
	// Text won't be visible on screen due to transparency (foreground colors match)
	Graphics_drawStringCentered(&g_sContext, "Normal Text", AUTO_STRING_LENGTH, 225, 120, TRANSPARENT_TEXT);
	// Text draws foreground and background for opacity
	Graphics_drawStringCentered(&g_sContext, "Opaque Text", AUTO_STRING_LENGTH, 225, 150, OPAQUE_TEXT);
	Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);

	Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_RED);
	// Text draws with inverted foreground color to become visible
	Graphics_drawStringCentered(&g_sContext, "Invert Text", AUTO_STRING_LENGTH, 225, 180, TRANSPARENT_TEXT);
	Delay(2000);
	Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
	Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
	// Invert the foreground and background colors
	Graphics_fillRectangle(&g_sContext, &myRectangle3);
	Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
	Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_RED);
	Graphics_drawStringCentered(&g_sContext, "Invert Colors", AUTO_STRING_LENGTH, 159, 15, TRANSPARENT_TEXT);
	Graphics_drawRectangle(&g_sContext, &myRectangle1);
	Graphics_fillRectangle(&g_sContext, &myRectangle2);
	// Text won't be visible on screen due to transparency
	Graphics_drawStringCentered(&g_sContext, "Normal Text", AUTO_STRING_LENGTH, 225, 120, TRANSPARENT_TEXT);
	// Text draws foreground and background for opacity
	Graphics_drawStringCentered(&g_sContext, "Opaque Text", AUTO_STRING_LENGTH, 225, 150, OPAQUE_TEXT);
	Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
	Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
	// Text draws with inverted color to become visible
	Graphics_drawStringCentered(&g_sContext, "Invert Text", AUTO_STRING_LENGTH, 225, 180, TRANSPARENT_TEXT);
	Delay(2000);
	Graphics_clearDisplay(&g_sContext);
	Graphics_drawStringCentered(&g_sContext, "Draw Circles", AUTO_STRING_LENGTH, 159, 15, TRANSPARENT_TEXT);
	Graphics_drawCircle(&g_sContext, 100, 100, 50);
	Graphics_fillCircle(&g_sContext, 200, 140, 70);
	Delay(2000);
	Graphics_clearDisplay(&g_sContext);
	// Add some more color
	Graphics_setForegroundColor(&g_sContext, ClrLawnGreen);
	Graphics_setBackgroundColor(&g_sContext, ClrBlack);
	Graphics_clearDisplay(&g_sContext);
	Graphics_drawStringCentered(&g_sContext, "Rainbow of Colored Lines", AUTO_STRING_LENGTH, 159, 15, TRANSPARENT_TEXT);
	// Draw a quarter rectangle sweep of lines from red to purple.
	for(ulIdx = 128; ulIdx >= 1; ulIdx--) {
		// Red Color
		* ((uint16_t*) (&color)+1)  = 255;
		// Blue and Green Colors
		* ((uint16_t*) (&color))  = ((((128 - ulIdx) * 255) >> 7) << ClrBlueShift);

		Graphics_setForegroundColor(&g_sContext, color);
		Graphics_drawLine(&g_sContext, 160, 200, 32, ulIdx + 72);
	}
	// Draw a quarter rectangle sweep of lines from purple to blue.
	for(ulIdx = 128; ulIdx >= 1; ulIdx--) {
		// Red Color
		* ((uint16_t*) (&color)+1)  = (ulIdx * 255) >> 7;
		// Blue and Green Colors
		* ((uint16_t*) (&color))  = 255 << ClrBlueShift;

		Graphics_setForegroundColor(&g_sContext, color);
		Graphics_drawLine(&g_sContext, 160, 200, 160 - ulIdx, 72);
	}
	// Clear Red Color
	* ((uint16_t*) (&color)+1)  = 0;
	// Draw a quarter rectangle sweep of lines from blue to teal.
	for(ulIdx = 128; ulIdx >= 1; ulIdx--) {
		// Blue and Green Colors
		* ((uint16_t*) (&color))  = ((((128 - ulIdx) * 255) >> 7) << ClrGreenShift) | (255 << ClrBlueShift);

		Graphics_setForegroundColor(&g_sContext, color);
		Graphics_drawLine(&g_sContext, 160, 200, 288 - ulIdx, 72);
	}
	// Draw a quarter rectangle sweep of lines from teal to green.
	for(ulIdx = 128; ulIdx >= 0; ulIdx--) {
		// Blue and Green Colors
		* ((uint16_t*) (&color))  = (255 << ClrGreenShift) | (((ulIdx * 255) >> 7) << ClrBlueShift);

		Graphics_setForegroundColor(&g_sContext, color);
		Graphics_drawLine(&g_sContext, 160, 200, 288, 200 - (ulIdx));
	}
	Delay(2000);
	g_ranDemo = true;

	drawRestarDemo();

}



void runImagesDemo(void) {
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

	drawRestarDemo();
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
	uint32_t i=0;
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
