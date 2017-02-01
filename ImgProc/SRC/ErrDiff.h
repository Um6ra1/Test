/*================================================================
 * Error diffusion
 *----------------------------------------------------------------
 * 
 *
 * ErrDiff.h
 *
 * Copyright (c) 2012 Um6ra1
 *
 *================================================================*/

#pragma once

#include "Typedefs.h"

namespace Imgproc
{
	void	RightDiff1(UINT32 *lpDst, UINT32 *lpSrc, int width, int height, UINT8 threshold);
	
	void Floyd1(UINT32 *lpDst, UINT32 *lpSrc, int width, int height, UINT8 threshold);
	void Floyd2(UINT32 *lpDst, UINT32 *lpSrc, int width, int height, UINT8 threshold);
	void Floyd3(UINT32 *lpDst, UINT32 *lpSrc, int width, int height, UINT8 threshold);
	void CuRightDiff(UINT32 *lpDst, UINT32 *lpSrc, int width, int height, UINT8 threshold);
}
