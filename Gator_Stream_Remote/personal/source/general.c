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
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~Includes-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
#include "driverlib.h"
#include "touch_P401R.h"
#include "LcdDriver/kitronix320x240x16_ssd2119_spi.h"
#include "grlib.h"


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Globals */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
touch_context g_sTouchContext;
Graphics_Context g_sContext;


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* Function Definitions */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
static void boardInit() {
  FPU_enableModule();
}


static void clockInit(void) {
  /* 2 flash wait states, VCORE = 1, running off DC-DC, 48 MHz */
  FlashCtl_setWaitState(FLASH_BANK0, 2);
  FlashCtl_setWaitState(FLASH_BANK1, 2);
  PCM_setPowerState(PCM_AM_DCDC_VCORE1 );
  CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48 );
  CS_setDCOFrequency(48000000);
  CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, 1);
  CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, 1);
  CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, 1);
}


void sysInit(void) {
  WDT_A_hold(__WDT_A_BASE__);

  /* Initialize the demo. */
  boardInit();
  clockInit();
//  initializeDemoButtons();

  /* Globally enable interrupts. */
  __enable_interrupt();

  // LCD setup using Graphics Library API calls
  Kitronix320x240x16_SSD2119Init();
  Graphics_initContext(&g_sContext, &g_sKitronix320x240x16_SSD2119);
  Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
  Graphics_setFont(&g_sContext, &g_sFontCmss20b);
  Graphics_clearDisplay(&g_sContext);

  touch_initInterface();
}


void Delay(uint16_t usec){
  volatile uint32_t i=0;
  volatile uint32_t time= usec*48;

  for(i=0;i<time;i++);
}
