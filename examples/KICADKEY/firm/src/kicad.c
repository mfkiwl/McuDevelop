/*
 * Copyright (c) 2018,2019 zhtlab
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#define _KICAD_C_

#include        <ctype.h>

#include        "config.h"

#include        "system.h"
#include        "gpio.h"
#include        "rtos.h"
#include        "devFlash.h"
#include        "crc32.h"

#include        "usbdif.h"
#include        "usbdhid.h"
#include        "usb_hid.h"
extern int      dUsbHid;


#include        "usb_hid_keys.h"
#include        "kicad.h"


#define KICAD_DEBUG_SHOW                0
#define KICAD_HOTKEY_NAME               1



uint8_t         kicadKeyScan[CONFIG_KEY_COL_CNT];
static uint8_t  kicadKeyScanPrev[CONFIG_KEY_COL_CNT];
#define         KICAD_KEYMAPSEL_SCH      0
#define         KICAD_KEYMAPSEL_PCB      1
int             kicadKeyMapSel = KICAD_KEYMAPSEL_SCH;
int             kicadKeyMapShift = 0;
uint8_t         kicadPushedSel = 0;

/*************************************
 * COL0:   na,   na,   na,   na,   na,  TAB,   na,
 * COL1:   na,  CAL,    -,   BS,    +,  ENT,    .,
 * COL2:    =,   na,    *,    9,    6,    3,   na,
 * COL3:  ESC,   na,    /,    8,    5,    2,   na,
 * COL4:   na,   na,  NUM,    7,    4,    1,    0,
 */
typedef struct {
  uint8_t       code;
  uint8_t       name[4];
} kicadKeyList_t;
const kicadKeyList_t    kicadKeyList[] = {
  /* rottay 22key */
  {0x05, "TAB"},
  {0x11, "CAL"}, {0x12,   "-"}, {0x13, "BS"}, {0x14, "+"}, {0x15, "ENT"}, {0x16, "."},
  {0x20,   "="}, {0x22,   "*"}, {0x23,  "9"}, {0x24, "6"}, {0x25,   "3"},
  {0x30, "ESC"}, {0x32,   "/"}, {0x33,  "8"}, {0x34, "5"}, {0x35,   "2"},
                 {0x42, "NUM"}, {0x43,  "7"}, {0x44, "4"}, {0x45,   "1"}, {0x46, "0"},
};
#define KICAD_KEYLIST_COUNT     (sizeof(kicadKeyList)/sizeof(kicadKeyList_t))
/*************************************
 *  default key assign for EESCHEMA
 *  +-------------+-------------+-------------+-------------+
 *  |(ESC)        |(CALC)       |(TAB)        |(=)          |
 *  |             |sch/pcb sel  |             |             |
 *  |             |             |             |             |
 *  |S            |S            |S            |S            |
 *  +-------------+-------------+-------------+-------------+
 *  |(NUM)        |(/)          |(*)          |(BS)         |
 *  |             |  polyline   |  text       |             |
 *  |S            |S            |S            |S            |
 *  +-------------+-------------+-------------+-------------+
 *  |(7)          |(8)          |(9)          |(-)          |
 *  |S add nc     |  add power  |  add comp   |  leave sht  |
 *  |S add junct  |S add glabel |S edit comp  |S            |
 *  +-------------+-------------+-------------+-------------+
 *  |(4)          |(5)          |(6)          |(+)          |
 *  |  start bus  |  fit        |  start wire |  (grid next)|
 *  |S            |S find item  |S bus entry  |S (grid prev)|
 *  +-------------+-------------+-------------+-------------+
 *  |(1)          |(2)          |(3)          |(ENTER)      |
 *  |  mirrorX    |  rotate     |  move       |SHIFT        |
 *  |S mirrorY    |S            |S drag       |             |
 *  +-------------+-------------+-------------+             |
 *  |(0/INS)                    |(./DEL)      |             |
 *  |  repeat                   |  del        |             |
 *  |S                          |S            |             |
 *  +---------------------------+-------------+-------------+
 *  "(xxx)" is character print on keys
 *  "yyy" is the normal character to send
 *  "zzz" is the character to send, if shift key was pushed
 */
kicadKeyMap_t   kicadKeyMapSch[CONFIG_KEY_COL_CNT][CONFIG_KEY_ROW_CNT] = {
  /* Column 0     na,   na,   na,   na,   na,  TAB,   na,  */
  {{KISCH_NONE,          0, 0, 0,  0, 0,   /* na  */
    KISCH_NONE,          0, 0, 0,  0, 0},
   {KISCH_NONE,          0, 0, 0,  0, 0,   /* na  */
    KISCH_NONE,          0, 0, 0,  0, 0},
   {KISCH_NONE,          0, 0, 0,  0, 0,   /* na  */
    KISCH_NONE,          0, 0, 0,  0, 0},
   {KISCH_NONE,          0, 0, 0,  0, 0,   /* na  */
    KISCH_NONE,          0, 0, 0,  0, 0},
   {KISCH_NONE,          0, 0, 0,  0, 0,   /* na  */
    KISCH_NONE,          0, 0, 0,  0, 0},
   {KISCH_NONE,          0, 0, 0,  0, 0,   /* TAB */
    KISCH_NONE,          0, 0, 0,  0, 0},
   {KISCH_NONE,          0, 0, 0,  0, 0,   /* na  */
    KISCH_NONE,          0, 0, 0,  0, 0}},
  /* Column 1      na,  CAL,    -,   BS,    +,  ENT,    .,   */
  {{KISCH_NONE,          0, 0, 0,  0, 0,   /* na  */
    KISCH_NONE,          0, 0, 0,  0, 0},
   {KISCH_NONE,          0, 0, 0,  0, 0,   /* CAL */
    KISCH_NONE,          0, 0, 0,  0, 0},
   {KISCH_LEAVESHT,      0, 0, 0,  1, 0,   /* -   */
    KISCH_NONE,          0, 0, 0,  0, 0},
   {KISCH_NONE,          0, 0, 0,  0, 0,   /* BS  */
    KISCH_NONE,          0, 0, 0,  0, 0},
   {KISCH_NONE,          0, 0, 0,  0, 0,   /* +   */
    KISCH_NONE,          0, 0, 0,  0, 0},
   {KICAD_SHIFT,         0, 0, 0,  1, 0,   /* ENT */
    KICAD_SHIFT,         0, 0, 0,  1, 0},
   {KISCH_NONE,          0, 0, 0,  0, 0,   /* .   */
    KISCH_NONE,          0, 0, 0,  0, 0}},
  /* Column 2       =,   na,    *,    9,    6,    3,   na,   */
  {{KISCH_NONE,          0, 0, 0,  0, 0,   /* =   */
    KISCH_NONE,          0, 0, 0,  0, 0},
   {KISCH_NONE,          0, 0, 0,  0, 0,   /* na  */
    KISCH_NONE,          0, 0, 0,  0, 0},
   {KISCH_TEXT,          0, 0, 0,  1, 0,   /* *   */
    KISCH_NONE,          0, 0, 0,  0, 0},
   {KISCH_ADDCOMP,       0, 0, 0,  1, 0,   /* 9   */
    KISCH_EDIT,          0, 0, 0,  1, 0},
   {KISCH_WIRE,          0, 0, 0,  1, 0,   /* 6   */
    KISCH_WIREENTRY,     0, 0, 0,  1, 0},
   {KISCH_ITEMMOVE,      0, 0, 0,  1, 0,   /* 3   */
    KISCH_DRAG,          0, 0, 0,  1, 0},
   {KISCH_NONE,          0, 0, 0,  0, 0,   /* na  */
    KISCH_NONE,          0, 0, 0,  0, 0}},
  /* Column 3     ESC,   na,    /,    8,    5,    2,   na,   */
  {{KISCH_ESCAPE,        0, 0, 0,  1, 0,   /* ESC */
    KISCH_NONE,          0, 0, 0,  0, 0},
   {KISCH_NONE,          0, 0, 0,  0, 0,   /* na  */
    KISCH_NONE,          0, 0, 0,  0, 0},
   {KISCH_POLYLINE,      0, 0, 0,  1, 0,   /* /   */
    KISCH_NONE,          0, 0, 0,  0, 0},
   {KISCH_ADDPOWER,      0, 0, 0,  1, 0,   /* 8   */
    KISCH_ADDGLABEL,     0, 0, 0,  1, 0},
   {KISCH_FIT,           0, 0, 0,  1, 0,   /* 5   */
    KISCH_FIND,          0, 0, 0,  1, 0},
   {KISCH_ROTATE,        0, 0, 0,  1, 0,   /* 2   */
    KISCH_NONE,          0, 0, 0,  0, 0},
   {KISCH_NONE,          0, 0, 0,  0, 0,   /* na  */
    KISCH_NONE,          0, 0, 0,  0, 0}},
  /* Column 4      na,   na,  NUM,    7,    4,    1,  INS,   */
  {{KISCH_NONE,          0, 0, 0,  0, 0,   /* na  */
    KISCH_NONE,          0, 0, 0,  0, 0},
   {KISCH_NONE,          0, 0, 0,  0, 0,   /* na  */
    KISCH_NONE,          0, 0, 0,  0, 0},
   {KISCH_NONE,          0, 0, 0,  0, 0,   /* NUM */
    KISCH_NONE,          0, 0, 0,  0, 0},
   {KISCH_NC,            0, 0, 0,  1, 0,   /* 7   */
    KISCH_JUNCTION,      0, 0, 0,  1, 0},
   {KISCH_BUS,           0, 0, 0,  1, 0,   /* 4   */
    KISCH_NONE,          0, 0, 0,  1, 0},
   {KISCH_MIRRORX,       0, 0, 0,  1, 0,   /* 1   */
    KISCH_MIRRORY,       0, 0, 0,  1, 0},
   {KISCH_REPEAT,        0, 0, 0,  1, 0,   /* 0   */
    KISCH_NONE,          0, 0, 0,  0, 0}},
};


/*  default key assign for PCBNEW
 *  +-------------+-------------+-------------+-------------+
 *  |(ESC)        |(CALC)       |(TAB)        |(=)          |
 *  |ESC          |sch/pcb sel  |             |  view OpenGL|
 *  |             |             |             |  view Normal|
 *  |S            |S            |S            |S            |
 *  +-------------+-------------+-------------+-------------+
 *  |(NUM)        |(/)          |(*)          |(BS)         |
 *  |  macro 0    |  macro 1    | macro 2     |  macro 3    |
 *  |S            |S            |S            |S            |
 *  +-------------+-------------+-------------+-------------+
 *  |(7)          |(8)          |(9)          |(-)          |
 *  |             |             |  add via    |  next layer |
 *  |S via disp???|S            |S add m-via  |S fast grid1 |
 *  |             |             |             |  fast grid2 |
 *  +-------------+-------------+-------------+-------------+
 *  |(4)          |(5)          |(6)          |(+)          |
 *  |  width next |  auto zoom  |  add track  |  grid next  |
 *  |             |             |             |             |
 *  |S tr display |S find item  |S posture    |S grid prev  |
 *  |             |             |             |             |
 *  +-------------+-------------+-------------+-------------+
 *  |(1)          |(2)          |(3)          |(ENTER)      |
 *  |  flip       |  rotate     |  move       |SHIFT        |
 *  |S zone fill  |S            |S drag       |             |
 *  |  zone unfill|             |             |             |
 *  +-------------+-------------+-------------+             |
 *  |(0/INS)                    |(./DEL)      |             |
 *  |                           |  del segment|             |
 *  |S                          |S del        |             |
 *  +---------------------------+-------------+-------------+
 *  "(xxx)" is character print on keys
 *  "yyy" is the normal character to send
 *  "zzz" is the character to send, if shift key was pushed
 */
kicadKeyMap_t   kicadKeyMapPcb[CONFIG_KEY_COL_CNT][CONFIG_KEY_ROW_CNT] = {
  /* Column 0     na,   na,   na,   na,   na,  TAB,   na,  */
  {{KIPCB_NONE,          0, 0, 0,  0, 0,   /* na  */
    KIPCB_NONE,          0, 0, 0,  0, 0},
   {KIPCB_NONE,          0, 0, 0,  0, 0,   /* na  */
    KIPCB_NONE,          0, 0, 0,  0, 0},
   {KIPCB_NONE,          0, 0, 0,  0, 0,   /* na  */
    KIPCB_NONE,          0, 0, 0,  0, 0},
   {KIPCB_NONE,          0, 0, 0,  0, 0,   /* na  */
    KIPCB_NONE,          0, 0, 0,  0, 0},
   {KIPCB_NONE,          0, 0, 0,  0, 0,   /* na  */
    KIPCB_NONE,          0, 0, 0,  0, 0},
   {KIPCB_NONE,          0, 0, 0,  0, 0,   /* TAB */
    KIPCB_NONE,          0, 0, 0,  0, 0},
   {KIPCB_NONE,          0, 0, 0,  0, 0,   /* na  */
    KIPCB_NONE,          0, 0, 0,  0, 0}},
  /* Column 1      na,  CAL,    -,   BS,    +,  ENT,    .,   */
  {{KIPCB_NONE,          0, 0, 0,  0, 0,   /* na  */
    KIPCB_NONE,          0, 0, 0,  0, 0},
   {KIPCB_NONE,          0, 0, 0,  0, 0,   /* CAL */
    KIPCB_NONE,          0, 0, 0,  0, 0},
   {KIPCB_CUNEXT,        0, 0, 0,  1, 0,   /* -   */
    KIPCB_GRIDFAST1,     KIPCB_GRIDFAST2,  0, 0,  2, 0},
   {KIPCB_MACRO3,        0, 0, 0,  1, 0,   /* BS  */
    KIPCB_NONE,          0, 0, 0,  0, 0},
   {KIPCB_GRIDNEXT,      0, 0, 0,  1, 0,   /* +   */
    KIPCB_GRIDPREV,      0, 0, 0,  1, 0},
   {KICAD_SHIFT,         0, 0, 0,  1, 0,   /* ENT */
    KICAD_SHIFT,         0, 0, 0,  1, 0},
   {KIPCB_DELTRSEG,      0, 0, 0,  1, 0,   /* .   */
    KIPCB_DEL,           0, 0, 0,  1, 0}},
  /* Column 2   n    =,   na,    *,    9,    6,    3,   na,   */
  {{KIPCB_VIEWNORMAL,    KIPCB_VIEWGL, 1,  0,  2, 0,   /* =   */
    KIPCB_NONE,          0, 0, 0,  0, 0},
   {KIPCB_NONE,          0, 0, 0,  0, 0,   /* na  */
    KIPCB_NONE,          0, 0, 0,  0, 0},
   {KIPCB_MACRO2,        0, 0, 0,  1, 0,   /* *   */
    KIPCB_NONE,          0, 0, 0,  0, 0},
   {KIPCB_ADDVIA,        0, 0, 0,  1, 0,   /* 9   */
    KIPCB_ADDMVIA,       0, 0, 0,  1, 0},
   {KIPCB_ADDTR,         0, 0, 0,  1, 0,   /* 6   */
    KIPCB_TRPOSTURE,     0, 0, 0,  1 ,0},
   {KIPCB_MOVE,          0, 0, 0,  1 ,0,   /* 3   */
    KIPCB_DRAG,          0, 0, 0,  1 ,0},
   {KIPCB_NONE,          0, 0, 0,  0 ,0,   /* na  */
    KIPCB_NONE,          0, 0, 0,  0 ,0}},
  /* Column 3     ESC,   na,    /,    8,    5,    2,   na,   */
  {{KIPCB_ESCAPE,        0, 0, 0,  1, 0,   /* ESC */
    KIPCB_NONE,          0, 0, 0,  0, 0},
   {KIPCB_NONE,          0, 0, 0,  0, 0,   /* na  */
    KIPCB_NONE,          0, 0, 0,  0, 0},
   {KIPCB_MACRO1,        0, 0, 0,  1, 0,   /* /   */
    KIPCB_NONE,          0, 0, 0,  0, 0},
   {KIPCB_NONE,          0, 0, 0,  0, 0,   /* 8   */
    KIPCB_NONE,          0, 0, 0,  0, 0},
   {KIPCB_AUTOZOOM,      0, 0, 0,  1, 0,   /* 5   */
    KIPCB_FIND,          0, 0, 0,  1, 0},
   {KIPCB_ROTATE,        0, 0, 0,  1, 0,   /* 2   */
    KIPCB_NONE,          0, 0, 0,  0, 0},
   {KIPCB_NONE,          0, 0, 0,  0, 0,   /* na  */
    KIPCB_NONE,          0, 0, 0,  0, 0}},
  /* Column 4      na,   na,  NUM,    7,    4,    1,  INS,   */
  {{KIPCB_NONE,          0, 0, 0,  0, 0,   /* na  */
    KIPCB_NONE,          0, 0, 0,  0, 0},
   {KIPCB_NONE,          0, 0, 0,  0, 0,   /* na  */
    KIPCB_NONE,          0, 0, 0,  0, 0},
   {KIPCB_MACRO0,        0, 0, 0,  1, 0,   /* NUM */
    KIPCB_NONE,          0, 0, 0,  0, 0},
   {KIPCB_NONE,          0, 0, 0,  0, 0,   /* 7   */
    KIPCB_NONE,          0, 0, 0,  0, 0},
   {KIPCB_TRWNEXT,       0, 0, 0,  1, 0,   /* 4   */
    KIPCB_TRDISPLAY,     0, 0, 0,  1, 0},
   {KIPCB_FLIP,          0, 0, 0,  1, 0,   /* 1   */
    KIPCB_FILLZONE,      KIPCB_UNFILLZONE, 0, 0,  2, 0},
   {KIPCB_NONE,          0, 0, 0,  0, 0,   /* 0    */
    KIPCB_NONE,          0, 0, 0,  0, 0}},
};
kicadKeyMap_t   (*kicadKeyMapPtr)[CONFIG_KEY_COL_CNT][CONFIG_KEY_ROW_CNT];

static uint8_t          bufHidReport[8];
#define KICAD_MAXNUM_KEY        6

/*************************************
 * switches
 */
void
KicadSwitchInit(void)
{
  memset(kicadKeyScan,     0xff, sizeof(kicadKeyScan));
  memset(kicadKeyScanPrev, 0xff, sizeof(kicadKeyScanPrev));

  KicadLoadFromEeprom();
  kicadKeyMapPtr = &kicadKeyMapSch;

  return;
}
void
KicadSwitchLoop(void)
{
  uint32_t      val;
  uint32_t      col, row;

  /* Infinite loop */
  col = GPIO_SET_COL0;
  for(int i = 0; i < CONFIG_KEY_COL_CNT; i++) {
    GpioDisCols(GPIO_COL0);             /* disable all column signal */
    GpioSelectCol(GPIO_COL0, col);      /* select column */
    RtosTaskSleep(6);                   /* wait */
    row  = GpioGetRows(GPIO_ROW0);      /* read */
    row |= 0x80;                        /* MSB is invalid */
    kicadKeyScan[i] = row;

    if(i == 1) KicadScanSpecialKeysSHIFT(1, 5);
    if(i == 1) KicadScanSpecialKeysSEL(1, 1);
    kicadKeyScan[1] |= (1<<1) | (1<<5);   /* mask CALC, SHIFT */



    if(!(kicadKeyScan[0] & (1<<5))) {
      KicadShowKeyMapAll();
    }



    /* keys were pushed or released */
    if((~kicadKeyScan[i] & 0xff) &&
       kicadKeyScanPrev[i] != kicadKeyScan[i]) {

      KicadBuildHidReport();

#if KICAD_DEBUG_SHOW
      printf("# key %x %x %x %x %x\n",
             kicadKeyScan[0], kicadKeyScan[1],
             kicadKeyScan[2], kicadKeyScan[3], kicadKeyScan[4]);
      printf("# report mod:%x key[%x %x]\n",
             bufHidReport[0], bufHidReport[2], bufHidReport[3]);
#endif

      /* make code is sent */
      while(UsbdHidIsTxBusy(dUsbHid)) {
        RtosTaskSleep(1);
      }
      UsbdHidTransmit(dUsbHid, bufHidReport, sizeof(bufHidReport));

      /* release code is send immediately */
      while(UsbdHidIsTxBusy(dUsbHid)) {
        RtosTaskSleep(1);
      }
      memset(bufHidReport, 0, sizeof(bufHidReport));
      UsbdHidTransmit(dUsbHid, bufHidReport, sizeof(bufHidReport));

    }
    kicadKeyScanPrev[i] = kicadKeyScan[i];
    col <<= 1;

  }

  return;
}


static void
KicadScanSpecialKeysSEL(int col, int row)
{
  if(!(kicadKeyScan[col] & (1<<row))) {
    /* if CALC key was pushed */
    if(!kicadPushedSel) {
      kicadPushedSel = 1;
      if(kicadKeyMapSel == KICAD_KEYMAPSEL_SCH) {
        kicadKeyMapSel = KICAD_KEYMAPSEL_PCB;
        SystemGpioSetPowerLedOn();
        kicadKeyMapPtr = &kicadKeyMapPcb;
      } else {
        kicadKeyMapSel = KICAD_KEYMAPSEL_SCH;
        SystemGpioSetPowerLedOff();
        kicadKeyMapPtr = &kicadKeyMapSch;
      }
    }
  } else {
    kicadPushedSel = 0;
  }

  return;
}
static void
KicadScanSpecialKeysSHIFT(int col, int row)
{
  /* if SHIFT key was pushed */
  kicadKeyMapShift = 0;
  if(!(kicadKeyScan[col] & (1<<row))) {
    kicadKeyMapShift = 1;
  }
  return;
}


static void
KicadBuildHidReport(void)
{
  uint16_t      key;
  int           cnt;
  int           bit;
  kicadKeyToggle_t *tog;

  memset(&bufHidReport[0], 0, sizeof(bufHidReport));
  cnt = 0;
  for(int k = 0; k < CONFIG_KEY_COL_CNT; k++) {
    bit = 1;
    for(int j = 0; j < CONFIG_KEY_ROW_CNT; j++) {
      if(!(kicadKeyScan[k] & bit)) {
        if(cnt >= KICAD_MAXNUM_KEY) {
          k = CONFIG_KEY_COL_CNT;   /* stop outer loop */
          break;
        }

        tog = &(*kicadKeyMapPtr)[k][j].map[kicadKeyMapShift];
        tog->pos++;
        if(tog->pos >= tog->cnt) tog->pos = 0;
        key = tog->key[tog->pos];
#if KICAD_DEBUG_SHOW
        printf("# press col:%d row:%d  mod:key[%x]\r\n", k, j, key);
#endif

        if(!(key & KICAD_CTRL_MASK)) {
          bufHidReport[0] = KICAD_GET_MOD(key);
          bufHidReport[1] = 0;
          bufHidReport[cnt+2] = KICAD_GET_KEY(key);
          cnt++;
        }
      }
      bit <<= 1;
    }
  }

  return;
}


static void
KicadShowKeyMap(kicadKeyMap_t p[][CONFIG_KEY_ROW_CNT], uint8_t *str, const kicadKeyText_t *pText, int lenText)
{
  kicadKeyToggle_t      *map0, *map1, *map;
  int                   code;
  kicadKeyText_t        *pList;

  for(int j = 0; j < CONFIG_KEY_COL_CNT; j++) {
    for(int i = 0; i < CONFIG_KEY_ROW_CNT; i++) {
      map0 = &p[j][i].map[0];
      map1 = &p[j][i].map[1];
      if((map0->cnt && map0->key[0]) ||
         (map1->cnt && map1->key[0])) {
        code = (j << 4) | i;
        printf("setkey %s %d%d %3s", str, j, i, KicadGetKeyName(code));

        for(int s = 0; s < 2; s++) {
          map = &p[j][i].map[s];
          printf("  %d  ", map->cnt);
          for(int k = 0; k < map->cnt; k++) {
#if     KICAD_HOTKEY_NAME
            for(int n = 0; n < lenText; n++) {
              if(pText[n].code == map->key[k]) {
                printf("%-20s ", pText[n].name);
                break;
              }
            }
#else
            printf("%04x ", map->key[k]);
#endif
          }
        }
        puts("\n");
      }
    }
  }

  return;
}


static uint8_t *
KicadGetKeyName(uint8_t code)
{
  uint8_t               *re = NULL;
  const kicadKeyList_t  *p;
  int                   start, end;
  int                   i;

  p = &kicadKeyList[0];
  start = 0;
  end = KICAD_KEYLIST_COUNT-1;
  while(1) {
    i = (start + end) / 2;
    p = &kicadKeyList[i];
    if(p->code == code) {
      re = (uint8_t *)&p->name[0];
      break;
    } else {
      if(start == end) break;

      if(code < p->code) end = i-1;
      else if(code > p->code) start = i+1;
    }
  }

end:

  return re;
}


const kicadKeyText_t    KicadHotkeySchList[] = {
  {"KICAD_SHIFT",          KICAD_SHIFT},
  {"KISCH_ADDCOMP",        KISCH_ADDCOMP},
  {"KISCH_ADDGLABEL",      KISCH_ADDGLABEL},
  {"KISCH_ADDPOWER",       KISCH_ADDPOWER},
  {"KISCH_BUS",            KISCH_BUS},
  {"KISCH_DRAG",           KISCH_DRAG},
  {"KISCH_EDIT",           KISCH_EDIT},
  {"KISCH_ESCAPE",         KISCH_ESCAPE},
  {"KISCH_FIND",           KISCH_FIND},
  {"KISCH_FIT",            KISCH_FIT},
  {"KISCH_ITEMMOVE",       KISCH_ITEMMOVE},
  {"KISCH_JUNCTION",       KISCH_JUNCTION},
  {"KISCH_LEAVESHT",       KISCH_LEAVESHT},
  {"KISCH_MIRRORX",        KISCH_MIRRORX},
  {"KISCH_MIRRORY",        KISCH_MIRRORY},
  {"KISCH_NC",             KISCH_NC},
  {"KISCH_POLYLINE",       KISCH_POLYLINE},
  {"KISCH_REPEAT",         KISCH_REPEAT},
  {"KISCH_ROTATE",         KISCH_ROTATE},
  {"KISCH_TEXT",           KISCH_TEXT},
  {"KISCH_WIRE",           KISCH_WIRE},
  {"KISCH_WIREENTRY",      KISCH_WIREENTRY},
};
#define KICAD_HOSTKEY_SCHLIST_NUM       \
  (sizeof(KicadHotkeySchList)/sizeof(kicadKeyText_t))
const kicadKeyText_t    KicadHotkeyPcbList[] = {
  {"KICAD_SHIFT",          KICAD_SHIFT},
  {"KIPCB_ADDMVIA",        KIPCB_ADDMVIA},
  {"KIPCB_ADDTR",          KIPCB_ADDTR},
  {"KIPCB_ADDTR",          KIPCB_ADDTR},
  {"KIPCB_ADDVIA",         KIPCB_ADDVIA},
  {"KIPCB_AUTOZOOM",       KIPCB_AUTOZOOM},
  {"KIPCB_CUTOP",          KIPCB_CUTOP},
  {"KIPCB_CUIN1",          KIPCB_CUIN1},
  {"KIPCB_CUIN2",          KIPCB_CUIN2},
  {"KIPCB_CUBOTTOM",       KIPCB_CUBOTTOM},
  {"KIPCB_CUNEXT",         KIPCB_CUNEXT},
  {"KIPCB_CUPREV",         KIPCB_CUPREV},
  {"KIPCB_DEL",            KIPCB_DEL},
  {"KIPCB_DELTRSEG",       KIPCB_DELTRSEG},
  {"KIPCB_DRAG",           KIPCB_DRAG},
  {"KIPCB_ESCAPE",         KIPCB_ESCAPE},
  {"KIPCB_FILLZONE",       KIPCB_FILLZONE},
  {"KIPCB_UNFILLZONE",     KIPCB_UNFILLZONE},
  {"KIPCB_FIND",           KIPCB_FIND},
  {"KIPCB_FLIP",           KIPCB_FLIP},
  {"KIPCB_GRIDFAST1",      KIPCB_GRIDFAST1},
  {"KIPCB_GRIDFAST2",      KIPCB_GRIDFAST2},
  {"KIPCB_GRIDNEXT",       KIPCB_GRIDNEXT},
  {"KIPCB_GRIDPREV",       KIPCB_GRIDPREV},
  {"KIPCB_MACRO0",         KIPCB_MACRO0},
  {"KIPCB_MACRO1",         KIPCB_MACRO1},
  {"KIPCB_MACRO2",         KIPCB_MACRO2},
  {"KIPCB_MACRO3",         KIPCB_MACRO3},
  {"KIPCB_MOVE",           KIPCB_MOVE},
  {"KIPCB_ROTATE",         KIPCB_ROTATE},
  {"KIPCB_TRDISPLAY",      KIPCB_TRDISPLAY},
  {"KIPCB_TRPOSTURE",      KIPCB_TRPOSTURE},
  {"KIPCB_TRWNEXT",        KIPCB_TRWNEXT},
  {"KIPCB_VIEWNORMAL",     KIPCB_VIEWNORMAL},
  {"KIPCB_VIEWGL",         KIPCB_VIEWGL},
};
#define KICAD_HOSTKEY_PCBLIST_NUM       \
  (sizeof(KicadHotkeyPcbList)/sizeof(kicadKeyText_t))


void
KicadShowKeyMapAll(void)
{
  puts("# keymap for sch\n");
  KicadShowKeyMap(kicadKeyMapSch, "sch",
                  KicadHotkeySchList, KICAD_HOSTKEY_PCBLIST_NUM);
  puts("# keymap pcb\n");
  KicadShowKeyMap(kicadKeyMapPcb, "pcb",
                  KicadHotkeyPcbList, KICAD_HOSTKEY_PCBLIST_NUM);
  puts("# keymap end\n");

  return;
}


void
KicadSetKeyMap(int ctg, int scancode, int shift, kicadKeyToggle_t *map)
{
  kicadKeyToggle_t      *p;
  int                   row, col;

#if 1
  printf("# ctg:%x code:%x shift:%x cnt:%x",
         ctg, scancode, shift, map->cnt);
  for(int i = 0; i < map->cnt; i++) {
    printf(" %x", map->key[i]);
  }
  puts("\n");
#endif

  row =  scancode       & 0xf;
  col = (scancode >> 4) & 0xf;

  if(!ctg) {
    p = &(kicadKeyMapSch[col][row].map[shift]);
  } else {
    p = &(kicadKeyMapPcb[col][row].map[shift]);
  }
  memcpy(p, map, sizeof(kicadKeyToggle_t));

  return;
}



int
KicadCommandSetKey(int ac, uint8_t *av[])
{
  int               ctg = 0;
  int               code;           /* scan code */
  uint8_t           **p;
  int               pos;

  kicadKeyToggle_t  map, mapShift;
  if(!strncmp(av[1], "sch", 3)) {
    ctg = KICAD_KEYMAP_SCH;
  } else if(!strncmp(av[1], "pcb", 3)) {
    ctg = KICAD_KEYMAP_PCB;
  }

  p = &av[2];
  pos = 2;          /* present position */
  code = strtoul(*p++, NULL, 16);
  pos++;
  p++;              /* skip key top text */
  pos++;
  for(int j = 0; j < 2; j++) {
    memset(&map, 0, sizeof(map));
    if(pos < ac) {
      int           i = 0;
      map.cnt = strtoul(*p, NULL, 10);
      p++;  pos++;
      if(map.cnt > KICAD_NUMOF_KEYS) map.cnt = KICAD_NUMOF_KEYS;
      for( ; i < map.cnt && pos < ac; i++) {
        if(!isdigit(*p[0])) {
          const kicadKeyText_t  *pText;
          int                   cntText;
          if(ctg == KICAD_KEYMAP_SCH) {
            pText = KicadHotkeySchList;
            cntText = KICAD_HOSTKEY_SCHLIST_NUM;
          } else {
            pText = KicadHotkeyPcbList;
            cntText = KICAD_HOSTKEY_SCHLIST_NUM;
          }
          for(int k = 0; k < cntText; k++) {
            if(!strncmp(pText->name, *p, strlen(*p))) {
              map.key[i] = pText->code;
              break;
            }
            pText++;
          }
        } else {
          map.key[i] = strtoul(*p, NULL, 16);
        }
        p++;  pos++;
      }
      if(map.cnt > i) map.cnt = i;
    }
    KicadSetKeyMap(ctg, code, j, &map);
  }

  return 0;
}


int
KicadEraseEeprom(void)
{
  int         size, cnt;
  uint32_t    addr;

  /* size of one table */
  size = sizeof(kicadKeyMap_t) * CONFIG_KEY_COL_CNT * CONFIG_KEY_ROW_CNT;

  /* erase */
  addr = CONFIG_MEMORY_KEYMAP;
  cnt = 2*size + sizeof(uint32_t);
  while(cnt) {
    DevFlashWrite32(0, addr, 0);
    addr += sizeof(uint32_t);
    cnt  -= sizeof(uint32_t);
  }

  return 0;
}


int
KicadSaveToEeprom(void)
{
  int         size;
  uint32_t    addr;
  uint32_t    *p;
  uint32_t    crc;

  /* size of one table */
  size = sizeof(kicadKeyMap_t) * CONFIG_KEY_COL_CNT * CONFIG_KEY_ROW_CNT;
 
  /* program */
  DevFlashProgram(0, CONFIG_MEMORY_KEYMAP,      size, (uint8_t *)kicadKeyMapSch);
  DevFlashProgram(0, CONFIG_MEMORY_KEYMAP+size, size, (uint8_t *)kicadKeyMapPcb);
  /* calc crc and burn */
  crc = 0;
  crc = Crc32Calc(crc, (uint8_t *) CONFIG_MEMORY_KEYMAP, 2*size);
  DevFlashWrite32(0, CONFIG_MEMORY_KEYMAP+ 2*size, crc);

  return 0;
}


int
KicadLoadFromEeprom(void)
{
  int           result = -1;
  uint32_t      crc = 0;
  int           size;
  volatile uint32_t     *pCrc;

  /* size of one table */
  size = sizeof(kicadKeyMap_t) * CONFIG_KEY_COL_CNT * CONFIG_KEY_ROW_CNT;

  pCrc = (volatile uint32_t *) (CONFIG_MEMORY_KEYMAP + 2*size);
  crc = Crc32Calc(crc, (uint8_t *) CONFIG_MEMORY_KEYMAP, 2*size);
  if(*pCrc == crc) {
    /* copy the key map in eeprom to local key map */
    memcpy(kicadKeyMapSch, (uint8_t *)CONFIG_MEMORY_KEYMAP,      size);
    memcpy(kicadKeyMapPcb, (uint8_t *)CONFIG_MEMORY_KEYMAP+size, size);
    result = 0;
#if 0
  } else {
    puts("  no data or broken\n");
#endif
  }


  return result;
}


int
KicadDumpEeprom(void)
{
  int           size, cnt;
  uint16_t      *p = (uint16_t *)CONFIG_MEMORY_KEYMAP;
  uint32_t      crc;

  /* size of one table */
  size = sizeof(kicadKeyMap_t) * CONFIG_KEY_COL_CNT * CONFIG_KEY_ROW_CNT;

  cnt = 2*size;
  while(cnt > 0) {
    printf("%4x %4x %4x %4x %4x", p[0], p[1], p[2], p[3], p[4]);
    p += 5;
    cnt -= 5*sizeof(uint16_t);

    printf("   %4x %4x %4x %4x %4x\n", p[0], p[1], p[2], p[3], p[4]);
    p += 5;
    cnt -= 5*sizeof(uint16_t);
  }

  crc = 0;
  crc = Crc32Calc(crc, (uint8_t *) CONFIG_MEMORY_KEYMAP, 2*size);

  p = (uint16_t *)(CONFIG_MEMORY_KEYMAP + 2*size);
  printf("crc: %08x, calc: %x\n", *(uint32_t *)p, crc);

  return 0;
}
