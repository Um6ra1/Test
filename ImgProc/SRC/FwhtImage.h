/**
 * @file		FwhtImage.h
 * @brief	Fast Walsh hadamard transform for image processing
 * @author	Um6r41
 * @date 2018/3/05
 */
 
#pragma once

#include "Typedefs.h"

namespace ImgProc {
	void	FwhtMagnitudeImage(Image &img);
	void	FwhtConvImage(Image &img, Image &ker);
}
