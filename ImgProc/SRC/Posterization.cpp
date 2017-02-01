/**
 * @file		Posterization.cpp
 * @brief	Apply Posterization filter
 * @author	Um6r41
 * @date 2017/1/31
 */
 
#include "Posterization.h"

void ImgProc::Posterization(Image &img, UINT8 step) {
	UINT8	colors[0x100];
	
	REP(i, step)	colors[i]	= 255 * i / (step - 1);
	
	REP(y, img.height) REP(x, img.width) REP(c, CHANNELS_RGB)
		img.buf[img.width * y + x].c[c] = colors[(img.buf[img.width * y + x].c[c] * step) >> 8];
}
