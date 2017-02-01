/**
 * @file		BmpUtil.cpp
 * @brief Bitmap utility
 * @author	Um6r41
 * @date 2017/2/1
 */
 
#include <cstring>
#include <cstdio>
#include "BmpUtil.h"

int CBitmap::Load(LPCSTR pszFileName) {
	FILE	*hFile = ::fopen(pszFileName, "rb");
	if(!hFile) { return(false); }
	
	::fread(&m_bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, hFile);
	::fread(&m_bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, hFile);

	UINT32	bytesPerPixel	= m_bmpInfoHeader.biBitCount >> 3;
	if(bytesPerPixel == 0)	bytesPerPixel = 1;
	UINT32	pixelMask	= 0xFFFFFFFF >> (32 - m_bmpInfoHeader.biBitCount);
	int	imgWidth	= m_bmpInfoHeader.biWidth;
	m_lineBytes		= (imgWidth + 1) * bytesPerPixel >> 2 << 2;	// Multiple of 4

	int imgHeight	= (m_bmpInfoHeader.biHeight < 0) ? -m_bmpInfoHeader.biHeight : m_bmpInfoHeader.biHeight;
	m_image.SetSize(imgWidth, imgHeight);
	UINT32	*pBuf = (UINT32 *)&m_image.buf[0];

	switch(m_bmpInfoHeader.biBitCount) {
		case 1:	break;	// Unimplemented
		case 4:	break;	// Unimplemented
		case 8: {
			RGBQUAD	palettes[0x100];
			BYTE	*lpPixel	= (BYTE *)pBuf;
			
			if(m_bmpInfoHeader.biHeight < 0) {	lpPixel += m_lineBytes * imgHeight * 3;	}

			::fseek(hFile, sizeof(BITMAPFILEHEADER) + m_bmpInfoHeader.biSize, SEEK_SET);
			::fread(palettes, sizeof(RGBQUAD) * 0x100, 1, hFile);
			
			::fseek(hFile, m_bmpFileHeader.bfOffBits, SEEK_SET);
			::fread(lpPixel, m_lineBytes * imgHeight * bytesPerPixel, 1, hFile);
			m_bmpInfoHeader.biBitCount	= 24;
			m_bmpFileHeader.bfOffBits	= sizeof(BITMAPFILEHEADER) + m_bmpInfoHeader.biSize;

			if(m_bmpInfoHeader.biHeight < 0) // start writing upper to lower
				REP(y, imgHeight)	REV(x, imgWidth)
					pBuf[y * imgWidth + x] = ((UINT32 *)palettes)[ lpPixel[y * m_lineBytes + x] ];
			else // start writing lower to upper 
				REV(y, imgHeight)	REV(x, imgWidth)
					pBuf[y * imgWidth + x] = ((UINT32 *)palettes)[ lpPixel[y * m_lineBytes + x] ];
			
			m_lineBytes	*= 3;
			m_bmpFileHeader.bfSize	= m_bmpFileHeader.bfOffBits + (m_lineBytes * imgHeight + imgWidth);
		}
		break;
		
		case 16:	break;
		case 24: {
			std::vector<BYTE>	bitmap( (m_lineBytes * imgHeight + 1) * bytesPerPixel);
			::fseek(hFile, m_bmpFileHeader.bfOffBits, SEEK_SET);
			::fread(&bitmap[0], m_lineBytes * imgHeight * bytesPerPixel, 1, hFile);

			if(m_bmpInfoHeader.biHeight < 0) // upper to lower
				REP(y, imgHeight)	REV(x, imgWidth)	pBuf[y * imgWidth + x]	= pixelMask & *(UINT32 *)&bitmap[y * m_lineBytes + x * bytesPerPixel];
			else // lower to upper 
				REV(y, imgHeight)	REV(x, imgWidth)	pBuf[y * imgWidth + x] = pixelMask & *(UINT32 *)&bitmap[y * m_lineBytes + x * bytesPerPixel];
		}
		break;
		
		case 32:	break;	// Unimplemented
	}

	::fclose(hFile);
	return(true);
}

int CBitmap::Save(LPCSTR pszFileName) {
	FILE	*hFile = ::fopen(pszFileName, "wb");
	if(!hFile)	{ return(false); }
	
	SetSize(m_image.width, m_image.height);
	::fwrite(&m_bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, hFile);
	::fwrite(&m_bmpInfoHeader, m_bmpInfoHeader.biSize, 1, hFile);
    
	UINT32	bytesPerPixel	= m_bmpInfoHeader.biBitCount >> 3;	// 1, 4 bit is not supported!
	UINT32	lineBytes	= m_lineBytes;
	UINT32	pixelMask	= 0xFFFFFFFF >> (32 - m_bmpInfoHeader.biBitCount);
	int	imgWidth	= m_image.width;
	int	imgHeight	= m_image.height;
	UINT32	*pBuf = (UINT32 *)&m_image.buf[0];
	
	std::vector<BYTE>	imgData(lineBytes * imgHeight + 4);

	if(m_bmpInfoHeader.biHeight < 0) // Top to bottom
		REP(y, imgHeight)	REP(x, imgWidth)
			*(UINT32 *)(&imgData[(y * imgWidth + x) * bytesPerPixel])	= pixelMask & pBuf[y * imgWidth + x];
	else // Bottom to top
		REV(y, imgHeight)	REP(x, imgWidth)
			*(UINT32 *)(&imgData[(y * imgWidth + x) * bytesPerPixel])	= pixelMask & pBuf[y * imgWidth + x];
	
	::fwrite(&imgData[0], lineBytes * imgHeight, 1, hFile);
	::fclose(hFile);
	
	return(true);
}

void CBitmap::Delete() {
	{	std::vector<RGBA>	arr;	m_image.buf.swap(arr);}
	{	std::vector<RGBQUAD>	arr;	m_palettes.swap(arr);	}
}

int CBitmap::Reserve(int width, int height) {
	Delete();
	
	m_image.SetSize(width, height);
	m_lineBytes	= (width + 1) * 3 >> 2 << 2;//((width + 1) * 3) & ~0x03;
	
	m_bmpFileHeader.bfType	= 0x424D;
	m_bmpFileHeader.bfSize	= sizeof(BITMAPFILEHEADER) + 40 + (m_lineBytes * height + width);//sizeof(BITMAPFILEHEADER) + 40 + (height * m_lineBytes);
	m_bmpFileHeader.bfReserved1	= 0;
	m_bmpFileHeader.bfReserved2	= 0;
	m_bmpFileHeader.bfOffBits	= sizeof(BITMAPFILEHEADER) + 40;
	::printf("%d\n", m_bmpFileHeader.bfOffBits);
	
	m_bmpInfoHeader.biSize	= 40;
	m_bmpInfoHeader.biWidth	= width;
	m_bmpInfoHeader.biHeight	= height;
	m_bmpInfoHeader.biPlanes	= 1;
	m_bmpInfoHeader.biBitCount	= 24;
	m_bmpInfoHeader.biCompression	= 0;
	m_bmpInfoHeader.biSizeImage	= 0;//(m_lineBytes * height + width);
	m_bmpInfoHeader.biXPelsPerMeter	= 0;
	m_bmpInfoHeader.biYPelsPerMeter	= 0;
	m_bmpInfoHeader.biClrUsed	= 0;
	m_bmpInfoHeader.biClrImportant	= 0;
	
	return(true);
}

void CBitmap::SetSize(int width, int height) {
	int bytesPerPixel	= m_bmpInfoHeader.biBitCount >> 3;
	
	m_image.SetSize(width, height);
	m_bmpInfoHeader.biWidth	= width;
	m_bmpInfoHeader.biHeight	= height;
	m_lineBytes	= (width + 1) * bytesPerPixel >> 2 << 2;	// Multiple of 4
}

CBitmap& CBitmap::operator =(const CBitmap &o) {
	Delete();

	m_bmpFileHeader	= o.m_bmpFileHeader;
	m_bmpInfoHeader	= o.m_bmpInfoHeader;
	m_lineBytes	= o.m_lineBytes;
	m_image = o.m_image;
    
	return(*this);
}
