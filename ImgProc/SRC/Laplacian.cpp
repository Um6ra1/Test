/**
 * @file		Laplacian.cpp
 * @brief	Apply laplacian filter
 * @author	Um6r41
 * @date 2017/1/31
 */
 
#include "Laplacian.h"
#include <cstdio>

void ImgProc::Laplacian(Image &img) {
	int kernel[3][3] = {
		{0, 1, 0},
		{1, -4, 1},
		{0, 1, 0},
	};
	std::vector<RGBA>	buf(img.buf.size());

	REP(y, img.height) REP(x, img.width) {
	//	printf("(%p: %d, %d)\n", buf.size(), y, x);
		int channels[CHANNELS_RGB] = {};
		EREP(yy, -1, +1) EREP(xx, -1, +1) {
			ImgProc::Point pt = { CLIP(x + xx, 0, img.width - 1), CLIP(y + yy, 0, img.height - 1)	};
			REP(c, CHANNELS_RGB) channels[c] += kernel[yy + 1][xx + 1] * img.buf[img.width * pt.y + pt.x].c[c];
		}
		REP(c, CHANNELS_RGB) buf[img.width * y + x].c[c] = (u8)channels[c];
	}
	
	img.SwapBuf(buf);
}
