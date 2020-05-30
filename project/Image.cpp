#include "Image.h"
Image::Image()
{

}

Image::Image(std::string name)
{
	this->name = name;
	this->image = imread(name);
	Mat src_copy = this->image.clone();
	float percentage = 0.7;
	Range rows((1-percentage)/2*this->image.rows,(percentage+(1-percentage)/2)*this->image.rows);
	Range cols((1-percentage)/2*this->image.cols,(percentage+(1-percentage)/2)*this->image.cols);
	this->subImage = this->image(rows,cols);
	Point p1 ( (1-percentage)/2*this->image.cols, (1-percentage)/2*this->image.rows);
	Point p2 ( (percentage+(1-percentage)/2)*this->image.cols , (percentage+(1-percentage)/2)*this->image.rows);
	rectangle(src_copy,p1,p2,Scalar(0,255,0),2);

	//std::cout << "\tCONSTRUCTOR 1" << std::endl;
	Mat img_gray;
	cvtColor(this->subImage, img_gray, COLOR_RGB2GRAY);
	threshold( img_gray, this->binarizadaCV, 0, 255, 1 | CV_THRESH_OTSU);

	//std::cout << "\tCalculando Histograma" << std::endl;
	calculateHistogram();
	//std::cout << "\tCalculando Textura" << std::endl;
	calculateTextureMat();
	//std::cout << "\tCalculando Forma" << std::endl;
	//calculateShape();
	/// Display

	//std::cout << "\tCONSTRUCTOR 2" << std::endl;
	std::stringstream ss( name );
	std::string basename;
	getline( ss, basename, '.' );


	/*imwrite(name+"_subImage.png",this->subImage);
	imwrite(name+"_rectImage.png",src_copy);
	imwrite(name+"_binarized.png", this->binarizadaCV);*/
}

Image::~Image()
{

}

void Image::calculateHistogram()
{
	std::vector<Mat> rgb_planes;
	split( this->subImage, rgb_planes );

	/// Establish the number of bins
	int histSize = 256;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 } ;
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat b_hist, g_hist, r_hist;

	/// Compute the histograms:
	calcHist( &rgb_planes[0], 1, 0, binarizadaCV, b_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &rgb_planes[1], 1, 0, binarizadaCV, g_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &rgb_planes[2], 1, 0, binarizadaCV, r_hist, 1, &histSize, &histRange, uniform, accumulate );
	this->bgr_hist.push_back(b_hist);
	this->bgr_hist.push_back(g_hist);
	this->bgr_hist.push_back(r_hist);

	Mat ycrcb;
	cvtColor(this->subImage, ycrcb, COLOR_RGB2YCrCb);
	std::vector<Mat> ycrcb_planes;
	split( ycrcb, ycrcb_planes );

	Mat y_hist, cr_hist, cb_hist;

	/// Compute the histograms:
	calcHist( &ycrcb_planes[0], 1, 0, binarizadaCV, y_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &ycrcb_planes[1], 1, 0, binarizadaCV, cr_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &ycrcb_planes[2], 1, 0, binarizadaCV, cb_hist, 1, &histSize, &histRange, uniform, accumulate );
	this->ycrcb_hist.push_back(y_hist);
	this->ycrcb_hist.push_back(cr_hist);
	this->ycrcb_hist.push_back(cb_hist);

	Mat hsv;
	cvtColor(this->subImage, hsv, COLOR_BGR2HSV);
	std::vector<Mat> hsv_planes;
	split( hsv, hsv_planes );

	/// Establish the number of bins
	int h_histSize = 360;
	int sv_histSize = 255;
	/// Set the ranges ( for B,G,R) )
	float h_range[] = { 0, 360 } ;
	float s_range[] = { 0, 255 } ;
	float v_range[] = { 0, 255 } ;
	const float* h_histRange = { h_range };
	const float* s_histRange = { s_range };
	const float* v_histRange = { v_range };

	Mat h_hist, s_hist, v_hist;
	// std::cout << hsv_planes[0] << std::endl;
	/// Compute the histograms:
	calcHist( &hsv_planes[0], 1, 0, binarizadaCV, h_hist, 1, &h_histSize, &h_histRange, uniform, accumulate );
	calcHist( &hsv_planes[1], 1, 0, binarizadaCV, s_hist, 1, &sv_histSize, &s_histRange, uniform, accumulate );
	calcHist( &hsv_planes[2], 1, 0, binarizadaCV, v_hist, 1, &sv_histSize, &v_histRange, uniform, accumulate );
	this->hsv_hist.push_back(h_hist);
	this->hsv_hist.push_back(s_hist);
	this->hsv_hist.push_back(v_hist);

	int h_max;
    int max = 0;
    for(int i = 0; i < h_hist.rows; i++)
    {
      int val = h_hist.at<float>(i,0);
      if(val > max)
      {
        max = val;
        h_max = i;
      }
    }
    this->mainColor = h_max*2;

}

void Image::calculateTextureMat()
{
	std::vector<double> vec_energy;
	float energy = 0, contrast = 0, homogenity = 0, IDM = 0, entropy = 0, mean1 = 0;
	int row = this->subImage.rows, col = this->subImage.cols;
	Mat gl = Mat::zeros(256, 256, CV_32FC1);

	//creating glcm matrix with 256 levels,radius=1 and in the horizontal direction
	for (int i = 0; i<row; i++)
	    for (int j = 0; j<col - 1; j++)
	        gl.at<float>(this->subImage.at<uchar>(i, j), this->subImage.at<uchar>(i, j + 1)) = gl.at<float>(this->subImage.at<uchar>(i, j), this->subImage.at<uchar>(i, j + 1)) + 1;

	// normalizing glcm matrix for parameter determination
	gl = gl + gl.t();
	gl = gl / sum(gl)[0];

	this->textureCoorMat = gl.clone();
}


void Image::calculateShape()
{
	int thresh = 100;
	int max_thresh = 255;
	Mat img_rgb,canny_output,drawing;
	std::vector< std::vector<Point> > contours;
	std::vector<Vec4i> hierarchy;
	Canny( this->binarizadaCV, canny_output, thresh, thresh*2, 3 );
	//imshow("Canny",canny_output);
	waitKey(0);
	findContours( this->binarizadaCV, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	drawing = Mat::zeros( canny_output.size(), CV_8UC3 );

	
	std::vector<Point> approxTriangle;
	//std::cout << contours.size();
	int mayorSize = 0;
	for(size_t i = 0; i < contours.size(); i++)
	{
		drawContours(drawing, contours, i, Scalar(0, 255, 255), CV_FILLED); // fill GREEN
		approxPolyDP(contours[i], approxTriangle, arcLength(Mat(contours[i]), true)*14, true);
		if(mayorSize < approxTriangle.size()){
			mayorSize = approxTriangle.size();
		}
		//std::cout << approxTriangle.size() << std::endl;
	}
	this->shapeVertexNum = mayorSize;
	//imshow("Triangles",drawing);
}

void Image::printBGRHistogram()
{
	int histSize = 256;
	float range[] = { 0, 256 } ;
	const float* histRange = { range };
	// Draw the histograms for B, G and R
	int hist_w = 512*4; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );

	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(this->bgr_hist[0], this->bgr_hist[0], 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(this->bgr_hist[1], this->bgr_hist[1], 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(this->bgr_hist[2], this->bgr_hist[2], 0, histImage.rows, NORM_MINMAX, -1, Mat() );

	/// Draw for each channel
	for( int i = 1; i < histSize; i++ )
	{
	  line( histImage, Point( bin_w*(i-1), hist_h - cvRound(this->bgr_hist[0].at<float>(i-1)) ) ,
	  Point( bin_w*(i), hist_h - cvRound(this->bgr_hist[0].at<float>(i)) ),
	  Scalar( 255, 0, 0), 1, 8, 0  );
	  line( histImage, Point( bin_w*(i-1), hist_h - cvRound(this->bgr_hist[1].at<float>(i-1)) ) ,
	  Point( bin_w*(i), hist_h - cvRound(this->bgr_hist[1].at<float>(i)) ),
	  Scalar( 0, 255, 0), 1, 8, 0  );
	  line( histImage, Point( bin_w*(i-1), hist_h - cvRound(this->bgr_hist[2].at<float>(i-1)) ) ,
	  Point( bin_w*(i), hist_h - cvRound(this->bgr_hist[2].at<float>(i)) ),
	  Scalar( 0, 0, 255), 1, 8, 0  );
	}

	std::stringstream ss( this->name );
	std::string basename;
	getline( ss, basename, '.' );

	imwrite(basename+"_histogram.png", histImage);
	std::cout << "RGB histogram saved" << std::endl;
}

void Image::printHSVHistogram()
{
	 /// Establish the number of bins
    int h_histSize = 360;
    int sv_histSize = 255;
	// Draw the histograms for B, G and R
	int hist_w = 512*4; int hist_h = 400;
	int h_bin_w = cvRound( (double) hist_w/h_histSize );
	int s_bin_w = cvRound( (double) hist_w/sv_histSize );
	int v_bin_w = cvRound( (double) hist_w/sv_histSize );

	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar(0,0,0) );


	/// Normalize the result to [ 0, histImage.rows ]
	normalize(this->hsv_hist[0], this->hsv_hist[0], 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(this->hsv_hist[1], this->hsv_hist[1], 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(this->hsv_hist[2], this->hsv_hist[2], 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	// std::cout << "??" << this->hsv_hist[1] << "??" << std::endl;
	/// Draw for each channel
	for( int i = 1; i < h_histSize; i++ )
	{
	  line( histImage, Point( h_bin_w*(i-1), hist_h - cvRound(this->hsv_hist[0].at<float>(i-1)) ) ,
	  Point( h_bin_w*(i), hist_h - cvRound(this->hsv_hist[0].at<float>(i)) ),
	  Scalar( 255, 255, 0), 1, 8, 0  );
	}
	for( int i = 1; i < sv_histSize; i++)
	{
	  line( histImage, Point( s_bin_w*(i-1), hist_h - cvRound(this->hsv_hist[1].at<float>(i-1)) ) ,
	  Point( s_bin_w*(i), hist_h - cvRound(this->hsv_hist[1].at<float>(i)) ),
	  Scalar( 0, 200, 200), 1, 8, 0  );
	  line( histImage, Point( v_bin_w*(i-1), hist_h - cvRound(this->hsv_hist[2].at<float>(i-1)) ) ,
	  Point( v_bin_w*(i), hist_h - cvRound(this->hsv_hist[2].at<float>(i)) ),
	  Scalar( 150, 150, 150), 1, 8, 0  );
	}
	std::stringstream ss( this->name );
	std::string basename;
	getline( ss, basename, '.' );

	imwrite(basename+"_HSVhistogram.png", histImage);
	std::cout << "HSV histogram saved" << std::endl;
}

void Image::printYCrCbHistogram()
{
	/// Establish the number of bins
	int histSize = 256;

	// Draw the histograms for B, G and R
	int hist_w = 512*4; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );

	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(this->ycrcb_hist[0], this->ycrcb_hist[0], 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(this->ycrcb_hist[1], this->ycrcb_hist[1], 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(this->ycrcb_hist[2], this->ycrcb_hist[2], 0, histImage.rows, NORM_MINMAX, -1, Mat() );

	/// Draw for each channel
	for( int i = 1; i < histSize; i++ )
	{
	  line( histImage, Point( bin_w*(i-1), hist_h - cvRound(this->ycrcb_hist[0].at<float>(i-1)) ) ,
	  Point( bin_w*(i), hist_h - cvRound(this->ycrcb_hist[0].at<float>(i)) ),
	  Scalar( 200, 200, 200), 1, 8, 0  );
	  line( histImage, Point( bin_w*(i-1), hist_h - cvRound(this->ycrcb_hist[1].at<float>(i-1)) ) ,
	  Point( bin_w*(i), hist_h - cvRound(this->ycrcb_hist[1].at<float>(i)) ),
	  Scalar(28 , 179, 230 ), 1, 8, 0  );
	  line( histImage, Point( bin_w*(i-1), hist_h - cvRound(this->ycrcb_hist[2].at<float>(i-1)) ) ,
	  Point( bin_w*(i), hist_h - cvRound(this->ycrcb_hist[2].at<float>(i)) ),
	  Scalar( 145, 214, 17), 1, 8, 0  );
	}
	std::stringstream ss( this->name );
	std::string basename;
	getline( ss, basename, '.' );

	imwrite(basename+"_YCrCbhistogram.png", histImage);
	std::cout << "YCrCb histogram saved" << std::endl;
}

int Image::getMainColor()
{
	return this->mainColor;
}

std::string Image::getName()
{
	return this->name;
}

int Image::getShapeVertexNum()
{
	return this->shapeVertexNum;
}

float Image::getTextureEnergy()
{
	float energy = 0;
	for (int i = 0; i<256; i++)
	{
		for (int j = 0; j<256; j++)
	    {
	        energy += this->textureCoorMat.at<float>(i, j)*this->textureCoorMat.at<float>(i, j);

	    }
	}

	return energy;
}


float Image::getTextureContrast()
{
	float contrast = 0;
	for (int i = 0; i<256; i++)
	{
		for (int j = 0; j<256; j++)
	    {
	        contrast += (i - j)*(i - j)*this->textureCoorMat.at<float>(i, j);        
	    }
	}
	return contrast;
}

float Image::getTextureHomogenity()
{
	float homogenity = 0;
	for (int i = 0; i<256; i++)
	{
		for (int j = 0; j<256; j++)
	    {
	        homogenity = homogenity + this->textureCoorMat.at<float>(i, j) / (1 + abs(i - j));
	    }
	}
	return homogenity;
}
float Image::getTextureEntropy()
{
	float entropy = 0;
	for (int i = 0; i<256; i++)
	{
		for (int j = 0; j<256; j++)
	    {
	        if (this->textureCoorMat.at<float>(i, j) != 0)
	            entropy = entropy - this->textureCoorMat.at<float>(i, j)*log10(this->textureCoorMat.at<float>(i, j));
	    }
	}
	return entropy;
}
float Image::getTextureIDM()
{
	float IDM = 0;
	for (int i = 0; i<256; i++)
	{
		for (int j = 0; j<256; j++)
	    {
	        if (i != j)
	            IDM = IDM + this->textureCoorMat.at<float>(i, j) / ((i - j)*(i - j));                      //Taking k=2; 
	    }
	}
	return IDM;
}

float Image::getTextureMean()
{
	float mean1 = 0;
	for (int i = 0; i<256; i++)
	{
		for (int j = 0; j<256; j++)
	    {
	        mean1 = mean1 + 0.5*(i*this->textureCoorMat.at<float>(i, j) + j*this->textureCoorMat.at<float>(i, j));
	    }
	}
	return mean1;
}

Mat Image::getImage()
{
	return this->image;
}

Mat Image::getBinarizedImage()
{
	return this->binarizadaCV;
}

std::vector<Mat> Image::getBGRHistogram()
{
	return this->bgr_hist;
}

std::vector<Mat> Image::getYCrCbHistogram()
{
	return this->ycrcb_hist;
}

std::vector<Mat> Image::getHSVHistogram()
{
	return this->hsv_hist;
}

Mat Image::getTextureMat()
{
	return this->textureCoorMat;
}