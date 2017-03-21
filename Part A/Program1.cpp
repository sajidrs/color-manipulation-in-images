#include "opencv2/highgui.hpp"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
using namespace cv;
using namespace std;


int main(int argc, char** argv) {
	
if (argc != 3) {
		cout << argv[0] << ": "
			<< "got " << argc - 1 << " arguments. Expecting two: width height."
			<< endl;
		return(-1);
	}
	
	int width = atoi(argv[1]);
	int height = atoi(argv[2]);
	int** RED1 = new int*[height];
	int** GREEN1 = new int*[height];
	int** BLUE1 = new int*[height];
	int** RED2 = new int*[height];
	int** GREEN2 = new int*[height];
	int** BLUE2 = new int*[height];

	for (int i = 0; i < height; i++) {
		RED1[i] = new int[width];
		GREEN1[i] = new int[width];
		BLUE1[i] = new int[width];
		RED2[i] = new int[width];
		GREEN2[i] = new int[width];
		BLUE2[i] = new int[width];
	}
	
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
		int r1, g1, b1;
		int r2, g2, b2;

		double x = (double)j / (double)width;
		double y = (double)i / (double)height;
		double Y = 1.0;

		double L = 90;
		double u = x * 512 - 255;
		double v = y * 512 - 255;

		//xyY to XYZ conversion
		double X = (x / y)*Y;
		double Z = ((1 - x - y) / y)*Y;
		
		//linear RGB
		double R = (3.240479*X) + (-1.53715*Y) + (-0.498535*Z);
		double G = (-0.969256*X) + (1.875991*Y) + (0.041556*Z);
		double B = (0.055648*X) + (-0.204043*Y) + (1.057311*Z);

		//clipping values>1 to 1 and values<0 to 0
		if (R > 1)
		{
			R = 1;
		}
		else if (R < 0)
		{
			R = 0;
		}
		if (G > 1)
		{
			G = 1;
		}
		else if (G < 0)
		{
			G = 0;
		}
		if (B > 1)
		{
			B = 1;
		}
		else if (B < 0)
		{
			B = 0;
		}

		//linear to non-linear RGB - gamma correction
		double Ir, Ig, Ib;
		if (R < 0.00304)
		{
			Ir = 12.92*R;
		}
		else
		{
			Ir = (1.055*pow(R,(1/2.4)))- 0.055;
		}
		
		if (G < 0.00304)
		{
			Ig = 12.92*G;
		}
		else
		{
			Ig = (1.055*pow(G, (1 / 2.4))) - 0.055;
		}

		if (B < 0.00304)
		{
			Ib = 12.92*B;
		}
		else
		{
			Ib = (1.055*pow(B, (1 / 2.4))) - 0.055;
		}
		
		//clipping
		if (Ir > 1)
		{
			Ir = 1;
		}
		else if (Ir < 0)
		{
			Ir = 0;
		}
		if (Ig > 1)
		{
			Ig = 1;
		}
		else if (Ig < 0)
		{
			Ig = 0;
		}
		if (Ib > 1)
		{
			Ib = 1;
		}
		else if (Ib < 0)
		{
			Ib = 0;
		}

		//multiply by 255 - conversion to sRGB
		r1 = (int)(Ir * 255);
		g1 = (int)(Ig * 255);
		b1 = (int)(Ib * 255);
		
		//cout << sR << "/" << sG << "/" << sB << endl;

		//Luv to sRGB conversion
		double Xw = 0.95;
		double Yw = 1;
		double Zw = 1.09;
		double Uw = (4 * Xw) / (Xw + (15 * Yw) + (3 * Zw));
		double Vw = (9 * Yw) / (Xw + (15 * Yw) + (3 * Zw));

		double u1 = (u + (13 * Uw * L)) / (13 * L);
		double v1 = (v + (13 * Vw * L)) / (13 * L);
		
		if (L > 7.9996)
		{
			Y = (pow(((L + 16) / 116), 3)) * Yw;
		}
		else
		{
			Y = (L / 903.3) * Yw;
		}

		if (v1 == 0)
		{
			X = 0;
			Z = 0;
		}
		else
		{
			X = Y*(2.25)*(u1 / v1);
			Z = (Y*(3 - (0.75 * u1) - (5 * v1))) / v1;
		}

		//linear RGB
		 R = (3.240479*X) + (-1.53715*Y) + (-0.498535*Z);
		 G = (-0.969256*X) + (1.875991*Y) + (0.041556*Z);
		 B = (0.055648*X) + (-0.204043*Y) + (1.057311*Z);

		//clipping values>1 to 1 and values<0 to 0
		if (R > 1)
		{
			R = 1;
		}
		else if (R < 0)
		{
			R = 0;
		}
		if (G > 1)
		{
			G = 1;
		}
		else if (G < 0)
		{
			G = 0;
		}
		if (B > 1)
		{
			B = 1;
		}
		else if (B < 0)
		{
			B = 0;
		}

		//linear to non-linear RGB - gamma correction
		if (R < 0.00304)
		{
			Ir = 12.92*R;
		}
		else
		{
			Ir = (1.055*pow(R, (1 / 2.4))) - 0.055;
		}

		if (G < 0.00304)
		{
			Ig = 12.92*G;
		}
		else
		{
			Ig = (1.055*pow(G, (1 / 2.4))) - 0.055;
		}

		if (B < 0.00304)
		{
			Ib = 12.92*B;
		}
		else
		{
			Ib = (1.055*pow(B, (1 / 2.4))) - 0.055;
		}

		//clipping
		if (Ir > 1)
		{
			Ir = 1;
		}
		else if (Ir < 0)
		{
			Ir = 0;
		}
		if (Ig > 1)
		{
			Ig = 1;
		}
		else if (Ig < 0)
		{
			Ig = 0;
		}
		if (Ib > 1)
		{
			Ib = 1;
		}
		else if (Ib < 0)
		{
			Ib = 0;
		}
	
		//multiply by 255 - conversion to sRGB
		r2 = (int)(Ir * 255);
		g2 = (int)(Ig * 255);
		b2 = (int)(Ib * 255);

		
		// this is the end of your code
		RED1[i][j] = r1;
		GREEN1[i][j] = g1;
		BLUE1[i][j] = b1;
		RED2[i][j] = r2;
		GREEN2[i][j] = g2;
		BLUE2[i][j] = b2;
		}


	Mat R1(height, width, CV_8UC1);
	Mat G1(height, width, CV_8UC1);
	Mat B1(height, width, CV_8UC1);

	Mat R2(height, width, CV_8UC1);
	Mat G2(height, width, CV_8UC1);
	Mat B2(height, width, CV_8UC1);

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {

		R1.at<uchar>(i, j) = RED1[i][j];
		G1.at<uchar>(i, j) = GREEN1[i][j];
		B1.at<uchar>(i, j) = BLUE1[i][j];

		R2.at<uchar>(i, j) = RED2[i][j];
		G2.at<uchar>(i, j) = GREEN2[i][j];
		B2.at<uchar>(i, j) = BLUE2[i][j];
		}

	Mat xyY;
	Mat xyY_planes[] = { B1, G1, R1 };
	merge(xyY_planes, 3, xyY);
	namedWindow("xyY", CV_WINDOW_AUTOSIZE);
	imshow("xyY", xyY);

	Mat Luv;
	Mat Luv_planes[] = { B2, G2, R2 };
	merge(Luv_planes, 3, Luv);
	namedWindow("Luv", CV_WINDOW_AUTOSIZE);
	imshow("Luv", Luv);
	waitKey(0); // Wait for a keystroke
	return(0);
}