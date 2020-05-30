#ifndef IMAGE__H
#define IMAGE__H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <vector>

using namespace cv;

class Image
{
private:
	std::string name;
	Mat image;
	Mat binarizadaCV;
	Mat subImage;
	std::vector<Mat> bgr_hist;
	std::vector<Mat> ycrcb_hist;
	std::vector<Mat> hsv_hist;
	int mainColor;
	int shapeVertexNum;
	Mat textureCoorMat;
	void calculateHistogram();
	void calculateTextureMat();
	void calculateShape();
	

public:
	Image();
	Image(std::string name);
	virtual ~Image();
	void printBGRHistogram();
	void printHSVHistogram();
	void printYCrCbHistogram();
	int getMainColor();
	int getShapeVertexNum();
	float getTextureEnergy();
	float getTextureContrast();
	float getTextureHomogenity();
	float getTextureEntropy();
	float getTextureIDM();
	float getTextureMean();
	Mat getImage();
	Mat getBinarizedImage();
	std::vector<Mat> getBGRHistogram();
	std::vector<Mat> getYCrCbHistogram();
	std::vector<Mat> getHSVHistogram();
	Mat getTextureMat();
	std::string getName();
};

#endif
