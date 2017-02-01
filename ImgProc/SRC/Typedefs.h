/**
 * @file		Typedefs.h
 * @brief	Define types nad macros
 * @author	Um6r41
 * @date 2017/1/31
 */

#pragma once

#define LOOP(max)	for(int i##__LINE__  = 0; i##__LINE__ < max; i##__LINE__ ++)	// DO NOT USE IN THE SAME LINE!!

#define REP(i, max)	for(int i = 0, i##Max = max; i < i##Max; i ++)
#define RREP(i, min, max)	for(int i = min, i##Max = max; i < i##Max; i ++)
#define EREP(i, min, max)	for(int i = min, i##Max = max; i <= i##Max; i ++)
#define REV(i, max)	for(int i = (max) - 1; i >= 0; i --)
#define RREV(i, min, max)	for(int i = max, i##Min = min; i > i##Min; i --)
#define EREV(i, min, max)	for(int i = (max) - 1, i##Min = min; i >= i##Min; i --)

#define EXTBYTE(x, bit)	(((x) >> bit) & 0xFF)
#define BYTES2RGB(r, g, b)	( ((BYTE)(b)) | (((BYTE)(g)) << 8) | (((BYTE)(r)) << 16) )
#define BYTE_OF_DWORD(dw, byte)	(((BYTE *)&(dw))[byte])

#define CLIP(x, minX, maxX)	min(max(x, minX), maxX)
#define CHANNELS_RGB	3
#define IN_LCRO(x, min, max)	(((min) <= (x)) && ((x) < (max)))	/* x in [min, max) */

#include <vector>

#ifndef _MSC_VER

#include <inttypes.h>

typedef __INT8_TYPE__	INT8;
typedef __INT16_TYPE__	INT16;
typedef __INT32_TYPE__	INT32;
typedef __INT64_TYPE__	INT64;

/*
typedef int8_t	INT8;
typedef int16_t	INT16;
typedef int32_t	INT32;
typedef int64_t	INT64;
*/

typedef unsigned __INT8_TYPE__	UINT8;
typedef unsigned __INT16_TYPE__	UINT16;
typedef unsigned __INT32_TYPE__	UINT32;
typedef unsigned __INT64_TYPE__	UINT64;

/*
typedef uint8_t		UINT8;
typedef uint16_t	UINT16;
typedef uint32_t	UINT32;
typedef uint64_t	UINT64;
*/

typedef UINT8	BYTE;
typedef UINT16	WORD;
typedef UINT32	DWORD;
typedef UINT64	QWORD;

#ifdef UNICODE
typedef wchar_t	TCHAR;
#else
typedef char	TCHAR;
#endif

typedef void        *LPVOID;
typedef TCHAR		*LPSTR;
typedef const TCHAR	*LPCSTR;
typedef unsigned int	UINT;
typedef INT32	LONG;

typedef LPVOID  HANDLE;
typedef long	FXPT2DOT30;

#else
#include <Windows.h>
#endif	// _MSC_VER

typedef union _RGBA {
	BYTE c[4];
	struct {	BYTE b, g, r, a;	};
	UINT32 dw;
} RGBA;

typedef struct _DIM2 {	INT32	x, y;	} DIM2, *LPDIM2;

typedef struct _DIM3 {
	INT32	x, y, z;
} DIM3, *LPDIM3;

namespace ImgProc {
	struct Point {	int	x, y;	};
	struct Size {	int	width, height;	};
	
	struct	Image {
		std::vector<RGBA>	buf;
		int	width, height;

		inline void	SetSize(int width_, int height_)
		{	buf.resize( (width = width_) * (height = height_) );	}
		inline void SwapBuf(std::vector<RGBA> &buf2)
		{	buf.swap(buf2);	}

		inline Image()	{}
		inline Image(int width_, int height_)	{ SetSize(width_, height_); }
	};
}

