#define _PLLPID_C_


#include        <stdint.h>

#include        "stm32f7.h"
#include        "si5351.h"
#include        "devCounter.h"


struct _stPllpid {
  int           val;

  int           clk;
  int           div;

  int           kp;
  int           ki;
  int           kd;
};
static struct _stPllpid         pllpid;


void
PllpidInit(int clk, int div)
{

  pllpid.clk = clk;
  pllpid.div = div;

  pllpid.kp = 0x80;
  pllpid.ki = 0x80;
  pllpid.kd = 0x00;

  return;
}


void
PllpidLoop(uint32_t val)
{
  int           ve, vp, vd, vu;
  static int    vi = 0;
  static int           add  = 0;
  static int           cnt = 0;

  static int    buf[16];
  static int    pos = 0;
  int           ave;
  int           i;

  ve = (pllpid.clk >> pllpid.div) - val;

  if(abs(ve) >= 100000) goto fail;

  vp  = ve * pllpid.kp / 0x100;
  vi += ve * pllpid.ki / 0x100;
  vd -= ve * pllpid.kd / 0x100;

  vu = vp + vi + vd;
  add = vu;

  buf[pos] = vu;
  ave = 0;
  for(i = 0; i < 16; i++) {
    ave += buf[i];
  }
  ave /= 16;

  Si5351Init(0,
             0,
             26000000,                  // IN   VCO are not changed
             786432000 + ave, 0,              // VCO0, VCO1
             0, 0, 0 /*+ (u >> 8)*/,            // OUT0, 1, 2
             SI5351_OUT0_USE_VCO0 | SI5351_OUT2_USE_VCO0 | SI5351_OUT2_USE_VCO0);

#if 0
  DevCounterSetPwmDutyValue(TIM3_NUM, 1, add);
#endif

  printf("xx v:%d  e:%d p:%d i:%d d:%d  u:%d\n", val, ve, vp, vi, vd,  vu);

  pos++;
  pos &= 0xf;

 fail:
  return;
}
