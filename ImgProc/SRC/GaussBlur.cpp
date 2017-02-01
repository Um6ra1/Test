/**
 * @file		GaussBlur.h
 * @brief	Apply gaussian blur
 * @author	Um6r41
 * @date 2017/1/31
 */
 
 #include "GaussBlur.h"
#include <cmath>

// SIMD
#include <mmintrin.h>	// mmx
#include <xmmintrin.h>	// sse (Pentium!!! or later)
#include <emmintrin.h>	// sse2 (Pentium4 or later)
#include <pmmintrin.h>	// sse3 (Pentium4 or later)

void ImgProc::GaussBlur(Image &img, double r) {
	int sr = ::ceil(2.57 * r);
	std::vector<double>	kernel(sr + 2);
	std::vector<RGBA>	buf(img.buf.size());
	double	totalWeight = 0;

	EREP(i, 0, sr)	totalWeight += kernel[i] = ::exp(-(double)(i * i) / (2.0 * r * r));
	totalWeight = ::pow(2 * totalWeight - 1, 2);

	REP(y, img.height) REP(x, img.width) {
		double	channels[CHANNELS_RGB] = {};
		EREP(yy, -sr, +sr) EREP(xx, -sr, +sr) {
			Point	pt = { CLIP(x + xx, 0, img.width - 1), CLIP(y + yy, 0, img.height - 1)	};
			double	weight = kernel[::abs(xx)] * kernel[::abs(yy)];
			REP(c, CHANNELS_RGB)	channels[c] += weight * (double)img.buf[img.width * pt.y + pt.x].c[c];
		}
		REP(c, CHANNELS_RGB)	buf[img.width * y + x].c[c] = channels[c] / totalWeight;
	}
	img.SwapBuf(buf);
}
