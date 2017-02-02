#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>
#include <string>

#include "BmpUtil.h"

#include "FFTImage.h"
#include "Effect3D.h"
#include "Laplacian.h"
#include "GaussBlur.h"
#include "ConvImage.h"
#include "Rot.h"
#include "Posterization.h"

int main(int argc, char *argv[]) {
	std::string	saveFileName("a.bmp");
	CBitmap bmp1, bmp2, kernel;

	if(argc < 1) {
		::printf("a.exe <src> <dst>\n");
		return(0);
	}

	if(!bmp1.Load(argv[1])) {
		::printf("failed\n");
		return(1);	
	}
	
	bmp2	= bmp1;
	
	int param;
	std::string	kernelImageFileName;

	::printf("Param:\n");
	std::cin >> param;
	::printf("KernelImage:\n");
	std::cin >> kernelImageFileName;

	if (!kernel.Load(kernelImageFileName.c_str()))	::printf("Failed to load Kernel.\n");
	
	::puts("Start");
	if(param == 9979)	ImgProc::FFTMagnitudeImage(bmp2.Image());
	else {
		ImgProc::Point center = {bmp2.Image().width / 2, bmp2.Image().height / 2};
		//ImgProc::LensBlur(bmp2.Buff(), bmp1.Buff(), bmp1.Width(), bmp1.Height(), 0);
		//ImgProc::GaussBlur(bmp2.Image(), param);
		//ImgProc::Laplacian(bmp2.Image());
		//ImgProc::Posterization(bmp2.Image(), param);
		//ImgProc::Effect3D(bmp2.Image(), param);
		//ImgProc::ConvImage(&bmp2.Image(), &bmp1.Image(), &kernel.Image());
		ImgProc::FFTConvImage(bmp2.Image(), kernel.Image());
		//ImgProc::Rot(bmp2.Image(), center, 3.14 / 3);
	}
	::puts("Done");
	bmp1.Save("a_org.bmp");
	bmp2.Save(saveFileName.c_str());
	return(0);
}
