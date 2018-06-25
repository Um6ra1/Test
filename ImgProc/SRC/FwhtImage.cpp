/**
 * @file		FwhtImage.cpp
 * @brief	Fast Walsh hadamard transform for image processing
 * @author	Um6r41
 * @date 2018/3/05
 */
 
#include "FwhtImage.h"
#include <cmath>
#include <cstring>

#include <cstdio>
#include <vector>
#include <algorithm>

using namespace ImgProc;

typedef enum {
	TdForward, TdBackward
} TransDir;

void Fwht1D(float *dst, int log2n, TransDir dir) {
	int n = 1 << log2n;
	
	// Bit reversal
	for(int i = 0, j = 0; i < n - 1; i ++) {
		if(i < j)	std::swap(dst[i], dst[j]);
		int k;
		for(k = n >> 1; k <= j; k >>= 1) j -= k;
		j += k;
	}

	// FWHT
	int l2 = 1;
	LOOP(log2n) {
		int l1 = l2;
		l2	<<= 1;
		REP(i, l1) for(int j = i; j < n; j += l2) {
			float a = dst[j];
			float b = dst[j + l1];
			dst[j] = a + b;
			dst[j + l1] = a - b;
		}
	}
	
	if (dir == TdBackward) REP(i, n) dst[i] /= (float)n;
}

void Fwht2D(float *dst, int width, int height, TransDir dir) {
	std::vector<float>	t1(width + height);

	/*float x[] = {19, -1, 11, -9, -7, 13, -15, 5};
	Fwht1D(x, 2, TdBackward);
	REP(i, 4) printf("%1.1f, ", x[i]);
	puts("");
	*/
	
	// Row FFT
	int log2n = (int)log2(width);
	REP(y, height) Fwht1D(&dst[width * y], log2n, dir);

	// Column FFT
	log2n = (int)log2(height);
	REP(x, width) {
		REP(y, height)	t1[y] = dst[width * y + x];
		Fwht1D(&t1[0], log2n, dir);
		REP(y, height)	dst[width * y + x] = t1[y];
	}
}

struct FloatImage {
	int width, height;
	int dx, dy;
	std::vector<float> buf;

	void	SetImage(Image &src, int channel, float scale = 1.0) {
		dx = (width - src.width) / 2, dy = (height - src.height) / 2;
		REP(y, src.height) REP(x, src.width)
			buf[width * (y+dy) + x+dx] = src.buf[src.width * y + x].c[channel] * scale;
	}
	void	GetImage(Image &dst, int channel) {
		float m = std::max( *std::max_element(ALL(buf)), -*std::min_element(ALL(buf)) );
		REP(y, dst.height) REP(x, dst.width)
			dst.buf[dst.width * y + x].c[channel] = (u8)abs(buf[width * (y+dy) + x+dx] * 255 / m);
	}
	void	GetImageMagnitude(Image &dst, int channel) {
		std::vector<double>	mag(dst.height * dst.width);
		double max	= 0.0;
		REP(y, dst.height) REP(x, dst.width) {
			double norm2	= abs( buf[width * (y+dy) + x+dx] );
			double m = mag[dst.width * y + x] = (norm2 >= 1.0) ? ::log10(norm2) : 0;
			if(max < m)	max = m;
		}
		if(max < 1e-10)	max = 1.0;
		REP(y, dst.height) REP(x, dst.width) dst.buf[dst.width * y + x].c[channel] = NORM255(mag[dst.width * y + x], max);
	}
	
	void Padding(Image &src) {
		RREP(y, dy, src.height + dy) {
			REP(x, dx) buf[width * y + x] = buf[width * y + (0 + dx)];
			RREP(x, src.width + dx, width)	buf[width * y + x] = buf[width * y + (src.width - 1 + dx)];
		}
		REP(x, width) {
			REP(y, dy) buf[width * y + x] = buf[width * (0 + dy) + x];
			RREP(y, src.height + dy, height) buf[width * y + x] = buf[width * (src.height - 1 + dy) + x];
		}
	}
	
	void	SetImageLog(Image &src, int channel, double factor) {
		dx = (width - src.width) / 2, dy = (height - src.height) / 2;
		REP(y, src.height) REP(x, src.width)
			buf[width * (y+dy) + x+dx] = exp(src.buf[src.width * y + x].c[channel] * factor / 255.0);
		Padding(src);
	}
	void	GetImageLog(Image &dst, int channel, int factor) {
		REP(y, dst.height) REP(x, dst.width)
			dst.buf[dst.width * y + x].c[channel] = (u8)(log(buf[width * (y+dy) + x+dx]) * 255.0 / factor);
	}
	
	void	Zero() { memset(&buf[0], 0, buf.size() * sizeof(buf[0])); }
	void	FFT(TransDir dir) {	Fwht2D(&buf[0], width, height, dir); }
	void	FFTShift() {
		REV(y, height / 2) REV(x, width / 2) {
			int	idx = width * y + x;
			std::swap(buf[idx], buf[idx + (height + 1) * width / 2]);
			std::swap(buf[idx + width / 2], buf[idx + height * width / 2]);
		}
	}
	
	FloatImage &operator *=(FloatImage &o) {
		REP(y, height) REP(x, width) buf[width * y + x] *= o.buf[width * y + x];
		return *this;
	}
	
	FloatImage(int width_, int height_) : width(width_), height(height_) {
		buf.resize(width_ * height_);
	}
};

void ImgProc::FwhtMagnitudeImage(Image &img) {
	FloatImage	ciImg(NEXT_POW2(img.width), NEXT_POW2(img.height));
	REP(c, 3) {
		ciImg.Zero();
		ciImg.SetImageLog(img, c, 1);
		ciImg.FFT(TdForward);
		//ciImg.FFT(TdBackward);
		ciImg.GetImageMagnitude(img, c);
		//ciImg.GetImage(img, c);
	}
}

void	ImgProc::FwhtConvImage(Image &img, Image &ker) {
	FloatImage	ciImg(NEXT_POW2(img.width), NEXT_POW2(img.height));
	FloatImage	ciKer(NEXT_POW2(img.width), NEXT_POW2(img.height));
	double factor	= 1;

	REP(c, 3) {
		u32 kerSum = 0;
		
		REP(i, ker.height * ker.width)	kerSum += ker.buf[i].c[c];
		ciImg.Zero();
		ciImg.SetImageLog(img, c, factor);
		ciKer.Zero();
		ciKer.SetImage(ker, c, 1 / (double)kerSum);
		//ciKer.FFTShift();
		ciImg.FFT(TdForward);
		ciKer.FFT(TdForward);
		ciImg *= ciKer;
		ciImg.FFT(TdBackward);
		ciImg.GetImageLog(img, c, factor);
	}
}
