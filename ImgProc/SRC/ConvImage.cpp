/**
 * @file		ConvImage.cpp
 * @brief	Convolution
 * @author	Um6r41
 * @date 2017/1/31
 */

#include "ConvImage.h"
#include <vector>

void	ImgProc::ConvImage(Image *pDst, Image *pSrc, Image *pKernel) {
	int kh = pKernel->height;
	int kw = pKernel->width;
	int weights[CHANNELS_RGB] = {};
	
	REP(c, CHANNELS_RGB) REP(i, kh * kw)	weights[c] += pKernel->buf[i].c[c];
	
	REP(y, pSrc->height) REP(x, pSrc->width) {
		int channels[CHANNELS_RGB] = {};

		REP(yy, kh) REP(xx, kw) {
			ImgProc::Point	pt = { CLIP(x + xx - kw/2, 0, pSrc->width - 1), CLIP(y + yy - kh/2, 0, pSrc->height - 1)	};
			RGBA	pixel = pSrc->buf[pSrc->width * pt.y + pt.x];
			REP(c, CHANNELS_RGB)	channels[c] += pixel.c[c] * pKernel->buf[kw * yy + xx].c[c];
		}

		RGBA	rgba = {};
		REP(c, CHANNELS_RGB)	rgba.c[c]	= channels[c] / weights[c];
		pDst->buf[pDst->width * y + x]	= rgba;
	}
}
