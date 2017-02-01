/*================================================================
 * Error diffusion -Floyd Steinberg-
 *----------------------------------------------------------------
 * Licence isn't exists.
 *
 * vmg.c
 *
 * Copyright (c) 2012 NULL
 *
 *================================================================*/

#include "ErrDiff.h"
#include <cstring>
#include <cstdio>

void	Imgproc::RightDiff1(UINT32 *lpDst, UINT32 *lpSrc, int width, int height, UINT8 threshold)
{
	INT32 quantErr[4];
	INT32 *lpErrTbl;
	
	if(!(lpErrTbl = new INT32[width * 3]))
	{	return;	}
	
	::memset(lpErrTbl, 0, width * 3 * sizeof(INT32));

	for(int i = 1; i < height - 1; i ++)
	{
		for(int j = 1; j < width - 1; j ++)
		{
			int idx = width * i + j;
			UINT32	pixel = lpSrc[idx];
			UINT32	newPixel = 0;

			quantErr[0] = lpErrTbl[width * 0 + j] + ((pixel) & 0xFF);
			quantErr[1] = lpErrTbl[width * 1 + j] + ((pixel >> 8) & 0xFF);
			quantErr[2] = lpErrTbl[width * 2 + j] + ((pixel >> 16) & 0xFF);
			
			if(quantErr[0] >= threshold)
			{
				quantErr[0] -= threshold;
				newPixel |= 0x0000FF;
			}
			if(quantErr[1] >= threshold)
			{
				quantErr[1] -= threshold;
				newPixel |= 0x00FF00;
			}
			if(quantErr[2] >= threshold)
			{
				quantErr[2] -= threshold;
				newPixel |= 0xFF0000;
			}
			lpDst[idx] = newPixel;
			
			lpErrTbl[width * 0 + j + 1] = quantErr[0];
			lpErrTbl[width * 1 + j + 1] = quantErr[1];
			lpErrTbl[width * 2 + j + 1] = quantErr[2];
		}
	}

	delete[] lpErrTbl;
}

void Imgproc::Floyd1(UINT32 *lpDst, UINT32 *lpSrc, int width, int height, UINT8 threshold)
{
	UINT8 pixel[4];
	INT32 quantErr[4];
	INT16 *lpErrTbl;

	lpErrTbl	= new INT16[width * 2 * 3];

	::memset(lpErrTbl, 0, width * 2 * 3 * sizeof(UINT16));
	
	for(int i = 1; i < height - 1; i ++)
	{
		for(int j = 1; j < width - 1; j ++)
		{
			int idx = width * i + j;
			INT32 newPixel = 0;
			
			*(UINT32 *)pixel = lpSrc[idx];

			quantErr[0] = lpErrTbl[width * (3 * (i & 0x01) + 0) + j] + pixel[0];
			quantErr[1] = lpErrTbl[width * (3 * (i & 0x01) + 1) + j] + pixel[1];
			quantErr[2] = lpErrTbl[width * (3 * (i & 0x01) + 2) + j] + pixel[2];
			
			if(quantErr[0] >= threshold)
			{
				quantErr[0] -= threshold;
				newPixel |= 0x0000FF;
			}
			if(quantErr[1] >= threshold)
			{
				quantErr[1] -= threshold;
				newPixel |= 0x00FF00;
			}
			if(quantErr[2] >= threshold)
			{
				quantErr[2] -= threshold;
				newPixel |= 0xFF0000;
			}
			lpDst[idx] = newPixel;

			idx = width * (3 * (i & 0x01) + 0) + j + 1;
			lpErrTbl[idx + width * 0]	+= quantErr[0] * 7 / 16;
			lpErrTbl[idx + width * 1]	+= quantErr[1] * 7 / 16;
			lpErrTbl[idx + width * 2]	+= quantErr[2] * 7 / 16;
			
			idx = width * (3 * ((i + 1) & 0x01) + 0) + j + 1;
			lpErrTbl[idx + width * 0]	+= quantErr[0] * 1 / 16;
			lpErrTbl[idx + width * 1]	+= quantErr[1] * 1 / 16;
			lpErrTbl[idx + width * 2]	+= quantErr[2] * 1 / 16;
			
			idx --;
			lpErrTbl[idx + width * 0]	+= quantErr[0] * 5 / 16;
			lpErrTbl[idx + width * 1]	+= quantErr[1] * 5 / 16;
			lpErrTbl[idx + width * 2]	+= quantErr[2] * 5 / 16;
			
			idx --;
			lpErrTbl[idx + width * 0]	+= quantErr[0] * 3 / 16;
			lpErrTbl[idx + width * 1]	+= quantErr[1] * 3 / 16;
			lpErrTbl[idx + width * 2]	+= quantErr[2] * 3 / 16;

			//printf("%d, %d\n", i, j);
		}
		
		::memset(lpErrTbl + width * (3 * (i & 0x01)), 0, width * 3 * sizeof(UINT16));
	}

	delete[]	lpErrTbl;
}

void Imgproc::Floyd2(UINT32 *lpDst, UINT32 *lpSrc, int width, int height, UINT8 threshold)
{
	UINT8 pixel[4];
	INT32 quantErr[4];
	INT16 *lpErrTbl[3];

	lpErrTbl[0] = new INT16[width * height * 3];
	lpErrTbl[1] = &lpErrTbl[0][width * height * 1];
	lpErrTbl[2] = &lpErrTbl[0][width * height * 2];

	::memset(lpErrTbl[0], 0, width * height * sizeof(INT16));
	::memset(lpErrTbl[1], 0, width * height * sizeof(INT16));
	::memset(lpErrTbl[2], 0, width * height * sizeof(INT16));

	for(int i = 1; i < height - 1; i ++)
	{
		for(int j = 1; j < width - 1; j ++)
		{
			int idx = width * i + j;
			INT32 newPixel = 0;
			
			idx = i * width + j;
			*(UINT32 *)pixel = lpSrc[idx];
			
			quantErr[0] = lpErrTbl[0][idx] + pixel[0];
			quantErr[1] = lpErrTbl[1][idx] + pixel[1];
			quantErr[2] = lpErrTbl[2][idx] + pixel[2];
			
			if(quantErr[0] >= threshold)
			{
				quantErr[0] -= threshold;
				newPixel |= 0x0000FF;
			}
			if(quantErr[1] >= threshold)
			{
				quantErr[1] -= threshold;
				newPixel |= 0x00FF00;
			}
			if(quantErr[2] >= threshold)
			{
				quantErr[2] -= threshold;
				newPixel |= 0xFF0000;
			}
			lpDst[idx] = newPixel;
			
			idx ++;
			lpErrTbl[0][idx] += quantErr[0] * 7 / 16;
			lpErrTbl[1][idx] += quantErr[1] * 7 / 16;
			lpErrTbl[2][idx] += quantErr[2] * 7 / 16;
			 
			idx += width;
			lpErrTbl[0][idx] += quantErr[0] * 1 / 16;
			lpErrTbl[1][idx] += quantErr[1] * 1 / 16;
			lpErrTbl[2][idx] += quantErr[2] * 1 / 16;
			 
			idx --;
			lpErrTbl[0][idx] += quantErr[0] * 5 / 16;
			lpErrTbl[1][idx] += quantErr[1] * 5 / 16;
			lpErrTbl[2][idx] += quantErr[2] * 5 / 16;
			 
			idx --;
			lpErrTbl[0][idx] += quantErr[0] * 3 / 16;
			lpErrTbl[1][idx] += quantErr[1] * 3 / 16;
			lpErrTbl[2][idx] += quantErr[2] * 3 / 16;
			
			//printf("%d, %d\n", i, j);
		}
	}
	
	delete[] lpErrTbl[0];
}

#ifdef _OPENMP
#include <omp.h>
#endif

// junk
void Imgproc::Floyd3(UINT32 *lpDst, UINT32 *lpSrc, int width, int height, UINT8 threshold)
{
	INT32 quantErr[4];
	INT32 *lpErrTbl;
	
	if(!(lpErrTbl = new INT32[width * height]))
	{	return;	}
	
	::memset(lpErrTbl, 0, width * height * sizeof(INT32));

	for(int i = 1; i < height - 1; i ++)
	{
		for(int j = 1; j < width - 1; j ++)
		{
			int idx = width * i + j;

			// Get nearest color
			((UINT8 *)&lpDst[idx])[0] = ((((UINT8 *)&lpSrc[idx])[0] + ((INT8 *)&lpErrTbl[idx])[0]) >> 7) * 0xFF;
			((UINT8 *)&lpDst[idx])[1] = ((((UINT8 *)&lpSrc[idx])[1] + ((INT8 *)&lpErrTbl[idx])[1]) >> 7) * 0xFF;
			((UINT8 *)&lpDst[idx])[2] = ((((UINT8 *)&lpSrc[idx])[2] + ((INT8 *)&lpErrTbl[idx])[2]) >> 7) * 0xFF;

			quantErr[0]	=	(int)((UINT8 *)&lpSrc[idx])[0] - (int)((UINT8 *)&lpDst[idx])[0];
			quantErr[1]	=	(int)((UINT8 *)&lpSrc[idx])[1] - (int)((UINT8 *)&lpDst[idx])[1];
			quantErr[2]	=	(int)((UINT8 *)&lpSrc[idx])[2] - (int)((UINT8 *)&lpDst[idx])[2];

			idx ++;
			((INT8 *)&lpErrTbl[idx])[0] += quantErr[0] * 7 / 16;
			((INT8 *)&lpErrTbl[idx])[1] += quantErr[1] * 7 / 16;
			((INT8 *)&lpErrTbl[idx])[2] += quantErr[2] * 7 / 16;

			idx += width;
			((INT8 *)&lpErrTbl[idx])[0] += quantErr[0] * 1 / 16;
			((INT8 *)&lpErrTbl[idx])[1] += quantErr[1] * 1 / 16;
			((INT8 *)&lpErrTbl[idx])[2] += quantErr[2] * 1 / 16;

			idx --;
			((INT8 *)&lpErrTbl[idx])[0] += quantErr[0] * 5 / 16;
			((INT8 *)&lpErrTbl[idx])[1] += quantErr[1] * 5 / 16;
			((INT8 *)&lpErrTbl[idx])[2] += quantErr[2] * 5 / 16;

			idx --;
			((INT8 *)&lpErrTbl[idx])[0] += quantErr[0] * 3 / 16;
			((INT8 *)&lpErrTbl[idx])[1] += quantErr[1] * 3 / 16;
			((INT8 *)&lpErrTbl[idx])[2] += quantErr[2] * 3 / 16;

			//printf("%d, %d\n", i, j);
		}
	}

	delete[] lpErrTbl;
}

/*
void Imgproc::Floyd(UINT32 *lpDst, UINT32 *lpSrc, int width, int height)
{
	int	xy[9];
	int i, j;
	int x, y;
	INT8 pixel[4];
	INT8 newPixel[4];
	INT8 quantErr[4];
	BYTE colorTbl[2] = {0x00, 0xFF};
	INT32 *lpErrTbl;
	
	if(!(lpErrTbl = new INT32[(width + 1) * 2]))
	{
		return;
	}
	
	::memset(lpErrTbl, 0, (width + 1) * sizeof(INT32));//width * height * sizeof(INT32));
	
	for(i = 1; i < height - 1; i ++)
	{
		for(j = 1; j < width - 1; j ++)
		{
			*(UINT32 *)pixel = lpSrc[i * width + j];
			
			quantErr[0] = *((INT8 *)lpErrTbl + (0 * width + j + 0)) + pixel[0];
			quantErr[1] = *((INT8 *)lpErrTbl + (0 * width + j + 1)) + pixel[1];
			quantErr[2] = *((INT8 *)lpErrTbl + (0 * width + j + 2)) + pixel[2];
			
			if(quantErr)
				*(UINT32 *)pixel = lpSrc[i * width + (j + 1)];
			pixel[0] += quantErr[0] * 7 / 16;
			pixel[1] += quantErr[1] * 7 / 16;
			pixel[2] += quantErr[2] * 7 / 16;
			lpSrc[i * width + (j + 1)] = *(UINT32 *)pixel;
			
			*(UINT32 *)pixel = lpSrc[(i + 1) * width + (j + 1)];
			pixel[0] += quantErr[0] * 1 / 16;
			pixel[1] += quantErr[1] * 1 / 16;
			pixel[2] += quantErr[2] * 1 / 16;
			lpSrc[(i + 1) * width + (j + 1)] = *(UINT32 *)pixel;
			
			*(UINT32 *)pixel = lpSrc[(i + 1) * width + j];
			pixel[0] += quantErr[0] * 5 / 16;
			pixel[1] += quantErr[1] * 5 / 16;
			pixel[2] += quantErr[2] * 5 / 16;
			lpSrc[(i + 1) * width + j] = *(UINT32 *)pixel;
			
			*(UINT32 *)pixel = lpSrc[(i + 1) * width + (j - 1)];
			pixel[0] += quantErr[0] * 3 / 16;
			pixel[1] += quantErr[1] * 3 / 16;
			pixel[2] += quantErr[2] * 3 / 16;
			lpSrc[(i + 1) * width + (j - 1)] = *(UINT32 *)pixel;
		}
	}
	
	delete[] lpErrTbl;
}
*/