#ifndef _UPDATE_SDMMC_H_
#define _UPDATE_SDMMC_H_





void                    UpdateSdmmcInit(void);
int                     UpdateSdmmcProgram(void);

#ifdef  _UPDATE_SDMMC_C_

#define LEN_FNAME               48
#define LEN_VERSION             (17)

typedef struct {
  uint8_t       product[32];
  uint8_t       verFirm[LEN_VERSION+1];
  uint8_t       verBoot[LEN_VERSION+1];

  uint8_t       productFile[32];
  uint8_t       verFile[LEN_VERSION+1];
  uint32_t      addr;
} firmInfo_t;

static int              UpdateSdmmcInitDevice(void);
static int              UpdateSdmmcSearchLatestFile(uint8_t *fname, int len, uint8_t *name);
static int              UpdateSdmmcGetMcuInfo(firmInfo_t *p);

static int              UpdateSdmmcChomp(char *str, int size);
static int              UpdateSdmmcDelimit(char *av[], int ac, char *p, int len);
static int              UpdateSdmmcReadAndBurn(char *strFile, char *strProduct, char *strVersion, uint32_t addr);

#endif

#endif
