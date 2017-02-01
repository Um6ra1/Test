/**
 * @file		BmpUtil.h
 * @brief Bitmap utility
 * @author	Um6r41
 * @date 2017/2/1
 */
 
#pragma once

#include <vector>
#include "Typedefs.h"

#ifndef _MSC_VER

typedef struct tagCIEXYZ {
	FXPT2DOT30	ciexyzX;
	FXPT2DOT30	ciexyzY;
	FXPT2DOT30	ciexyzZ;
} CIEXYZ;

typedef struct _CIEXYZTRIPLE {
	CIEXYZ	ciexyzRed;
	CIEXYZ	ciexyzGreen;
	CIEXYZ	ciexyzBlue;
} CIEXYZTRIPLE, *LPCIEXYZTRIPLE;

typedef struct __attribute__((packed)) tagBITMAPFILEHEADER {
    WORD    bfType;		// =="BM"==0x424D
    DWORD   bfSize;		// entire size of this file[bytes]
    WORD    bfReserved1;	// ==0
    WORD    bfReserved2;	// ==0
    DWORD   bfOffBits;		// Offset of pixel bit
} BITMAPFILEHEADER;

typedef union __attribute__((packed)) tagBITMAPINFOHEADER {
	// OS/2
	struct {
		DWORD	bcSize;
		INT16		bcWidth;
		INT16		bcHeight;
		WORD	bcPlanes;
		WORD	bcBitCount;
	};

	// Windows
	struct {
		DWORD	biSize;		// Structure size(40:windows, 12:OS/2)
		INT32	biWidth;		// Image width
		INT32	biHeight;		// Image height
		WORD	biPlanes;		// ==1(Windows初期の名残）
		WORD	biBitCount;	// Color bit count (1, 4, 8, 16, 24, 32)
		DWORD	biCompression;	// Compression code (BI_RGB, BI_RLE4, BI_RLE8, BI_BITFIELDSのいずれか)
		DWORD	biSizeImage;		// Bytes of image
		INT32	biXPelsPerMeter;	// Horizontal resolution
		INT32	biYPelsPerMeter;	// Vertical resolution
		DWORD	biClrUsed;			// Color table count
		DWORD	biClrImportant;		// Important color count
	};

	// Ver 4(BITMAPV4HEADER)
	struct {
		DWORD	bV4Size;
		INT32	bV4Width;
		INT32	bV4Height;	
		WORD	bV4Planes;
		WORD	bV4BitCount;
		DWORD  bV4Compression;	
		DWORD  bV4SizeImage;
		INT32   bV4XPelsPerMeter;
		INT32   bV4YPelsPerMeter;
		DWORD  bV4ClrUsed;
		DWORD  bV4ClrImportant;

		DWORD	bV4RedMask;	// Masks
		DWORD	bV4GreenMask;
		DWORD	bV4BlueMask;
		DWORD	bV4AlphaMask;
		
		DWORD	bV4CSType;	// Color domain type
		CIEXYZTRIPLE	bV4Endpoints;	// CIEXYZ(bV4CSType == 0)
		
		DWORD	bV4GammaRed;	// Gamma values
		DWORD	bV4GammaGreen;
		DWORD	bV4GammaBlue;
	};

	// Ver 5(BITMAPV5HEADER)
	struct {
		DWORD	bV5Size;
		INT32	bV5Width;
		INT32	bV5Height;	
		WORD	bV5Planes;
		WORD	bV5BitCount;
		DWORD  bV5Compression;	
		DWORD  bV5SizeImage;
		INT32	bV5XPelsPerMeter;
		INT32 	bV5YPelsPerMeter;
		DWORD  bV5ClrUsed;
		DWORD  bV5ClrImportant;

		DWORD	bV5RedMask;	// Masks
		DWORD	bV5GreenMask;
		DWORD	bV5BlueMask;
		DWORD	bV5AlphaMask;
		
		DWORD	bV5CSType;	// Color domain type
		CIEXYZTRIPLE	bV5Endpoints;	// CIEXYZ(bV4CSType == 0)
		
		DWORD	bV5GammaRed;	// Gamma values
		DWORD	bV5GammaGreen;
		DWORD	bV5GammaBlue;

		DWORD	bV5Intent;			// sRGB color domain type
		DWORD	bV5ProfileData;	// Color domain profile offset
		DWORD	bV5ProfileSize;	// Color domain profile size
		DWORD	bV5Reserved;
	};
} BITMAPINFOHEADER;

/*================
	Color palette
 ================*/
typedef struct _RGBTRIPLE {
	BYTE	rgbtBlue;
	BYTE	rgbtGreen;
	BYTE	rgbtRed;
} RGBTRIPLE;

typedef struct _RGBQUAD {
	BYTE	rgbBlue;
	BYTE	rgbGreen;
	BYTE	rgbRed;
	BYTE	rgbReserved;
} RGBQUAD;

#endif	// _MSC_VER

class CBitmap {
	BITMAPFILEHEADER	m_bmpFileHeader;
	BITMAPINFOHEADER	m_bmpInfoHeader;
	int	m_lineBytes;
	std::vector<RGBQUAD>	m_palettes;
	ImgProc::Image	m_image;

public:
	int	Load(LPCSTR lpszFileName);
	int	Save(LPCSTR lpszFileName);
	void	Delete();
	int	Reserve(int width, int height);
	void	SetSize(int width, int height);
	
	ImgProc::Image	&Image()	{	return m_image;	}
	RGBA	*Buff()	{	return &m_image.buf[0];	}
	UINT	BuffSize()	{	return m_image.width * m_image.height * 4;	}
	int		Width()		{	return m_image.width;	}
	int		Height()		{	return m_image.height;	}

	CBitmap& operator =(const CBitmap &o);
	CBitmap()	{}
	~CBitmap()	{}
};
