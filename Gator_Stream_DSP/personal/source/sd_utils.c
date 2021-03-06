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
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~Includes-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */

#include "F28x_Project.h"
#include <string.h>
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "utils/uartstdio.h"
#include "utils/ustdlib.h"
#include "fatfs/src/ff.h"
#include "fatfs/src/diskio.h"
#include "personal/headers/sd_utils.h"


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~Globals~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */

// This buffer holds the full path to the current working
// directory. Initially it is root ("/").
char g_cCwdBuf[PATH_BUF_SIZE] = "/";

// A temporary data buffer used when manipulating file paths,
// or reading data from the SD card.
char g_cTmpBuf[PATH_BUF_SIZE];

// The buffer that holds the command line.
char g_cCmdBuf[CMD_BUF_SIZE];

// Struct that will contain the credentials of the mounted FAT32 file system.
FATFS g_sFatFs;

// Struct that will contain the credentials of the current directory.
DIR g_sDirObject;

// Struct that will contain the credentials of the current file's metadata.
FILINFO g_sFileInfo;

// Struct that will contain the credentials of the currently opened file.
FIL g_sFileObject;

// Storage for the filename listbox widget string table.
const char *g_ppcDirListStrings[NUM_LIST_STRINGS];

// Storage for the names of the files in the current directory. Filenames
// are stored in format "(D) filename.ext" for directories or "(F) filename.ext" for files.
char g_pcFilenames[NUM_LIST_STRINGS][MAX_FILENAME_STRING_LEN];

// Storage for the strings which appear in the status box at the bottom of the display.
char g_pcStatus[NUM_STATUS_STRINGS][MAX_STATUS_STRING_LEN];

// Storage for the status listbox widget string table.
const char *g_ppcStatusStrings[NUM_STATUS_STRINGS] = {
  g_pcStatus[0],
  g_pcStatus[1],
  g_pcStatus[2],
  g_pcStatus[3],
  g_pcStatus[4],
  g_pcStatus[5]
};

// This is the table that holds the command names, implementing functions,
// and brief description.
tCmdLineEntry g_psCmdTable[] = {
  { "help",  Cmd_help,   " : Display list of commands" },
  { "h",   Cmd_help,  "  : alias for help" },
  { "?",   Cmd_help,  "  : alias for help" },
  { "ls",   Cmd_ls,   "  : Display list of files" },
  { "chdir",  Cmd_cd,    ": Change directory" },
  { "cd",   Cmd_cd,   "  : alias for chdir" },
  { "pwd",  Cmd_pwd,   "  : Show current working directory" },
  { "cat",  Cmd_cat,   "  : Show contents of a text file" },
  { 0, 0, 0 }
};

// A table that holds a mapping between the numerical FRESULT code and
// it's name as a string.  This is used for looking up error codes for
// printing to the console.
tFresultString g_sFresultStrings[] = {
  FRESULT_ENTRY(FR_OK),
  FRESULT_ENTRY(FR_NOT_READY),
  FRESULT_ENTRY(FR_NO_FILE),
  FRESULT_ENTRY(FR_NO_PATH),
  FRESULT_ENTRY(FR_INVALID_NAME),
  FRESULT_ENTRY(FR_INVALID_DRIVE),
  FRESULT_ENTRY(FR_DENIED),
  FRESULT_ENTRY(FR_EXIST),
  FRESULT_ENTRY(FR_RW_ERROR),
  FRESULT_ENTRY(FR_WRITE_PROTECTED),
  FRESULT_ENTRY(FR_NOT_ENABLED),
  FRESULT_ENTRY(FR_NO_FILESYSTEM),
  FRESULT_ENTRY(FR_INVALID_OBJECT),
  FRESULT_ENTRY(FR_MKFS_ABORTED)
};


/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~Function Definitions-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */

// ChangeToDirectory - This function implements the "cd" command.  It takes an
//          argument that specifies the directory to make the
//          current working directory. Path separators must use a
//          forward slash "/".  The argument to cd can be one of the
//          following:
//          * root ("/")
//          * a fully specified path ("/my/path/to/mydir")
//          * a single directory name that is in the current
//           directory ("mydir")
//          * parent directory ("..")
//          It does not understand relative paths, so don't try
//          something like this: ("../my/new/path")
//          Once the new directory is specified, it attempts to open
//          the directory to make sure it exists.  If the new path
//          is opened successfully, then the current working
//          directory (cwd) is changed to the new path. In cases of
//          error, the pulReason parameter will be written with one
//          of the following values:
FRESULT ChangeToDirectory(char *pcDirectory, unsigned long *pulReason) {
  unsigned int uIdx;
  FRESULT fresult;

  // Copy the current working path into a temporary buffer so
  // it can be manipulated.
  strcpy(g_cTmpBuf, g_cCwdBuf);

  // If the first character is /, then this is a fully specified
  // path, and it should just be used as-is.
  if(pcDirectory[0] == '/') {
    // Make sure the new path is not bigger than the cwd buffer.
    if(strlen(pcDirectory) + 1 > sizeof(g_cCwdBuf)) {
      *pulReason = NAME_TOO_LONG_ERROR;
      return(FR_OK);
    } else {
      // If the new path name (in argv[1])  is not too long, then
      // copy it into the temporary buffer so it can be checked.
      strncpy(g_cTmpBuf, pcDirectory, sizeof(g_cTmpBuf));
    }
  } else if(!strcmp(pcDirectory, "..")) {
    // If the argument is .. then attempt to remove the lowest level
    // on the CWD.
    // Get the index to the last character in the current path.
    uIdx = strlen(g_cTmpBuf) - 1;

    // Back up from the end of the path name until a separator (/)
    // is found, or until we bump up to the start of the path.
    while((g_cTmpBuf[uIdx] != '/') && (uIdx > 1)) {
      // Back up one character.
      uIdx--;
    }

    // Now we are either at the lowest level separator in the
    // current path, or at the beginning of the string (root).
    // So set the new end of string here, effectively removing
    // that last part of the path.
    g_cTmpBuf[uIdx] = 0;
  } else {
    // Otherwise this is just a normal path name from the current
    // directory, and it needs to be appended to the current path.
    // Test to make sure that when the new additional path is
    // added on to the current path, there is room in the buffer
    // for the full new path.  It needs to include a new separator,
    // and a trailing null character.
    if(strlen(g_cTmpBuf) + strlen(pcDirectory) + 1 + 1 > sizeof(g_cCwdBuf)) {
      *pulReason = NAME_TOO_LONG_ERROR;
      return(FR_INVALID_OBJECT);
    } else {
      // The new path is okay, so add the separator and then append
      // the new directory to the path.
      // If not already at the root level, then append a /.
      if(strcmp(g_cTmpBuf, "/")) {
        strcat(g_cTmpBuf, "/");
      }

      // Append the new directory to the path.
      strcat(g_cTmpBuf, pcDirectory);
    }
  }

  // At this point, a candidate new directory path is in chTmpBuf.
  // Try to open it to make sure it is valid.
  fresult = f_opendir(&g_sDirObject, g_cTmpBuf);

  // If it can't be opened, then it is a bad path.  Inform
  // user and return.
  if(fresult != FR_OK) {
    *pulReason = OPENDIR_ERROR;
    return(fresult);
  } else {
    // Otherwise, it is a valid new path, so copy it into the CWD and update
    // the screen.
    strncpy(g_cCwdBuf, g_cTmpBuf, sizeof(g_cCwdBuf));
  }

  // Return success.
  return(FR_OK);
}


// Cmd_cat - This function implements the "cat" command.  It reads the contents
//      of a file and prints it to the console.  This should only be used
//      on text files.  If it is used on a binary file, then a bunch of
//      garbage is likely to printed on the console.
int Cmd_cat(int argc, char *argv[]) {
  FRESULT fresult;
  unsigned short usBytesRead;

  // First, check to make sure that the current path (CWD), plus
  // the file name, plus a separator and trailing null, will all
  // fit in the temporary buffer that will be used to hold the
  // file name.  The file name must be fully specified, with path,
  // to FatFs.
  if(strlen(g_cCwdBuf) + strlen(argv[1]) + 1 + 1 > sizeof(g_cTmpBuf)) {
    UARTprintf("Resulting path name is too long\n");
    return(0);
  }

  // Copy the current path to the temporary buffer so it can be manipulated.
  strcpy(g_cTmpBuf, g_cCwdBuf);

  // If not already at the root level, then append a separator.
  if(strcmp("/", g_cCwdBuf)) {
    strcat(g_cTmpBuf, "/");
  }

  // Now finally, append the file name to result in a fully specified file.
  strcat(g_cTmpBuf, argv[1]);

  // Open the file for reading.
  fresult = f_open(&g_sFileObject, g_cTmpBuf, FA_READ);

  // If there was some problem opening the file, then return
  // an error.
  if(fresult != FR_OK) {
    return(fresult);
  }

  // Enter a loop to repeatedly read data from the file and display it,
  // until the end of the file is reached.
  do
  {
    // Read a block of data from the file.  Read as much as can fit
    // in the temporary buffer, including a space for the trailing null.
    fresult = f_read(&g_sFileObject, g_cTmpBuf, sizeof(g_cTmpBuf) - 1,
            &usBytesRead);

    // If there was an error reading, then print a newline and
    // return the error to the user.
    if(fresult != FR_OK) {
      UARTprintf("\n");
      return(fresult);
    }

    // Null terminate the last block that was read to make it a
    // null terminated string that can be used with printf.
    g_cTmpBuf[usBytesRead] = 0;

    // Print the last chunk of the file that was received.
    UARTprintf("%s", g_cTmpBuf);

    // Continue reading until less than the full number of bytes are
    // read.  That means the end of the buffer was reached.
  }
  while(usBytesRead == sizeof(g_cTmpBuf) - 1);

  // Return success.
  return(0);
}


// Cmd_cd - This function implements the "cd" command.  It takes an argument
//     that specifies the directory to make the current working directory.
//     Path separators must use a forward slash "/".  The argument to "cd"
//     can be one of the following:
//     * root ("/")
//     * a fully specified path ("/my/path/to/mydir")
//     * a single directory name that is in the current directory("mydir")
//     * parent directory ("..")
//     It does not understand relative paths, so don't try something like
//     this: ("../my/new/path")
//     Once the new directory is specified, it attempts to open the
//     directory to make sure it exists.  If the new path is opened
//     successfully, then the current working directory (cwd) is changed
//     to the new path.
int Cmd_cd(int argc, char *argv[]) {
  unsigned long ulReason;
  FRESULT fresult;

  // Try to change to the directory provided on the command line.
  fresult = ChangeToDirectory(argv[1], &ulReason);

  // If an error was reported, try to offer some helpful information.
  if(fresult != FR_OK) {
    switch(ulReason) {
      case OPENDIR_ERROR:
        UARTprintf("Error opening new directory.\n");
        break;

      case NAME_TOO_LONG_ERROR:
        UARTprintf("Resulting path name is too long.\n");
        break;

      default:
        UARTprintf("An unrecognized error was reported.\n");
        break;
    }
  }

  // Return the appropriate error code.
  return(fresult);
}


// Cmd_help - This function implements the "help" command.  It prints a simple
//      list of the available commands with a brief description.
int Cmd_help(int argc, char *argv[]) {
  tCmdLineEntry *pEntry;

  // Print some header text.
  UARTprintf("\nAvailable commands\n");
  UARTprintf("------------------\n");

  // Point at the beginning of the command table.
  pEntry = &g_psCmdTable[0];

  // Enter a loop to read each entry from the command table.  The
  // end of the table has been reached when the command name is NULL.
  while(pEntry->pcCmd) {
    // Print the command name and the brief description.
    UARTprintf("%s%s\n", pEntry->pcCmd, pEntry->pcHelp);

    // Advance to the next entry in the table.
    pEntry++;
  }

  // Return success.
  return(0);
}


// Cmd_ls - This function implements the "ls" command.  It opens the current
//     directory and enumerates through the contents, and prints a line
//     for each item it finds.  It shows details such as file attributes,
//     time and date, and the file size, along with the name.  It shows a
//     summary of file sizes at the end along with free space.
int Cmd_ls(int argc, char *argv[]) {
  unsigned long ulTotalSize, ulItemCount, ulFileCount, ulDirCount;
  FRESULT fresult;
  FATFS *pFatFs;

  // Open the current directory for access.
  fresult = f_opendir(&g_sDirObject, g_cCwdBuf);

  // Check for error and return if there is a problem.
  if(fresult != FR_OK) {
    return(fresult);
  }

  ulTotalSize = 0;
  ulFileCount = 0;
  ulDirCount = 0;
  ulItemCount = 0;

  // Give an extra blank line before the listing.
  UARTprintf("\n");

  // Enter loop to enumerate through all directory entries.
  for(;;) {
    // Read an entry from the directory.
    fresult = f_readdir(&g_sDirObject, &g_sFileInfo);

    // Check for error and return if there is a problem.
    if(fresult != FR_OK) {
      return(fresult);
    }

    // If the file name is blank, then this is the end of the
    // listing.
    if(!g_sFileInfo.fname[0]) {
      break;
    }

    // Print the entry information on a single line with formatting
    // to show the attributes, date, time, size, and name.
    UARTprintf("%c%c%c%c%c %u/%02u/%02u %02u:%02u %9u  %s\n",
         (g_sFileInfo.fattrib & AM_DIR) ? (uint32_t)'D' : (uint32_t)'-',
         (g_sFileInfo.fattrib & AM_RDO) ? (uint32_t)'R' : (uint32_t)'-',
         (g_sFileInfo.fattrib & AM_HID) ? (uint32_t)'H' : (uint32_t)'-',
         (g_sFileInfo.fattrib & AM_SYS) ? (uint32_t)'S' : (uint32_t)'-',
         (g_sFileInfo.fattrib & AM_ARC) ? (uint32_t)'A' : (uint32_t)'-',
         (uint32_t)((g_sFileInfo.fdate >> 9) + 1980),
         (uint32_t)((g_sFileInfo.fdate >> 5) & 15),
         (uint32_t)(g_sFileInfo.fdate & 31),
         (uint32_t)((g_sFileInfo.ftime >> 11)),
         (uint32_t)((g_sFileInfo.ftime >> 5) & 63),
         (uint32_t)(g_sFileInfo.fsize),
         g_sFileInfo.fname);

    // Add the information as a line in the listbox widget.
    if(ulItemCount < NUM_LIST_STRINGS) {
      usprintf(g_pcFilenames[ulItemCount], "(%c) %12s",
          (g_sFileInfo.fattrib & AM_DIR) ? 'D' : 'F',
          g_sFileInfo.fname);
    }

    // If the attribute is directory, then increment the directory count.
    if(g_sFileInfo.fattrib & AM_DIR) {
      ulDirCount++;
    }
    // Otherwise, it is a file.  Increment the file count, and
    // add in the file size to the total.
    else {
      ulFileCount++;
      ulTotalSize += g_sFileInfo.fsize;
    }

    // Move to the next entry in the item array we use to populate the
    // list box.
    ulItemCount++;

    // Wait for the UART transmit buffer to empty.
//    UARTFlushTx(false);
  }

  // Print summary lines showing the file, dir, and size totals.
  UARTprintf("\n%4u File(s),%10u bytes total\n%4u Dir(s)",
       ulFileCount, ulTotalSize, ulDirCount);

  // Get the free space.
  fresult = f_getfree("/", &ulTotalSize, &pFatFs);

  // Check for error and return if there is a problem.
  if(fresult != FR_OK) {
    return(fresult);
  }

  // Display the amount of free space that was calculated.
  UARTprintf(", %10uK bytes free\n", ulTotalSize * pFatFs->sects_clust / 2);

  // Wait for the UART transmit buffer to empty.
//  UARTFlushTx(false);

  // Made it to here, return with no errors.
  return(0);
}

// Cmd_pwd - This function implements the "pwd" command.  It simply prints the
//      current working directory.
int Cmd_pwd(int argc, char *argv[]) {
  // Print the CWD to the console.
  UARTprintf("%s\n", g_cCwdBuf);

  //
  // Wait for the UART transmit buffer to empty.
  //
//  UARTFlushTx(false);

  // Return success.
  return(0);
}


// ConfigureUART - Configure the UART and its pins.  This must be called
//        before UARTprintf().
void ConfigureUART(void) {
#ifndef CUSTOM_HW
  // Enable UART0
  SysCtlPeripheralEnable(SYSCTL_PERIPH_SCI1);

  // Configure GPIO Pins for UART mode.
  // Launchpad HW
  GPIO_SetupPinMux(43, GPIO_MUX_CPU1, 15);
  GPIO_SetupPinOptions(43, GPIO_INPUT, GPIO_PUSHPULL);
  GPIO_SetupPinMux(42, GPIO_MUX_CPU1, 15);
  GPIO_SetupPinOptions(42, GPIO_OUTPUT, GPIO_ASYNC);
  UARTStdioConfig(0, 115200, SysCtlLowSpeedClockGet(SYSTEM_CLOCK_SPEED));
#else
  // Enable UART0
  SysCtlPeripheralEnable(SYSCTL_PERIPH_SCI3);

  // Configure GPIO Pins for UART mode.
  // Launchpad HW
  // Custom HW
  GPIO_SetupPinMux(62, GPIO_MUX_CPU1, 1);
  GPIO_SetupPinOptions(62, GPIO_INPUT, GPIO_PUSHPULL);
  GPIO_SetupPinMux(63, GPIO_MUX_CPU1, 1);
  GPIO_SetupPinOptions(63, GPIO_OUTPUT, GPIO_ASYNC);
  UARTStdioConfig(2, 115200, SysCtlLowSpeedClockGet(SYSTEM_CLOCK_SPEED));
#endif
}


// StringFromFresult - This function returns a string representation of an
//          error code that was returned from a function call to
//          FatFs.  It can be used for printing human readable
//          error messages.
const char* StringFromFresult(FRESULT fresult) {
  // Enter a loop to search the error code table for a matching
  // error code.
  for(unsigned int uIdx = 0; uIdx < NUM_FRESULT_CODES; uIdx++) {
    // If a match is found, then return the string name of the
    // error code.
    if(g_sFresultStrings[uIdx].fresult == fresult) {
      return(g_sFresultStrings[uIdx].pcResultStr);
    }
  }

  // At this point no matching code was found, so return a
  // string indicating unknown error.
  return("UNKNOWN ERROR CODE");
}


FRESULT initMMC(void) {
  FRESULT fresult;
  // Set the system tick to fire 100 times per second.
  SysTickInit();
  SysTickPeriodSet(SysCtlClockGet(SYSTEM_CLOCK_SPEED) / 100);
  SysTickIntRegister(SysTickHandler);
  SysTickIntEnable();
  SysTickEnable();

  // Configure UART0 for debug output.
  ConfigureUART();
//  UARTprintf("Hello word!");
  // Initialize the connection to the SD card.
  disk_initialize(0);
  // Mount the file system, using logical disk 0.
  fresult = f_mount(0, &g_sFatFs);
  if(fresult != FR_OK) {
    UARTprintf("f_mount error: %s\n", StringFromFresult(fresult));
  }
  return fresult;
}

// SysTickHandler - This is the handler for this SysTick interrupt.  FatFs
//        requires a timer tick every 10 ms for internal timing
//        purposes.
interrupt void SysTickHandler(void) {
  // Call the FatFs tick timer.
  disk_timerproc();
  PieCtrlRegs.PIEACK.all |= 1;
}
