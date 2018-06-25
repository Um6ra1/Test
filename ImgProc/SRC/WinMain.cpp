#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>
#include <string>

#include "BmpUtil.h"

#include "FFTImage.h"
#include "FwhtImage.h"
#include "Effect3D.h"
#include "Laplacian.h"
#include "GaussBlur.h"
#include "ConvImage.h"

#define NUM	256

int main(int argc, char *argv[]) {
	int ret;
	std::string	saveFileName("a.bmp");
	CBitmap bmp1, bmp2, kernel;

	if(argc <= 1) {
		printf("a.exe <src> <dst>\n");
		return 0;
	}

	if(bmp1.Load(argv[1]) < 0) {
		printf("failed\n");
		return 1;	
	}
	
	//printf("Saving original...");
	//bmp1.Save("a_org.bmp");
	//printf("Done\n");
	//if(bmp2.GetClipboard() < 0)  fprintf(stderr, "No BMP\n");
	bmp2 = bmp1;
	bmp2.Save("black.bmp");
	//return 0;
	//bmp2	= bmp1;
	
	int param;
	std::string kernelImageFileName(argc > 2 ? argv[2] : "../fc.bmp");
/*
	::printf("Param:\n");
	std::cin >> param;
	::printf("KernelImage:\n");
	std::cin >> kernelImageFileName;
*/
	if (kernel.Load(kernelImageFileName.c_str()) < 0)	::printf("Failed to load Kernel.\n");
	
	::puts("Start");
	if(param == 9979)	ImgProc::FFTMagnitudeImage(bmp2.Image());
	else {
		//	ImgProc::LensBlur(bmp2.Buff(), bmp1.Buff(), bmp1.Width(), bmp1.Height(), 0);
		//	ImgProc::GaussBlur(bmp2.Buff(), bmp1.Buff(), bmp1.Width(), bmp1.Height(), param);
		//ImgProc::Laplacian(bmp2.Image());
		//	ImgProc::Effect3D(bmp2.Buff(), bmp1.Buff(), bmp1.Width(), bmp1.Height(), param);
		//ImgProc::ConvImage(&bmp2.Image(), &bmp1.Image(), &kernel.Image());
		ImgProc::FFTConvImage(bmp2.Image(), kernel.Image());
		//ImgProc::FFTMagnitudeImage(bmp2.Image());
		//ImgProc::FwhtMagnitudeImage(bmp2.Image());
		//ImgProc::FwhtConvImage(bmp2.Image(), kernel.Image());
	}
	::puts("Done");
//	if(bmp2.SetClipboard() < 0)  fprintf(stderr, "Could not set BMP\n");
	if(bmp2.Save(saveFileName.c_str()) < 0)  fprintf(stderr, "Couldnt save!\n");
//	if(bmp2.SetClipboard() < 0)  fprintf(stderr, "Couldnt set clip\n");
	return(0);
}
