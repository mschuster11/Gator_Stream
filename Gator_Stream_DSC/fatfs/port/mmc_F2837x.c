/*-----------------------------------------------------------------------*/
/* MMC/SDC (in SPI mode) control module  (C)ChaN, 2007                   */
/*-----------------------------------------------------------------------*/
/* Only rcvr_spi(), xmit_spi(), disk_timerproc() and some macros         */
/* are platform dependent.                                               */
/*-----------------------------------------------------------------------*/

/*
 * This file was modified from a sample available from the FatFs
 * web site. It was modified to work with a F2837xD device.
 */

#include "F28x_Project.h"
#include "inc/hw_types.h"
#include "fatfs/src/diskio.h"

/*
 * The following header defines the hardware connections used to connect
 * the SDCard.  This can be found under the relevant board directory.
 */
//*****************************************************************************
//
// SDCard SPI port
//
//*****************************************************************************
/* @ SPI B                                                                   */
//*****************************************************************************
//
// GPIO for SDCard pins
//
//*****************************************************************************
/* @ CD   - O - GPIO                                                             */
/* @ DO   - O - GPIO64                                                           */
/* @ DI   - I - GPIO63                                                           */
/* @ CS   - I - GPIO130                                                          */
/* @ SCLK - I - GPIO65                                                           */
/* @ VCC  - ~ - Vcc                                                              */
/* @ GND  - ~ - GND                                                              */

/* Definitions for MMC/SDC command */
#define CMD0    (0x40+0)    /* GO_IDLE_STATE */
#define CMD1    (0x40+1)    /* SEND_OP_COND */
#define CMD8    (0x40+8)    /* SEND_IF_COND */
#define CMD9    (0x40+9)    /* SEND_CSD */
#define CMD10   (0x40+10)   /* SEND_CID */
#define CMD12   (0x40+12)   /* STOP_TRANSMISSION */
#define CMD16   (0x40+16)   /* SET_BLOCKLEN */
#define CMD17   (0x40+17)   /* READ_SINGLE_BLOCK */
#define CMD18   (0x40+18)   /* READ_MULTIPLE_BLOCK */
#define CMD23   (0x40+23)   /* SET_BLOCK_COUNT */
#define CMD24   (0x40+24)   /* WRITE_BLOCK */
#define CMD25   (0x40+25)   /* WRITE_MULTIPLE_BLOCK */
#define CMD41   (0x40+41)   /* SEND_OP_COND (ACMD) */
#define CMD55   (0x40+55)   /* APP_CMD */
#define CMD58   (0x40+58)   /* READ_OCR */
/*--------------------------------------------------------------------------

   Module Private Variables

---------------------------------------------------------------------------*/

static volatile DSTATUS Stat = STA_NOINIT;    // Disk status
static volatile BYTE Timer1, Timer2;          // 100Hz decrement timer
static BYTE CardType;                         // b0:MMC, b1:SDC, b2:Block addressing
static BYTE PowerFlag = 0;                    // indicates if "power" is on


/*--------------------------------------------------------------------------

   Module Private Functions

---------------------------------------------------------------------------*/

// asserts the CS pin to the card
static void SELECT (void) {
  // Select the SD card.
  GpioDataRegs.GPECLEAR.bit.GPIO130 = 1;

}


// De-asserts the CS pin to the card.
static void DESELECT (void) {
  GpioDataRegs.GPESET.bit.GPIO130 = 1;
}


/*-----------------------------------------------------------------------*/
/* Transmit a byte to MMC via SPI  (Platform dependent)                  */
/*-----------------------------------------------------------------------*/
static void xmit_spi (BYTE dat) {
  volatile DWORD rcvdat;                      //included file like integer.h for DWORD definition

  /* Write the data to the tx fifo */
  while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);    //Wait for room to write data
  SpibRegs.SPITXBUF = ((DWORD)dat)<<8;        //Write data

  /* flush data read during the write */
  while(SpibRegs.SPISTS.bit.INT_FLAG !=1);    //May be possible to switch to '!SpibRegs.SPISTS.bit.INT_FLAG'
  rcvdat = (SpibRegs.SPIRXBUF && 0xFF);       //Clear Receive Buffer
}


/*-----------------------------------------------------------------------*/
/* Receive a byte from MMC via SPI  (Platform dependent)                 */
/*-----------------------------------------------------------------------*/
static BYTE rcvr_spi (void) {
  volatile DWORD rcvdat;

  //Disable transmission channel
  //SpibRegs.SPICTL.bit.TALK = 0;

  /* write dummy data */
  while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);    //Wait for space to write
  SpibRegs.SPITXBUF = 0xFF00;                 //Write dummy data

  /* read data from RX fifo */
  while(SpibRegs.SPISTS.bit.INT_FLAG !=1);    //May be possible to switch to '!SpibRegs.SPISTS.bit.INT_FLAG'
  rcvdat = (SpibRegs.SPIRXBUF & 0xFF);        //Read Transferred data

  return (BYTE)rcvdat;
}


static void rcvr_spi_m (BYTE *dst) {
    *dst = rcvr_spi();
}


/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/
static BYTE wait_ready (void) {
  BYTE res;

  Timer2 = 50;    /* Wait for ready in timeout of 500ms */
  rcvr_spi();
  do {
      res = rcvr_spi();
      DELAY_US(175);
  } while ((res != 0xFF) && Timer2);

  return res;
}


/*-----------------------------------------------------------------------*/
/* Send 80 or so clock transitions with CS and DI held high. This is     */
/* required after card power up to get it into SPI mode                  */
/*-----------------------------------------------------------------------*/
static void send_initial_clock_train (void) {
  volatile DWORD dat;

  /* Ensure CS (STE) is held high. */
  DESELECT();

  /* Switch the SPI TX line to a GPIO and drive it high too. */
  EALLOW;
  GpioCtrlRegs.GPBLOCK.bit.GPIO63 = 0;
  GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 0;
  GpioCtrlRegs.GPBDIR.bit.GPIO63  = 1;
  GpioCtrlRegs.GPBLOCK.bit.GPIO63 = 1;
  EDIS;
  GpioDataRegs.GPBSET.bit.GPIO63  = 1;

  /* Send 10 bytes over the SPI. This causes the clock to wiggle the */
  /* required number of times. */
  for(unsigned int i = 0; i < 10; i++) {
      /* Write DUMMY data */
      while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
      SpibRegs.SPITXBUF = 0xFF00;

      /* Flush data read during data write. */
      while(SpibRegs.SPISTS.bit.INT_FLAG !=1);        //May be possible to switch to '!SpibRegs.SPISTS.bit.INT_FLAG'
      dat = (SpibRegs.SPIRXBUF & 0xFF);
  }

  /* Revert to hardware control of the SPI TX line. */
  EALLOW;
  GpioCtrlRegs.GPBLOCK.bit.GPIO63 = 0;
  GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 3;
  GpioCtrlRegs.GPBLOCK.bit.GPIO63 = 1;
  EDIS;
}


/*-----------------------------------------------------------------------*/
/* Power Control  (Platform dependent)                                   */
/*-----------------------------------------------------------------------*/
/* When the target system does not support socket power control, there   */
/* is nothing to do in these functions and chk_power always returns 1.   */
static void power_on (void) {
  /*
   * This doesn't really turn the power on, but initializes the
   * SPI port and pins needed to talk to the SD card.
   */
  EALLOW;
  /* Enable the peripherals used to drive the SDC on SPI */
  CpuSysRegs.PCLKCR8.bit.SPI_B = 1;

  /*
   * Configure the appropriate pins to be SPI instead of GPIO. The CS
   * signal is directly driven to ensure that we can hold it low through a
   * complete transaction with the SD card.
   */

  //Unlock the SD-Card SPI GPIOs
  GpioCtrlRegs.GPBLOCK.bit.GPIO63   = 0;
  GpioCtrlRegs.GPCLOCK.bit.GPIO64   = 0;
  GpioCtrlRegs.GPCLOCK.bit.GPIO65   = 0;
  GpioCtrlRegs.GPELOCK.bit.GPIO130  = 0;

  //Set up  MUX & DIR
  GpioCtrlRegs.GPEMUX1.bit.GPIO130  = 0;    //Leave as GPIO for manual CS control

  GpioCtrlRegs.GPBGMUX2.bit.GPIO63  = 3;
  GpioCtrlRegs.GPCGMUX1.bit.GPIO64  = 3;
  GpioCtrlRegs.GPCGMUX1.bit.GPIO65  = 3;

  GpioCtrlRegs.GPBMUX2.bit.GPIO63   = 3;
  GpioCtrlRegs.GPCMUX1.bit.GPIO64   = 3;
  GpioCtrlRegs.GPCMUX1.bit.GPIO65   = 3;

  GpioCtrlRegs.GPEDIR.bit.GPIO130   = 1;

  //Set up GPIO Pull-up disables/enables
  GpioCtrlRegs.GPBPUD.bit.GPIO63    = 0;    //Needs to be normally pulled high
  GpioCtrlRegs.GPCPUD.bit.GPIO64    = 0;    //Needs to be normally pulled high
  GpioCtrlRegs.GPCPUD.bit.GPIO65    = 1;
  GpioCtrlRegs.GPEPUD.bit.GPIO130   = 1;

  //Set up GPIOs in asynch mode
  GpioCtrlRegs.GPBQSEL2.bit.GPIO63  = 3; // Asynch input
  GpioCtrlRegs.GPCQSEL1.bit.GPIO64  = 3;
  GpioCtrlRegs.GPCQSEL1.bit.GPIO65  = 3;
  GpioCtrlRegs.GPEQSEL1.bit.GPIO130 = 3;

  //Configure GPIOs for CPU1
  GpioCtrlRegs.GPBCSEL4.bit.GPIO63  = 0;
  GpioCtrlRegs.GPCCSEL1.bit.GPIO64  = 0;
  GpioCtrlRegs.GPCCSEL1.bit.GPIO65  = 0;
  GpioCtrlRegs.GPECSEL1.bit.GPIO130 = 0;

  //Lock the SD-Card SPI GPIOs
  GpioCtrlRegs.GPBLOCK.bit.GPIO63   = 1;
  GpioCtrlRegs.GPCLOCK.bit.GPIO64   = 1;
  GpioCtrlRegs.GPCLOCK.bit.GPIO65   = 1;
  GpioCtrlRegs.GPELOCK.bit.GPIO130  = 1;
  EDIS;


  /* Deassert the SPIC chip selects for both the SD card and serial flash */
  DESELECT();

  /* Configure the SPI C port */
  SpibRegs.SPICCR.bit.SPISWRESET = 0;         //Set reset bit low
  SpibRegs.SPICTL.bit.CLK_PHASE = 0;
  SpibRegs.SPICCR.bit.CLKPOLARITY = 1;
  SpibRegs.SPICTL.bit.MASTER_SLAVE = 1;       //Master mode
  SpibRegs.SPIBRR.all = 63;                       //update value to proper setting for correct bitrate ( current: ~500kHz)
  SpibRegs.SPICCR.bit.SPICHAR = 0x7;          //Set char length to 8 bits
  SpibRegs.SPICTL.bit.TALK = 1;
  SpibRegs.SPICCR.bit.SPISWRESET = 1;         //Release SPI from reset
  SpibRegs.SPIPRI.bit.FREE = 1;
  SpibRegs.SPIPRI.bit.SOFT = 1;

  PowerFlag = 1;
}


// set the SPI speed to the max setting
static void set_max_speed(void) {
  /* Disable the SPI*/
  //DevCfgRegs.DC9.bit.SPI_C = 0;
  SpibRegs.SPICCR.bit.SPISWRESET = 0;     //Place in reset

  /* Configure the SPI C port */
  SpibRegs.SPICTL.bit.CLK_PHASE = 0;
  SpibRegs.SPICCR.bit.CLKPOLARITY = 1;
  SpibRegs.SPICTL.bit.MASTER_SLAVE = 1;   //Set Master Mode

  SpibRegs.SPIBRR.all = 1; // <- (No enforced limit)

  SpibRegs.SPICCR.bit.SPICHAR = 0x7;      //8 bit char length
  SpibRegs.SPICTL.bit.TALK = 1;

  /* Enable the SPI */
  SpibRegs.SPICCR.bit.SPISWRESET = 1;     //Release from reset
  //DevCfgRegs.DC9.bit.SPI_C = 1;
}


static void power_off (void) {
    PowerFlag = 0;
}


/* Socket power state: 0=off, 1=on */
static int chk_power(void) {
    return PowerFlag;
}


/*-----------------------------------------------------------------------*/
/* Receive a data packet from MMC                                        */
/* @ BYTE *buff - Data buffer to store received data                     */
/* @ UINT btr   - Byte count (must be even number)                       */
/*-----------------------------------------------------------------------*/
static BOOL rcvr_datablock (BYTE *buff, UINT btr) {
  BYTE token;

  Timer1 = 10;
  /* Wait for data packet in timeout of 100ms */
  do {
    token = rcvr_spi();
  }
  while ((token == 0xFF) && Timer1);
  if(token != 0xFE) return FALSE;    /* If not valid data token, retutn with error */

  /* Receive the data block into buffer */
  do {
    rcvr_spi_m(buff++);
    rcvr_spi_m(buff++);
  }
  while (btr -= 2);
  rcvr_spi();                        /* Discard CRC */
  rcvr_spi();

  return TRUE;                    /* Return with success */
}

/*-----------------------------------------------------------------------*/
/* Receive a data packet from MMC                                        */
/* @ BYTE *buff       - Data buffer to store received data               */
/* @ UINT wordsToRead - Word count (must be even number)                 */
/*-----------------------------------------------------------------------*/
static BOOL rcvr_datablock_pcm (BYTE *buff, UINT wordsToRead) {
  BYTE token;

  Timer1 = 10;
  /* Wait for data packet in timeout of 100ms */
  do {
    token = rcvr_spi();
  }
  while ((token == 0xFF) && Timer1);
  if(token != 0xFE) return FALSE;    /* If not valid data token, return with error */
  BYTE hi, low;
  /* Receive the data block into buffer */
  do {
    rcvr_spi_m(&low);
    rcvr_spi_m(&hi);
    *buff = hi << 8 | low;
    buff++;
  }
  while (wordsToRead--);
  rcvr_spi();                        /* Discard CRC */
  rcvr_spi();

  return TRUE;                    /* Return with success */
}


/*-----------------------------------------------------------------------*/
/* Send a data packet to MMC                                             */
/* @ const BYTE *buff - 512 byte data block to be transmitted            */
/* @ BYTE token       - Data/Stop token                                  */
/*-----------------------------------------------------------------------*/
#if _READONLY == 0
static BOOL xmit_datablock (const BYTE *buff, BYTE token) {
  BYTE resp, wc;

  if (wait_ready() != 0xFF) return FALSE;

  xmit_spi(token);                    /* Xmit data token */
  /* Is data token */
  if (token != 0xFD) {
    wc = 256;
    /* Xmit the 512 byte data block to MMC */
    do {
      xmit_spi(*buff++);
      xmit_spi(*buff++);
    }
    while (--wc);
    xmit_spi(0xFF);                    /* CRC (Dummy) */
    xmit_spi(0xFF);
    resp = rcvr_spi();                /* Reveive data response */
    if ((resp & 0x1F) != 0x05)        /* If not accepted, return with error */
      return FALSE;
  }

  return TRUE;
}
#endif /* _READONLY */


/*-----------------------------------------------------------------------*/
/* Send a data packet to MMC                                             */
/* @ const BYTE *buff - 512 byte data block to be transmitted            */
/* @ BYTE token       - Data/Stop token                                  */
/*-----------------------------------------------------------------------*/
#if _READONLY == 0
static BOOL xmit_datablock_pcm (const BYTE *buff, BYTE token) {
  BYTE resp, wc;

  if (wait_ready() != 0xFF) return FALSE;

  xmit_spi(token);                    /* Xmit data token */
  /* Is data token */
  if (token != 0xFD) {
    wc = 256;
    /* Xmit the 512 byte data block to MMC */
    do {
      xmit_spi((*buff & 0x00FF));
      xmit_spi((*buff >> 8));
      *buff++;
    }
    while (--wc);
    xmit_spi(0xFF);                    /* CRC (Dummy) */
    xmit_spi(0xFF);
    resp = rcvr_spi();                /* Receive data response */
    if ((resp & 0x1F) != 0x05)        /* If not accepted, return with error */
      return FALSE;
  }

  return TRUE;
}
#endif /* _READONLY */

/*-----------------------------------------------------------------------*/
/* Send a command packet to MMC                                          */
/* @ BYTE cmd  - Command byte                                            */
/* @ DWORD arg - Argument                                                */
/*-----------------------------------------------------------------------*/
static BYTE send_cmd ( BYTE cmd, DWORD arg) {
  BYTE n, res;

  if (wait_ready() != 0xFF) return 0xFF;

  /* Send command packet */
  xmit_spi(cmd);                        /* Command */
  xmit_spi((BYTE)(arg >> 24));        /* Argument[31..24] */
  xmit_spi((BYTE)(arg >> 16));        /* Argument[23..16] */
  xmit_spi((BYTE)(arg >> 8));            /* Argument[15..8] */
  xmit_spi((BYTE)arg);                /* Argument[7..0] */
  n = 0;
  if (cmd == CMD0) n = 0x95;            /* CRC for CMD0(0) */
  if (cmd == CMD8) n = 0x87;            /* CRC for CMD8(0x1AA) */
  xmit_spi(n);

  /* Receive command response */
  if (cmd == CMD12) rcvr_spi();        /* Skip a stuff byte when stop reading */
  n = 10;                                /* Wait for a valid response in timeout of 10 attempts */
  do {
    res = rcvr_spi();
  } while ((res & 0x80) && --n);

  return res;            /* Return with the response value */
}


/*-----------------------------------------------------------------------*
 * Send the special command used to terminate a multi-sector read.
 *
 * This is the only command which can be sent while the SDCard is sending
 * data. The SDCard spec indicates that the data transfer will stop 2 bytes
 * after the 6 byte CMD12 command is sent and that the card will then send
 * 0xFF for between 2 and 6 more bytes before the R1 response byte.  This
 * response will be followed by another 0xFF byte.  In testing, however, it
 * seems that some cards don't send the 2 to 6 0xFF bytes between the end of
 * data transmission and the response code.  This function, therefore, merely
 * reads 10 bytes and, if the last one read is 0xFF, returns the value of the
 * latest non-0xFF byte as the response code.
 *
 *-----------------------------------------------------------------------*/
static BYTE send_cmd12 (void) {
  BYTE n, res, val;

  /* For CMD12, we don't wait for the card to be idle before we send
   * the new command.
   */

  /* Send command packet - the argument for CMD12 is ignored. */
  xmit_spi(CMD12);
  xmit_spi(0);
  xmit_spi(0);
  xmit_spi(0);
  xmit_spi(0);
  xmit_spi(0);

  /* Read up to 10 bytes from the card, remembering the value read if it's
     not 0xFF */
  for(n = 0; n < 10; n++) {
    val = rcvr_spi();
    if(val != 0xFF) {
      res = val;
    }
  }

  return res;            /* Return with the response value */
}

/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/* @ BYTE drv - Physical drive number (0)                                */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (BYTE drv) {
  BYTE n, ty, ocr[4];

  if (drv) return STA_NOINIT;            /* Supports only single drive */
  if (Stat & STA_NODISK) return Stat;    /* No card in the socket */

  power_on();                            /* Force socket power on */
  send_initial_clock_train();

  SELECT();                /* CS = L */
  ty = 0;
  /* Enter Idle state */
  if (send_cmd(CMD0, 0) == 1) {
    Timer1 = 100;                        /* Initialization timeout of 1000 msec */
    /* SDC Ver2+ */
    if (send_cmd(CMD8, 0x1AA) == 1) {
      for (n = 0; n < 4; n++) ocr[n] = rcvr_spi();
      /* The card can work at vdd range of 2.7-3.6V */
      if (ocr[2] == 0x01 && ocr[3] == 0xAA) {
        do {
          if (send_cmd(CMD55, 0) <= 1 && send_cmd(CMD41, 1UL << 30) == 0)    break;    /* ACMD41 with HCS bit */
        } while (Timer1);
        /* Check CCS bit */
        if (Timer1 && send_cmd(CMD58, 0) == 0) {
          for (n = 0; n < 4; n++) ocr[n] = rcvr_spi();
          ty = (ocr[0] & 0x40) ? 6 : 2;
        }
      }
    }
    /* SDC Ver1 or MMC */
    else {
      ty = (send_cmd(CMD55, 0) <= 1 && send_cmd(CMD41, 0) <= 1) ? 2 : 1;    /* SDC : MMC */
      do {
        if (ty == 2) {
          if (send_cmd(CMD55, 0) <= 1 && send_cmd(CMD41, 0) == 0) break;    /* ACMD41 */
        }
        else {
          if (send_cmd(CMD1, 0) == 0) break;                                /* CMD1 */
        }
      }
      while (Timer1);
      if (!Timer1 || send_cmd(CMD16, 512) != 0)    /* Select R/W block length */
        ty = 0;
    }
  }
  CardType = ty;
  DESELECT();            /* CS = H */
  rcvr_spi();            /* Idle (Release DO) */

  /* Initialization succeed */
  if (ty) {
    Stat &= ~STA_NOINIT;        /* Clear STA_NOINIT */
    set_max_speed();
  }
  /* Initialization failed */
  else {
    power_off();
  }

  return Stat;
}


/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/* @ BYTE drv - Physical drive number (0)                                */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (BYTE drv) {
  if (drv) return STA_NOINIT;        /* Supports only single drive */
  return Stat;
}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/* @ BYTE drv,    - Physical drive nmuber (0)                            */
/* @ BYTE *buff,  - Pointer to the data buffer to store read data        */
/* @ DWORD sector - Start sector number (LBA)                            */
/* @ BYTE count   - Sector count (1..255)                                */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (BYTE drv, BYTE *buff, DWORD sector, BYTE count) {
  if (drv || !count) return RES_PARERR;
  if (Stat & STA_NOINIT) return RES_NOTRDY;

  if (!(CardType & 4)) sector *= 512;    /* Convert to byte address if needed */

  SELECT();            /* CS = L */
  /* Single block read */
  if (count == 1) {
    /* READ_SINGLE_BLOCK */
    if ((send_cmd(CMD17, sector) == 0) && rcvr_datablock(buff, 512))
      count = 0;
  }
  /* Multiple block read */
  else {
    /* READ_MULTIPLE_BLOCK */
    if (send_cmd(CMD18, sector) == 0) {
      do {
        if (!rcvr_datablock(buff, 512)) break;
        buff += 512;
      }
      while (--count);
      send_cmd12();        /* STOP_TRANSMISSION */
    }
  }

  DESELECT();            /* CS = H */
  rcvr_spi();            /* Idle (Release DO) */

  return count ? RES_ERROR : RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/* @ BYTE drv,    - Physical drive nmuber (0)                            */
/* @ BYTE *buff,  - Pointer to the data buffer to store read data        */
/* @ DWORD sector - Start sector number (LBA)                            */
/* @ BYTE count   - Sector count (1..255)                                */
/*-----------------------------------------------------------------------*/
DRESULT disk_read_pcm (BYTE drv, BYTE *buff, DWORD sector, BYTE count) {
  if (drv || !count) return RES_PARERR;
  if (Stat & STA_NOINIT) return RES_NOTRDY;

  if (!(CardType & 4)) sector *= 256;    /* Convert to byte address if needed */

  SELECT();            /* CS = L */
  /* Single block read */
  if (count == 1) {
    /* READ_SINGLE_BLOCK */
    if ((send_cmd(CMD17, sector) == 0) && rcvr_datablock_pcm(buff, 256))
      count = 0;
  }
  /* Multiple block read */
  else {
    /* READ_MULTIPLE_BLOCK */
    if (send_cmd(CMD18, sector) == 0) {
      do {
        if (!rcvr_datablock_pcm(buff, 256)) break;
        buff += 256;
      }
      while (--count);
      send_cmd12();        /* STOP_TRANSMISSION */
    }
  }

  DESELECT();            /* CS = H */
  rcvr_spi();            /* Idle (Release DO) */

  return count ? RES_ERROR : RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/* @ BYTE drive        - Physical drive number (0)                       */
/* @ const BYTE *buff  - Pointer to the data to be written               */
/* @ DWORD sector      - Start sector number (LBA)                       */
/* @ BYTE count        - Sector count (1..255)                           */
/*-----------------------------------------------------------------------*/
#if _READONLY == 0
DRESULT disk_write (BYTE drv, const BYTE *buff, DWORD sector, BYTE count) {
  if (drv || !count) return RES_PARERR;
  if (Stat & STA_NOINIT) return RES_NOTRDY;
  if (Stat & STA_PROTECT) return RES_WRPRT;

  if (!(CardType & 4)) sector *= 512;    /* Convert to byte address if needed */

  SELECT();            /* CS = L */

  /* Single block write */
  if (count == 1) {
    /* WRITE_BLOCK */
    if ((send_cmd(CMD24, sector) == 0) && xmit_datablock(buff, 0xFE))
      count = 0;
  }
  /* Multiple block write */
  else {
    if (CardType & 2) {
      send_cmd(CMD55, 0);
      send_cmd(CMD23, count);    /* ACMD23 */
    }
    /* WRITE_MULTIPLE_BLOCK */
    if (send_cmd(CMD25, sector) == 0) {
      do {
        if (!xmit_datablock(buff, 0xFC)) break;
        buff += 512;
      }
      while (--count);
      if (!xmit_datablock(0, 0xFD))    /* STOP_TRAN token */
        count = 1;
    }
  }

  DESELECT();            /* CS = H */
  rcvr_spi();            /* Idle (Release DO) */

  return count ? RES_ERROR : RES_OK;
}
#endif /* _READONLY */

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/* @ BYTE drive        - Physical drive number (0)                       */
/* @ const BYTE *buff  - Pointer to the data to be written               */
/* @ DWORD sector      - Start sector number (LBA)                       */
/* @ BYTE count        - Sector count (1..255)                           */
/*-----------------------------------------------------------------------*/
#if _READONLY == 0
DRESULT disk_write_pcm (BYTE drv, const BYTE *buff, DWORD sector, BYTE count) {
  if (drv || !count) return RES_PARERR;
  if (Stat & STA_NOINIT) return RES_NOTRDY;
  if (Stat & STA_PROTECT) return RES_WRPRT;

  if (!(CardType & 4)) sector *= 256;    /* Convert to byte address if needed */

  SELECT();            /* CS = L */

  /* Single block write */
  if (count == 1) {
    /* WRITE_BLOCK */
    if ((send_cmd(CMD24, sector) == 0) && xmit_datablock_pcm(buff, 0xFE))
      count = 0;
  } else {
    /* Multiple block write */
    if (CardType & 2) {
      send_cmd(CMD55, 0);
      send_cmd(CMD23, count);    /* ACMD23 */
    }
    /* WRITE_MULTIPLE_BLOCK */
    if (send_cmd(CMD25, sector) == 0) {
      do {
        if (!xmit_datablock_pcm(buff, 0xFC))
          break;
        buff += 256;
      }
      while (--count);
      if (!xmit_datablock_pcm(0, 0xFD))    /* STOP_TRAN token */
        count = 1;
    }
  }

  DESELECT();            /* CS = H */
  rcvr_spi();            /* Idle (Release DO) */

  return count ? RES_ERROR : RES_OK;
}
#endif /* _READONLY */

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/* @ BYTE drv   - Physical drive nmuber (0)                              */
/* @ BYTE ctrl  - Control code                                           */
/* @ void *buff - Buffer to send/receive control data                    */
/*-----------------------------------------------------------------------*/
DRESULT disk_ioctl (BYTE drv, BYTE ctrl, void *buff) {
  DRESULT res;
  BYTE n, csd[16], *ptr = buff;
  WORD csize;

  if (drv) return RES_PARERR;

  res = RES_ERROR;

  if (ctrl == CTRL_POWER) {
    switch (*ptr) {
    case 0:        /* Sub control code == 0 (POWER_OFF) */
      if (chk_power())
        power_off();        /* Power off */
      res = RES_OK;
      break;
    case 1:        /* Sub control code == 1 (POWER_ON) */
      power_on();                /* Power on */
      res = RES_OK;
      break;
    case 2:        /* Sub control code == 2 (POWER_GET) */
      *(ptr+1) = (BYTE)chk_power();
      res = RES_OK;
      break;
    default:
      res = RES_PARERR;
    }
  }
  else {
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    SELECT();        /* CS = L */

    switch (ctrl) {
    case GET_SECTOR_COUNT:    /* Get number of sectors on the disk (DWORD) */
      if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {
        /* SDC ver 2.00 */
        if ((csd[0] >> 6) == 1) {
          csize = csd[9] + ((WORD)csd[8] << 8) + 1;
          *(DWORD*)buff = (DWORD)csize << 10;
        }
        /* MMC or SDC ver 1.XX */
        else {
          n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
          csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
          *(DWORD*)buff = (DWORD)csize << (n - 9);
        }
        res = RES_OK;
      }
      break;

    case GET_SECTOR_SIZE:    /* Get sectors on the disk (WORD) */
      *(WORD*)buff = 512;
      res = RES_OK;
      break;

    case CTRL_SYNC:    /* Make sure that data has been written */
      if (wait_ready() == 0xFF)
        res = RES_OK;
      break;

    case MMC_GET_CSD:    /* Receive CSD as a data block (16 bytes) */
      /* READ_CSD */
      if (send_cmd(CMD9, 0) == 0 && rcvr_datablock(ptr, 16))
        res = RES_OK;
      break;

    case MMC_GET_CID:    /* Receive CID as a data block (16 bytes) */
      /* READ_CID */
      if (send_cmd(CMD10, 0) == 0 && rcvr_datablock(ptr, 16))
        res = RES_OK;
      break;

    case MMC_GET_OCR:    /* Receive OCR as an R3 resp (4 bytes) */
      /* READ_OCR */
      if (send_cmd(CMD58, 0) == 0) {
        for (n = 0; n < 4; n++)
          *ptr++ = rcvr_spi();
        res = RES_OK;
      }

    default:
      res = RES_PARERR;
    }

    DESELECT();            /* CS = H */
    rcvr_spi();            /* Idle (Release DO) */
  }

  return res;
}


/*-----------------------------------------------------------------------*/
/* Device Timer Interrupt Procedure  (Platform dependent)                */
/*-----------------------------------------------------------------------*/
/* This function must be called in period of 10ms                        */
void disk_timerproc (void) {
  BYTE n;

  n = Timer1;                        /* 100Hz decrement timer */
  if (n) Timer1 = --n;
  n = Timer2;
  if (n) Timer2 = --n;
}


/*---------------------------------------------------------*/
/* User Provided Timer Function for FatFs module           */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support a real time clock.          */
DWORD get_fattime (void) {
  return  ((2018UL-1980) << 25)  // Year = 2008
          | (10UL << 21)          // Month = February
          | (23UL << 16)         // Day = 26
          | (20U << 11)          // Hour = 14
          | (0U << 5)            // Min = 0
          | (0U >> 1);           // Sec = 0
}
