#ifndef	_STM32F7_H_
#define	_STM32F7_H_

#if 0
#define	AHB1_BASE		(0x40020000)
#define	AHB2_BASE		(0x48000000)
#define	AHB3_BASE		(0x60000000)
#define	APB1_BASE		(0x40000000)
#define	APB2_BASE		(0x40010000)
#endif
#if 1
#define	AHB1_BASE		(AHB1PERIPH_BASE)
#define	AHB2_BASE		(AHB2PERIPH_BASE)
#define	AHB3_BASE		(AHB3PERIPH_BASE)
#define	APB1_BASE		(APB1PERIPH_BASE)
#define	APB2_BASE		(APB2PERIPH_BASE)
#endif

#ifndef	__IO
#define	__IO		volatile
#endif

struct _stStm32_GPIO {
  __IO uint32_t		moder;
#define	MODE_INPUT(x)		(0 << ((x)*2))
#define	MODE_OUTPUT(x)		(1 << ((x)*2))
#define	MODE_FUNC(x)		(2 << ((x)*2))
#define	MODE_ANALOG(x)		(3 << ((x)*2))
#define	MODE_MASK(x)		(3 << ((x)*2))
#define	MODE_SET(x, v)		((v) << ((x)*2))
  __IO uint32_t		otyper;
#define	OTYPE_PUSHPULL(x)	(0 << (x))
#define OTYPE_OD(x)		(1 << (x))
#define	OTYPE_MASK(x)		(1 << (x))
  __IO uint32_t		ospeedr;
#define	OSPEED_LOW(x)		(0 << ((x)*2))
#define OSPEED_MED(x)		(1 << ((x)*2))
#define	OSPEED_HIGH(x)		(2 << ((x)*2))
#define OSPEED_EXHIGH(x)	(3 << ((x)*2))
#define	OSPEED_MASK(x)		(3 << ((x)*2))
  __IO uint32_t		pupdr;
#define	PUPD_NONE(x)		(0 << ((x)*2))
#define PUPD_PU(x)		(1 << ((x)*2))
#define	PUPD_PD(x)		(2 << ((x)*2))
#define PUPD_RESERVED(x)	(3 << ((x)*2))
#define PUPD_X(x, d)		((d) << ((x)*2))
#define	PUPD_MASK(x)		(3 << ((x)*2))
  __IO uint32_t		idr;
#define	IDR_MASK(x)		(1 << (x))
  __IO uint32_t		odr;
#define	ODR_0(x)		( 0  << (x))
#define ODR_1(x)		( 1  << (x))
#define ODR_X(x, d)		((d) << (x))
#define	ODR_MASK(x)		( 1  << (x))
  __IO uint32_t		bsrr;
#define	BSRR_SET(x)		( 1  << (x))
#define	BSRR_RESET(x)		( 1  << ((x)+16))
  __IO uint32_t		lckr;
#define	LCKR_LCKK_SHIFT		(16)
#define	LCKR_LCKK_0		(0 << (LCKR_LCKK_SHIFT))
#define LCKR_LCKK_1		(1 << (LCKR_LCKK_SHIFT))
#define	LCKR_LCKK_MASK		(1 << (LCKR_LCKK_SHIFT))
  __IO uint32_t		afrl;
#define	AFRL_X(x, d)		(((d)&15) << (((x) & 7)*4))
#define	AFRL_MASK(x)		((0xf)    << (((x) & 7)*4))
#define	AFL_0		0
#define	AFL_1		1
#define	AFL_2		2
#define	AFL_3		3
#define	AFL_4		4
#define	AFL_5		5
#define	AFL_6		6
#define	AFL_7		7

#define	FUNC0_SYS	0
#define	FUNC1_TIM	1
#define	FUNC2_TIM	2
#define	FUNC3_USART	3
#define	FUNC4_I2C	4
#define	FUNC5_SPI	5
#define	FUNC6_SPI	6
#define	FUNC7_USART	7
#define	FUNC8_LPUART	8
#define	FUNC9_CAN	9
#define	FUNC10_USB	10
#define	FUNC11_LCD	11
#define	FUNC12_SD	12
#define	FUNC12_COMP	12
#define	FUNC13_SAI	13
#define	FUNC14_TIM	14
#define	FUNC15		15

  __IO uint32_t		afrh;
#define	AFRH_X(x, d)		(((d)&15) << (((x) & 7)*4))
#define	AFRH_MASK(x)		((0xf)    << (((x) & 7)*4))
#define	AFH_8		0
#define	AFH_9		1
#define	AFH_10		2
#define	AFH_11		3
#define	AFH_12		4
#define	AFH_13		5
#define	AFH_14		6
#define	AFH_15		7
  
#if 0
  __IO uint32_t		brr;
#define	BRR_RESET(x)		( 1  << (x))
  
  __IO uint32_t		gpio_reserved[0xf5];
#else
  __IO uint32_t		gpio_reserved[0xf6];
#endif
};
#define	GPIO_MODULE_A	0
#define	GPIO_MODULE_B	1
#define	GPIO_MODULE_C	2
#define	GPIO_MODULE_D	3
#define	GPIO_MODULE_E	4
#define	GPIO_MODULE_F	5
#define	GPIO_MODULE_G	6
#define	GPIO_MODULE_H	7
#define	GPIO_MODULE_I	8
#define	GPIO_MODULE_J	9
#define	GPIO_MODULE_K	10


struct _stStm32_RCC {
  __IO uint32_t		cr;		/* 0x00 */
#define	PLLSAIRDY_SHIFT		29
#define	PLLSAIRDY_MASK		(1 << (PLLSAIRDY_SHIFT))
#define	PLLSAIRDY_NO		(0 << (PLLSAIRDY_SHIFT))
#define	PLLSAIRDY_YES		(1 << (PLLSAIRDY_SHIFT))
#define	PLLSAION_SHIFT		27
#define	PLLSAION_MASK		(1 << (PLLSAION_SHIFT))
#define	PLLSAION_NO		(0 << (PLLSAION_SHIFT))
#define	PLLSAION_YES		(1 << (PLLSAION_SHIFT))
#define	PLLI2SRDY_SHIFT		27
#define	PLLI2SRDY_MASK		(1 << (PLLI2SRDY_SHIFT))
#define	PLLI2SRDY_NO		(0 << (PLLI2SRDY_SHIFT))
#define	PLLI2SRDY_YES		(1 << (PLLI2SRDY_SHIFT))
#define	PLLI2SON_SHIFT		26
#define	PLLI2SON_MASK		(1 << (PLLI2SON_SHIFT))
#define	PLLI2SON_NO		(0 << (PLLI2SON_SHIFT))
#define	PLLI2SON_YES		(1 << (PLLI2SON_SHIFT))
#define	PLLRDY_SHIFT		25
#define	PLLRDY_MASK		(1 << (PLLRDY_SHIFT))
#define	PLLRDY_NO		(0 << (PLLRDY_SHIFT))
#define	PLLRDY_YES		(1 << (PLLRDY_SHIFT))
#define	PLLON_SHIFT		24
#define	PLLON_MASK		(1 << (PLLON_SHIFT))
#define	PLLON_NO		(0 << (PLLON_SHIFT))
#define	PLLON_YES		(1 << (PLLON_SHIFT))
#define	CSSON_SHIFT		19
#define	CSSON_MASK		(1 << (CSSON_SHIFT))
#define	CSSON_NO		(0 << (CSSON_SHIFT))
#define	CSSON_YES		(1 << (CSSON_SHIFT))
#define	HSEBYP_SHIFT		18
#define	HSEBYP_MASK		(1 << (HSEBYP_SHIFT))
#define	HSEBYP_NO		(0 << (HSEBYP_SHIFT))
#define	HSEBYP_YES		(1 << (HSEBYP_SHIFT))
#define	HSERDY_SHIFT		17
#define	HSERDY_MASK		(1 << (HSERDY_SHIFT))
#define	HSERDY_NO		(0 << (HSERDY_SHIFT))
#define	HSERDY_YES		(1 << (HSERDY_SHIFT))
#define	HSEON_SHIFT		16
#define	HSEON_MASK		(1 << (HSEON_SHIFT))
#define	HSEON_NO		(0 << (HSEON_SHIFT))
#define	HSEON_YES		(1 << (HSEON_SHIFT))
#define	HSICAL_SHIFT		8
#define	HSICAL_MASK		(0xff << (HSICAL_SHIFT))
#define	HSITRIM_SHIFT		3
#define	HSITRIM_MASK		(0x1f << (HSITRIM_SHIFT))
#define	MSIRDY_SHIFT		1
#define	MSIRDY_MASK		(1 << (MSIRDY_SHIFT))
#define	MSIRDY_NO		(0 << (MSIRDY_SHIFT))
#define	MSIRDY_YES		(1 << (MSIRDY_SHIFT))
#define	MSION_SHIFT		0
#define	MSION_MASK		(1 << (MSION_SHIFT))
#define	MSION_NO		(0 << (MSION_SHIFT))
#define	MSION_YES		(1 << (MSION_SHIFT))

  __IO uint32_t		pllcfgr;	/* 0x04 */
#define	PLLR_SHIFT		28
#define	PLLR_MASK		(7 << (PLLR_SHIFT))
#define	PLLR_DIV_NG0		(0 << (PLLR_SHIFT))
#define	PLLR_DIV_NG1		(1 << (PLLR_SHIFT))
#define	PLLR_DIV2		(2 << (PLLR_SHIFT))
#define	PLLR_DIV3		(3 << (PLLR_SHIFT))
#define	PLLR_DIV31		(31 << (PLLR_SHIFT))
#define	PLLQ_SHIFT		24
#define	PLLQ_MASK		(0xf << (PLLQ_SHIFT))
#define	PLLQ_DIV_NG0		(0 << (PLLQ_SHIFT))
#define	PLLQ_DIV_NG1		(1 << (PLLQ_SHIFT))
#define	PLLQ_DIV2		(2 << (PLLQ_SHIFT))
#define	PLLQ_DIV3		(3 << (PLLQ_SHIFT))
#define	PLLQ_DIV15		(15 << (PLLQ_SHIFT))
#define	PLLSRC_SHIFT		22
#define	PLLSRC_MASK		(1 << (PLLSRC_SHIFT))
#define	PLLSRC_HSI		(0 << (PLLSRC_SHIFT))
#define	PLLSRC_HSE		(1 << (PLLSRC_SHIFT))
#define	PLLP_SHIFT		16
#define	PLLP_MASK		(2 << (PLLP_SHIFT))
#define	PLLP_DIV2		(0 << (PLLP_SHIFT))
#define	PLLP_DIV4		(1 << (PLLP_SHIFT))
#define	PLLP_DIV6		(2 << (PLLP_SHIFT))
#define	PLLP_DIV8		(3 << (PLLP_SHIFT))
#define	PLLN_SHIFT		6
#define	PLLN_MASK		(0x1ff << (PLLN_SHIFT))
#define	PLLN_MULX(x)		(((x) & 0x1ff) << (PLLN_SHIFT))
#define	PLLN_MUL0_XXX		(0 << (PLLN_SHIFT))
#define	PLLN_MUL1_XXX		(1 << (PLLN_SHIFT))
#define	PLLN_MUL2		(2 << (PLLN_SHIFT))
#define	PLLN_MUL3		(3 << (PLLN_SHIFT))
#define	PLLN_MUL432		(432 << (PLLN_SHIFT))
#define	PLLN_MUL433_XXX		(433 << (PLLN_SHIFT))
#define	PLLN_MUL512_XXX		(512 << (PLLN_SHIFT))
#define	PLLM_SHIFT		0
#define	PLLM_MASK		(0x2f << (PLLM_SHIFT))
#define	PLLM_DIV0_XXX		(0 << (PLLM_SHIFT))
#define	PLLM_DIV1_XXX		(1 << (PLLM_SHIFT))
#define	PLLM_DIV2		(2 << (PLLM_SHIFT))
#define	PLLM_DIV3		(3 << (PLLM_SHIFT))
#define	PLLM_DIV63		(63 << (PLLM_SHIFT))

  __IO uint32_t		cfgr;		/* 0x08 */
#define	MCO2_SHIFT		30
#define	MCO2_MASK		(3 << (MCO2_SHIFT))
#define	MCO2_SYSCLK		(0 << (MCO2_SHIFT))
#define	MCO2_PLLI2S		(1 << (MCO2_SHIFT))
#define	MCO2_HSE		(2 << (MCO2_SHIFT))
#define	MCO2_PLL		(3 << (MCO2_SHIFT))
#define	MCO2PRE_SHIFT		29
#define	MCO2PRE_MASK		(7 << (MCO2PRE_SHIFT))
#define	MCO2PRE_DIV1		(0 << (MCO2PRE_SHIFT))
#define	MCO2PRE_DIV2		(4 << (MCO2PRE_SHIFT))
#define	MCO2PRE_DIV3		(5 << (MCO2PRE_SHIFT))
#define	MCO2PRE_DIV4		(6 << (MCO2PRE_SHIFT))
#define	MCO2PRE_DIV5		(7 << (MCO2PRE_SHIFT))
#define	MCO1PRE_SHIFT		24
#define	MCO1PRE_MASK		(7 << (MCO1PRE_SHIFT))
#define	MCO1PRE_DIV1		(0 << (MCO1PRE_SHIFT))
#define	MCO1PRE_DIV2		(4 << (MCO1PRE_SHIFT))
#define	MCO1PRE_DIV3		(5 << (MCO1PRE_SHIFT))
#define	MCO1PRE_DIV4		(6 << (MCO1PRE_SHIFT))
#define	MCO1PRE_DIV5		(7 << (MCO1PRE_SHIFT))
#define	I2SSRC_SHIFT		23
#define	I2SSRC_MASK		(3 << (I2SSRC_SHIFT))
#define	I2SSRC_PLLI2S		(0 << (I2SSRC_SHIFT))
#define	I2SSRC_I2SCKIN		(1 << (I2SSRC_SHIFT))
#define	MCO1_SHIFT		21
#define	MCO1_MASK		(3 << (MCO1_SHIFT))
#define	MCO1_HSI		(0 << (MCO1_SHIFT))
#define	MCO1_LSE		(1 << (MCO1_SHIFT))
#define	MCO1_HSE		(2 << (MCO1_SHIFT))
#define	MCO1_PLL		(3 << (MCO1_SHIFT))
#define	RTCPRE_SHIFT		16
#define	RTCPRE_MASK		(0x1f << (RTCPRE_SHIFT))
#define	RTCPRE_NOCLK		(0 << (RTCPRE_SHIFT))
#define	RTCPRE_DIV2		(2 << (RTCPRE_SHIFT))
#define	RTCPRE_DIV3		(3 << (RTCPRE_SHIFT))
#define	RTCPRE_DIV31		(31 << (RTCPRE_SHIFT))
#define	PPRE2_SHIFT		13		/* APB2 */
#define	PPRE2_MASK		(7 << (PPRE2_SHIFT))
#define	PPRE2_DIV1		(0 << (PPRE2_SHIFT))
#define	PPRE2_DIV2		(4 << (PPRE2_SHIFT))
#define	PPRE2_DIV4		(5 << (PPRE2_SHIFT))
#define	PPRE2_DIV8		(6 << (PPRE2_SHIFT))
#define	PPRE2_DIV16		(7 << (PPRE2_SHIFT))
#define	PPRE1_SHIFT		10		/* APB1 */
#define	PPRE1_MASK		(7 << (PPRE1_SHIFT))
#define	PPRE1_DIV1		(0 << (PPRE1_SHIFT))
#define	PPRE1_DIV2		(4 << (PPRE1_SHIFT))
#define	PPRE1_DIV4		(5 << (PPRE1_SHIFT))
#define	PPRE1_DIV8		(6 << (PPRE1_SHIFT))
#define	PPRE1_DIV16		(7 << (PPRE1_SHIFT))
#define	HPRE_SHIFT		4		/* AHB */
#define	HPRE_MASK		(0xf << (HPRE_SHIFT))
#define	HPRE_DIV1		(0 << (HPRE_SHIFT))
#define	HPRE_DIV2		(8 << (HPRE_SHIFT))
#define	HPRE_DIV4		(9 << (HPRE_SHIFT))
#define	HPRE_DIV8		(10 << (HPRE_SHIFT))
#define	HPRE_DIV16		(11 << (HPRE_SHIFT))
#define	HPRE_DIV64		(12 << (HPRE_SHIFT))
#define	HPRE_DIV128		(13 << (HPRE_SHIFT))
#define	HPRE_DIV256		(14 << (HPRE_SHIFT))
#define	HPRE_DIV512		(15 << (HPRE_SHIFT))
#define	SYSCLK_SWS_SHIFT	2	
#define	SYSCLK_SWS_MASK		(3 << (SYSCLK_SWS_SHIFT))
#define	SYSCLK_SWS_MSI		(0 << (SYSCLK_SWS_SHIFT))
#define	SYSCLK_SWS_HSI16	(1 << (SYSCLK_SWS_SHIFT))
#define	SYSCLK_SWS_HSE		(2 << (SYSCLK_SWS_SHIFT))
#define	SYSCLK_SWS_PLL		(3 << (SYSCLK_SWS_SHIFT))
#define	SYSCLK_SW_SHIFT		0
#define	SYSCLK_SW_MASK		(3 << (SYSCLK_SW_SHIFT))
#define	SYSCLK_SW_MSI		(0 << (SYSCLK_SW_SHIFT))
#define	SYSCLK_SW_HSI16		(1 << (SYSCLK_SW_SHIFT))
#define	SYSCLK_SW_HSE		(2 << (SYSCLK_SW_SHIFT))
#define	SYSCLK_SW_PLL		(3 << (SYSCLK_SW_SHIFT))

  __IO uint32_t		cir;		/* 0x0c */
  __IO uint32_t		ahb1rstr;	/* 0x10 */
  __IO uint32_t		ahb2rstr;	/* 0x14 */
  __IO uint32_t		ahb3rstr;	/* 0x18 */
  __IO uint32_t		reserved0x1c;	/* 0x1c */
  __IO uint32_t		apb1rstr1;	/* 0x20 */
  __IO uint32_t		apb1rstr2;	/* 0x24 */
  __IO uint32_t		reserved0x28;	/* 0x28 */
  __IO uint32_t		reserved0x2c;	/* 0x2c */
  __IO uint32_t		ahb1enr;	/* 0x30 */
#define	GPIOHEN_SHIFT		7
#define	GPIOHEN_MASK		(1 << (GPIOHEN_SHIFT))
#define	GPIOHEN_NO		(0 << (GPIOHEN_SHIFT))
#define	GPIOHEN_YES		(1 << (GPIOHEN_SHIFT))
#define	GPIOEEN_SHIFT		4
#define	GPIOEEN_MASK		(1 << (GPIOEEN_SHIFT))
#define	GPIOEEN_NO		(0 << (GPIOEEN_SHIFT))
#define	GPIOEEN_YES		(1 << (GPIOEEN_SHIFT))
#define	GPIODEN_SHIFT		3
#define	GPIODEN_MASK		(1 << (GPIODEN_SHIFT))
#define	GPIODEN_NO		(0 << (GPIODEN_SHIFT))
#define	GPIODEN_YES		(1 << (GPIODEN_SHIFT))
#define	GPIOCEN_SHIFT		2
#define	GPIOCEN_MASK		(1 << (GPIOCEN_SHIFT))
#define	GPIOCEN_NO		(0 << (GPIOCEN_SHIFT))
#define	GPIOCEN_YES		(1 << (GPIOCEN_SHIFT))
#define	GPIOBEN_SHIFT		1
#define	GPIOBEN_MASK		(1 << (GPIOBEN_SHIFT))
#define	GPIOBEN_NO		(0 << (GPIOBEN_SHIFT))
#define	GPIOBEN_YES		(1 << (GPIOBEN_SHIFT))
#define	GPIOAEN_SHIFT		0
#define	GPIOAEN_MASK		(1 << (GPIOAEN_SHIFT))
#define	GPIOAEN_NO		(0 << (GPIOAEN_SHIFT))
#define	GPIOAEN_YES		(1 << (GPIOAEN_SHIFT))


#define	TSCEN_SHIFT		16
#define	TSCEN_MASK		(1 << (TSCEN_SHIFT))
#define	TSCEN_NO		(0 << (TSCEN_SHIFT))
#define	TSCEN_YES		(1 << (TSCEN_SHIFT))
#define	CRCEN_SHIFT		12
#define	CRCEN_MASK		(1 << (CRCEN_SHIFT))
#define	CRCEN_NO		(0 << (CRCEN_SHIFT))
#define	CRCEN_YES		(1 << (CRCEN_SHIFT))
#define	FLASHEN_SHIFT		8
#define	FLASHEN_MASK		(1 << (FLASHEN_SHIFT))
#define	FLASHEN_NO		(0 << (FLASHEN_SHIFT))
#define	FLASHEN_YES		(1 << (FLASHEN_SHIFT))
#define	DMA2EN_SHIFT		1
#define	DMA2EN_MASK		(1 << (DMA2EN_SHIFT))
#define	DMA2EN_NO		(0 << (DMA2EN_SHIFT))
#define	DMA2EN_YES		(1 << (DMA2EN_SHIFT))
#define	DMA1EN_SHIFT		0
#define	DMA1EN_MASK		(1 << (DMA1EN_SHIFT))
#define	DMA1EN_NO		(0 << (DMA1EN_SHIFT))
#define	DMA1EN_YES		(1 << (DMA1EN_SHIFT))

  __IO uint32_t		ahb2enr;	/* 0x34 */

#define	ADCEN_SHIFT		13
#define	ADCEN_MASK		(1 << (ADCEN_SHIFT))
#define	ADCEN_NO		(0 << (ADCEN_SHIFT))
#define	ADCEN_YES		(1 << (ADCEN_SHIFT))
#define	AESEN_SHIFT		16
#define	AESEN_MASK		(1 << (AESEN_SHIFT))
#define	AESEN_NO		(0 << (AESEN_SHIFT))
#define	AESEN_YES		(1 << (AESEN_SHIFT))
#define	RNGEN_SHIFT		17
#define	RNGEN_MASK		(1 << (RNGEN_SHIFT))
#define	RNGEN_NO		(0 << (RNGEN_SHIFT))
#define	RNGEN_YES		(1 << (RNGEN_SHIFT))

  __IO uint32_t		ahb3enr;	/* 0x38 */
#define	QSPIEN_SHIFT		1
#define	QSPIEN_MASK		(1 << (QSPIEN_SHIFT))
#define	QSPIEN_NO		(0 << (QSPIEN_SHIFT))
#define	QSPIEN_YES		(1 << (QSPIEN_SHIFT))
#define	FMCEN_SHIFT		0
#define	FMCEN_MASK		(1 << (FMCEN_SHIFT))
#define	FMCEN_NO		(0 << (FMCEN_SHIFT))
#define	FMCEN_YES		(1 << (FMCEN_SHIFT))

  __IO uint32_t		reserved0x3c;	/* 0x3c */
  __IO uint32_t		apb1enr;	/* 0x40 */
#define	LPTIM1EN_SHIFT		31
#define	LPTIM1EN_MASK		(1 << (LPTIM1EN_SHIFT))
#define	LPTIM1EN_NO		(0 << (LPTIM1EN_SHIFT))
#define	LPTIM1EN_YES		(1 << (LPTIM1EN_SHIFT))
#define	OPAMPEN_SHIFT		30
#define	OPAMPEN_MASK		(1 << (OPAMPEN_SHIFT))
#define	OPAMPEN_NO		(0 << (OPAMPEN_SHIFT))
#define	OPAMPEN_YES		(1 << (OPAMPEN_SHIFT))
#define	DAC1EN_SHIFT		29
#define	DAC1EN_MASK		(1 << (DAC1EN_SHIFT))
#define	DAC1EN_NO		(0 << (DAC1EN_SHIFT))
#define	DAC1EN_YES		(1 << (DAC1EN_SHIFT))
#define	PWREN_SHIFT		28
#define	PWREN_MASK		(1 << (PWREN_SHIFT))
#define	PWREN_NO		(0 << (PWREN_SHIFT))
#define	PWREN_YES		(1 << (PWREN_SHIFT))
#define	USBFSEN_SHIFT		26
#define	USBFSEN_MASK		(1 << (USBFSEN_SHIFT))
#define	USBFSEN_NO		(0 << (USBFSEN_SHIFT))
#define	USBFSEN_YES		(1 << (USBFSEN_SHIFT))
#define	CAN1EN_SHIFT		25
#define	CAN1EN_MASK		(1 << (CAN1EN_SHIFT))
#define	CAN1EN_NO		(0 << (CAN1EN_SHIFT))
#define	CAN1EN_YES		(1 << (CAN1EN_SHIFT))
#define	CRSEN_SHIFT		24
#define	CRSEN_MASK		(1 << (CRSEN_SHIFT))
#define	CRSEN_NO		(0 << (CRSEN_SHIFT))
#define	CRSEN_YES		(1 << (CRSEN_SHIFT))
#define	I2C3EN_SHIFT		23
#define	I2C3EN_MASK		(1 << (I2C3EN_SHIFT))
#define	I2C3EN_NO		(0 << (I2C3EN_SHIFT))
#define	I2C3EN_YES		(1 << (I2C3EN_SHIFT))
#define	I2C2EN_SHIFT		22
#define	I2C2EN_MASK		(1 << (I2C2EN_SHIFT))
#define	I2C2EN_NO		(0 << (I2C2EN_SHIFT))
#define	I2C2EN_YES		(1 << (I2C2EN_SHIFT))
#define	I2C1EN_SHIFT		21
#define	I2C1EN_MASK		(1 << (I2C1EN_SHIFT))
#define	I2C1EN_NO		(0 << (I2C1EN_SHIFT))
#define	I2C1EN_YES		(1 << (I2C1EN_SHIFT))
#define	UART4EN_SHIFT		19
#define	UART4EN_MASK		(1 << (UART4EN_SHIFT))
#define	UART4EN_NO		(0 << (UART4EN_SHIFT))
#define	UART4EN_YES		(1 << (UART4EN_SHIFT))
#define	USART3EN_SHIFT		18
#define	USART3EN_MASK		(1 << (USART3EN_SHIFT))
#define	USART3EN_NO		(0 << (USART3EN_SHIFT))
#define	USART3EN_YES		(1 << (USART3EN_SHIFT))
#define	USART2EN_SHIFT		17
#define	USART2EN_MASK		(1 << (USART2EN_SHIFT))
#define	USART2EN_NO		(0 << (USART2EN_SHIFT))
#define	USART2EN_YES		(1 << (USART2EN_SHIFT))
#define	SPI3EN_SHIFT		15
#define	SPI3EN_MASK		(1 << (SPI3EN_SHIFT))
#define	SPI3EN_NO		(0 << (SPI3EN_SHIFT))
#define	SPI3EN_YES		(1 << (SPI3EN_SHIFT))
#define	SPI2EN_SHIFT		14
#define	SPI2EN_MASK		(1 << (SPI2EN_SHIFT))
#define	SPI2EN_NO		(0 << (SPI2EN_SHIFT))
#define	SPI2EN_YES		(1 << (SPI2EN_SHIFT))
#define	WWDGEN_SHIFT		11
#define	WWDGEN_MASK		(1 << (WWDGEN_SHIFT))
#define	WWDGEN_NO		(0 << (WWDGEN_SHIFT))
#define	WWDGEN_YES		(1 << (WWDGEN_SHIFT))
#define	RTCAPBEN_SHIFT		10
#define	RTCAPBEN_MASK		(1 << (RTCAPBEN_SHIFT))
#define	RTCAPBEN_NO		(0 << (RTCAPBEN_SHIFT))
#define	RTCAPBEN_YES		(1 << (RTCAPBEN_SHIFT))
#define	LEDEN_SHIFT		9
#define	LEDEN_MASK		(1 << (LEDEN_SHIFT))
#define	LEDEN_NO		(0 << (LEDEN_SHIFT))
#define	LEDEN_YES		(1 << (LEDEN_SHIFT))
#define	TIM7EN_SHIFT		5
#define	TIM7EN_MASK		(1 << (TIM7EN_SHIFT))
#define	TIM7EN_NO		(0 << (TIM7EN_SHIFT))
#define	TIM7EN_YES		(1 << (TIM7EN_SHIFT))
#define	TIM6EN_SHIFT		4
#define	TIM6EN_MASK		(1 << (TIM6EN_SHIFT))
#define	TIM6EN_NO		(0 << (TIM6EN_SHIFT))
#define	TIM6EN_YES		(1 << (TIM6EN_SHIFT))
#define	TIM3EN_SHIFT		1
#define	TIM3EN_MASK		(1 << (TIM3EN_SHIFT))
#define	TIM3EN_NO		(0 << (TIM3EN_SHIFT))
#define	TIM3EN_YES		(1 << (TIM3EN_SHIFT))
#define	TIM2EN_SHIFT		0
#define	TIM2EN_MASK		(1 << (TIM2EN_SHIFT))
#define	TIM2EN_NO		(0 << (TIM2EN_SHIFT))
#define	TIM2EN_YES		(1 << (TIM2EN_SHIFT))

#if 0
  __IO uint32_t		apb1enr2;	/* 0x5c */
#define	LPTIM2EN_SHIFT		5
#define	LPTIM2EN_MASK		(1 << (LPTIM2EN_SHIFT))
#define	LPTIM2EN_NO		(0 << (LPTIM2EN_SHIFT))
#define	LPTIM2EN_YES		(1 << (LPTIM2EN_SHIFT))
#define	SWPMI1EN_SHIFT		2
#define	SWPMI1EN_MASK		(1 << (SWPMI1EN_SHIFT))
#define	SWPMI1EN_NO		(0 << (SWPMI1EN_SHIFT))
#define	SWPMI1EN_YES		(1 << (SWPMI1EN_SHIFT))
#define	I2C4EN_SHIFT		1
#define	I2C4EN_MASK		(1 << (I2C4EN_SHIFT))
#define	I2C4EN_NO		(0 << (I2C4EN_SHIFT))
#define	I2C4EN_YES		(1 << (I2C4EN_SHIFT))
#define	LPUART1EN_SHIFT		0
#define	LPUART1EN_MASK		(1 << (LPUART1EN_SHIFT))
#define	LPUART1EN_NO		(0 << (LPUART1EN_SHIFT))
#define	LPUART1EN_YES		(1 << (LPUART1EN_SHIFT))
#endif
  
  __IO uint32_t		apb2enr;	/* 0x44 */
#define	DFSDM1EN_SHIFT		24
#define	DFSDM1EN_MASK		(1 << (DFSDM1EN_SHIFT))
#define	DFSDM1EN_NO		(0 << (DFSDM1EN_SHIFT))
#define	DFSDM1EN_YES		(1 << (DFSDM1EN_SHIFT))
#define	SAI1EN_SHIFT		21
#define	SAI1EN_MASK		(1 << (SAI1EN_SHIFT))
#define	SAI1EN_NO		(0 << (SAI1EN_SHIFT))
#define	SAI1EN_YES		(1 << (SAI1EN_SHIFT))
#define	TIM16EN_SHIFT		17
#define	TIM16EN_MASK		(1 << (TIM16EN_SHIFT))
#define	TIM16EN_NO		(0 << (TIM16EN_SHIFT))
#define	TIM16EN_YES		(1 << (TIM16EN_SHIFT))
#define	TIM15EN_SHIFT		16
#define	TIM15EN_MASK		(1 << (TIM15EN_SHIFT))
#define	TIM15EN_NO		(0 << (TIM15EN_SHIFT))
#define	TIM15EN_YES		(1 << (TIM15EN_SHIFT))
#define	USART1EN_SHIFT		14
#define	USART1EN_MASK		(1 << (USART1EN_SHIFT))
#define	USART1EN_NO		(0 << (USART1EN_SHIFT))
#define	USART1EN_YES		(1 << (USART1EN_SHIFT))
#define	SPI1EN_SHIFT		12
#define	SPI1EN_MASK		(1 << (SPI1EN_SHIFT))
#define	SPI1EN_NO		(0 << (SPI1EN_SHIFT))
#define	SPI1EN_YES		(1 << (SPI1EN_SHIFT))
#define	TIM1EN_SHIFT		11
#define	TIM1EN_MASK		(1 << (TIM1EN_SHIFT))
#define	TIM1EN_NO		(0 << (TIM1EN_SHIFT))
#define	TIM1EN_YES		(1 << (TIM1EN_SHIFT))
#define	SDMMC1EN_SHIFT		10
#define	SDMMC1EN_MASK		(1 << (SDMMC1EN_SHIFT))
#define	SDMMC1EN_NO		(0 << (SDMMC1EN_SHIFT))
#define	SDMMC1EN_YES		(1 << (SDMMC1EN_SHIFT))
#define	FWEN_SHIFT		7
#define	FWEN_MASK		(1 << (FWEN_SHIFT))
#define	FWEN_NO			(0 << (FWEN_SHIFT))
#define	FWEN_YES		(1 << (FWEN_SHIFT))
#define	SYSCFGEN_SHIFT		0
#define	SYSCFGEN_MASK		(1 << (SYSCFGEN_SHIFT))
#define	SYSCFGEN_NO		(0 << (SYSCFGEN_SHIFT))
#define	SYSCFGEN_YES		(1 << (SYSCFGEN_SHIFT))

  __IO uint32_t		reserved0x48;	/* 0x48 */
  __IO uint32_t		reserved0x4c;	/* 0x4c */

  __IO uint32_t		ahb1lprenr;	/* 0x50 */
  __IO uint32_t		ahb2lprenr;	/* 0x54 */
  __IO uint32_t		ahb3lprenr;	/* 0x58 */
  __IO uint32_t		reserved0x50;	/* 0x5c */

  __IO uint32_t		apb1lprenr;	/* 0x60 */
  __IO uint32_t		apb2lprenr;	/* 0x64 */
  __IO uint32_t		reserved0x68;	/* 0x68 */
  __IO uint32_t		reserved0x6c;	/* 0x6c */

  __IO uint32_t		bdcr;		/* 0x70 */
#define	LSCOSEL_SHIFT		25
#define	LSCOSEL_MASK		(1 << (LSCOSEL_SHIFT))
#define	LSCOSEL_NO		(0 << (LSCOSEL_SHIFT))
#define	LSCOSEL_YES		(1 << (LSCOSEL_SHIFT))
#define	LSCOEN_SHIFT		24
#define	LSCOEN_MASK		(1 << (LSCOEN_SHIFT))
#define	LSCOEN_NO		(0 << (LSCOEN_SHIFT))
#define	LSCOEN_YES		(1 << (LSCOEN_SHIFT))
#define	BDRST_SHIFT		16
#define	BDRST_MASK		(1 << (BDRST_SHIFT))
#define	BDRST_NO		(0 << (BDRST_SHIFT))
#define	BDRST_YES		(1 << (BDRST_SHIFT))
#define	RTCEN_SHIFT		15
#define	RTCEN_MASK		(1 << (RTCEN_SHIFT))
#define	RTCEN_NO		(0 << (RTCEN_SHIFT))
#define	RTCEN_YES		(1 << (RTCEN_SHIFT))
#define	RTCSELSEL_SHIFT		8
#define	RTCSELSEL_MASK		(3 << (RTCSELSEL_SHIFT))
#define	RTCSELSEL_NONE		(0 << (RTCSELSEL_SHIFT))
#define	RTCSELSEL_LSE		(1 << (RTCSELSEL_SHIFT))
#define	RTCSELSEL_LSI		(2 << (RTCSELSEL_SHIFT))
#define	RTCSELSEL_HSE		(3 << (RTCSELSEL_SHIFT))
#define	LSECSSD_SHIFT		6
#define	LSECSSD_MASK		(1 << (LSECSSD_SHIFT))
#define	LSECSSD_NO		(0 << (LSECSSD_SHIFT))
#define	LSECSSD_YES		(1 << (LSECSSD_SHIFT))
#define	LSECSSON_SHIFT		5
#define	LSECSSON_MASK		(1 << (LSECSSON_SHIFT))
#define	LSECSSON_NO		(0 << (LSECSSON_SHIFT))
#define	LSECSSON_YES		(1 << (LSECSSON_SHIFT))
#define	LSEDRVSEL_SHIFT		3
#define	LSEDRVSEL_MASK		(3 << (LSEDRVSEL_SHIFT))
#define	LSEDRVSEL_LOWERCAP	(0 << (LSEDRVSEL_SHIFT))
#define	LSEDRVSEL_MEDLOWCAP	(1 << (LSEDRVSEL_SHIFT))
#define	LSEDRVSEL_MEDHIGHCAP	(2 << (LSEDRVSEL_SHIFT))
#define	LSEDRVSEL_HIGHERCAP	(3 << (LSEDRVSEL_SHIFT))
#define	LSEBYP_SHIFT		2
#define	LSEBYP_MASK		(1 << (LSEBYP_SHIFT))
#define	LSEBYP_NO		(0 << (LSEBYP_SHIFT))
#define	LSEBYP_YES		(1 << (LSEBYP_SHIFT))
#define	LSERDY_SHIFT		1
#define	LSERDY_MASK		(1 << (LSERDY_SHIFT))
#define	LSERDY_NO		(0 << (LSERDY_SHIFT))
#define	LSERDY_YES		(1 << (EN_SHIFT))
#define	LSEON_SHIFT		0
#define	LSEON_MASK		(1 << (LSEON_SHIFT))
#define	LSEON_NO		(0 << (LSEON_SHIFT))
#define	LSEON_YES		(1 << (LSEON_SHIFT))

  __IO uint32_t		csr;		/* 0x74 */
#define	LPWRRSTF_SHIFT		31
#define	LPWRRSTF_MASK		(1 << (LPWRRSTF_SHIFT))
#define	LPWRRSTF_NO		(0 << (LPWRRSTF_SHIFT))
#define	LPWRRSTF_YES		(1 << (LPWRRSTF_SHIFT))
#define	WWDGRSTF_SHIFT		30
#define	WWDGRSTF_MASK		(1 << (WWDGRSTF_SHIFT))
#define	WWDGRSTF_NO		(0 << (WWDGRSTF_SHIFT))
#define	WWDGRSTF_YES		(1 << (WWDGRSTF_SHIFT))
#define	IWWGRSTF_SHIFT		29
#define	IWWGRSTF_MASK		(1 << (IWWGRSTF_SHIFT))
#define	IWWGRSTF_NO		(0 << (IWWGRSTF_SHIFT))
#define	IWWGRSTF_YES		(1 << (IWWGRSTF_SHIFT))
#define	SFTRSTF_SHIFT		28
#define	SFTRSTF_MASK		(1 << (SFTRSTF_SHIFT))
#define	SFTRSTF_NO		(0 << (SFTRSTF_SHIFT))
#define	SFTRSTF_YES		(1 << (SFTRSTF_SHIFT))
#define	BORRSTF_SHIFT		27
#define	BORRSTF_MASK		(1 << (BORRSTF_SHIFT))
#define	BORRSTF_NO		(0 << (BORRSTF_SHIFT))
#define	BORRSTF_YES		(1 << (BORRSTF_SHIFT))
#define	PINRSTF_SHIFT		26
#define	PINRSTF_MASK		(1 << (PINRSTF_SHIFT))
#define	PINRSTF_NO		(0 << (PINRSTF_SHIFT))
#define	PINRSTF_YES		(1 << (PINRSTF_SHIFT))
#define	OBLRSTF_SHIFT		25
#define	OBLRSTF_MASK		(1 << (OBLRSTF_SHIFT))
#define	OBLRSTF_NO		(0 << (OBLRSTF_SHIFT))
#define	OBLRSTF_YES		(1 << (OBLRSTF_SHIFT))
#define	FWRSTF_SHIFT		24
#define	FWRSTF_MASK		(1 << (FWRSTF_SHIFT))
#define	FWRSTF_NO		(0 << (FWRSTF_SHIFT))
#define	FWRSTF_YES		(1 << (FWRSTF_SHIFT))
#define	RMVF_SHIFT		23
#define	RMVF_MASK		(1 << (RMVF_SHIFT))
#define	RMVF_NO			(0 << (RMVF_SHIFT))
#define	RMVF_YES		(1 << (RMVF_SHIFT))
#define	MSISRANGE_SHIFT		8
#define	MSISRANGE_MASK		(0xf << (MSISRANGE_SHIFT))
#define	MSISRANGE_1MHZ		(4 << (MSISRANGE_SHIFT))
#define	MSISRANGE_2MHZ		(5 << (MSISRANGE_SHIFT))
#define	MSISRANGE_4MHZ		(6 << (MSISRANGE_SHIFT))
#define	MSISRANGE_8MHZ		(7 << (MSISRANGE_SHIFT))
#define	LSIRDY_SHIFT		1
#define	LSIRDY_MASK		(1 << (LSIRDY_SHIFT))
#define	LSIRDY_NO		(0 << (LSIRDY_SHIFT))
#define	LSIRDY_YES		(1 << (LSIRDY_SHIFT))
#define	LSION_SHIFT		0
#define	LSION_MASK		(1 << (LSION_SHIFT))
#define	LSION_NO		(0 << (LSION_SHIFT))
#define	LSION_YES		(1 << (LSION_SHIFT))
  __IO uint32_t		reserved0x78;	/* 0x78 */
  __IO uint32_t		reserved0x7c;	/* 0x7c */
  


  __IO uint32_t		pllsai1cfgr;		/* 0x10 */
  __IO uint32_t		reserved0x14;		/* 0x14 */
  __IO uint32_t		cier;		/* 0x1c */
#define	HSI48_RDYIE_SHIFT	10	
#define	HSI48_RDYIE_MASK	(1 << (HSI48_RDYIE_SHIFT))
#define	HSI48_RDYIE_NO		(0 << (HSI48_RDYIE_SHIFT))
#define	HSI48_RDYIE_YES		(1 << (HSI48_RDYIE_SHIFT))
#define	LSE_CSSIE_SHIFT		9
#define	LSE_CSSIE_MASK		(1 << (LSE_RDYIE_SHIFT))
#define	LSE_CSSIE_NO		(0 << (LSE_RDYIE_SHIFT))
#define	LSE_CSSIE_YES		(1 << (LSE_RDYIE_SHIFT))
#define	PLLSAI1_RDYIE_SHIFT	6
#define	PLLSAI1_RDYIE_MASK	(1 << (PLLSAI1_RDYIE_SHIFT))
#define	PLLSAI1_RDYIE_NO	(0 << (PLLSAI1_RDYIE_SHIFT))
#define	PLLSAI1_RDYIE_YES	(1 << (PLLSAI1_RDYIE_SHIFT))
#define	PLL_RDYIE_SHIFT		5
#define	PLL_RDYIE_MASK		(1 << (PLL_RDYIE_SHIFT))
#define	PLL_RDYIE_NO		(0 << (PLL_RDYIE_SHIFT))
#define	PLL_RDYIE_YES		(1 << (PLL_RDYIE_SHIFT))
#define	HSE_RDYIE_SHIFT		4
#define	HSE_RDYIE_MASK		(1 << (HSE_RDYIE_SHIFT))
#define	HSE_RDYIE_NO		(0 << (HSE_RDYIE_SHIFT))
#define	HSE_RDYIE_YES		(1 << (HSE_RDYIE_SHIFT))
#define	HSI_RDYIE_SHIFT		3
#define	HSI_RDYIE_MASK		(1 << (HSI_RDYIE_SHIFT))
#define	HSI_RDYIE_NO		(0 << (HSI_RDYIE_SHIFT))
#define	HSI_RDYIE_YES		(1 << (HSI_RDYIE_SHIFT))
#define	MSI_RDYIE_SHIFT		2
#define	MSI_RDYIE_MASK		(1 << (MSI_RDYIE_SHIFT))
#define	MSI_RDYIE_NO		(0 << (MSI_RDYIE_SHIFT))
#define	MSI_RDYIE_YES		(1 << (MSI_RDYIE_SHIFT))
#define	LSE_RDYIE_SHIFT		1
#define	LSE_RDYIE_MASK		(1 << (LSE_RDYIE_SHIFT))
#define	LSE_RDYIE_NO		(0 << (LSE_RDYIE_SHIFT))
#define	LSE_RDYIE_YES		(1 << (LSE_RDYIE_SHIFT))
#define	LSI_RDYIE_SHIFT		0
#define	LSI_RDYIE_MASK		(1 << (LSI_RDYIE_SHIFT))
#define	LSI_RDYIE_NO		(0 << (LSI_RDYIE_SHIFT))
#define	LSI_RDYIE_YES		(1 << (LSI_RDYIE_SHIFT))

  __IO uint32_t		cifr;		/* 0x1c */
#define	HSI48_RDYF_SHIFT	10	
#define	HSI48_RDYF_MASK		(1 << (HSI48_RDYF_SHIFT))
#define	HSI48_RDYF_NO		(0 << (HSI48_RDYF_SHIFT))
#define	HSI48_RDYF_YES		(1 << (HSI48_RDYF_SHIFT))
#define	LSE_CSSF_SHIFT		9
#define	LSE_CSSF_MASK		(1 << (LSE_CSSF_SHIFT))
#define	LSE_CSSF_NO		(0 << (LSE_CSSF_SHIFT))
#define	LSE_CSSF_YES		(1 << (LSE_CSSF_SHIFT))
#define	CSSF_SHIFT		8
#define	CSSF_MASK		(1 << (LSE_CSSF_SHIFT))
#define	CSSF_NO			(0 << (LSE_CSSF_SHIFT))
#define	CSSF_YES		(1 << (LSE_CSSF_SHIFT))
#define	PLLSAI1_RDYF_SHIFT	6
#define	PLLSAI1_RDYF_MASK	(1 << (PLLSAI1_RDYF_SHIFT))
#define	PLLSAI1_RDYF_NO		(0 << (PLLSAI1_RDYF_SHIFT))
#define	PLLSAI1_RDYF_YES	(1 << (PLLSAI1_RDYF_SHIFT))
#define	PLL_RDYF_SHIFT		5
#define	PLL_RDYF_MASK		(1 << (PLL_RDYF_SHIFT))
#define	PLL_RDYF_NO		(0 << (PLL_RDYF_SHIFT))
#define	PLL_RDYF_YES		(1 << (PLL_RDYF_SHIFT))
#define	HSE_RDYF_SHIFT		4
#define	HSE_RDYF_MASK		(1 << (HSE_RDYF_SHIFT))
#define	HSE_RDYF_NO		(0 << (HSE_RDYF_SHIFT))
#define	HSE_RDYF_YES		(1 << (HSE_RDYF_SHIFT))
#define	HSI_RDYF_SHIFT		3
#define	HSI_RDYF_MASK		(1 << (HSI_RDYF_SHIFT))
#define	HSI_RDYF_NO		(0 << (HSI_RDYF_SHIFT))
#define	HSI_RDYF_YES		(1 << (HSI_RDYF_SHIFT))
#define	MSI_RDYF_SHIFT		2
#define	MSI_RDYF_MASK		(1 << (MSI_RDYF_SHIFT))
#define	MSI_RDYF_NO		(0 << (MSI_RDYF_SHIFT))
#define	MSI_RDYF_YES		(1 << (MSI_RDYF_SHIFT))
#define	LSE_RDYF_SHIFT		1
#define	LSE_RDYF_MASK		(1 << (LSE_RDYF_SHIFT))
#define	LSE_RDYF_NO		(0 << (LSE_RDYF_SHIFT))
#define	LSE_RDYF_YES		(1 << (LSE_RDYF_SHIFT))
#define	LSI_RDYF_SHIFT		0
#define	LSI_RDYF_MASK		(1 << (LSI_RDYF_SHIFT))
#define	LSI_RDYF_NO		(0 << (LSI_RDYF_SHIFT))
#define	LSI_RDYF_YES		(1 << (LSI_RDYF_SHIFT))
  __IO uint32_t		cicr;		/* 0x20 */
#define	HSI48_RDYC_SHIFT	10	
#define	HSI48_RDYC_MASK		(1 << (HSI48_RDYC_SHIFT))
#define	HSI48_RDYC_NO		(0 << (HSI48_RDYC_SHIFT))
#define	HSI48_RDYC_YES		(1 << (HSI48_RDYC_SHIFT))
#define	LSE_CSSC_SHIFT		9
#define	LSE_CSSC_MASK		(1 << (LSE_CSSC_SHIFT))
#define	LSE_CSSC_NO		(0 << (LSE_CSSC_SHIFT))
#define	LSE_CSSC_YES		(1 << (LSE_CSSC_SHIFT))
#define	CSSC_SHIFT		8
#define	CSSC_MASK		(1 << (LSE_CSSC_SHIFT))
#define	CSSC_NO			(0 << (LSE_CSSC_SHIFT))
#define	CSSC_YES		(1 << (LSE_CSSC_SHIFT))
#define	PLLSAI1_RDYC_SHIFT	6
#define	PLLSAI1_RDYC_MASK	(1 << (PLLSAI1_RDYC_SHIFT))
#define	PLLSAI1_RDYC_NO		(0 << (PLLSAI1_RDYC_SHIFT))
#define	PLLSAI1_RDYC_YES	(1 << (PLLSAI1_RDYC_SHIFT))
#define	PLL_RDYC_SHIFT		5
#define	PLL_RDYC_MASK		(1 << (PLL_RDYC_SHIFT))
#define	PLL_RDYC_NO		(0 << (PLL_RDYC_SHIFT))
#define	PLL_RDYC_YES		(1 << (PLL_RDYC_SHIFT))
#define	HSE_RDYC_SHIFT		4
#define	HSE_RDYC_MASK		(1 << (HSE_RDYC_SHIFT))
#define	HSE_RDYC_NO		(0 << (HSE_RDYC_SHIFT))
#define	HSE_RDYC_YES		(1 << (HSE_RDYC_SHIFT))
#define	HSI_RDYC_SHIFT		3
#define	HSI_RDYC_MASK		(1 << (HSI_RDYC_SHIFT))
#define	HSI_RDYC_NO		(0 << (HSI_RDYC_SHIFT))
#define	HSI_RDYC_YES		(1 << (HSI_RDYC_SHIFT))
#define	MSI_RDYC_SHIFT		2
#define	MSI_RDYC_MASK		(1 << (MSI_RDYC_SHIFT))
#define	MSI_RDYC_NO		(0 << (MSI_RDYC_SHIFT))
#define	MSI_RDYC_YES		(1 << (MSI_RDYC_SHIFT))
#define	LSE_RDYC_SHIFT		1
#define	LSE_RDYC_MASK		(1 << (LSE_RDYC_SHIFT))
#define	LSE_RDYC_NO		(0 << (LSE_RDYC_SHIFT))
#define	LSE_RDYC_YES		(1 << (LSE_RDYC_SHIFT))
#define	LSI_RDYC_SHIFT		0
#define	LSI_RDYC_MASK		(1 << (LSI_RDYC_SHIFT))
#define	LSI_RDYC_NO		(0 << (LSI_RDYC_SHIFT))
#define	LSI_RDYC_YES		(1 << (LSI_RDYC_SHIFT))
  __IO uint32_t		reserved0x24;		/* 0x24 */
  __IO uint32_t		apb2rstr;		/* 0x40 */
#define	DFSDM1RSTR_SHIFT		24
#define	DFSDM1RSTR_MASK		(1 << (DFSDM1RSTR_SHIFT))
#define	DFSDM1RSTR_NO		(0 << (DFSDM1RSTR_SHIFT))
#define	DFSDM1RSTR_YES		(1 << (DFSDM1RSTR_SHIFT))
#define	SAI1RSTR_SHIFT		21
#define	SAI1RSTR_MASK		(1 << (SAI1RSTR_SHIFT))
#define	SAI1RSTR_NO		(0 << (SAI1RSTR_SHIFT))
#define	SAI1RSTR_YES		(1 << (SAI1RSTR_SHIFT))
#define	TIM16RSTR_SHIFT		17
#define	TIM16RSTR_MASK		(1 << (TIM16RSTR_SHIFT))
#define	TIM16RSTR_NO		(0 << (TIM16RSTR_SHIFT))
#define	TIM16RSTR_YES		(1 << (TIM16RSTR_SHIFT))
#define	TIM15RSTR_SHIFT		16
#define	TIM15RSTR_MASK		(1 << (TIM15RSTR_SHIFT))
#define	TIM15RSTR_NO		(0 << (TIM15RSTR_SHIFT))
#define	TIM15RSTR_YES		(1 << (TIM15RSTR_SHIFT))
#define	USART1RSTR_SHIFT		14
#define	USART1RSTR_MASK		(1 << (USART1RSTR_SHIFT))
#define	USART1RSTR_NO		(0 << (USART1RSTR_SHIFT))
#define	USART1RSTR_YES		(1 << (USART1RSTR_SHIFT))
#define	SPI1RSTR_SHIFT		12
#define	SPI1RSTR_MASK		(1 << (SPI1RSTR_SHIFT))
#define	SPI1RSTR_NO		(0 << (SPI1RSTR_SHIFT))
#define	SPI1RSTR_YES		(1 << (SPI1RSTR_SHIFT))
#define	TIM1RSTR_SHIFT		11
#define	TIM1RSTR_MASK		(1 << (TIM1RSTR_SHIFT))
#define	TIM1RSTR_NO		(0 << (TIM1RSTR_SHIFT))
#define	TIM1RSTR_YES		(1 << (TIM1RSTR_SHIFT))
#define	SDMMC1RSTR_SHIFT		10
#define	SDMMC1RSTR_MASK		(1 << (SDMMC1RSTR_SHIFT))
#define	SDMMC1RSTR_NO		(0 << (SDMMC1RSTR_SHIFT))
#define	SDMMC1RSTR_YES		(1 << (SDMMC1RSTR_SHIFT))
#define	FWRSTR_SHIFT		7
#define	FWRSTR_MASK		(1 << (FWRSTR_SHIFT))
#define	FWRSTR_NO		(0 << (FWRSTR_SHIFT))
#define	FWRSTR_YES		(1 << (FWRSTR_SHIFT))
#define	SYSCFGRSTR_SHIFT		0
#define	SYSCFGRSTR_MASK		(1 << (SYSCFGRSTR_SHIFT))
#define	SYSCFGRSTR_NO		(0 << (SYSCFGRSTR_SHIFT))
#define	SYSCFGRSTR_YES		(1 << (SYSCFGRSTR_SHIFT))

  __IO uint32_t		reserved0x44;		/* 0x44 */

  __IO uint32_t		reserved0x64;		/* 0x64 */
  __IO uint32_t		ahb1smenr;		/* 0x68 */
  __IO uint32_t		ahb2smenr;		/* 0x6c */
  __IO uint32_t		ahb3smenr;		/* 0x70 */
  __IO uint32_t		reserved0x74;		/* 0x74 */
  __IO uint32_t		apb1smenr1;		/* 0x78 */
  __IO uint32_t		apb1smenr2;		/* 0x7c */
  __IO uint32_t		apb2smenr;		/* 0x80 */
  __IO uint32_t		reserved0x84;		/* 0x84 */
  __IO uint32_t		ccipr;			/* 0x88 */
#define	SWPMI1SEL_SHIFT		30
#define	SWPMI1SEL_MASK		(1 << (SWPMI1SEL_SHIFT))
#define	SWPMI1SEL_PCLK1		(0 << (SWPMI1SEL_SHIFT))
#define	SWPMI1SEL_HSI16		(1 << (SWPMI1SEL_SHIFT))
#define	ADCSEL_SHIFT		28
#define	ADCSEL_MASK		(3 << (ADCSEL_SHIFT))
#define	ADCSEL_NONE		(0 << (ADCSEL_SHIFT))
#define	ADCSEL_PLLSAI1R		(1 << (ADCSEL_SHIFT))
#define	ADCSEL_RESERVED2	(2 << (ADCSEL_SHIFT))
#define	ADCSEL_SYSCLK		(3 << (ADCSEL_SHIFT))
#define	CLK48SEL_SHIFT		26
#define	CLK48SEL_MASK		(3 << (CLK48SEL_SHIFT))
#define	CLK48SEL_HSI48		(0 << (CLK48SEL_SHIFT))
#define	CLK48SEL_PLLSAIQ	(1 << (CLK48SEL_SHIFT))
#define	CLK48SEL_PLLQ		(2 << (CLK48SEL_SHIFT))
#define	CLK48SEL_MSI		(3 << (CLK48SEL_SHIFT))
#define	SAI1SEL_SHIFT		22
#define	SAI1SEL_MASK		(3 << (SAI1SEL_SHIFT))
#define	SAI1SEL_PLLSAI1P	(0 << (SAI1SEL_SHIFT))
#define	SAI1SEL_RESERVED1	(1 << (SAI1SEL_SHIFT))
#define	SAI1SEL_PLLP		(2 << (SAI1SEL_SHIFT))
#define	SAI1SEL_SAI1_EXTCLK	(3 << (SAI1SEL_SHIFT))
#define	LPTIM2SEL_SHIFT		20
#define	LPTIM2SEL_MASK		(3 << (LPTIM2SEL_SHIFT))
#define	LPTIM2SEL_PCLK		(0 << (LPTIM2SEL_SHIFT))
#define	LPTIM2SEL_LSI		(1 << (LPTIM2SEL_SHIFT))
#define	LPTIM2SEL_HSI16		(2 << (LPTIM2SEL_SHIFT))
#define	LPTIM2SEL_LSE		(3 << (LPTIM2SEL_SHIFT))
#define	LPTIM1SEL_SHIFT		18
#define	LPTIM1SEL_MASK		(3 << (LPTIM1SEL_SHIFT))
#define	LPTIM1SEL_PCLK		(0 << (LPTIM1SEL_SHIFT))
#define	LPTIM1SEL_LSI		(1 << (LPTIM1SEL_SHIFT))
#define	LPTIM1SEL_HSI16		(2 << (LPTIM1SEL_SHIFT))
#define	LPTIM1SEL_LSE		(3 << (LPTIM1SEL_SHIFT))
#define	I2C3SEL_SHIFT		16
#define	I2C3SEL_MASK		(3 << (I2C3SEL_SHIFT))
#define	I2C3SEL_PCLK		(0 << (I2C3SEL_SHIFT))
#define	I2C3SEL_SYSCLK		(1 << (I2C3SEL_SHIFT))
#define	I2C3SEL_HSI16		(2 << (I2C3SEL_SHIFT))
#define	I2C3SEL_RESERVED3	(3 << (I2C3SEL_SHIFT))
#define	I2C2SEL_SHIFT		14
#define	I2C2SEL_MASK		(3 << (I2C2SEL_SHIFT))
#define	I2C2SEL_PCLK		(0 << (I2C2SEL_SHIFT))
#define	I2C2SEL_SYSCLK		(1 << (I2C2SEL_SHIFT))
#define	I2C2SEL_HSI16		(2 << (I2C2SEL_SHIFT))
#define	I2C2SEL_RESERVED3	(3 << (I2C2SEL_SHIFT))
#define	I2C1SEL_SHIFT		12
#define	I2C1SEL_MASK		(3 << (I2C1SEL_SHIFT))
#define	I2C1SEL_PCLK		(0 << (I2C1SEL_SHIFT))
#define	I2C1SEL_SYSCLK		(1 << (I2C1SEL_SHIFT))
#define	I2C1SEL_HSI16		(2 << (I2C1SEL_SHIFT))
#define	I2C1SEL_RESERVED3	(3 << (I2C1SEL_SHIFT))
#define	LPUART1SEL_SHIFT	10	
#define	LPUART1SEL_MASK		(3 << (LPUART1SEL_SHIFT))
#define	LPUART1SEL_PCLK		(0 << (LPUART1SEL_SHIFT))
#define	LPUART1SEL_SYSCLK	(1 << (LPUART1SEL_SHIFT))
#define	LPUART1SEL_HSI16	(2 << (LPUART1SEL_SHIFT))
#define	LPUART1SEL_LSE		(3 << (LPUART1SEL_SHIFT))
#define	UART4SEL_SHIFT		6
#define	UART4SEL_MASK		(3 << (UART4SEL_SHIFT))
#define	UART4SEL_PCLK		(0 << (UART4SEL_SHIFT))
#define	UART4SEL_SYSCLK		(1 << (UART4SEL_SHIFT))
#define	UART4SEL_HSI16		(2 << (UART4SEL_SHIFT))
#define	UART4SEL_LSE		(3 << (UART4SEL_SHIFT))
#define	USART3SEL_SHIFT		4
#define	USART3SEL_MASK		(3 << (USART3SEL_SHIFT))
#define	USART3SEL_PCLK		(0 << (USART3SEL_SHIFT))
#define	USART3SEL_SYSCLK	(1 << (USART3SEL_SHIFT))
#define	USART3SEL_HSI16		(2 << (USART3SEL_SHIFT))
#define	USART3SEL_LSE		(3 << (USART3SEL_SHIFT))
#define	USART2SEL_SHIFT		2
#define	USART2SEL_MASK		(3 << (USART2SEL_SHIFT))
#define	USART2SEL_PCLK		(0 << (USART2SEL_SHIFT))
#define	USART2SEL_SYSCLK	(1 << (USART2SEL_SHIFT))
#define	USART2SEL_HSI16		(2 << (USART2SEL_SHIFT))
#define	USART2SEL_LSE		(3 << (USART2SEL_SHIFT))
#define	USART1SEL_SHIFT		0
#define	USART1SEL_MASK		(3 << (USART1SEL_SHIFT))
#define	USART1SEL_PCLK		(0 << (USART1SEL_SHIFT))
#define	USART1SEL_SYSCLK	(1 << (USART1SEL_SHIFT))
#define	USART1SEL_HSI16		(2 << (USART1SEL_SHIFT))
#define	USART1SEL_LSE		(3 << (USART1SEL_SHIFT))
  
  __IO uint32_t		reserved0x8c;	/* 0x8c */

  __IO uint32_t		crrcr;		/* 0x98 */
#define	HSI48CAL_SHIFT		8
#define	HSI48CAL_MASK		(0xff << (HSI48CAL_SHIFT))
#define	HSI48RDY_SHIFT		1
#define	HSI48RDY_MASK		(1 << (HSI48RDY_SHIFT))
#define	HSI48RDY_NO		(0 << (HSI48RDY_SHIFT))
#define	HSI48RDY_YES		(1 << (HSI48RDY_SHIFT))
#define	HSI48ON_SHIFT		0
#define	HSI48ON_MASK		(1 << (HSI48ON_SHIFT))
#define	HSI48ON_NO		(0 << (HSI48ON_SHIFT))
#define	HSI48ON_YES		(1 << (HSI48ON_SHIFT))

  __IO uint32_t		ccipr2;		/* 0x9c */
#define	I2C4SEL_SHIFT		
#define	I2C4SEL_MASK		(3 << (I2C4SEL_SHIFT))
#define	I2C4SEL_PCLK		(0 << (I2C4SEL_SHIFT))
#define	I2C4SEL_SYSCLK		(1 << (I2C4SEL_SHIFT))
#define	I2C4SEL_HSI16		(2 << (I2C4SEL_SHIFT))
#define	I2C4SEL_RESERVED3	(3 << (I2C4SEL_SHIFT))

#if 0
  __IO uint32_t		icscr;		/* 0x04 */
#define	HSITRIM_SHIFT		24
#define	HSITRIM_MASK		(0x1f << (HSITRIM_SHIFT))
#define	HSICAL_SHIFT		16
#define	HSICAL_MASK		(0xff << (HSICAL_SHIFT))
#define	MSITRIM_SHIFT		24
#define	MSITRIM_MASK		(0x1f << (MSITRIM_SHIFT))
#define	MSICAL_SHIFT		16
#define	MSICAL_MASK		(0xff << (MSICAL_SHIFT))
#endif

};


#define	CONFIG_STM32F7_POWERSW_FUNCBASE	(0)

struct _stStm32f7_IWDG {
  __IO uint32_t		kr;
#define	KR_KEY_CLEAR		0xaaaa
#define	KR_KEY_SET		0x5555
#define	KR_KEY_START		0xcccc
  __IO uint32_t		pr;
#define	PR_SHIFT		0
#define	PR_DIV4			(0 << (PR_SHIFT))
#define	PR_DIV8			(1 << (PR_SHIFT))
#define	PR_DIV16		(2 << (PR_SHIFT))
#define	PR_DIV32		(3 << (PR_SHIFT))
#define	PR_DIV64		(4 << (PR_SHIFT))
#define	PR_DIV128		(5 << (PR_SHIFT))
#define	PR_DIV256		(6 << (PR_SHIFT))
#define	PR_MASK			(7 << (PR_SHIFT))
  __IO uint32_t		rlr;
#define	RLR_SHIFT		0
#define	RLR_MASK		(0xfff << (RLR_SHIFT))
  __IO uint32_t		sr;
#define	SR_PVU_SHIFT		0
#define	SR_PVU_MASK		(1 << (SR_PVU_SHIFT))
#define	SR_RVU_SHIFT		1
#define	SR_RVU_MASK		(1 << (SR_RVU_SHIFT))
#define	SR_WVU_SHIFT		2
#define	SR_WVU_MASK		(1 << (SR_WVU_SHIFT))
  __IO uint32_t		winr;
#define	WINR_SHIFT		0
#define	WINR_MASK		(0xfff << (WINR_SHIFT))
};

#if 0
#define	IWDG_PTR	((struct _stStm32f7_IWDG *) ((APB1_BASE) + 0x3000))
#define	IWDG	(*IWDG_PTR)
#endif


struct _stStm32_USART {
  __IO uint32_t	cr1;
#define	CR1_M1_SHIFT		28
#define	CR1_M1_MASK		(1 << (CR1_M1_SHIFT))
#define	CR1_M1_NO		(0 << (CR1_M1_SHIFT))
#define	CR1_M1_YES		(1 << (CR1_M1_SHIFT))
#define	CR1_EOBIE_SHIFT		27
#define	CR1_EOBIE_MASK		(1 << (CR1_EOBIE_SHIFT))
#define	CR1_EOBIE_NO		(0 << (CR1_EOBIE_SHIFT))
#define	CR1_EOBIE_YES		(1 << (CR1_EOBIE_SHIFT))
#define	CR1_RTOIE_SHIFT		26
#define	CR1_RTOIE_MASK		(1 << (CR1_RTOIE_SHIFT))
#define	CR1_RTOIE_NO		(0 << (CR1_RTOIE_SHIFT))
#define	CR1_RTOIE_YES		(1 << (CR1_RTOIE_SHIFT))
#define	CR1_DEAT_SHIFT		21
#define	CR1_DEAT_MASK		(0x1f << (CR1_DEDT_SHIFT))
#define	CR1_DEDT_SHIFT		16
#define	CR1_DEDT_MASK		(0x1f << (CR1_DEDT_SHIFT))
#define	CR1_OVER8_SHIFT		15
#define	CR1_OVER8_MASK		(1 << (CR1_OVER8_SHIFT))
#define	CR1_OVER8_NO		(0 << (CR1_OVER8_SHIFT))
#define	CR1_OVER8_YES		(1 << (CR1_OVER8_SHIFT))
#define	CR1_CMIE_SHIFT		14
#define	CR1_CMIE_MASK		(1 << (CR1_CMIE_SHIFT))
#define	CR1_CMIE_NO		(0 << (CR1_CMIE_SHIFT))
#define	CR1_CMIE_YES		(1 << (CR1_CMIE_SHIFT))
#define	CR1_MMIE_SHIFT		13
#define	CR1_MMIE_MASK		(1 << (CR1_MMIE_SHIFT))
#define	CR1_MMIE_NO		(0 << (CR1_MMIE_SHIFT))
#define	CR1_MMIE_YES		(1 << (CR1_MMIE_SHIFT))
#define	CR1_M0_SHIFT		12
#define	CR1_M0_MASK		(1 << (CR1_M0_SHIFT))
#define	CR1_M0_NO		(0 << (CR1_M0_SHIFT))
#define	CR1_M0_YES		(1 << (CR1_M0_SHIFT))
#define	CR1_WAKE_SHIFT		11
#define	CR1_WAKE_MASK		(1 << (CR1_WAKE_SHIFT))
#define	CR1_WAKE_NO		(0 << (CR1_WAKE_SHIFT))
#define	CR1_WAKE_YES		(1 << (CR1_WAKE_SHIFT))
#define	CR1_PCE_SHIFT		10
#define	CR1_PCE_MASK		(1 << (CR1_PCE_SHIFT))
#define	CR1_PCE_NO		(0 << (CR1_PCE_SHIFT))
#define	CR1_PCE_YES		(1 << (CR1_PCE_SHIFT))
#define	CR1_PS_SHIFT		9
#define	CR1_PS_MASK		(1 << (CR1_PS_SHIFT))
#define	CR1_PS_EVEN		(0 << (CR1_PS_SHIFT))
#define	CR1_PS_ODD		(1 << (CR1_PS_SHIFT))
#define	CR1_PEIE_SHIFT		8
#define	CR1_PEIE_MASK		(1 << (CR1_PEIE_SHIFT))
#define	CR1_PEIE_NO		(0 << (CR1_PEIE_SHIFT))
#define	CR1_PEIE_YES		(1 << (CR1_PEIE_SHIFT))
#define	CR1_TXEIE_SHIFT		7
#define	CR1_TXEIE_MASK		(1 << (CR1_TXEIE_SHIFT))
#define	CR1_TXEIE_NO		(0 << (CR1_TXEIE_SHIFT))
#define	CR1_TXEIE_YES		(1 << (CR1_TXEIE_SHIFT))
#define	CR1_TCIE_SHIFT		6
#define	CR1_TCIE_MASK		(1 << (CR1_TCIE_SHIFT))
#define	CR1_TCIE_NO		(0 << (CR1_TCIE_SHIFT))
#define	CR1_TCIE_YES		(1 << (CR1_TCIE_SHIFT))
#define	CR1_RXNEIE_SHIFT	5
#define	CR1_RXNEIE_MASK		(1 << (CR1_RXNEIE_SHIFT))
#define	CR1_RXNEIE_NO		(0 << (CR1_RXNEIE_SHIFT))
#define	CR1_RXNEIE_YES		(1 << (CR1_RXNEIE_SHIFT))
#define	CR1_IDLEIE_SHIFT	4
#define	CR1_IDLEIE_MASK		(1 << (CR1_IDLEIE_SHIFT))
#define	CR1_IDLEIE_NO		(0 << (CR1_IDLEIE_SHIFT))
#define	CR1_IDLEIE_YES		(1 << (CR1_IDLEIE_SHIFT))
#define	CR1_TE_SHIFT		3
#define	CR1_TE_MASK		(1 << (CR1_TE_SHIFT))
#define	CR1_TE_NO		(0 << (CR1_TE_SHIFT))
#define	CR1_TE_YES		(1 << (CR1_TE_SHIFT))
#define	CR1_RE_SHIFT		2
#define	CR1_RE_MASK		(1 << (CR1_RE_SHIFT))
#define	CR1_RE_NO		(0 << (CR1_RE_SHIFT))
#define	CR1_RE_YES		(1 << (CR1_RE_SHIFT))
#define	CR1_UESM_SHIFT		1
#define	CR1_UESM_MASK		(1 << (CR1_UESM_SHIFT))
#define	CR1_UESM_NO		(0 << (CR1_UESM_SHIFT))
#define	CR1_UESM_YES		(1 << (CR1_UESM_SHIFT))
#define	CR1_UE_SHIFT		0
#define	CR1_UE_MASK		(1 << (CR1_UE_SHIFT))
#define	CR1_UE_NO		(0 << (CR1_UE_SHIFT))
#define	CR1_UE_YES		(1 << (CR1_UE_SHIFT))

  __IO uint32_t	cr2;
#define	CR2_ABRMOD_SHIFT		21
#define	CR2_ABRMOD_MASK		(3 << (CR2_ABRMOD_SHIFT))
#define	CR2_ABRMOD_MODE0_START	(0 << (CR2_ABRMOD_SHIFT))
#define	CR2_ABRMOD_MODE1_FALL	(1 << (CR2_ABRMOD_SHIFT))
#define	CR2_ABRMOD_MODE2_7F	(2 << (CR2_ABRMOD_SHIFT))
#define	CR2_ABRMOD_MODE3_55	(3 << (CR2_ABRMOD_SHIFT))
#define	CR2_ABREN_SHIFT		20
#define	CR2_ABREN_MASK		(1 << (CR2_ABREN_SHIFT))
#define	CR2_ABREN_NO		(0 << (CR2_ABREN_SHIFT))
#define	CR2_ABREN_YES		(1 << (CR2_ABREN_SHIFT))

  __IO uint32_t	cr3;
#define	CR3_DMAT_SHIFT		7
#define	CR3_DMAT_MASK		(1 << (CR3_DMAT_SHIFT))
#define	CR3_DMAT_NO		(0 << (CR3_DMAT_SHIFT))
#define	CR3_DMAT_YES		(1 << (CR3_DMAT_SHIFT))
#define	CR3_DMAR_SHIFT		6
#define	CR3_DMAR_MASK		(1 << (CR3_DMAR_SHIFT))
#define	CR3_DMAR_NO		(0 << (CR3_DMAR_SHIFT))
#define	CR3_DMAR_YES		(1 << (CR3_DMAR_SHIFT))

  __IO uint32_t	brr;
  __IO uint32_t	gtpr;
  __IO uint32_t	rtor;
  __IO uint32_t	rqr;
  __IO uint32_t	isr;
#define	ISR_TCBGT_SHIFT		25
#define	ISR_TCBGT_MASK		(1 << (ISR_TCBGT_SHIFT))
#define	ISR_REACK_SHIFT		22
#define	ISR_REACK_MASK		(1 << (ISR_REACK_SHIFT))
#define	ISR_TEACK_SHIFT		21
#define	ISR_TEACK_MASK		(1 << (ISR_TEACK_SHIFT))
#define	ISR_WUF_SHIFT		20
#define	ISR_WUF_MASK		(1 << (ISR_WUF_SHIFT))
#define	ISR_RWU_SHIFT		19
#define	ISR_RWU_MASK		(1 << (ISR_RWU_SHIFT))
#define	ISR_SBKF_SHIFT		18
#define	ISR_SBKF_MASK		(1 << (ISR_SBKF_SHIFT))
#define	ISR_CMF_SHIFT		17
#define	ISR_CMF_MASK		(1 << (ISR_CMF_SHIFT))
#define	ISR_BUSY_SHIFT		16
#define	ISR_BUSY_MASK		(1 << (ISR_BUSY_SHIFT))
#define	ISR_ABRF_SHIFT		15
#define	ISR_ABRF_MASK		(1 << (ISR_ABRF_SHIFT))
#define	ISR_ABRE_SHIFT		14
#define	ISR_ABRE_MASK		(1 << (ISR_ABRE_SHIFT))
#define	ISR_EOBF_SHIFT		12
#define	ISR_EOBF_MASK		(1 << (ISR_EOBF_SHIFT))
#define	ISR_RTOF_SHIFT		11
#define	ISR_RTOF_MASK		(1 << (ISR_RTOF_SHIFT))
#define	ISR_CTS_SHIFT		10
#define	ISR_CTS_MASK		(1 << (ISR_CTS_SHIFT))
#define	ISR_CTSIF_SHIFT		9
#define	ISR_CTSIF_MASK		(1 << (ISR_CTSIF_SHIFT))
#define	ISR_LBDF_SHIFT		8
#define	ISR_LBDF_MASK		(1 << (ISR_LBDF_SHIFT))
#define	ISR_TXE_SHIFT		7
#define	ISR_TXE_MASK		(1 << (ISR_TXE_SHIFT))
#define	ISR_TC_SHIFT		6
#define	ISR_TC_MASK		(1 << (ISR_TC_SHIFT))
#define	ISR_RXNE_SHIFT		5
#define	ISR_RXNE_MASK		(1 << (ISR_RXNE_SHIFT))
#define	ISR_IDLE_SHIFT		4
#define	ISR_IDLE_MASK		(1 << (ISR_IDLE_SHIFT))
#define	ISR_ORE_SHIFT		3
#define	ISR_ORE_MASK		(1 << (ISR_ORE_SHIFT))
#define	ISR_NF_SHIFT		2
#define	ISR_NF_MASK		(1 << (ISR_NF_SHIFT))
#define	ISR_FE_SHIFT		1
#define	ISR_FE_MASK		(1 << (ISR_FE_SHIFT))
#define	ISR_PE_SHIFT		0
#define	ISR_PE_MASK		(1 << (ISR_PE_SHIFT))
  __IO uint32_t	icr;
  __IO uint32_t	rdr;
  __IO uint32_t	tdr;
  __IO uint32_t	reserved[245];
};


struct _stStm32f7_DMACH {
  __IO uint32_t	ccr;
#define	CCR_MEM2MEM_SHIFT	14
#define	CCR_MEM2MEM_MASK	(1 << (CCR_MEM2MEM_SHIFT))
#define	CCR_MEM2MEM_NO		(0 << (CCR_MEM2MEM_SHIFT))
#define	CCR_MEM2MEM_YES		(1 << (CCR_MEM2MEM_SHIFT))
#define	CCR_PL_SHIFT		12
#define	CCR_PL_MASK		(1 << (CCR_PL_SHIFT))
#define	CCR_PL_LOW		(0 << (CCR_PL_SHIFT))
#define	CCR_PL_MID		(1 << (CCR_PL_SHIFT))
#define	CCR_PL_HID		(2 << (CCR_PL_SHIFT))
#define	CCR_PL_VERYHIGH		(3 << (CCR_PL_SHIFT))
#define	CCR_MSIZE_SHIFT		10
#define	CCR_MSIZE_MASK		(1 << (CCR_MSIZE_SHIFT))
#define	CCR_MSIZE_8BIT		(0 << (CCR_MSIZE_SHIFT))
#define	CCR_MSIZE_16BIT		(1 << (CCR_MSIZE_SHIFT))
#define	CCR_MSIZE_32BIT		(2 << (CCR_MSIZE_SHIFT))
#define	CCR_MSIZE_RESERVED	(3 << (CCR_MSIZE_SHIFT))
#define	CCR_PSIZE_SHIFT		8
#define	CCR_PSIZE_MASK		(1 << (CCR_PSIZE_SHIFT))
#define	CCR_PSIZE_8BIT		(0 << (CCR_PSIZE_SHIFT))
#define	CCR_PSIZE_16BIT		(1 << (CCR_PSIZE_SHIFT))
#define	CCR_PSIZE_32BIT		(2 << (CCR_PSIZE_SHIFT))
#define	CCR_PSIZE_RESERVED	(3 << (CCR_PL_SHIFT))
#define	CCR_MINC_SHIFT		7
#define	CCR_MINC_MASK		(1 << (CCR_MINC_SHIFT))
#define	CCR_MINC_NO		(0 << (CCR_MINC_SHIFT))
#define	CCR_MINC_YES		(1 << (CCR_MINC_SHIFT))
#define	CCR_PINC_SHIFT		6
#define	CCR_PINC_MASK		(1 << (CCR_PINC_SHIFT))
#define	CCR_PINC_NO		(0 << (CCR_PINC_SHIFT))
#define	CCR_PINC_YES		(1 << (CCR_PINC_SHIFT))
#define	CCR_CIRC_SHIFT		5
#define	CCR_CIRC_MASK		(1 << (CCR_CIRC_SHIFT))
#define	CCR_CIRC_NO		(0 << (CCR_CIRC_SHIFT))
#define	CCR_CIRC_YES		(1 << (CCR_CIRC_SHIFT))
#define	CCR_DIR_SHIFT		4
#define	CCR_DIR_MASK		(1 << (CCR_DIR_SHIFT))
#define	CCR_DIR_READ_PERI	(0 << (CCR_DIR_SHIFT))
#define	CCR_DIR_READ_MEM	(1 << (CCR_DIR_SHIFT))
#define	CCR_TEIE_SHIFT		3
#define	CCR_TEIE_MASK		(1 << (CCR_TEIE_SHIFT))
#define	CCR_TEIE_NO		(0 << (CCR_TEIE_SHIFT))
#define	CCR_TEIE_YES		(1 << (CCR_TEIE_SHIFT))
#define	CCR_HTIE_SHIFT		2
#define	CCR_HTIE_MASK		(1 << (CCR_HTIE_SHIFT))
#define	CCR_HTIE_NO		(0 << (CCR_HTIE_SHIFT))
#define	CCR_HTIE_YES		(1 << (CCR_HTIE_SHIFT))
#define	CCR_TCIE_SHIFT		1
#define	CCR_TCIE_MASK		(1 << (CCR_TCIE_SHIFT))
#define	CCR_TCIE_NO		(0 << (CCR_TCIE_SHIFT))
#define	CCR_TCIE_YES		(1 << (CCR_TCIE_SHIFT))
#define	CCR_EN_SHIFT		0
#define	CCR_EN_MASK		(1 << (CCR_EN_SHIFT))
#define	CCR_EN_NO		(0 << (CCR_EN_SHIFT))
#define	CCR_EN_YES		(1 << (CCR_EN_SHIFT))

  __IO uint32_t	cndtr;
#define	CNDTR_MASK		(0xffff)
  __IO uint32_t	cpar;
  __IO uint32_t	cmar;
  __IO uint32_t	reserved;
};
struct _stStm32f7_DMA {
  __IO uint32_t	isr;
#define	ISR_TEIF_MASK(d)	(1 << (((d)<<2) + 3))
#define	ISR_HTIF_MASK(d)	(1 << (((d)<<2) + 2))
#define	ISR_TCIF_MASK(d)	(1 << (((d)<<2) + 1))
#define	ISR_GIF_MASK(d)		(1 << (((d)<<2) + 0))
  
  __IO uint32_t	ifcr;
#define	ISR_CTEIF_CLEAR(d)	(1 << (((d)<<2) + 3))
#define	ISR_CHTIF_CLEAR(d)	(1 << (((d)<<2) + 2))
#define	ISR_CTCIF_CLEAR(d)	(1 << (((d)<<2) + 1))
#define	ISR_CGIF_CLEAR(d)	(1 << (((d)<<2) + 0))

  struct _stStm32f7_DMACH ch[7];
  __IO uint32_t	reserved90[5];
  __IO uint32_t	cselr;
#define	CSELR_DMA_MASK(d)	(0xfUL << ((d)*4))
#define	CSELR_DMA_SEL(d, x)	((x)   << ((d)*4))
  __IO uint32_t	reservedAC[213];
};
#define	DMA_PTR	((struct _stStm32f7_DMA *)  ((AHB1_BASE) + 0x0000))
/*#define	DMA	(DMA_PTR[2])*/

#define	DMA_MODULE1	(0)
#define	DMA_MODULE2	(1)

#define	DMA_CH1		(0)
#define	DMA_CH2		(1)
#define	DMA_CH3		(2)
#define	DMA_CH4		(3)
#define	DMA_CH5		(4)
#define	DMA_CH6		(5)
#define	DMA_CH7		(6)



struct _stStm32f7_FLASH {
  __IO uint32_t		acr;
#define	FLASH_LATENCY_SHIFT	(0)
#define	FLASH_LATENCY_NONE	(0 << (FLASH_LATENCY_SHIFT))
#define	FLASH_LATENCY_1CLK	(1 << (FLASH_LATENCY_SHIFT))
#define	FLASH_LATENCY_2CLK	(2 << (FLASH_LATENCY_SHIFT))
#define	FLASH_LATENCY_3CLK	(3 << (FLASH_LATENCY_SHIFT))
#define	FLASH_LATENCY_4CLK	(4 << (FLASH_LATENCY_SHIFT))

  __IO uint32_t		pdkeyr;
#define	PDKEYR_KEY1	0x04152637
#define	PDKEYR_KEY2	0xfafbfcfd
  __IO uint32_t		keyr;
#define	KEYR_KEY1	0x45670123
#define	KEYR_KEY2	0xcdef89ab
  __IO uint32_t		optkeyr;
#define	OPTKEYR_KEY1	0x08192a3b
#define	OPTKEYR_KEY2	0x4c5d6e7f

  __IO uint32_t		sr;
#define	SR_PEMPTY_SHIFT		17
#define	SR_PEMPTY_MASK		(1 << (SR_PEMPTY_SHIFT))
#define	SR_PEMPTY_NO		(0 << (SR_PEMPTY_SHIFT))
#define	SR_PEMPTY_YES		(1 << (SR_PEMPTY_SHIFT))
#define	SR_BSY_SHIFT		16
#define	SR_BSY_MASK		(1 << (SR_BSY_SHIFT))
#define	SR_BSY_NO		(0 << (SR_BSY_SHIFT))
#define	SR_BSY_YES		(1 << (SR_BSY_SHIFT))
#define	SR_OPTVERR_SHIFT	15	
#define	SR_OPTVERR_MASK		(1 << (SR_OPTVERR_SHIFT))
#define	SR_OPTVERR_NO		(0 << (SR_OPTVERR_SHIFT))
#define	SR_OPTVERR_YES		(1 << (SR_OPTVERR_SHIFT))
#define	SR_RDERR_SHIFT		14
#define	SR_RDERR_MASK		(1 << (SR_RDERR_SHIFT))
#define	SR_RDERR_NO		(0 << (SR_RDERR_SHIFT))
#define	SR_RDERR_YES		(1 << (SR_RDERR_SHIFT))
#define	SR_FASTERR_SHIFT	9	
#define	SR_FASTERR_MASK		(1 << (SR_FASTERR_SHIFT))
#define	SR_FASTERR_NO		(0 << (SR_FASTERR_SHIFT))
#define	SR_FASTERR_YES		(1 << (SR_FASTERR_SHIFT))
#define	SR_MISERR_SHIFT		8
#define	SR_MISERR_MASK		(1 << (SR_MISERR_SHIFT))
#define	SR_MISERR_NO		(0 << (SR_MISERR_SHIFT))
#define	SR_MISERR_YES		(1 << (SR_MISERR_SHIFT))
#define	SR_PGSERR_SHIFT		7
#define	SR_PGSERR_MASK		(1 << (SR_PGSERR_SHIFT))
#define	SR_PGSERR_NO		(0 << (SR_PGSERR_SHIFT))
#define	SR_PGSERR_YES		(1 << (SR_PGSERR_SHIFT))
#define	SR_SIZERR_SHIFT		6
#define	SR_SIZERR_MASK		(1 << (SR_SIZERR_SHIFT))
#define	SR_SIZERR_NO		(0 << (SR_SIZERR_SHIFT))
#define	SR_SIZERR_YES		(1 << (SR_SIZERR_SHIFT))
#define	SR_PGAERR_SHIFT		5
#define	SR_PGAERR_MASK		(1 << (SR_PGAERR_SHIFT))
#define	SR_PGAERR_NO		(0 << (SR_PGAERR_SHIFT))
#define	SR_PGAERR_YES		(1 << (SR_PGAERR_SHIFT))
#define	SR_WRPERR_SHIFT		4
#define	SR_WRPERR_MASK		(1 << (SR_WRPERR_SHIFT))
#define	SR_WRPERR_NO		(0 << (SR_WRPERR_SHIFT))
#define	SR_WRPERR_YES		(1 << (SR_WRPERR_SHIFT))
#define	SR_PROGERR_SHIFT	3
#define	SR_PROGERR_MASK		(1 << (SR_PROGERR_SHIFT))
#define	SR_PROGERR_NO		(0 << (SR_PROGERR_SHIFT))
#define	SR_PROGERR_YES		(1 << (SR_PROGERR_SHIFT))
#define	SR_OPERR_SHIFT		1
#define	SR_OPERR_MASK		(1 << (SR_OPERR_SHIFT))
#define	SR_OPERR_NO		(0 << (SR_OPERR_SHIFT))
#define	SR_OPERR_YES		(1 << (SR_OPERR_SHIFT))
#define	SR_EOP_SHIFT		0
#define	SR_EOP_MASK		(1 << (SR_EOP_SHIFT))
#define	SR_EOP_NO		(0 << (SR_EOP_SHIFT))
#define	SR_EOP_YES		(1 << (SR_EOP_SHIFT))

  __IO uint32_t		cr;
#define	CR_LOCK_SHIFT		31
#define	CR_LOCK_MASK		(1UL << (CR_LOCK_SHIFT))
#define	CR_LOCK_NO		(0UL << (CR_LOCK_SHIFT))
#define	CR_LOCK_YES		(1UL << (CR_LOCK_SHIFT))
#define	CR_OPTLOCK_SHIFT	30	
#define	CR_OPTLOCK_MASK		(1 << (CR_OPTLOCK_SHIFT))
#define	CR_OPTLOCK_NO		(0 << (CR_OPTLOCK_SHIFT))
#define	CR_OPTLOCK_YES		(1 << (CR_OPTLOCK_SHIFT))
#define	CR_OBL_LAUNCH_SHIFT	27	
#define	CR_OBL_LAUNCH_MASK	(1 << (CR_OBL_LAUNCH_SHIFT))
#define	CR_OBL_LAUNCH_NO	(0 << (CR_OBL_LAUNCH_SHIFT))
#define	CR_OBL_LAUNCH_YES	(1 << (CR_OBL_LAUNCH_SHIFT))
#define	CR_RDERRIE_SHIFT	26	
#define	CR_RDERRIE_MASK		(1 << (CR_RDERRIE_SHIFT))
#define	CR_RDERRIE_NO		(0 << (CR_RDERRIE_SHIFT))
#define	CR_RDERRIE_YES		(1 << (CR_RDERRIE_SHIFT))
#define	CR_ERRIE_SHIFT		25
#define	CR_ERRIE_MASK		(1 << (CR_ERRIE_SHIFT))
#define	CR_ERRIE_NO		(0 << (CR_ERRIE_SHIFT))
#define	CR_ERRIE_YES		(1 << (CR_ERRIE_SHIFT))
#define	CR_EOPIE_SHIFT		24
#define	CR_EOPIE_MASK		(1 << (CR_EOPIE_SHIFT))
#define	CR_EOPIE_NO		(0 << (CR_EOPIE_SHIFT))
#define	CR_EOPIE_YES		(1 << (CR_EOPIE_SHIFT))
#define	CR_FSTPG_SHIFT		18
#define	CR_FSTPG_MASK		(1 << (CR_FSTPG_SHIFT))
#define	CR_FSTPG_NO		(0 << (CR_FSTPG_SHIFT))
#define	CR_FSTPG_YES		(1 << (CR_FSTPG_SHIFT))
#define	CR_OPTSTRT_SHIFT	17
#define	CR_OPTSTRT_MASK		(1 << (CR_OPTSTRT_SHIFT))
#define	CR_OPTSTRT_NO		(0 << (CR_OPTSTRT_SHIFT))
#define	CR_OPTSTRT_YES		(1 << (CR_OPTSTRT_SHIFT))
#define	CR_STRT_SHIFT		16
#define	CR_STRT_MASK		(1 << (CR_STRT_SHIFT))
#define	CR_STRT_NO		(0 << (CR_STRT_SHIFT))
#define	CR_STRT_YES		(1 << (CR_STRT_SHIFT))
#define	CR_PNB_SHIFT		3
#define	CR_PNB_MASK		(0xff << (CR_PNB_SHIFT))
#define	CR_PNB_SECT(x)		((x) << (CR_PNB_SHIFT))
#define	CR_MER1_SHIFT		2
#define	CR_MER1_MASK		(1 << (CR_MER1_SHIFT))
#define	CR_MER1_NO		(0 << (CR_MER1_SHIFT))
#define	CR_MER1_YES		(1 << (CR_MER1_SHIFT))
#define	CR_PER_SHIFT		1
#define	CR_PER_MASK		(1 << (CR_PER_SHIFT))
#define	CR_PER_NO		(0 << (CR_PER_SHIFT))
#define	CR_PER_YES		(1 << (CR_PER_SHIFT))
#define	CR_PG_SHIFT		0
#define	CR_PG_MASK		(1 << (CR_PG_SHIFT))
#define	CR_PG_NO		(0 << (CR_PG_SHIFT))
#define	CR_PG_YES		(1 << (CR_PG_SHIFT))

  __IO uint32_t		eccr;
  __IO uint32_t		optr;
  __IO uint32_t		pcrop1st;
  __IO uint32_t		pcrop1er;
  __IO uint32_t		wrp1ar;
  __IO uint32_t		wrp1br;
};


struct _stStm32f7_TIM {
  __IO uint32_t	cr1;		/* 0x00 1, 2, 3, 15 */
#define	CR1_UIFREMAP_SHIFT	11
#define	CR1_UIFREMAP_MASK	(1 << (CR1_UIFREMAP_SHIFT))
#define	CR1_UIFREMAP_NO		(0 << (CR1_UIFREMAP_SHIFT))
#define	CR1_UIFREMAP_YES	(1 << (CR1_UIFREMAP_SHIFT))
#define	CR1_CKD_SHIFT		8
#define	CR1_CKD_MASK		(3 << (CR1_CKD_SHIFT))
#define	CR1_CKD_DIV1		(0 << (CR1_CKD_SHIFT))
#define	CR1_CKD_DIV2		(1 << (CR1_CKD_SHIFT))
#define	CR1_CKD_DIV4		(2 << (CR1_CKD_SHIFT))
#define	CR1_CKD_RESERVED	(3 << (CR1_CKD_SHIFT))
#define	CR1_ARPE_SHIFT		7
#define	CR1_ARPE_MASK		(1 << (CR1_ARPE_SHIFT))
#define	CR1_ARPE_NO		(0 << (CR1_ARPE_SHIFT))
#define	CR1_ARPE_YES		(1 << (CR1_ARPE_SHIFT))
#define	CR1_CMS_SHIFT		5
#define	CR1_CMS_MASK		(3 << (CR1_CMS_SHIFT))
#define	CR1_CMS_EDGEALIGN	(0 << (CR1_CMS_SHIFT))
#define	CR1_CMS_CENTERALIGN1	(1 << (CR1_CMS_SHIFT))
#define	CR1_CMS_CENTERALIGN2		(2 << (CR1_CMS_SHIFT))
#define	CR1_CMS_CENTERALIGN3	(3 << (CR1_CMS_SHIFT))
#define	CR1_DIR_SHIFT		4
#define	CR1_DIR_MASK		(1 << (CR1_DIR_SHIFT))
#define	CR1_DIR_UP		(0 << (CR1_DIR_SHIFT))
#define	CR1_DIR_DOWN		(1 << (CR1_DIR_SHIFT))
#define	CR1_OPM_SHIFT		3
#define	CR1_OPM_MASK		(1 << (CR1_OPM_SHIFT))
#define	CR1_OPM_NO		(0 << (CR1_OPM_SHIFT))
#define	CR1_OPM_YES		(1 << (CR1_OPM_SHIFT))
#define	CR1_URS_SHIFT		2
#define	CR1_URS_MASK		(1 << (CR1_URS_SHIFT))
#define	CR1_URS_NO		(0 << (CR1_URS_SHIFT))
#define	CR1_URS_YES		(1 << (CR1_URS_SHIFT))
#define	CR1_UDIS_SHIFT		1
#define	CR1_UDIS_MASK		(1 << (CR1_UDIS_SHIFT))
#define	CR1_UDIS_NO		(0 << (CR1_UDIS_SHIFT))
#define	CR1_UDIS_YES		(1 << (CR1_UDIS_SHIFT))
#define	CR1_CEN_SHIFT		0
#define	CR1_CEN_MASK		(1 << (CR1_CEN_SHIFT))
#define	CR1_CEN_NO		(0 << (CR1_CEN_SHIFT))
#define	CR1_CEN_YES		(1 << (CR1_CEN_SHIFT))

  __IO uint32_t	cr2;		/* 0x04 1, 2, 3, 15 */
#define	CR2_TI1S_SHIFT		7
#define	CR2_TI1S_MASK		(1 << (CR2_TI1S_SHIFT))
#define	CR2_TI1S_CH1		(0 << (CR2_TI1S_SHIFT))
#define	CR2_TI1S_CH123XOR	(1 << (CR2_TI1S_SHIFT))
#define	CR2_MMS_SHIFT		4
#define	CR2_MMS_MASK		(7 << (CR2_MMS_SHIFT))
#define	CR2_MMS_RESET		(0 << (CR2_MMS_SHIFT))
#define	CR2_MMS_ENABLE		(1 << (CR2_MMS_SHIFT))
#define	CR2_MMS_UPDATE		(2 << (CR2_MMS_SHIFT))
#define	CR2_MMS_COMPARE_PULSE	(3 << (CR2_MMS_SHIFT))
#define	CR2_MMS_COMPARE_OC1REF	(4 << (CR2_MMS_SHIFT))
#define	CR2_MMS_COMPARE_OC2REF	(5 << (CR2_MMS_SHIFT))
#define	CR2_MMS_COMPARE_OC3REF	(6 << (CR2_MMS_SHIFT))
#define	CR2_MMS_COMPARE_OC4REF	(7 << (CR2_MMS_SHIFT))
#define	CR2_CCDS_SHIFT		3
#define	CR2_CCDS_MASK		(1 << (CR2_CCDS_SHIFT))
#define	CR2_CCDS_CC		(0 << (CR2_CCDS_SHIFT))
#define	CR2_CCDS_UPDATE		(1 << (CR2_CCDS_SHIFT))

  __IO uint32_t	smcr;		/* 0x08 1, 2, 3, 15 */
#define	SMCR_SMS3_SHIFT		16
#define	SMCR_SMS3_MASK		(1 << (SMCR_SMS3_SHIFT))
#define	SMCR_ETP_SHIFT		15
#define	SMCR_ETP_MASK		(1 << (SMCR_ETP_SHIFT))
#define	SMCR_ETP_RISING		(0 << (SMCR_ETP_SHIFT))
#define	SMCR_ETP_FALLING	(1 << (SMCR_ETP_SHIFT))
#define	SMCR_ECE_SHIFT		14
#define	SMCR_ECE_MASK		(1 << (SMCR_ECE_SHIFT))
#define	SMCR_ECE_MODE2DIS	(0 << (SMCR_ECE_SHIFT))
#define	SMCR_ECE_MODE2EN	(1 << (SMCR_ECE_SHIFT))
#define	SMCR_ETPS_SHIFT		12
#define	SMCR_ETPS_MASK		(3 << (SMCR_ETPS_SHIFT))
#define	SMCR_ETPS_DIV1		(0 << (SMCR_ETPS_SHIFT))
#define	SMCR_ETPS_DIV2		(1 << (SMCR_ETPS_SHIFT))
#define	SMCR_ETPS_DIV4		(2 << (SMCR_ETPS_SHIFT))
#define	SMCR_ETPS_DIV8		(3 << (SMCR_ETPS_SHIFT))
#define	SMCR_ETF_SHIFT		8
#define	SMCR_ETF_MASK		(0xf << (SMCR_ETF_SHIFT))
#define	SMCR_ETF_NO_FLT		(0 << (SMCR_ETF_SHIFT))
#define	SMCR_ETF_CKINT_N2	(1 << (SMCR_ETF_SHIFT))
#define	SMCR_ETF_CKINT_N4	(2 << (SMCR_ETF_SHIFT))
#define	SMCR_ETF_CKINT_N8	(3 << (SMCR_ETF_SHIFT))
#define	SMCR_ETF_DTS2_N6	(4 << (SMCR_ETF_SHIFT))
#define	SMCR_ETF_DTS2_N8	(5 << (SMCR_ETF_SHIFT))
#define	SMCR_ETF_DTS4_N6	(6 << (SMCR_ETF_SHIFT))
#define	SMCR_ETF_DTS4_N8	(7 << (SMCR_ETF_SHIFT))
#define	SMCR_ETF_DTS8_N6	(8 << (SMCR_ETF_SHIFT))
#define	SMCR_ETF_DTS8_N8	(9 << (SMCR_ETF_SHIFT))
#define	SMCR_ETF_DTS16_N5	(10 << (SMCR_ETF_SHIFT))
#define	SMCR_ETF_DTS16_N6	(11 << (SMCR_ETF_SHIFT))
#define	SMCR_ETF_DTS16_N8	(12 << (SMCR_ETF_SHIFT))
#define	SMCR_ETF_DTS32_N5	(13 << (SMCR_ETF_SHIFT))
#define	SMCR_ETF_DTS32_N6	(14 << (SMCR_ETF_SHIFT))
#define	SMCR_ETF_DTS32_N8	(15 << (SMCR_ETF_SHIFT))
#define	SMCR_MSM_SHIFT		7
#define	SMCR_MSM_MASK		(1 << (SMCR_MSM_SHIFT))
#define	SMCR_MSM_NONE		(0 << (SMCR_MSM_SHIFT))
#define	SMCR_MSM_SYNC		(1 << (SMCR_MSM_SHIFT))
#define	SMCR_TS_SHIFT		4
#define	SMCR_TS_MASK		(0x7 << (SMCR_TS_SHIFT))
#define	SMCR_TS_ITR0		(0 << (SMCR_TS_SHIFT))
#define	SMCR_TS_ITR1		(1 << (SMCR_TS_SHIFT))
#define	SMCR_TS_RESERVED2	(2 << (SMCR_TS_SHIFT))
#define	SMCR_TS_RESERVED3	(3 << (SMCR_TS_SHIFT))
#define	SMCR_TS_TI1F_ED		(4 << (SMCR_TS_SHIFT))
#define	SMCR_TS_TI1FP1		(5 << (SMCR_TS_SHIFT))
#define	SMCR_TS_IT2FP2		(6 << (SMCR_TS_SHIFT))
#define	SMCR_TS_ETRF		(7 << (SMCR_TS_SHIFT))
#define	SMCR_OCCS_SHIFT		3
#define	SMCR_OCCS_MASK		(1 << (SMCR_OCCS_SHIFT))
#define	SMCR_OCCS_OCREF		(0 << (SMCR_OCCS_SHIFT))
#define	SMCR_OCCS_ETRF		(1 << (SMCR_OCCS_SHIFT))
#define	SMCR_SMS_SHIFT		0
#define	SMCR_SMS_MASK		(0x10007 << (SMCR_SMS_SHIFT))
#define	SMCR_SMS_INTCLK		(0 << (SMCR_SMS_SHIFT))
#define	SMCR_SMS_ENC1		(1 << (SMCR_SMS_SHIFT))
#define	SMCR_SMS_ENC2		(2 << (SMCR_SMS_SHIFT))
#define	SMCR_SMS_ENC3		(3 << (SMCR_SMS_SHIFT))
#define	SMCR_SMS_RESET		(4 << (SMCR_SMS_SHIFT))
#define	SMCR_SMS_GATED		(5 << (SMCR_SMS_SHIFT))
#define	SMCR_SMS_TRIGGER	(6 << (SMCR_SMS_SHIFT))
#define	SMCR_SMS_EXT1		(7 << (SMCR_SMS_SHIFT))
#define	SMCR_SMS_COMBINED	(0x10000 << (SMCR_SMS_SHIFT))

  __IO uint32_t	dier;		/* 0x0c 1, 2, 3, 15 */
#define	DIER_TDE_SHIFT		14
#define	DIER_TDE_MASK		(1 << (DIER_TDE_SHIFT))
#define	DIER_TDE_NO		(0 << (DIER_TDE_SHIFT))
#define	DIER_TDE_YES		(1 << (DIER_TDE_SHIFT))
#define	DIER_CC4DE_SHIFT	12	
#define	DIER_CC4DE_MASK		(1 << (DIER_CC4DE_SHIFT))
#define	DIER_CC4DE_NO		(0 << (DIER_CC4DE_SHIFT))
#define	DIER_CC4DE_YES		(1 << (DIER_CC4DE_SHIFT))
#define	DIER_CC3DE_SHIFT	11	
#define	DIER_CC3DE_MASK		(1 << (DIER_CC3DE_SHIFT))
#define	DIER_CC3DE_NO		(0 << (DIER_CC3DE_SHIFT))
#define	DIER_CC3DE_YES		(1 << (DIER_CC3DE_SHIFT))
#define	DIER_CC2DE_SHIFT	10	
#define	DIER_CC2DE_MASK		(1 << (DIER_CC2DE_SHIFT))
#define	DIER_CC2DE_NO		(0 << (DIER_CC2DE_SHIFT))
#define	DIER_CC2DE_YES		(1 << (DIER_CC2DE_SHIFT))
#define	DIER_CC1EN_SHIFT	9	
#define	DIER_CC1EN_MASK		(1 << (DIER_CC1EN_SHIFT))
#define	DIER_CC1EN_NO		(0 << (DIER_CC1EN_SHIFT))
#define	DIER_CC1EN_YES		(1 << (DIER_CC1EN_SHIFT))
#define	DIER_UDE_SHIFT		8
#define	DIER_UDE_MASK		(1 << (DIER_UDE_SHIFT))
#define	DIER_UDE_NO		(0 << (DIER_UDE_SHIFT))
#define	DIER_UDE_YES		(1 << (DIER_UDE_SHIFT))
#define	DIER_TIE_SHIFT		6
#define	DIER_TIE_MASK		(1 << (DIER_TIE_SHIFT))
#define	DIER_TIE_NO		(0 << (DIER_TIE_SHIFT))
#define	DIER_TIE_YES		(1 << (DIER_TIE_SHIFT))
#define	DIER_CC4IE_SHIFT	4	
#define	DIER_CC4IE_MASK		(1 << (DIER_CC4IE_SHIFT))
#define	DIER_CC4IE_NO		(0 << (DIER_CC4IE_SHIFT))
#define	DIER_CC4IE_YES		(1 << (DIER_CC4IE_SHIFT))
#define	DIER_CC3IE_SHIFT	3	
#define	DIER_CC3IE_MASK		(1 << (DIER_CC3IE_SHIFT))
#define	DIER_CC3IE_NO		(0 << (DIER_CC3IE_SHIFT))
#define	DIER_CC3IE_YES		(1 << (DIER_CC3IE_SHIFT))
#define	DIER_CC2IE_SHIFT	2	
#define	DIER_CC2IE_MASK		(1 << (DIER_CC2IE_SHIFT))
#define	DIER_CC2IE_NO		(0 << (DIER_CC2IE_SHIFT))
#define	DIER_CC2IE_YES		(1 << (DIER_CC2IE_SHIFT))
#define	DIER_CC1IE_SHIFT	1	
#define	DIER_CC1IE_MASK		(1 << (DIER_CC1IE_SHIFT))
#define	DIER_CC1IE_NO		(0 << (DIER_CC1IE_SHIFT))
#define	DIER_CC1IE_YES		(1 << (DIER_CC1IE_SHIFT))
#define	DIER_UIE_SHIFT		0
#define	DIER_UIE_MASK		(1 << (DIER_UIE_SHIFT))
#define	DIER_UIE_NO		(0 << (DIER_UIE_SHIFT))
#define	DIER_UIE_YES		(1 << (DIER_UIE_SHIFT))

  __IO uint32_t	sr;		/* 0x10 1, 2, 3, 15 */
#define	SR_CC4OF_SHIFT		12
#define	SR_CC4OF_MASK		(1 << (SR_CC4OF_SHIFT))
#define	SR_CC4OF_NO		(0 << (SR_CC4OF_SHIFT))
#define	SR_CC4OF_YES		(1 << (SR_CC4OF_SHIFT))
#define	SR_CC3OF_SHIFT		11
#define	SR_CC3OF_MASK		(1 << (SR_CC3OF_SHIFT))
#define	SR_CC3OF_NO		(0 << (SR_CC3OF_SHIFT))
#define	SR_CC3OF_YES		(1 << (SR_CC3OF_SHIFT))
#define	SR_CC2OF_SHIFT		10
#define	SR_CC2OF_MASK		(1 << (SR_CC2OF_SHIFT))
#define	SR_CC2OF_NO		(0 << (SR_CC2OF_SHIFT))
#define	SR_CC2OF_YES		(1 << (SR_CC2OF_SHIFT))
#define	SR_CC1OF_SHIFT		9
#define	SR_CC1OF_MASK		(1 << (SR_CC1OF_SHIFT))
#define	SR_CC1OF_NO		(0 << (SR_CC1OF_SHIFT))
#define	SR_CC1OF_YES		(1 << (SR_CC1OF_SHIFT))
#define	SR_TIF_SHIFT		6
#define	SR_TIF_MASK		(1 << (SR_TIF_SHIFT))
#define	SR_TIF_NO		(0 << (SR_TIF_SHIFT))
#define	SR_TIF_YES		(1 << (SR_TIF_SHIFT))
#define	SR_CC4IF_SHIFT		4
#define	SR_CC4IF_MASK		(1 << (SR_CC4IF_SHIFT))
#define	SR_CC4IF_NO		(0 << (SR_CC4IF_SHIFT))
#define	SR_CC4IF_YES		(1 << (SR_CC4IF_SHIFT))
#define	SR_CC3IF_SHIFT		3
#define	SR_CC3IF_MASK		(1 << (SR_CC3IF_SHIFT))
#define	SR_CC3IF_NO		(0 << (SR_CC3IF_SHIFT))
#define	SR_CC3IF_YES		(1 << (SR_CC3IF_SHIFT))
#define	SR_CC2IF_SHIFT		2
#define	SR_CC2IF_MASK		(1 << (SR_CC2IF_SHIFT))
#define	SR_CC2IF_NO		(0 << (SR_CC2IF_SHIFT))
#define	SR_CC2IF_YES		(1 << (SR_CC2IF_SHIFT))
#define	SR_CC1IF_SHIFT		1
#define	SR_CC1IF_MASK		(1 << (SR_CC1IF_SHIFT))
#define	SR_CC1IF_NO		(0 << (SR_CC1IF_SHIFT))
#define	SR_CC1IF_YES		(1 << (SR_CC1IF_SHIFT))
#define	SR_UIF_SHIFT		0
#define	SR_UIF_MASK		(1 << (SR_UIF_SHIFT))
#define	SR_UIF_NO		(0 << (SR_UIF_SHIFT))
#define	SR_UIF_YES		(1 << (SR_UIF_SHIFT))

  __IO uint32_t	egr;		/* 0x14 1, 2, 3, 15 */
#define	EGR_TG_SHIFT		6
#define	EGR_TG_MASK		(1 << (EGR_TG_SHIFT))
#define	EGR_TG_NO		(0 << (EGR_TG_SHIFT))
#define	EGR_TG_YES		(1 << (EGR_TG_SHIFT))
#define	EGR_CC4G_SHIFT		4
#define	EGR_CC4G_MASK		(1 << (EGR_CC4G_SHIFT))
#define	EGR_CC4G_NO		(0 << (EGR_CC4G_SHIFT))
#define	EGR_CC4G_YES		(1 << (EGR_CC4G_SHIFT))
#define	EGR_CC3G_SHIFT		3
#define	EGR_CC3G_MASK		(1 << (EGR_CC3G_SHIFT))
#define	EGR_CC3G_NO		(0 << (EGR_CC3G_SHIFT))
#define	EGR_CC3G_YES		(1 << (EGR_CC3G_SHIFT))
#define	EGR_CC2G_SHIFT		2
#define	EGR_CC2G_MASK		(1 << (EGR_CC2G_SHIFT))
#define	EGR_CC2G_NO		(0 << (EGR_CC2G_SHIFT))
#define	EGR_CC2G_YES		(1 << (EGR_CC2G_SHIFT))
#define	EGR_CC1G_SHIFT		1
#define	EGR_CC1G_MASK		(1 << (EGR_CC1G_SHIFT))
#define	EGR_CC1G_NO		(0 << (EGR_CC1G_SHIFT))
#define	EGR_CC1G_YES		(1 << (EGR_CC1G_SHIFT))
#define	EGR_UG_SHIFT		0
#define	EGR_UG_MASK		(1 << (EGR_UG_SHIFT))
#define	EGR_UG_NO		(0 << (EGR_UG_SHIFT))
#define	EGR_UG_YES		(1 << (EGR_UG_SHIFT))

  __IO uint32_t	ccmr1;		/* 0x18 1, 2, 3, 15 */
#define	CCMR1_IC2F_SHIFT	12
#define	CCMR1_IC2F_MASK		(0xf << (CCMR1_IC2F_SHIFT))
#define	CCMR1_IC2F_NO_FLT	(0 << (CCMR1_IC2F_SHIFT))
#define	CCMR1_IC2F_CKINT_N2	(1 << (CCMR1_IC2F_SHIFT))
#define	CCMR1_IC2F_CKINT_N4	(2 << (CCMR1_IC2F_SHIFT))
#define	CCMR1_IC2F_CKINT_N8	(3 << (CCMR1_IC2F_SHIFT))
#define	CCMR1_IC2F_DTS2_N6	(4 << (CCMR1_IC2F_SHIFT))
#define	CCMR1_IC2F_DTS2_N8	(5 << (CCMR1_IC2F_SHIFT))
#define	CCMR1_IC2F_DTS4_N6	(6 << (CCMR1_IC2F_SHIFT))
#define	CCMR1_IC2F_DTS4_N8	(7 << (CCMR1_IC2F_SHIFT))
#define	CCMR1_IC2F_DTS8_N6	(8 << (CCMR1_IC2F_SHIFT))
#define	CCMR1_IC2F_DTS8_N8	(9 << (CCMR1_IC2F_SHIFT))
#define	CCMR1_IC2F_DTS16_N5	(10 << (CCMR1_IC2F_SHIFT))
#define	CCMR1_IC2F_DTS16_N6	(11 << (CCMR1_IC2F_SHIFT))
#define	CCMR1_IC2F_DTS16_N8	(12 << (CCMR1_IC2F_SHIFT))
#define	CCMR1_IC2F_DTS32_N5	(13 << (CCMR1_IC2F_SHIFT))
#define	CCMR1_IC2F_DTS32_N6	(14 << (CCMR1_IC2F_SHIFT))
#define	CCMR1_IC2F_DTS32_N8	(15 << (CCMR1_IC2F_SHIFT))
#define	CCMR1_IC2PSC_SHIFT	10
#define	CCMR1_IC2PSC_MASK	(3 << (CCMR1_IC2PSC_SHIFT))
#define	CCMR1_IC2PSC_DIV1	(0 << (CCMR1_IC2PSC_SHIFT))
#define	CCMR1_IC2PSC_DIV2	(1 << (CCMR1_IC2PSC_SHIFT))
#define	CCMR1_IC2PSC_DIV4	(2 << (CCMR1_IC2PSC_SHIFT))
#define	CCMR1_IC2PSC_DIV8	(3 << (CCMR1_IC2PSC_SHIFT))
#define	CCMR1_CC2S_SHIFT	8
#define	CCMR1_CC2S_MASK		(3 << (CCMR1_CC2S_SHIFT))
#define	CCMR1_CC2S_OUTPUT	(0 << (CCMR1_CC2S_SHIFT))
#define	CCMR1_CC2S_TI2		(1 << (CCMR1_CC2S_SHIFT))
#define	CCMR1_CC2S_TI1		(2 << (CCMR1_CC2S_SHIFT))
#define	CCMR1_CC2S_TRC		(3 << (CCMR1_CC2S_SHIFT))
#define	CCMR1_IC1F_SHIFT	4
#define	CCMR1_IC1F_MASK		(0xf << (CCMR1_IC1F_SHIFT))
#define	CCMR1_IC1F_NO_FLT	(0 << (CCMR1_IC1F_SHIFT))
#define	CCMR1_IC1F_CKINT_N2	(1 << (CCMR1_IC1F_SHIFT))
#define	CCMR1_IC1F_CKINT_N4	(2 << (CCMR1_IC1F_SHIFT))
#define	CCMR1_IC1F_CKINT_N8	(3 << (CCMR1_IC1F_SHIFT))
#define	CCMR1_IC1F_DTS2_N6	(4 << (CCMR1_IC1F_SHIFT))
#define	CCMR1_IC1F_DTS2_N8	(5 << (CCMR1_IC1F_SHIFT))
#define	CCMR1_IC1F_DTS4_N6	(6 << (CCMR1_IC1F_SHIFT))
#define	CCMR1_IC1F_DTS4_N8	(7 << (CCMR1_IC1F_SHIFT))
#define	CCMR1_IC1F_DTS8_N6	(8 << (CCMR1_IC1F_SHIFT))
#define	CCMR1_IC1F_DTS8_N8	(9 << (CCMR1_IC1F_SHIFT))
#define	CCMR1_IC1F_DTS16_N5	(10 << (CCMR1_IC1F_SHIFT))
#define	CCMR1_IC1F_DTS16_N6	(11 << (CCMR1_IC1F_SHIFT))
#define	CCMR1_IC1F_DTS16_N8	(12 << (CCMR1_IC1F_SHIFT))
#define	CCMR1_IC1F_DTS32_N5	(13 << (CCMR1_IC1F_SHIFT))
#define	CCMR1_IC1F_DTS32_N6	(14 << (CCMR1_IC1F_SHIFT))
#define	CCMR1_IC1F_DTS32_N8	(15 << (CCMR1_IC1F_SHIFT))
#define	CCMR1_IC1PSC_SHIFT	2
#define	CCMR1_IC1PSC_MASK	(3 << (CCMR1_IC1PSC_SHIFT))
#define	CCMR1_IC1PSC_DIV1	(0 << (CCMR1_IC1PSC_SHIFT))
#define	CCMR1_IC1PSC_DIV2	(1 << (CCMR1_IC1PSC_SHIFT))
#define	CCMR1_IC1PSC_DIV4	(2 << (CCMR1_IC1PSC_SHIFT))
#define	CCMR1_IC1PSC_DIV8	(3 << (CCMR1_IC1PSC_SHIFT))
#define	CCMR1_CC1S_SHIFT	0
#define	CCMR1_CC1S_MASK		(3 << (CCMR1_CC1S_SHIFT))
#define	CCMR1_CC1S_OUTPUT	(0 << (CCMR1_CC1S_SHIFT))
#define	CCMR1_CC1S_TI1		(1 << (CCMR1_CC1S_SHIFT))
#define	CCMR1_CC1S_TI2		(2 << (CCMR1_CC1S_SHIFT))
#define	CCMR1_CC1S_TRC		(3 << (CCMR1_CC1S_SHIFT))
  
  __IO uint32_t	ccmr2;		/* 0x1c 1, 2, 3, 15 */
#define	CCMR2_IC4F_SHIFT	12
#define	CCMR2_IC4F_MASK		(0xf << (CCMR2_IC4F_SHIFT))
#define	CCMR2_IC4F_NO_FLT	(0 << (CCMR2_IC4F_SHIFT))
#define	CCMR2_IC4F_CKINT_N2	(1 << (CCMR2_IC4F_SHIFT))
#define	CCMR2_IC4F_CKINT_N4	(2 << (CCMR2_IC4F_SHIFT))
#define	CCMR2_IC4F_CKINT_N8	(3 << (CCMR2_IC4F_SHIFT))
#define	CCMR2_IC4F_DTS2_N6	(4 << (CCMR2_IC4F_SHIFT))
#define	CCMR2_IC4F_DTS2_N8	(5 << (CCMR2_IC4F_SHIFT))
#define	CCMR2_IC4F_DTS4_N6	(6 << (CCMR2_IC4F_SHIFT))
#define	CCMR2_IC4F_DTS4_N8	(7 << (CCMR2_IC4F_SHIFT))
#define	CCMR2_IC4F_DTS8_N6	(8 << (CCMR2_IC4F_SHIFT))
#define	CCMR2_IC4F_DTS8_N8	(9 << (CCMR2_IC4F_SHIFT))
#define	CCMR2_IC4F_DTS16_N5	(10 << (CCMR2_IC4F_SHIFT))
#define	CCMR2_IC4F_DTS16_N6	(11 << (CCMR2_IC4F_SHIFT))
#define	CCMR2_IC4F_DTS16_N8	(12 << (CCMR2_IC4F_SHIFT))
#define	CCMR2_IC4F_DTS32_N5	(13 << (CCMR2_IC4F_SHIFT))
#define	CCMR2_IC4F_DTS32_N6	(14 << (CCMR2_IC4F_SHIFT))
#define	CCMR2_IC4F_DTS32_N8	(15 << (CCMR2_IC4F_SHIFT))
#define	CCMR2_IC4PSC_SHIFT	10
#define	CCMR2_IC4PSC_MASK	(3 << (CCMR2_IC4PSC_SHIFT))
#define	CCMR2_IC4PSC_DIV1	(0 << (CCMR2_IC4PSC_SHIFT))
#define	CCMR2_IC4PSC_DIV2	(1 << (CCMR2_IC4PSC_SHIFT))
#define	CCMR2_IC4PSC_DIV4	(2 << (CCMR2_IC4PSC_SHIFT))
#define	CCMR2_IC4PSC_DIV8	(3 << (CCMR2_IC4PSC_SHIFT))
#define	CCMR2_CC4S_SHIFT	8
#define	CCMR2_CC4S_MASK		(3 << (CCMR2_CC4S_SHIFT))
#define	CCMR2_CC4S_OUTPUT	(0 << (CCMR2_CC4S_SHIFT))
#define	CCMR2_CC4S_TI4		(1 << (CCMR2_CC4S_SHIFT))
#define	CCMR2_CC4S_TI3		(2 << (CCMR2_CC4S_SHIFT))
#define	CCMR2_CC4S_TRC		(3 << (CCMR2_CC4S_SHIFT))
#define	CCMR2_IC3F_SHIFT	4
#define	CCMR2_IC3F_MASK		(0xf << (CCMR2_IC3F_SHIFT))
#define	CCMR2_IC3F_NO_FLT	(0 << (CCMR2_IC3F_SHIFT))
#define	CCMR2_IC3F_CKINT_N2	(1 << (CCMR2_IC3F_SHIFT))
#define	CCMR2_IC3F_CKINT_N4	(2 << (CCMR2_IC3F_SHIFT))
#define	CCMR2_IC3F_CKINT_N8	(3 << (CCMR2_IC3F_SHIFT))
#define	CCMR2_IC3F_DTS2_N6	(4 << (CCMR2_IC3F_SHIFT))
#define	CCMR2_IC3F_DTS2_N8	(5 << (CCMR2_IC3F_SHIFT))
#define	CCMR2_IC3F_DTS4_N6	(6 << (CCMR2_IC3F_SHIFT))
#define	CCMR2_IC3F_DTS4_N8	(7 << (CCMR2_IC3F_SHIFT))
#define	CCMR2_IC3F_DTS8_N6	(8 << (CCMR2_IC3F_SHIFT))
#define	CCMR2_IC3F_DTS8_N8	(9 << (CCMR2_IC3F_SHIFT))
#define	CCMR2_IC3F_DTS16_N5	(10 << (CCMR2_IC3F_SHIFT))
#define	CCMR2_IC3F_DTS16_N6	(11 << (CCMR2_IC3F_SHIFT))
#define	CCMR2_IC3F_DTS16_N8	(12 << (CCMR2_IC3F_SHIFT))
#define	CCMR2_IC3F_DTS32_N5	(13 << (CCMR2_IC3F_SHIFT))
#define	CCMR2_IC3F_DTS32_N6	(14 << (CCMR2_IC3F_SHIFT))
#define	CCMR2_IC3F_DTS32_N8	(15 << (CCMR2_IC3F_SHIFT))
#define	CCMR2_IC3PSC_SHIFT	2
#define	CCMR2_IC3PSC_MASK	(3 << (CCMR2_IC3PSC_SHIFT))
#define	CCMR2_IC3PSC_DIV1	(0 << (CCMR2_IC3PSC_SHIFT))
#define	CCMR2_IC3PSC_DIV2	(1 << (CCMR2_IC3PSC_SHIFT))
#define	CCMR2_IC3PSC_DIV4	(2 << (CCMR2_IC3PSC_SHIFT))
#define	CCMR2_IC3PSC_DIV8	(3 << (CCMR2_IC3PSC_SHIFT))
#define	CCMR2_CC3S_SHIFT	0
#define	CCMR2_CC3S_MASK		(3 << (CCMR2_CC3S_SHIFT))
#define	CCMR2_CC3S_OUTPUT	(0 << (CCMR2_CC3S_SHIFT))
#define	CCMR2_CC3S_TI3		(1 << (CCMR2_CC3S_SHIFT))
#define	CCMR2_CC3S_TI4		(2 << (CCMR2_CC3S_SHIFT))
#define	CCMR2_CC3S_TRC		(3 << (CCMR2_CC3S_SHIFT))

  __IO uint32_t	ccer;		/* 0x20 1, 2, 3, 15 */
#define	CCER_CC4NP_SHIFT	15
#define	CCER_CC4NP_MASK		(1 << (CCER_CC4NP_SHIFT))
#define	CCER_CC4NP_NO		(0 << (CCER_CC4NP_SHIFT))
#define	CCER_CC4NP_YES		(1 << (CCER_CC4NP_SHIFT))
#define	CCER_CC4P_SHIFT		13
#define	CCER_CC4P_MASK		(1 << (CCER_CC4P_SHIFT))
#define	CCER_CC4P_NO		(0 << (CCER_CC4P_SHIFT))
#define	CCER_CC4P_YES		(1 << (CCER_CC4P_SHIFT))
#define	CCER_CC4E_SHIFT		12
#define	CCER_CC4E_MASK		(1 << (CCER_CC4E_SHIFT))
#define	CCER_CC4E_NO		(0 << (CCER_CC4E_SHIFT))
#define	CCER_CC4E_YES		(1 << (CCER_CC4E_SHIFT))
#define	CCER_CC3NP_SHIFT	11
#define	CCER_CC3NP_MASK		(1 << (CCER_CC3NP_SHIFT))
#define	CCER_CC3NP_NO		(0 << (CCER_CC3NP_SHIFT))
#define	CCER_CC3NP_YES		(1 << (CCER_CC3NP_SHIFT))
#define	CCER_CC3P_SHIFT		9
#define	CCER_CC3P_MASK		(1 << (CCER_CC3P_SHIFT))
#define	CCER_CC3P_NO		(0 << (CCER_CC3P_SHIFT))
#define	CCER_CC3P_YES		(1 << (CCER_CC3P_SHIFT))
#define	CCER_CC3E_SHIFT		8
#define	CCER_CC3E_MASK		(1 << (CCER_CC3E_SHIFT))
#define	CCER_CC3E_NO		(0 << (CCER_CC3E_SHIFT))
#define	CCER_CC3E_YES		(1 << (CCER_CC3E_SHIFT))
#define	CCER_CC2NP_SHIFT	7
#define	CCER_CC2NP_MASK		(1 << (CCER_CC2NP_SHIFT))
#define	CCER_CC2NP_NO		(0 << (CCER_CC2NP_SHIFT))
#define	CCER_CC2NP_YES		(1 << (CCER_CC2NP_SHIFT))
#define	CCER_CC2P_SHIFT		5
#define	CCER_CC2P_MASK		(1 << (CCER_CC2P_SHIFT))
#define	CCER_CC2P_NO		(0 << (CCER_CC2P_SHIFT))
#define	CCER_CC2P_YES		(1 << (CCER_CC2P_SHIFT))
#define	CCER_CC2E_SHIFT		4
#define	CCER_CC2E_MASK		(1 << (CCER_CC2E_SHIFT))
#define	CCER_CC2E_NO		(0 << (CCER_CC2E_SHIFT))
#define	CCER_CC2E_YES		(1 << (CCER_CC2E_SHIFT))
#define	CCER_CC1NP_SHIFT	3
#define	CCER_CC1NP_MASK		(1 << (CCER_CC1NP_SHIFT))
#define	CCER_CC1NP_NO		(0 << (CCER_CC1NP_SHIFT))
#define	CCER_CC1NP_YES		(1 << (CCER_CC1NP_SHIFT))
#define	CCER_CC1P_SHIFT		1
#define	CCER_CC1P_MASK		(1 << (CCER_CC1P_SHIFT))
#define	CCER_CC1P_NO		(0 << (CCER_CC1P_SHIFT))
#define	CCER_CC1P_YES		(1 << (CCER_CC1P_SHIFT))
#define	CCER_CC1E_SHIFT		0
#define	CCER_CC1E_MASK		(1 << (CCER_CC1E_SHIFT))
#define	CCER_CC1E_NO		(0 << (CCER_CC1E_SHIFT))
#define	CCER_CC1E_YES		(1 << (CCER_CC1E_SHIFT))

  __IO uint32_t	cnt;		/* 0x24 1, 2, 3, 15 */
#define	CNT_UIFCPY_SHIFT		15
#define	CNT_UIFCPY_MASK		(1 << (CNT_UIFCPY_SHIFT))
#define	CNT_UIFCPY_NO		(0 << (CNT_UIFCPY_SHIFT))
#define	CNT_UIFCPY_YES		(1 << (CNT_UIFCPY_SHIFT))

  __IO uint32_t	spc;		/* 0x28 1, 2, 3, 15 */
  __IO uint32_t	arr;		/* 0x2c 1, 2, 3, 15 */
  __IO uint32_t	rcr;		/* 0x30 1,       15 */
  __IO uint32_t	ccr1;		/* 0x34 1, 2, 3, 15 */
  __IO uint32_t	ccr2;		/* 0x38 1, 2, 3, 15 */
  __IO uint32_t	ccr3;		/* 0x3c 1, 2, 3     */
  __IO uint32_t	ccr4;		/* 0x40 1, 2, 3     */
  __IO uint32_t	bdtr;		/* 0x44 1,       15 */
#define	BDTR_MOE_SHIFT		15
#define	BDTR_MOE_MASK		(1 << (BDTR_MOE_SHIFT))
#define	BDTR_MOE_NO		(0 << (BDTR_MOE_SHIFT))
#define	BDTR_MOE_YES		(1 << (BDTR_MOE_SHIFT))

  __IO uint32_t	dcr;		/* 0x48 1,       15 */
  __IO uint32_t	dmar;		/* 0x4c 1, 2, 3, 15 */
  __IO uint32_t	or1;		/* 0x50 1, 2, 3, 15 */
#define	OR1_TI4RMP_SHIFT	2
#define	OR1_TI4RMP_MASK		(1 << (OR1_TI4RMP_SHIFT))
#define	OR1_TI4RMP_IO		(0 << (OR1_TI4RMP_SHIFT))
#define	OR1_TI4RMP_COMP1OUT	(0 << (OR1_TI4RMP_SHIFT))
#define	OR1_TI4RMP_COMP2OUT	(0 << (OR1_TI4RMP_SHIFT))
#define	OR1_TI4RMP_COMP12OUT_OR	(0 << (OR1_TI4RMP_SHIFT))
#define	OR1_ETR1RMP_SHIFT	1
#define	OR1_ETR1RMP_MASK	(1 << (OR1_ETR1RMP_SHIFT))
#define	OR1_ETR1RMP_IO		(0 << (OR1_ETR1RMP_SHIFT))
#define	OR1_ETR1RMP_LSE		(1 << (OR1_ETR1RMP_SHIFT))
#define	OR1_ITR1RMP_SHIFT	0
#define	OR1_ITR1RMP_MASK	(1 << (OR1_ITR1RMP_SHIFT))
#define	OR1_ITR1RMP_TIM2ITR1	(0 << (OR1_ITR1RMP_SHIFT))
#define	OR1_ITR1RMP_USBSOF	(1 << (OR1_ITR1RMP_SHIFT))

  __IO uint32_t	ccmr3;		/* 0x54 1,          */
  __IO uint32_t	ccr5;		/* 0x58 1,          */
  __IO uint32_t	ccr6;		/* 0x5c 1,          */
  __IO uint32_t	or2;		/* 0x60 1, 2, 3, 15 */
#define	OR2_ETRSEL_SHIFT	0
#define	OR2_ETRSEL_MASK		(1 << (OR2_ETRSEL_SHIFT))
#define	OR2_ETRSEL_ETR1TMP	(0 << (OR2_ETRSEL_SHIFT))
#define	OR2_ETRSEL_COMP1OUT	(1 << (OR2_ETRSEL_SHIFT))
#define	OR2_ETRSEL_COMP2OUT	(2 << (OR2_ETRSEL_SHIFT))

  __IO uint32_t	or3;		/* 0x64 1,          */
};

#define	TIM1_PTR	((struct _stStm32f7_TIM *) (APB2_BASE + 0x2c00))
#define	TIM2_PTR	((struct _stStm32f7_TIM *) (APB1_BASE + 0x0000))
#define	TIM7_PTR	((struct _stStm32f7_TIM *) (APB1_BASE + 0x1000))
#define	TIM8_PTR	((struct _stStm32f7_TIM *) (APB1_BASE + 0x1400))
#define	TIM15_PTR	((struct _stStm32f7_TIM *) (APB2_BASE + 0x4000))
#define	TIM16_PTR	((struct _stStm32f7_TIM *) (APB2_BASE + 0x4400))


struct _stStm32_I2C {
  __IO uint32_t	cr1;		/* 0x00 */
#define	CR1_PECEN_SHIFT		23
#define	CR1_PECEN_MASK		(1 << (CR1_PECEN_SHIFT))
#define	CR1_PECEN_NO		(0 << (CR1_PECEN_SHIFT))
#define	CR1_PECEN_YES		(1 << (CR1_PECEN_SHIFT))
#define	CR1_ALERTEN_SHIFT	22	
#define	CR1_ALERTEN_MASK	(1 << (CR1_ALERTEN_SHIFT))
#define	CR1_ALERTEN_NO		(0 << (CR1_ALERTEN_SHIFT))
#define	CR1_ALERTEN_YES		(1 << (CR1_ALERTEN_SHIFT))
#define	CR1_SMBDEN_SHIFT	21	
#define	CR1_SMBDEN_MASK		(1 << (CR1_SMBDEN_SHIFT))
#define	CR1_SMBDEN_NO		(0 << (CR1_SMBDEN_SHIFT))
#define	CR1_SMBDEN_YES		(1 << (CR1_SMBDEN_SHIFT))
#define	CR1_SMBHEN_SHIFT	20	
#define	CR1_SMBHEN_MASK		(1 << (CR1_SMBHEN_SHIFT))
#define	CR1_SMBHEN_NO		(0 << (CR1_SMBHEN_SHIFT))
#define	CR1_SMBHEN_YES		(1 << (CR1_SMBHEN_SHIFT))
#define	CR1_GCEN_SHIFT		19
#define	CR1_GCEN_MASK		(1 << (CR1_GCEN_SHIFT))
#define	CR1_GCEN_NO		(0 << (CR1_GCEN_SHIFT))
#define	CR1_GCEN_YES		(1 << (CR1_GCEN_SHIFT))
#define	CR1_WUPEN_SHIFT		18
#define	CR1_WUPEN_MASK		(1 << (CR1_WUPEN_SHIFT))
#define	CR1_WUPEN_NO		(0 << (CR1_WUPEN_SHIFT))
#define	CR1_WUPEN_YES		(1 << (CR1_WUPEN_SHIFT))
#define	CR1_NOSTRETCH_SHIFT	17	
#define	CR1_NOSTRETCH_MASK	(1 << (CR1_NOSTRETCH_SHIFT))
#define	CR1_NOSTRETCH_NO	(0 << (CR1_NOSTRETCH_SHIFT))
#define	CR1_NOSTRETCH_YES	(1 << (CR1_NOSTRETCH_SHIFT))
#define	CR1_SBC_SHIFT		16
#define	CR1_SBC_MASK		(1 << (CR1_SBC_SHIFT))
#define	CR1_SBC_NO		(0 << (CR1_SBC_SHIFT))
#define	CR1_SBC_YES		(1 << (CR1_SBC_SHIFT))
#define	CR1_RXDDMAEN_SHIFT	15	
#define	CR1_RXDDMAEN_MASK      	(1 << (CR1_RXDDMAEN_SHIFT))
#define	CR1_RXDDMAEN_NO		(0 << (CR1_RXDDMAEN_SHIFT))
#define	CR1_RXDDMAEN_YES       	(1 << (CR1_RXDDMAEN_SHIFT))
#define	CR1_TXDMAEN_SHIFT	14	
#define	CR1_TXDMAEN_MASK	(1 << (CR1_TXDMAEN_SHIFT))
#define	CR1_TXDMAEN_NO		(0 << (CR1_TXDMAEN_SHIFT))
#define	CR1_TXDMAEN_YES		(1 << (CR1_TXDMAEN_SHIFT))
#define	CR1_ANFOFF_SHIFT	12	
#define	CR1_ANFOFF_MASK		(1 << (CR1_ANFOFF_SHIFT))
#define	CR1_ANFOFF_NO		(0 << (CR1_ANFOFF_SHIFT))
#define	CR1_ANFOFF_YES		(1 << (CR1_ANFOFF_SHIFT))
#define	CR1_DNF_SHIFT		8
#define	CR1_DNF_MASK		(15 << (CR1_DNF_SHIFT))
#define	CR1_DNF_OFF		(0 << (CR1_DNF_SHIFT))
#define	CR1_DNF_CLK1		(1 << (CR1_DNF_SHIFT))
#define	CR1_DNF_CLK2		(2 << (CR1_DNF_SHIFT))
#define	CR1_DNF_CLK15		(15 << (CR1_DNF_SHIFT))
#define	CR1_ERRIE_SHIFT		7
#define	CR1_ERRIE_MASK		(1 << (CR1_ERRIE_SHIFT))
#define	CR1_ERRIE_NO		(0 << (CR1_ERRIE_SHIFT))
#define	CR1_ERRIE_YES		(1 << (CR1_ERRIE_SHIFT))
#define	CR1_TCIE_SHIFT		6
#define	CR1_TCIE_MASK		(1 << (CR1_TCIE_SHIFT))
#define	CR1_TCIE_NO		(0 << (CR1_TCIE_SHIFT))
#define	CR1_TCIE_YES		(1 << (CR1_TCIE_SHIFT))
#define	CR1_STOPIE_SHIFT	5	
#define	CR1_STOPIE_MASK		(1 << (CR1_STOPIE_SHIFT))
#define	CR1_STOPIE_NO		(0 << (CR1_STOPIE_SHIFT))
#define	CR1_STOPIE_YES		(1 << (CR1_STOPIE_SHIFT))
#define	CR1_NACKIE_SHIFT	4	
#define	CR1_NACKIE_MASK		(1 << (CR1_NACKIE_SHIFT))
#define	CR1_NACKIE_NO		(0 << (CR1_NACKIE_SHIFT))
#define	CR1_NACKIE_YES		(1 << (CR1_NACKIE_SHIFT))
#define	CR1_ADDRIE_SHIFT	3	
#define	CR1_ADDRIE_MASK		(1 << (CR1_ADDRIE_SHIFT))
#define	CR1_ADDRIE_NO		(0 << (CR1_ADDRIE_SHIFT))
#define	CR1_ADDRIE_YES		(1 << (CR1_ADDRIE_SHIFT))
#define	CR1_RXIE_SHIFT		2
#define	CR1_RXIE_MASK		(1 << (CR1_RXIE_SHIFT))
#define	CR1_RXIE_NO		(0 << (CR1_RXIE_SHIFT))
#define	CR1_RXIE_YES		(1 << (CR1_RXIE_SHIFT))
#define	CR1_TXIE_SHIFT		1
#define	CR1_TXIE_MASK		(1 << (CR1_TXIE_SHIFT))
#define	CR1_TXIE_NO		(0 << (CR1_TXIE_SHIFT))
#define	CR1_TXIE_YES		(1 << (CR1_TXIE_SHIFT))
#define	CR1_PE_SHIFT		0
#define	CR1_PE_MASK		(1 << (CR1_PE_SHIFT))
#define	CR1_PE_NO		(0 << (CR1_PE_SHIFT))
#define	CR1_PE_YES		(1 << (CR1_PE_SHIFT))
  
  __IO uint32_t	cr2;		/* 0x04 */
#define	CR2_PECBYTE_SHIFT	26	
#define	CR2_PECBYTE_MASK	(1 << (CR2_PECBYTE_SHIFT))
#define	CR2_PECBYTE_NO		(0 << (CR2_PECBYTE_SHIFT))
#define	CR2_PECBYTE_YES		(1 << (CR2_PECBYTE_SHIFT))
#define	CR2_AUTOEND_SHIFT	25	
#define	CR2_AUTOEND_MASK	(1 << (CR2_AUTOEND_SHIFT))
#define	CR2_AUTOEND_NO		(0 << (CR2_AUTOEND_SHIFT))
#define	CR2_AUTOEND_YES		(1 << (CR2_AUTOEND_SHIFT))
#define	CR2_RELOAD_SHIFT	24	
#define	CR2_RELOAD_MASK		(1 << (CR2_RELOAD_SHIFT))
#define	CR2_RELOAD_NO		(0 << (CR2_RELOAD_SHIFT))
#define	CR2_RELOAD_YES		(1 << (CR2_RELOAD_SHIFT))
#define	CR2_NBYTES_SHIFT	16	
#define	CR2_NBYTES_MASK		(0xff << (CR2_NBYTES_SHIFT))
#define	CR2_NACK_SHIFT		15
#define	CR2_NACK_MASK		(1 << (CR2_NACK_SHIFT))
#define	CR2_NACK_NO		(0 << (CR2_NACK_SHIFT))
#define	CR2_NACK_YES		(1 << (CR2_NACK_SHIFT))
#define	CR2_STOP_SHIFT		14
#define	CR2_STOP_MASK		(1 << (CR2_STOP_SHIFT))
#define	CR2_STOP_NO		(0 << (CR2_STOP_SHIFT))
#define	CR2_STOP_YES		(1 << (CR2_STOP_SHIFT))
#define	CR2_START_SHIFT		13
#define	CR2_START_MASK		(1 << (CR2_START_SHIFT))
#define	CR2_START_NO		(0 << (CR2_START_SHIFT))
#define	CR2_START_YES		(1 << (CR2_START_SHIFT))
#define	CR2_HEAD10R_SHIFT	12	
#define	CR2_HEAD10R_MASK	(1 << (CR2_HEAD10R_SHIFT))
#define	CR2_HEAD10R_NO		(0 << (CR2_HEAD10R_SHIFT))
#define	CR2_HEAD10R_YES		(1 << (CR2_HEAD10R_SHIFT))
#define	CR2_ADDR10_SHIFT	11	
#define	CR2_ADDR10_MASK		(1 << (CR2_ADDR10_SHIFT))
#define	CR2_ADDR10_NO		(0 << (CR2_ADDR10_SHIFT))
#define	CR2_ADDR10_YES		(1 << (CR2_ADDR10_SHIFT))
#define	CR2_RD_WDN_SHIFT	10	
#define	CR2_RD_WDN_MASK		(1 << (CR2_RD_WDN_SHIFT))
#define	CR2_RD_WDN_NO		(0 << (CR2_RD_WDN_SHIFT))
#define	CR2_RD_WDN_YES		(1 << (CR2_RD_WDN_SHIFT))
#define	CR2_SADD_SHIFT		0
#define	CR2_SADD_MASK		(0x3ff << (CR2_SADD_SHIFT))

  __IO uint32_t	oar1;		/* 0x08 */
#define	OAR1_OA1EN_SHIFT	15
#define	OAR1_OA1EN_MASK		(1 << (OAR1_OA1EN_SHIFT))
#define	OAR1_OA1EN_NO		(0 << (OAR1_OA1EN_SHIFT))
#define	OAR1_OA1EN_YES		(1 << (OAR1_OA1EN_SHIFT))
#define	OAR1_OA1MODE_SHIFT	10
#define	OAR1_OA1MODE_MASK	(1 << (OAR1_OA1MODE_SHIFT))
#define	OAR1_OA1MODE_NO		(0 << (OAR1_OA1MODE_SHIFT))
#define	OAR1_OA1MODE_YES	(1 << (OAR1_OA1MODE_SHIFT))
#define	OAR1_OA1_7BIT_SHIFT	1
#define	OAR1_OA1_7BIT_MASK	(1 << (OAR1_OA1_7BIT_SHIFT))
#define	OAR1_OA1_10BIT_SHIFT	0
#define	OAR1_OA1_10BIT_MASK	(1 << (OAR1_OA1_10BIT_SHIFT))
  
  __IO uint32_t	oar2;		/* 0x0c */
  __IO uint32_t	timingr;	/* 0x10 */
  __IO uint32_t	timeoutr;	/* 0x14 */
  __IO uint32_t	isr;		/* 0x18 */
  /* use IISR,_XX, because ISR_XX word is already used in USART */
#define	IISR_ADDCODE_SHIFT	17	
#define	IISR_ADDCODE_MASK	(0x7f << (IISR_ADDCODE_SHIFT))
#define	IISR_DIR_SHIFT		16
#define	IISR_DIR_MASK		(1 << (IISR_DIR_SHIFT))
#define	IISR_BUSY_SHIFT		15
#define	IISR_BUSY_MASK		(1 << (IISR_BUSY_SHIFT))
#define	IISR_ALERT_SHIFT	13
#define	IISR_ALERT_MASK		(1 << (IISR_ALERT_SHIFT))
#define	IISR_TIMEOUT_SHIFT	12	
#define	IISR_TIMEOUT_MASK	(1 << (IISR_TIMEOUT_SHIFT))
#define	IISR_PECERR_SHIFT	11	
#define	IISR_PECERR_MASK	(1 << (IISR_PECERR_SHIFT))
#define	IISR_OVR_SHIFT		10
#define	IISR_OVR_MASK		(1 << (IISR_OVR_SHIFT))
#define	IISR_ARLO_SHIFT		9
#define	IISR_ARLO_MASK		(1 << (IISR_ARLO_SHIFT))
#define	IISR_PE_SHIFT		8
#define	IISR_BERR_MASK		(1 << (IISR_BERR_SHIFT))
#define	IISR_TCR_SHIFT		7
#define	IISR_TCR_MASK		(1 << (IISR_TCR_SHIFT))
#define	IISR_TC_SHIFT		6
#define	IISR_TC_MASK		(1 << (IISR_TC_SHIFT))
#define	IISR_STOPF_SHIFT	5
#define	IISR_STOPF_MASK		(1 << (IISR_STOPF_SHIFT))
#define	IISR_NACKF_SHIFT	4
#define	IISR_NACKF_MASK		(1 << (IISR_NACKF_SHIFT))
#define	IISR_ADDR_SHIFT		3
#define	IISR_ADDR_MASK		(1 << (IISR_ADDR_SHIFT))
#define	IISR_RXNE_SHIFT		2
#define	IISR_RXNE_MASK		(1 << (IISR_RXNE_SHIFT))
#define	IISR_RXIS_SHIFT		1
#define	IISR_RXIS_MASK		(1 << (IISR_RXIS_SHIFT))
#define	IISR_RXIS_FLUSH		(1 << (IISR_RXIS_SHIFT))
#define	IISR_TXE_SHIFT		0
#define	IISR_TXE_MASK		(1 << (IISR_TXE_SHIFT))
#define	IISR_TXE_FLUSH		(1 << (IISR_TXE_SHIFT))

  __IO uint32_t	icr;		/* 0x1c */
#define	ICR_ALERTCF_SHIFT	13	
#define	ICR_ALERTCF_CLEAR	(1 << (ICR_ALERTCF_SHIFT))
#define	ICR_TIMEOUTCF_SHIFT	12	
#define	ICR_TIMEOUTCF_CLEAR	(1 << (ICR_TIMEOUTCF_SHIFT))
#define	ICR_PECCF_SHIFT		11
#define	ICR_PECCF_CLEAR		(1 << (ICR_PECCF_SHIFT))
#define	ICR_OVRCF_SHIFT		10
#define	ICR_OVRCF_CLEAR		(1 << (ICR_OVRCF_SHIFT))
#define	ICR_ARLOCKF_SHIFT	9	
#define	ICR_ARLOCKF_CLEAR	(1 << (ICR_ARLOCKF_SHIFT))
#define	ICR_BERRCF_SHIFT	8	
#define	ICR_BERRCF_CLEAR	(1 << (ICR_BERRCF_SHIFT))
#define	ICR_STOPCF_SHIFT	5	
#define	ICR_STOPCF_CLEAR	(1 << (ICR_STOPCF_SHIFT))
#define	ICR_NACKCF_SHIFT	4	
#define	ICR_NACKCF_CLEAR	(1 << (ICR_NACKCF_SHIFT))
#define	ICR_ADDRCF_SHIFT	3
#define	ICR_ADDRCF_CLEAR	(1 << (ICR_ADDRCF_SHIFT))

  __IO uint32_t	pecr;		/* 0x20 */
  __IO uint32_t	rxdr;		/* 0x24 */
  __IO uint32_t	txdr;		/* 0x28 */
};
#define	I2C_PTR		((struct _stStm32_I2C *) (APB1_BASE + 0x5400))
#define	I2C1_PTR	((struct _stStm32_I2C *) (APB1_BASE + 0x5400))
#define	I2C2_PTR	((struct _stStm32_I2C *) (APB1_BASE + 0x5800))
#define	I2C3_PTR	((struct _stStm32_I2C *) (APB1_BASE + 0x5c00))

struct _stStm32f7_DAC {
  __IO uint32_t	cr;				/* 0x00 */
#define	CR_CEN2_SHIFT		30
#define	CR_CEN2_MASK		(1 << (CR_CEN2_SHIFT))
#define	CR_CEN2_NO		(0 << (CR_CEN2_SHIFT))
#define	CR_CEN2_YES		(1 << (CR_CEN2_SHIFT))
#define	CR_DMAUDRIE2_SHIFT	29
#define	CR_DMAUDRIE2_MASK	(1 << (CR_DMAUDRIE2_SHIFT))
#define	CR_DMAUDRIE2_NO		(0 << (CR_DMAUDRIE2_SHIFT))
#define	CR_DMAUDRIE2_YES	(1 << (CR_DMAUDRIE2_SHIFT))
#define	CR_DMAEN2_SHIFT		28
#define	CR_DMAEN2_MASK		(1 << (CR_DMAEN2_SHIFT))
#define	CR_DMAEN2_NO		(0 << (CR_DMAEN2_SHIFT))
#define	CR_DMAEN2_YES		(1 << (CR_DMAEN2_SHIFT))
#define	CR_MAMP2_SHIFT		24
#define	CR_MAMP2_MASK		(1 << (CR_MAMP2_SHIFT))
#define	CR_MAMP2_UNMASK_1LSB	(0 << (CR_MAMP2_SHIFT))
#define	CR_MAMP2_UNMASK_2LSB	(1 << (CR_MAMP2_SHIFT))
#define	CR_MAMP2_UNMASK_3LSB	(2 << (CR_MAMP2_SHIFT))
#define	CR_MAMP2_UNMASK_4LSB	(3 << (CR_MAMP2_SHIFT))
#define	CR_MAMP2_UNMASK_5LSB	(4 << (CR_MAMP2_SHIFT))
#define	CR_MAMP2_UNMASK_6LSB	(5 << (CR_MAMP2_SHIFT))
#define	CR_MAMP2_UNMASK_7LSB	(6 << (CR_MAMP2_SHIFT))
#define	CR_MAMP2_UNMASK_8LSB	(7 << (CR_MAMP2_SHIFT))
#define	CR_MAMP2_UNMASK_9LSB	(8 << (CR_MAMP2_SHIFT))
#define	CR_MAMP2_UNMASK_10LSB	(9 << (CR_MAMP2_SHIFT))
#define	CR_MAMP2_UNMASK_11LSB	(10 << (CR_MAMP2_SHIFT))
#define	CR_WAVE2_SHIFT		22
#define	CR_WAVE2_MASK		(1 << (CR_WAVE2_SHIFT))
#define	CR_WAVE2_DISABLE	(0 << (CR_WAVE2_SHIFT))
#define	CR_WAVE2_GEN_NOISE	(1 << (CR_WAVE2_SHIFT))
#define	CR_WAVE2_GEN_TRIANGLE	(2 << (CR_WAVE2_SHIFT))
#define	CR_TSEL2_SHIFT		21
#define	CR_TSEL2_MASK		(1 << (CR_TSEL2_SHIFT))
#define	CR_TSEL2_TIM6TRGO	(0 << (CR_TSEL2_SHIFT))
#define	CR_TSEL2_RESERVED1	(1 << (CR_TSEL2_SHIFT))
#define	CR_TSEL2_TIM7TRGO	(2 << (CR_TSEL2_SHIFT))
#define	CR_TSEL2_RESERVED3	(3 << (CR_TSEL2_SHIFT))
#define	CR_TSEL2_TIM2TRGO	(4 << (CR_TSEL2_SHIFT))
#define	CR_TSEL2_RESERVED5	(5 << (CR_TSEL2_SHIFT))
#define	CR_TSEL2_EXTERNAL_LINE9	(6 << (CR_TSEL2_SHIFT))
#define	CR_TSEL2_SOFTWARE_TRG	(7 << (CR_TSEL2_SHIFT))
#define	CR_TEN2_SHIFT		18
#define	CR_TEN2_MASK		(1 << (CR_TEN2_SHIFT))
#define	CR_TEN2_NO		(0 << (CR_TEN2_SHIFT))
#define	CR_TEN2_YES		(1 << (CR_TEN2_SHIFT))
#define	CR_EN2_SHIFT		16
#define	CR_EN2_MASK		(1 << (CR_EN2_SHIFT))
#define	CR_EN2_NO		(0 << (CR_EN2_SHIFT))
#define	CR_EN2_YES		(1 << (CR_EN2_SHIFT))
#define	CR_CEN1_SHIFT		14
#define	CR_CEN1_MASK		(1 << (CR_CEN1_SHIFT))
#define	CR_CEN1_NO		(0 << (CR_CEN1_SHIFT))
#define	CR_CEN1_YES		(1 << (CR_CEN1_SHIFT))
#define	CR_DMAUDRIE1_SHIFT	13
#define	CR_DMAUDRIE1_MASK	(1 << (CR_DMAUDRIE1_SHIFT))
#define	CR_DMAUDRIE1_NO		(0 << (CR_DMAUDRIE1_SHIFT))
#define	CR_DMAUDRIE1_YES	(1 << (CR_DMAUDRIE1_SHIFT))
#define	CR_DMAEN1_SHIFT		12
#define	CR_DMAEN1_MASK		(1 << (CR_DMAEN1_SHIFT))
#define	CR_DMAEN1_NO		(0 << (CR_DMAEN1_SHIFT))
#define	CR_DMAEN1_YES		(1 << (CR_DMAEN1_SHIFT))
#define	CR_MAMP1_SHIFT		8
#define	CR_MAMP1_MASK		(1 << (CR_MAMP1_SHIFT))
#define	CR_MAMP1_UNMASK_1LSB	(0 << (CR_MAMP1_SHIFT))
#define	CR_MAMP1_UNMASK_2LSB	(1 << (CR_MAMP1_SHIFT))
#define	CR_MAMP1_UNMASK_3LSB	(2 << (CR_MAMP1_SHIFT))
#define	CR_MAMP1_UNMASK_4LSB	(3 << (CR_MAMP1_SHIFT))
#define	CR_MAMP1_UNMASK_5LSB	(4 << (CR_MAMP1_SHIFT))
#define	CR_MAMP1_UNMASK_6LSB	(5 << (CR_MAMP1_SHIFT))
#define	CR_MAMP1_UNMASK_7LSB	(6 << (CR_MAMP1_SHIFT))
#define	CR_MAMP1_UNMASK_8LSB	(7 << (CR_MAMP1_SHIFT))
#define	CR_MAMP1_UNMASK_9LSB	(8 << (CR_MAMP1_SHIFT))
#define	CR_MAMP1_UNMASK_10LSB	(9 << (CR_MAMP1_SHIFT))
#define	CR_MAMP1_UNMASK_11LSB	(10 << (CR_MAMP1_SHIFT))
#define	CR_WAVE1_SHIFT		6
#define	CR_WAVE1_MASK		(1 << (CR_WAVE1_SHIFT))
#define	CR_WAVE1_DISABLE	(0 << (CR_WAVE1_SHIFT))
#define	CR_WAVE1_GEN_NOISE	(1 << (CR_WAVE1_SHIFT))
#define	CR_WAVE1_GEN_TRIANGLE	(2 << (CR_WAVE1_SHIFT))
#define	CR_TSEL1_SHIFT		3
#define	CR_TSEL1_MASK		(1 << (CR_TSEL1_SHIFT))
#define	CR_TSEL1_TIM6TRGO	(0 << (CR_TSEL1_SHIFT))
#define	CR_TSEL1_RESERVED1	(1 << (CR_TSEL1_SHIFT))
#define	CR_TSEL1_TIM7TRGO	(2 << (CR_TSEL1_SHIFT))
#define	CR_TSEL1_RESERVED3	(3 << (CR_TSEL1_SHIFT))
#define	CR_TSEL1_TIM2TRGO	(4 << (CR_TSEL1_SHIFT))
#define	CR_TSEL1_RESERVED5	(5 << (CR_TSEL1_SHIFT))
#define	CR_TSEL1_EXTERNAL_LINE9	(6 << (CR_TSEL1_SHIFT))
#define	CR_TSEL1_SOFTWARE_TRG	(7 << (CR_TSEL1_SHIFT))
#define	CR_TEN1_SHIFT		2
#define	CR_TEN1_MASK		(1 << (CR_TEN1_SHIFT))
#define	CR_TEN1_NO		(0 << (CR_TEN1_SHIFT))
#define	CR_TEN1_YES		(1 << (CR_TEN1_SHIFT))
#define	CR_EN1_SHIFT		0
#define	CR_EN1_MASK		(1 << (CR_EN1_SHIFT))
#define	CR_EN1_NO		(0 << (CR_EN1_SHIFT))
#define	CR_EN1_YES		(1 << (CR_EN1_SHIFT))

  __IO uint32_t	swtrgr;			/* 0x04 */
  __IO uint32_t	dhr12r1;			/* 0x08 */
  __IO uint32_t	dhr12l1;			/* 0x0c */
  __IO uint32_t	dhr8r1;			/* 0x10 */
  __IO uint32_t	dhr12r2;			/* 0x14 */
  __IO uint32_t	dhr12l2;			/* 0x18 */
  __IO uint32_t	dhr8r2;			/* 0x1c */
  __IO uint32_t	dhr12rd;			/* 0x20 */
  __IO uint32_t	dhr12ld;			/* 0x24 */
  __IO uint32_t	dhr8rd;			/* 0x28 */
  __IO uint32_t	dor1;			/* 0x2c */
  __IO uint32_t	dor2;			/* 0x30 */
  __IO uint32_t	sr;				/* 0x34 */
  __IO uint32_t	ccr;			/* 0x38 */
  __IO uint32_t	mcr;			/* 0x3e */
#define	MCR_MODE2_SHIFT			16
#define	MCR_MODE2_MASK			(1 << (MCR_MODE2_SHIFT))
#define	MCR_MODE2_NORMAL_EXTPIN_BUF	(0 << (MCR_MODE2_SHIFT))
#define	MCR_MODE2_NORMAL_ONCHIP_BUF	(1 << (MCR_MODE2_SHIFT))
#define	MCR_MODE2_NORMAL_EXTPIN		(2 << (MCR_MODE2_SHIFT))
#define	MCR_MODE2_NORMAL_ONCHIP		(3 << (MCR_MODE2_SHIFT))
#define	MCR_MODE2_SH_EXTPIN_BUF		(4 << (MCR_MODE2_SHIFT))
#define	MCR_MODE2_SH_ONCHIP_BUF		(5 << (MCR_MODE2_SHIFT))
#define	MCR_MODE2_SH_EXTPIN		(6 << (MCR_MODE2_SHIFT))
#define	MCR_MODE2_SH_ONCHIP		(7 << (MCR_MODE2_SHIFT))
#define	MCR_MODE1_SHIFT			0
#define	MCR_MODE1_MASK			(1 << (MCR_MODE1_SHIFT))
#define	MCR_MODE1_NORMAL_EXTPIN_BUF	(0 << (MCR_MODE1_SHIFT))
#define	MCR_MODE1_NORMAL_ONCHIP_BUF	(1 << (MCR_MODE1_SHIFT))
#define	MCR_MODE1_NORMAL_EXTPIN		(2 << (MCR_MODE1_SHIFT))
#define	MCR_MODE1_NORMAL_ONCHIP		(3 << (MCR_MODE1_SHIFT))
#define	MCR_MODE1_SH_EXTPIN_BUF		(4 << (MCR_MODE1_SHIFT))
#define	MCR_MODE1_SH_ONCHIP_BUF		(5 << (MCR_MODE1_SHIFT))
#define	MCR_MODE1_SH_EXTPIN		(6 << (MCR_MODE1_SHIFT))
#define	MCR_MODE1_SH_ONCHIP		(7 << (MCR_MODE1_SHIFT))

  __IO uint32_t	shsr1;			/* 0x40 */
  __IO uint32_t	shsr2;			/* 0x44 */
  __IO uint32_t	shhr;			/* 0x48 */
  __IO uint32_t	shrr;			/* 0x4c */
};

#define	DAC_PTR	((struct _stStm32f7_DAC *) (APB1_BASE + 0x7400))

#define	GPIO_PTR	((struct _stStm32_GPIO *) ((AHB1_BASE) + 0x0000))
#define	RCC_PTR		((struct _stStm32_RCC *) ((AHB1_BASE) + 0x3800))
#define	USART1_PTR	((struct _stStm32_USART *) (USART1_BASE))
#define	USART2_PTR	((struct _stStm32_USART *) (USART2_BASE))
#define	USART3_PTR	((struct _stStm32_USART *) (USART3_BASE))
/*#define	FLASH_PTR	((struct _stStm32f7_FLASH *) ((AHB1_BASE) + 0x2000))*/
#define	FLASH_PTR	((struct _stStm32f7_FLASH *) (FLASH_R_BASE))


#endif
