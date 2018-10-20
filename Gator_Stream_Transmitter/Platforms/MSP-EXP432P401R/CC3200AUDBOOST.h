//*****************************************************************************
// CC3200AUDBOOST.h
//
// Driver for TI TLV320AIC3254 CODEC
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
// CC3200AUDBOOST.h - Macros and Prototypes for TI TLV320AIC3254 CODEC
//
//*****************************************************************************
#ifndef __TI_CODEC_H__
#define __TI_CODEC_H__

//*****************************************************************************
//                          LOCAL DEFINES
//*****************************************************************************
#define SLAVE_ADDRESS					((0x30 >> 1))
#define PAGE_CTRL_REG   				0x00

//******************************************************************************
//
// Supported InputLine
//
//******************************************************************************

#define NO_INPUT						0x00
#define CODEC_ONBOARD_MIC				0x01
#define CODEC_MONO_IN					0x02
#define CODEC_LINE_IN					0x03

//******************************************************************************
//
// Supported OutputLine
//
//******************************************************************************

#define NO_OUTPUT						0x00
#define CODEC_LINE_OUT					0x01
#define CODEC_SPEAKER					0x02

//******************************************************************************
//
// Codec Page 0 Registers
//
//******************************************************************************

#define ADC_REG_SOFT_RESET            0x01
#define ADC_REG_CLK_MUX               0x04
#define ADC_REG_PLL_P_R               0x05
#define ADC_REG_PLL_J                 0x06
#define ADC_REG_PLL_D_MSB             0x07
#define ADC_REG_PLL_D_LSB             0x08
#define ADC_REG_NADC                  0x12
#define ADC_REG_MADC                  0x13
#define ADC_REG_AOSR                  0x14
#define ADC_REG_IADC                  0x15
#define ADC_REG_DED                   0x16
#define ADC_REG_CLKOUT_MUX            0x19
#define ADC_REG_CLKOUT_DIV            0x1A
#define ADC_REG_ADC_AUDIO_IC1         0x1B
#define ADC_REG_DSOP1                 0x1C
#define ADC_REG_ADC_IC2               0x1D
#define ADC_REG_BCLK_DIV              0x1E
#define ADC_REG_2ND_AUDIO_IC1         0x1F
#define ADC_REG_2ND_AUDIO_IC2         0x20
#define ADC_REG_2ND_AUDIO_IC3         0x21
#define ADC_REG_I2S_SYNC              0x22
#define ADC_REG_ADC_FLAG_REG          0x24
#define ADC_REG_DSOP2                 0x25
#define ADC_REG_I2S_TDM_CR            0x26
#define ADC_REG_INT_FLAGS_OF1         0x2A
#define ADC_REG_INT_FLAGS_OF2         0x2B
#define ADC_REG_ADC_INT_FLAGS1        0x2D
#define ADC_REG_ADC_INT_FLAGS2        0x2F
#define ADC_REG_INT1_IC               0x30
#define ADC_REG_INT2_IC               0x31
#define ADC_REG_GPIO1_C               0x34
#define ADC_REG_DOUT_C                0x35
#define ADC_REG_ADC_SYNC_C1           0x39
#define ADC_REG_ADC_SYNC_C2           0x3A
#define ADC_REG_ADC_CIC_FGC           0x3B
#define ADC_REG_ADC_PBS               0x3D
#define ADC_REG_PIMC_BITS             0x3E
#define ADC_REG_DIGITAL_CTRL          0x51
#define ADC_REG_FINE_VOLUME_CTRL      0x52
#define ADC_REG_LEFT_VOLUME_CTRL      0x53
#define ADC_REG_RIGHT_VOLUME_CTRL     0x53
#define ADC_REG_PHASE_COMPENSATION    0x54


//******************************************************************************
//
// Codec Page 1 Registers
//
//******************************************************************************

#define ADC_REG_DITHER_CTRL           0x1A
#define ADC_REG_MICBIAS_CTRL          0x33
#define ADC_REG_L_IN_SEL_L_PGA1       0x34
#define ADC_REG_L_IN_SEL_L_PGA2       0x36
#define ADC_REG_R_IN_SEL_R_PGA1       0x37
#define ADC_REG_R_IN_SEL_R_PGA2       0x39
#define ADC_REG_L_ANLG_PGA_SETTING    0x3B
#define ADC_REG_R_ANLG_PGA_SETTING    0x3C
#define ADC_REG_LOW_CURRENT_MODES     0x3D
#define ADC_REG_ANLG_PGA_FLAGS        0x3E

// Prototypes for the APIs.
//
//******************************************************************************

extern unsigned long adcRegWrite(unsigned char ucRegAddr,unsigned char ucRegValue);
extern unsigned long adcPageSelect(unsigned char ucPageAddress);
extern void adcReset(void);
extern void adcInit(void);
extern unsigned char adcChangeVolume(unsigned char ucVol);

#endif
