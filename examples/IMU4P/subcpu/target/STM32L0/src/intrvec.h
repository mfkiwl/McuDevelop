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


void            DevUsart1Interrupt(void);
void            DevUsart2Interrupt(void);
void            MainTim2Intr(void);
void            MainUsbdifIntr(void);
void            DevCrsInterrupt(void);
void            MainInterruptTim(void);

#ifdef _INTR_C_
void *__vector_table_user[] __attribute__((section(".isr_vector_usr"))) = {
  IntrException,                        /*   0 WWDG                         */
  IntrException,                        /*   1 PVD                          */
  IntrException,                        /*   2 RTC                          */
  IntrException,                        /*   3 FLASH                        */
  DevCrsInterrupt,                      /*   4 RCC CRS                      */
  IntrException,                        /*   5 EXTI[1:0]                    */
  IntrException,                        /*   6 EXTI[3:2]                    */
  IntrException,                        /*   7 EXTI[15:4]                   */
  IntrException,                        /*   8 TSC                          */
  IntrException,                        /*   9 DMA CH1                      */
  IntrException,                        /*  10 DMA CH3:2                    */
  IntrException,                        /*  11 DMA CH7:4                    */
  IntrException,                        /*  12 ADC COMP                     */
  IntrException,                        /*  13 LPTIM1                       */
  IntrException,                        /*  14 USART4/5                     */
  MainInterruptTim,                     /*  15 TIM2                         */
  IntrException,                        /*  16 TIM3                         */
  IntrException,                        /*  17 TIM6 DAC                     */
  IntrException,                        /*  18 TIM7                         */
  IntrException,                        /*  19                              */
  IntrException,                        /*  20 TIM21                        */
  IntrException,                        /*  21 I2C3                         */
  IntrException,                        /*  22 TIM22                        */
  IntrException,                        /*  23 I2C1                         */
  IntrException,                        /*  24 I2C2                         */
  IntrException,                        /*  25 SPI1                         */
  IntrException,                        /*  26 SPI2                         */
  DevUsart1Interrupt,                   /*  27 USART1                       */
  DevUsart2Interrupt,                   /*  28 USART2                       */
  IntrException,                        /*  29 TIM3                         */
  IntrException,                        /*  30 LPUART1, AES, RNG            */
  IntrException,                        /*  31 USB                          */
#if 0
  IntrException,                        /*  32                              */
  IntrException,                        /*  33                              */
  IntrException,                        /*  34                              */
  IntrException,                        /*  35                              */
  IntrException,                        /*  36                              */
  IntrException,                        /*  37                              */
  IntrException,                        /*  38                              */
  IntrException,                        /*  39                              */
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
  IntrException,                        /* 101                              */
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
#endif
};
#endif

#endif
