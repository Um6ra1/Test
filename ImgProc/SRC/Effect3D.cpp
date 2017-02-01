/**
 * @file		Effect3D.cpp
 * @brief	Apply misregistration effect 
 * @author	Um6r41
 * @date 2017/1/31
 */
 
#include "Effect3D.h"

#define DRAW_MUL(x, y)	(BYTE)((double)((x) * (y)) / 255.0)
#define DRAW_SCREEN(x, y)	(BYTE)(255.0 - (double)((255 - (x)) * (255 - (y))) / 255.0)

void	ImgProc::Effect3D(Image &img, int displacement) {
	std::vector<RGBA>	buf(img.buf.size());
	RGBA white = {0xFFFFFFFF};
	
	REP(y, img.height) REP(x, img.width) {
		RGBA	pixel		= img.buf[img.width * y + x];
		RGBA	red		= (x > displacement) ? img.buf[img.width * y + (x - displacement)] : white;
		RGBA	cyan	= (x < img.width - displacement) ? img.buf[img.width * y + (x + displacement)]: white;

		pixel.r	= DRAW_SCREEN(pixel.r, red.r);
		pixel.g	= DRAW_SCREEN(pixel.g, cyan.g);
		pixel.b	= DRAW_SCREEN(pixel.b, cyan.b);

		buf[img.width * y + x]	= pixel;
	}
	img.SwapBuf(buf);
}
