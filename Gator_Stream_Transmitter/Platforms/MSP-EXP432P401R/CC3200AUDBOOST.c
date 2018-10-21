//*****************************************************************************
// CC3200AUDBOOST.c
//
// Driver for TI TLV320AIC3254 ADC
//
// Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
//
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//    Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the
//    distribution.
//
//    Neither the name of Texas Instruments Incorporated nor the names of
//    its contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup audio_app
//! @{
//
//*****************************************************************************
/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* Other Includes */
#include "CC3200AUDBOOST.h"
#include "HRDWCFG.h"
#include "BTPSKRNL.h"

//******************************************************************************
//
// Writes to specified register
// ucRegAddr - 8 bit Register Address
// ucRegValue - 8 bit Register Value
//
//******************************************************************************
unsigned long adcRegWrite(unsigned char ucRegAddr,unsigned char ucRegValue) {
  /* Make sure the last transaction has been completely sent out.     */
  while(I2C_masterIsStopSent(HRDWCFG_I2C_MODULE) == EUSCI_B_I2C_SENDING_STOP);

  /* Send twice the first data byte to clean out whatever is in       */
  /* the buffer from a previous send.                                 */
  I2C_masterSendMultiByteStart(HRDWCFG_I2C_MODULE, ucRegAddr);
  I2C_masterSendMultiByteNext(HRDWCFG_I2C_MODULE, ucRegAddr);
  I2C_masterSendMultiByteFinish(HRDWCFG_I2C_MODULE, ucRegValue);
  
  /* Wait until the transaction completes.                            */
  while(I2C_isBusBusy(HRDWCFG_I2C_MODULE) == UCBBUSY);

  /* Delay for a short amount of time, I2C hang ups were seen when    */
  /* this delay was not included.  Note that this time could probably */
  /* be made much shorter if it every becomes an issue, originally the*/
  /* delay was 10s of microseconds.                                   */
  BTPS_Delay(20);

  return(0);
}

//******************************************************************************
//
// Selects the Page for reading/writing
//
//******************************************************************************
unsigned long adcPageSelect(unsigned char ucPageAddress) {
  return adcRegWrite(PAGE_CTRL_REG,ucPageAddress);
}

//******************************************************************************
//
// ADC Soft Reset
//
//******************************************************************************
void adcReset(void) {
  /* Select page 0	*/
  adcPageSelect(0);

  /* Soft RESET	*/
  adcRegWrite(ADC_REG_SOFT_RESET,1);
}

//******************************************************************************
//
// ADC Configure
//
//******************************************************************************
void adcInit(void) {

	/*	Initialization	*/
  adcReset();

  /*	Digital Configuration
   * 		-> PLL_CLK = (PLL_CLKIN x R x J.D)/P
   * 			-> ADC_CLKIN = PLL_CLK
   * 				-> ADC_Fs = ADC_CLK_IN / (NADC.MADC.AOSR)
   * 	Options:
   * 		-> 84.672MHz = (3.528MHz  x 1 x 24.0)/1 --> For Fs = 44.1KHz
   * 		-> 92.160MHz = (3.840MHz  x 1 x 24.0)/1 --> For Fs = 48KHz
   * 				-> 44.1KHz = 84.672MHz / (3.5.128)
   * 				-> 48KHz = 92.160MHz / (3.5.128)
   * 	Considerations:
   * 		-> MDAC * xOSR >= ResourceClass * 32
   *			- 5 * 128 (640) >= 8 * 32 (256)
   *    -> 2.8MHz < xOSR x ADC_Fs < 6.758MHz
   *      - 128 * 44,100 = 5.644MHz
   *      - 128 * 48,000 = 6.144MHz 
   *    -> xOSR must be a multiple of 8 (48KHz High-Performance)
   *      -> xOSR = 64, for Low-Power Mode
   *      -> xOSR = 128, for High-Performance Mode
   *    -> ADC_CLKIN = NADC x MADC x AOSR x ADC_fs
   *      -> 84.672MHz = 3 * 5 * 128 * 44100

   */
  adcPageSelect(0);
  // P0, R4, b3-2.       	PLL_CLKIN = BCLK (01)
  // P0, R4, b1-0.       	ADC_CLKIN = PLL_CLK (11)
  adcRegWrite(ADC_REG_CLK_MUX, 0x07);
  // P0, R5, b7.         	PLL = Power Up (1)
  // P0, R5, b6-4.       	PLL Divider P = 1 (001)
  // P0, R5, b3-0.       	PLL Divider R = 1 (0001)
  adcRegWrite(ADC_REG_PLL_P_R, 0x91);
  // P0, R6, b5-0.       	PLL Divider J = 24 (11000)
  adcRegWrite(ADC_REG_PLL_J, 0x18);
  // P0, R7, b5-0. (MSB) 	PLL Divider D = 0
  // P0, R8, b7-0. (LSB)
  adcRegWrite(ADC_REG_PLL_D_MSB, 0x00);
  adcRegWrite(ADC_REG_PLL_D_LSB, 0x00);
  // P0, R18, b7.        	NADC = Power Up
  // P0, R18, b6-0.      	NADC = 3
  adcRegWrite(ADC_REG_NADC, 0x83);
  // P0, R19, b7.        	MADC = Power Up
  // P0, R19, b6-0.      	MADC = 5
  adcRegWrite(ADC_REG_MADC, 0x85);
  // P0, R20, b7-0.	  	AOSR = 128
  adcRegWrite(ADC_REG_AOSR, 0x80);
  // P0, R27, b7-6.      	Interface Mode = I2S Mode (00)
  // P0, R27, b5-4.      	Data Length = 16bits (00)
  // P0, R27, b3.        	BCLK = Input (0)
  // P0, R27, b2.        	WCLK = Input (0)
  // P0, R27, b0.        	DOUT = Output (0)
  adcRegWrite(ADC_REG_ADC_AUDIO_IC1, 0x40);
  // P0, R28, b7-0.      	Data Offset = 1
  adcRegWrite(ADC_REG_DSOP1, 0x01);
  // P0, R61, b4-0.     	ADC Processing Block: PRB_R1
  adcRegWrite(ADC_REG_ADC_PBS, 0x01);

  // Select Page 1
  adcPageSelect(1);
  // P1, R51, b6-5		MICBIAS = 2.0V (01)
  adcRegWrite(ADC_REG_MICBIAS_CTRL, 0x01);
  // P1, R52, b7-6		Differential pair of IN2L(P) & IN3L(M) = Not routed (11)
  // P1, R52, b5-4		IN3L-Left_MICPGA = Not routed (11)
  	// P1, R52, b3-2    IN2L-Left_MICPGA = 0dB IN2L(P) left channel input (00)
  adcRegWrite(ADC_REG_L_IN_SEL_L_PGA1, 0xF0);
  // P1, R54, b7   		Bypass left PGA = Bypass diabled (0)
  // P1, R54, b6      Left ADC channel unselected inputs weakly biased = Not biased (0)
  // P1, R54, b5-4    Differential pair of IN2L(P) & IN2R(M) = Not routed (11)
  // P1, R54, b3-2		Differential pair of IN2R(P) & IN3R(M) = Not routed (11)
  adcRegWrite(ADC_REG_L_IN_SEL_L_PGA2, 0x3C);
  // P1, R52, b7-6    Differential pair of IN2R(P) & IN3R(M) = Not routed (11)
  // P1, R52, b5-4    IN3R-Right_MICPGA = Not routed (11)
  // P1, R52, b3-2    IN2R-Right_MICPGA = 0dB IN2R(P) left channel input (00)
  adcRegWrite(ADC_REG_R_IN_SEL_R_PGA1, 0xF0);
  // P1, R54, b7      Bypass right PGA = Bypass diabled (0)
  // P1, R54, b6      Right ADC channel unselected inputs weakly biased = Not biased (0)
  // P1, R54, b5-4    Differential pair of IN2L(P) & IN2R(M) = Not routed (11)
  // P1, R54, b3-2    Differential pair of IN2L(P) & IN3L(M) = Not routed (11)
  adcRegWrite(ADC_REG_R_IN_SEL_R_PGA2, 0x3C);
  // P1, R59, b7        	Left_MICPGA is enabled (0)
  // P1, R59, b6-0      	Left_MICPGA Gain = +0dB (0)
  adcRegWrite(ADC_REG_L_ANLG_PGA_SETTING, 0x00);
  // P1, R60, b7        	Right_MICPGA is enabled (0)
  // P1, R60, b6-0      	Right_MICPGA Gain = +0dB (0)
  adcRegWrite(ADC_REG_R_ANLG_PGA_SETTING, 0x00);

  // Select Page 0
  adcPageSelect(0);
  // P0, R81, b7			Left_ADC = Power Up (1)
  // P0, R81, b6			Right_ADC = Power Up (1)
  // P0, R81, b1-0    ADC_Soft-Stepping = 1 step/ FSYNC (00)
  adcRegWrite(ADC_REG_DIGITAL_CTRL, 0xC0);
  // P0, R82, b7			Left_ADC = Un-muted (0)
  // P0, R82, b6-4		Left_ADC Fine Gain = 0dB (000)
  // P0, R82, b3      	Right_ADC = Un-muted (0)
  // P0, R82, b2-0      	Right_ADC Fine Gain = 0dB (000)
  adcRegWrite(ADC_REG_FINE_VOLUME_CTRL, 0x00);
}

//******************************************************************************
//
// ADC Configure
//
//******************************************************************************
unsigned char adcChangeVolume(char ucVol) {
  if((ucVol & 0x8000) || ((ucVol << 1) < -48) ||  ((ucVol << 1) > 80)) {
    return -1;
  }
    // Select Page 0
  adcPageSelect(0);
  // P0, R82, b6-0    Left ADC Gain = ucVol
  adcRegWrite(ADC_REG_LEFT_VOLUME_CTRL, ucVol);
  // P0, R82, b6-0    Right ADC Gain = ucVol
  adcRegWrite(ADC_REG_RIGHT_VOLUME_CTRL, ucVol);
  return 0;
}
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
