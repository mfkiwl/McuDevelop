/*
 * Copyright (c) 2019 zhtlab
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

#ifndef _DEV_ERRNO_H_
#define _DEV_ERRNO_H_

enum sdmmc_cmd {
  DEV_ERRNO_SUCCESS             = 0,
  DEV_ERRNO_UNKNOWN             = -1,
  DEV_ERRNO_TIMEOUT             = -2,
  DEV_ERRNO_UNIT_EXCEEDED       = -3,
  DEV_ERRNO_NONBLOCK            = -4,
  DEV_ERRNO_RECV_ERROR          = -5,
  DEV_ERRNO_PARAM_EXCEEDED      = -6,

  DEV_ERRNO_NOTREADY            = -10,
  DEV_ERRNO_NODISK              = -11,
  DEV_ERRNO_PROTECTED           = -12,
};

#endif
