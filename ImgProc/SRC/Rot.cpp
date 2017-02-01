/**
 * @file		Rot.cpp
 * @brief	Rotate image
 * @author	Um6r41
 * @date 2017/1/31
 */
 
#include "Rot.h"
#include <cmath>
#include <cstdio>

void ImgProc::Rot(Image &img, Point &center, double angle) {
	int	icos = (int)(::cos(angle) * 1024.0);
	int	isin = (int)(::sin(angle) * 1024.0);
	Size scaled = {
		(int)(::fabs(img.width * icos) + ::fabs(img.height * isin) + 0.5 * 1024.0) >> 10,
		(int)(::fabs(img.width * isin) + ::fabs(img.height * icos) + 0.5 * 1024.0) >> 10
	};
	std::vector<RGBA>	buf(scaled.width * scaled.height);
	
	Point center2 = {
		center.x * icos - center.y * isin,
		center.x * isin + center.y * icos
	};

	REP(y, scaled.height) REP(x, scaled.width) {
		int dx = (x << 10) - center2.x, dy = (y << 10) - center2.y;
		//int xx	= ((int)(x * icos + y * isin) >> 10) + center.x;
		//int yy	= ((int)(-x * isin + y * icos) >> 10) + center.y;
		int xx	= ((int)(dx * icos + dy * isin) >> 10) + center.x;
		int yy	= ((int)(-dx * isin + dy * icos) >> 10) + center.y;

		if(IN_LCRO(xx, 0, img.width) && IN_LCRO(yy, 0, img.height))
			buf[scaled.width * y + x] = img.buf[img.width * yy + xx];
	}
	img.width = scaled.width;
	img.height = scaled.height;
	img.SwapBuf(buf);
}
