/**
 * @file		FFTImage.h
 * @brief	FFT for image processing
 * @author	Um6r41
 * @date 2017/1/31
 */
 
#pragma once

#include "Typedefs.h"

namespace ImgProc {
	void	FFTMagnitudeImage(Image &img);
	void	FFTPhaseImage(Image &img);
	void	FFTConvImage(Image &img, Image &ker);
}
