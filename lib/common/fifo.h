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

#ifndef	_FIFO_H_
#define	_FIFO_H_


#define	FIFO_ERRNO_SUCCESS		0
#define	FIFO_ERRNO_UNKNOWN		(-1)	/* unknow fifo error */
#define	FIFO_ERRNO_NOMEMORY		(-2)	/* can't allocate memory */
#define	FIFO_ERRNO_NOSPACE		(-3)	/* no space in the system */
#define	FIFO_ERRNO_INVALIDARG		(-4)	/* invalid argument */
#define	FIFO_ERRNO_PARAMEXCEEDED	(-5)	/* parameter exceeded */
#define	FIFO_ERRNO_PERM			(-6)	/* operation not permitted */


#ifdef	_FIFO_C_

#define	FIFO_ENTRY		32
#define	FIFO_SIZEEXP_MAX	30
#define	FIFO_SIZE_MAX		(1 << 30)

typedef struct {
  uint8_t	*buf;
  int		flag;
#define	FIFO_UP				(1 < 0)	/* up */
#define	FIFO_EXTERMAL_BUFFER		(1 < 1)	/* WithBuffer() */
  int		start;
  int		end;
  int		size;
} ring_fifo_t;

#endif	/* _FIFO_H_ */

/*
 * prototype
 */
void		FifoInit(void);
int 		FifoCreate(int sizeExp);
int		FifoCreateWithBuf(int sizeExp, void *buf);
int 		FifoCreateInt(int size);
int		FifoCreateIntWithBuf(int size, void *buf);
void		FifoDestroy(int no);
void		FifoClear(int no);
int 		FifoWriteIn(int no, uint8_t *buf, uint32_t size);
int 		FifoReadOut(int no, uint8_t *buf, uint32_t size);

int		FifoGetWritePointer(int no, uint8_t **buf, int *size);
int		FifoGetReadPointer(int no, uint8_t **buf, int *size);
int		FifoAddWritePointer(int no, int size);
int		FifoAddReadPointer(int no, int size);

int 		FifoGetDirtyLen(int no);
int 		FifoGetEmptyLen(int no);

#endif	/* _FIFO_H_ */
