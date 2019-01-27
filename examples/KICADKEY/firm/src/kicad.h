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

#ifndef _KICAD_H_
#define _KICAD_H_


/*********************************************************
 * KICAD EESCHEMA  key assignment
 */
#define KICAD_MOD(x)            ((x) << 8)
#define KICAD_GET_KEY(x)        ((x) & 0xff)
#define KICAD_GET_MOD(x)        (((x)>>8) & 0xff)
#define KICAD_SHIFT             (0xffff)
#define KICAD_CTRL_MASK         (0x8000)

/*********************************************************
 * KICAD EESCHEMA  key assignment
 */
#define KISCH_NONE              (KEYBOARD_NOEVENT)

#define KISCH_ESCAPE            (KEYBOARD_ESCAPE)
#define KISCH_FIT               (KEYBOARD_HOME)
#define KISCH_EDIT              (KEYBOARD_E)
#define KISCH_DELETE            (KEYBOARD_DELETEFW)
#define KISCH_ROTATE            (KEYBOARD_R)
#define KISCH_DRAG              (KEYBOARD_G)
#define KISCH_FIND              ((KEYBOARD_F) | (KICAD_MOD(KEYBOARD_LCTRL)))
#define KISCH_REPEAT            (KEYBOARD_INSERT)
#define KISCH_ITEMMOVE          (KEYBOARD_M)
#define KISCH_MIRRORX           (KEYBOARD_X)
#define KISCH_MIRRORY           (KEYBOARD_Y)
#define KISCH_WIRE              (KEYBOARD_W)
#define KISCH_BUS               (KEYBOARD_B)
#define KISCH_ADDGLABEL         ((KEYBOARD_H) | (KICAD_MOD(KEYBOARD_LCTRL)))
#define KISCH_JUNCTION          (KEYBOARD_J)
#define KISCH_NC                (KEYBOARD_Q)
#define KISCH_WIREENTRY         (KEYBOARD_Z)
#define KISCH_ADDCOMP           (KEYBOARD_A)
#define KISCH_ADDPOWER          (KEYBOARD_P)
#define KISCH_LEAVESHT          ((KEYBOARD_BS) | (KICAD_MOD(KEYBOARD_LALT)))
#define KISCH_POLYLINE          (KEYBOARD_I)
#define KISCH_TEXT              (KEYBOARD_T)

/*********************************************************
 * KICAD PCBNEW  key assignment
 */
#define KIPCB_NONE              (KEYBOARD_NOEVENT)

#define KIPCB_ESCAPE            (KEYBOARD_ESCAPE)
#define KIPCB_DEL               (KEYBOARD_DELETEFW)
#define KIPCB_DELTRSEG          (KEYBOARD_BS)
#define KIPCB_ADDTR             (KEYBOARD_X)
#define KIPCB_ADDVIA            (KEYBOARD_V)
#define KIPCB_ADDMVIA           ((KEYBOARD_V) | KICAD_MOD(KEYBOARD_LCTRL))
#define KIPCB_TRPOSTURE         (KEYPAD_SLASH)
#define KIPCB_DRAGTR            (KEYBOARD_D)
#define KIPCB_PLACE             (KEYBOARD_P)
#define KIPCB_COPY              (KEYBOARD_C)
#define KIPCB_ENDTR             (KEYBOARD_END)
#define KIPCB_MOVE              (KEYBOARD_M)
#define KIPCB_FLIP              (KEYBOARD_F)
#define KIPCB_ROTATE            (KEYBOARD_R)
#define KIPCB_DRAG              (KEYBOARD_G)
#define KIPCB_GETMOVEFP         (KEYBOARD_T)
#define KIPCB_LOCKUNLOCKFP      (KEYBOARD_L)
#define KIPCB_SAVE              ((KEYBOARD_S) | KICAD_MOD(KEYBOARD_LCTRL))
#define KIPCB_LOAD              ((KEYBOARD_L) | KICAD_MOD(KEYBOARD_LCTRL))
#define KIPCB_FIND              ((KEYBOARD_F) | KICAD_MOD(KEYBOARD_LCTRL))
#define KIPCB_EDIT              (KEYBOARD_END)
#define KIPCB_CUTOP             (KEYBOARD_PAGEUP)
#define KIPCB_CUIN1             (KEYBOARD_F5)
#define KIPCB_CUIN2             (KEYBOARD_F6)
#define KIPCB_CUBOTTOM          (KEYBOARD_PAGEDOWN)
#define KIPCB_CUNEXT            (KEYPAD_PLUS)
#define KIPCB_CUPREV            (KEYPAD_MINUS)
#define KIPCB_ADDMOD            (KEYBOARD_O)
#define KIPCB_TRWNEXT           (KEYBOARD_W)
#define KIPCB_TRWPREV           ((KEYBOARD_W) | KICAD_MOD(KEYBOARD_LCTRL))
#define KIPCB_TRDISPLAY         (KEYBOARD_K)
#define KIPCB_GRIDFAST1         ((KEYBOARD_1) | KICAD_MOD(KEYBOARD_LALT))
#define KIPCB_GRIDFAST2         ((KEYBOARD_2) | KICAD_MOD(KEYBOARD_LALT))
#define KIPCB_GRIDNEXT          ((KEYBOARD_N))
#define KIPCB_GRIDPREV          ((KEYBOARD_N) | KICAD_MOD(KEYBOARD_LSHIFT))
#define KIPCB_MACRO0            (KEYBOARD_0)
#define KIPCB_MACRO1            (KEYBOARD_1)
#define KIPCB_MACRO2            (KEYBOARD_2)
#define KIPCB_MACRO3            (KEYBOARD_3)
#define KIPCB_MACRO4            (KEYBOARD_4)
#define KIPCB_MACRO5            (KEYBOARD_5)
#define KIPCB_MACRO6            (KEYBOARD_6)
#define KIPCB_MACRO7            (KEYBOARD_7)
#define KIPCB_MACRO8            (KEYBOARD_8)
#define KIPCB_CONTRUST          (KEYBOARD_H)
#define KIPCB_VIEWNORMAL        (KEYBOARD_F9)
#define KIPCB_VIEW2D            (KEYBOARD_F12)
#define KIPCB_VIEWGL            (KEYBOARD_F11)
#define KIPCB_FILLZONE          (KEYBOARD_B)
#define KIPCB_UNFILLZONE        ((KEYBOARD_B) | KICAD_MOD(KEYBOARD_LCTRL))
#define KIPCB_AUTOZOOM          (KEYBOARD_HOME)



typedef struct {
#define KICAD_NUMOF_KEYS        4
  uint16_t              key[KICAD_NUMOF_KEYS];  /* key toggle list */
  uint8_t               cnt;            /* total toggle count */
  uint8_t               pos;            /* present toggle position */
} kicadKeyToggle_t;
typedef struct {
  kicadKeyToggle_t      map[2];         /* 0: normal, 1: shifted */
} kicadKeyMap_t;



/* prototypes */
void                    KicadSwitchInit(void);
void                    KicadSwitchLoop(void);
void                    KicadShowKeyMapAll(void);
void                    KicadSetKeyMap(int ctg, int scancode, int shift, kicadKeyToggle_t *map);
#define KICAD_KEYMAP_SCH        0
#define KICAD_KEYMAP_PCB        1
#define KICAD_KEYMAP_NOTSHIFT   0
#define KICAD_KEYMAP_SHIFT      1

int                     KicadCommandSetKey(int ac, uint8_t *av[]);
int                     KicadEraseEeprom(void);
int                     KicadSaveToEeprom(void);
int                     KicadLoadFromEeprom(void);
int                     KicadDumpEeprom(void);


#ifdef  _KICAD_C_
typedef struct {
  uint8_t       name[20];
  uint16_t      code;
} kicadKeyText_t;


static void             KicadScanSpecialKeysSEL(int col, int row);
static void             KicadScanSpecialKeysSHIFT(int col, int row);
static void             KicadBuildHidReport(void);

static void             KicadShowKeyMap(kicadKeyMap_t p[][CONFIG_KEY_ROW_CNT], uint8_t *str, const kicadKeyText_t *pText, int lenText);
static uint8_t *        KicadGetKeyName(uint8_t code);
#endif

#endif
