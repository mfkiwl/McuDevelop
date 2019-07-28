/*
 * Copyright (c) 2018 zhtlab
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

#ifndef _INTRVEC_H_
#define _INTRVEC_H_

#include        "devUsart.h"
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void ADC_IRQHandler(void);
void DMA1_Stream0_IRQHandler(void);
void DMA1_Stream1_IRQHandler(void);
void DMA1_Stream2_IRQHandler(void);
void DMA1_Stream3_IRQHandler(void);
void DMA1_Stream4_IRQHandler(void);
void DevI2c1Interrupt(void);
void DevI2c2Interrupt(void);
void DevUsbInterruptUsb2(void);

enum interruptLines {
  INTR_MAIN_STACK =            0, /* Main Stack                   */
  INTR_RESETI =                1, /* Reset                        */
  INTR_NMII =                  2, /* Non-maskable Intr       */
  INTR_HFI =                   3, /* Hard Fault                   */
  INTR_MMI =                   4, /* Memory Management            */
  INTR_BFI =                   5, /* Bus Fault                    */
  INTR_UFI =                   6, /* Usage Fault                  */
  INTR_SVCI =                 11, /* SVCall                       */
  INTR_DMI =                  12, /* Debug Monitor                */
  INTR_PSI =                  14, /* PendSV                       */
  INTR_STI =                  15, /* SysTick                      */

  INTR_MASKABLE_FACTOR_START = 16,
  INTR_MASKABLE_FACTOR_END   = 240,
};


#ifdef _INTR_C_
void *__vector_table_user[] __attribute__((section(".isr_vector_usr"))) = {
  IntrException,                        /*   0                              */
  IntrException,                        /*   1                              */
  IntrException,                        /*   2                              */
  IntrException,                        /*   3                              */
  IntrException,                        /*   4                              */
  IntrException,                        /*   5                              */
  IntrException,                        /*   6                              */
  IntrException,                        /*   7                              */
  IntrException,                        /*   8                              */
  IntrException,                        /*   9                              */
  IntrException,                        /*  10                              */
  IntrException,                        /*  11 DMA1 Stream0                 */
  IntrException,                        /*  12 DMA1 Stream1                 */
  IntrException,                        /*  13 DMA1 Stream2                 */
  IntrException,                        /*  14 DMA1 Stream3                 */
  IntrException,                        /*  15 DMA1 Stream4                 */
  IntrException,                        /*  16 DMA1 Stream5                 */
  IntrException,                        /*  17 DMA1 Stream6                 */
  IntrException,                        /*  18 ADC                          */
  IntrException,                        /*  19 FDCAN1 intr0                 */
  IntrException,                        /*  20 FDCAN2 intr0                 */
  IntrException,                        /*  21 FDCAN1 intr1                 */
  IntrException,                        /*  22 FDCAN2 intr1                 */
  IntrException,                        /*  23 EXTI                         */
  IntrException,                        /*  24 TIM1 break                   */
  IntrException,                        /*  25 TIM1 update                  */
  IntrException,                        /*  26 TIM1 trigger                 */
  IntrException,                        /*  27 TIM1 capture/compare         */
  IntrException,                        /*  28 TIM2                         */
  IntrException,                        /*  29 TIM3                         */
  IntrException,                        /*  30 TIM4                         */
  DevI2c1Interrupt,                     /*  31 I2C1 Event                   */
  IntrException,                        /*  32 I2C1 Error                   */
  DevI2c2Interrupt,                     /*  33 I2C2 Event                   */
  IntrException,                        /*  34 I2C2 Error                   */
  IntrException,                        /*  35 SPI1                         */
  IntrException,                        /*  36 SPI2                         */
  DevUsart1Interrupt,                   /*  37 USART1                       */
  IntrException,                        /*  38 USART2                       */
  IntrException,                        /*  39 USART3                       */
  IntrException,                        /*  40                              */
  IntrException,                        /*  41                              */
  IntrException,                        /*  42                              */
  IntrException,                        /*  43                              */
  IntrException,                        /*  44                              */
  IntrException,                        /*  45                              */
  IntrException,                        /*  46                              */
  IntrException,                        /*  47                              */
  IntrException,                        /*  48                              */
  IntrException,                        /*  49                              */
  IntrException,                        /*  50                              */
  IntrException,                        /*  51                              */
  IntrException,                        /*  52                              */
  IntrException,                        /*  53                              */
  IntrException,                        /*  54                              */
  IntrException,                        /*  55                              */
  IntrException,                        /*  56                              */
  IntrException,                        /*  57                              */
  IntrException,                        /*  58                              */
  IntrException,                        /*  59                              */
  IntrException,                        /*  60                              */
  IntrException,                        /*  61                              */
  IntrException,                        /*  62                              */
  IntrException,                        /*  63                              */
  IntrException,                        /*  64                              */
  IntrException,                        /*  65                              */
  IntrException,                        /*  66                              */
  IntrException,                        /*  67                              */
  IntrException,                        /*  68                              */
  IntrException,                        /*  69                              */
  IntrException,                        /*  70                              */
  IntrException,                        /*  71                              */
  IntrException,                        /*  72                              */
  IntrException,                        /*  73                              */
  IntrException,                        /*  74                              */
  IntrException,                        /*  75                              */
  IntrException,                        /*  76                              */
  IntrException,                        /*  77                              */
  IntrException,                        /*  78                              */
  IntrException,                        /*  79                              */
  IntrException,                        /*  80                              */
  IntrException,                        /*  81                              */
  IntrException,                        /*  82                              */
  IntrException,                        /*  83                              */
  IntrException,                        /*  84                              */
  IntrException,                        /*  85                              */
  IntrException,                        /*  86                              */
  IntrException,                        /*  87                              */
  IntrException,                        /*  88                              */
  IntrException,                        /*  89                              */
  IntrException,                        /*  90                              */
  IntrException,                        /*  91                              */
  IntrException,                        /*  92                              */
  IntrException,                        /*  93                              */
  IntrException,                        /*  94                              */
  IntrException,                        /*  95                              */
  IntrException,                        /*  96                              */
  IntrException,                        /*  97                              */
  IntrException,                        /*  98                              */
  IntrException,                        /*  99                              */
  IntrException,                        /* 100                              */
  DevUsbInterruptUsb2,                  /* 101 USB FS                       */
  IntrException,                        /* 102                              */
  IntrException,                        /* 103                              */
  IntrException,                        /* 104                              */
  IntrException,                        /* 105                              */
  IntrException,                        /* 106                              */
  IntrException,                        /* 107                              */
  IntrException,                        /* 108                              */
  IntrException,                        /* 109                              */
  IntrException,                        /* 110                              */
  IntrException,                        /* 111                              */
  IntrException,                        /* 112                              */
  IntrException,                        /* 113                              */
  IntrException,                        /* 114                              */
  IntrException,                        /* 115                              */
  IntrException,                        /* 116                              */
  IntrException,                        /* 117                              */
  IntrException,                        /* 118                              */
  IntrException,                        /* 119                              */
  IntrException,                        /* 120                              */
  IntrException,                        /* 121                              */
  IntrException,                        /* 122                              */
  IntrException,                        /* 123                              */
  IntrException,                        /* 124                              */
  IntrException,                        /* 125                              */
  IntrException,                        /* 126                              */
  IntrException,                        /* 127                              */
  IntrException,                        /* 128                              */
  IntrException,                        /* 129                              */
  IntrException,                        /* 130                              */
  IntrException,                        /* 131                              */
  IntrException,                        /* 132                              */
  IntrException,                        /* 133                              */
  IntrException,                        /* 134                              */
  IntrException,                        /* 135                              */
  IntrException,                        /* 136                              */
  IntrException,                        /* 137                              */
  IntrException,                        /* 138                              */
  IntrException,                        /* 139                              */
  IntrException,                        /* 140                              */
  IntrException,                        /* 141                              */
  IntrException,                        /* 142                              */
  IntrException,                        /* 143                              */
  IntrException,                        /* 144                              */
  IntrException,                        /* 145                              */
  IntrException,                        /* 146                              */
  IntrException,                        /* 147                              */
  IntrException,                        /* 148                              */
  IntrException,                        /* 149                              */
  IntrException,                        /* 150                              */
  IntrException,                        /* 151                              */
  IntrException,                        /* 152                              */
  IntrException,                        /* 153                              */
  IntrException,                        /* 154                              */
  IntrException,                        /* 155                              */
  IntrException,                        /* 156                              */
  IntrException,                        /* 157                              */
  IntrException,                        /* 158                              */
  IntrException,                        /* 159                              */
  IntrException,                        /* 160                              */
  IntrException,                        /* 161                              */
  IntrException,                        /* 162                              */
  IntrException,                        /* 163                              */
  IntrException,                        /* 164                              */
  IntrException,                        /* 165                              */
  IntrException,                        /* 166                              */
  IntrException,                        /* 167                              */
  IntrException,                        /* 168                              */
  IntrException,                        /* 169                              */
  IntrException,                        /* 170                              */
  IntrException,                        /* 171                              */
  IntrException,                        /* 172                              */
  IntrException,                        /* 173                              */
  IntrException,                        /* 174                              */
  IntrException,                        /* 175                              */
  IntrException,                        /* 176                              */
  IntrException,                        /* 177                              */
  IntrException,                        /* 178                              */
  IntrException,                        /* 179                              */
  IntrException,                        /* 180                              */
  IntrException,                        /* 181                              */
  IntrException,                        /* 182                              */
  IntrException,                        /* 183                              */
  IntrException,                        /* 184                              */
  IntrException,                        /* 185                              */
  IntrException,                        /* 186                              */
  IntrException,                        /* 187                              */
  IntrException,                        /* 188                              */
  IntrException,                        /* 189                              */
  IntrException,                        /* 190                              */
  IntrException,                        /* 191                              */
  IntrException,                        /* 192                              */
  IntrException,                        /* 193                              */
  IntrException,                        /* 194                              */
  IntrException,                        /* 195                              */
  IntrException,                        /* 196                              */
  IntrException,                        /* 197                              */
  IntrException,                        /* 198                              */
  IntrException,                        /* 199                              */
  IntrException,                        /* 200                              */
  IntrException,                        /* 201                              */
  IntrException,                        /* 202                              */
  IntrException,                        /* 203                              */
  IntrException,                        /* 204                              */
  IntrException,                        /* 205                              */
  IntrException,                        /* 206                              */
  IntrException,                        /* 207                              */
  IntrException,                        /* 208                              */
  IntrException,                        /* 209                              */
  IntrException,                        /* 210                              */
  IntrException,                        /* 211                              */
  IntrException,                        /* 212                              */
  IntrException,                        /* 213                              */
  IntrException,                        /* 214                              */
  IntrException,                        /* 215                              */
  IntrException,                        /* 216                              */
  IntrException,                        /* 217                              */
  IntrException,                        /* 218                              */
  IntrException,                        /* 219                              */
  IntrException,                        /* 220                              */
  IntrException,                        /* 221                              */
  IntrException,                        /* 222                              */
  IntrException,                        /* 223                              */
  IntrException,                        /* 224                              */
  IntrException,                        /* 225                              */
  IntrException,                        /* 226                              */
  IntrException,                        /* 227                              */
  IntrException,                        /* 228                              */
  IntrException,                        /* 229                              */
  IntrException,                        /* 230                              */
  IntrException,                        /* 231                              */
  IntrException,                        /* 232                              */
  IntrException,                        /* 233                              */
  IntrException,                        /* 234                              */
  IntrException,                        /* 235                              */
  IntrException,                        /* 236                              */
  IntrException,                        /* 237                              */
  IntrException,                        /* 238                              */
  IntrException,                        /* 239                              */
};
#endif

#endif
