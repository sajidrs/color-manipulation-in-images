#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>
using namespace cv;
using namespace std;

void runOnWindow(int W1, int H1, int W2, int H2, Mat inputImage, char *outName) {
	int rows = inputImage.rows;
	int cols = inputImage.cols;
	vector<Mat> i_planes;
	split(inputImage, i_planes);
	Mat iB = i_planes[0];
	Mat iG = i_planes[1];
	Mat iR = i_planes[2];

	// dynamically allocate RGB arrays of size rows x cols
	int** R = new int*[rows];
	int** G = new int*[rows];
	int** B = new int*[rows];

	double X1, Y1, Z1;
	for (int i = 0; i < rows; i++) {
		R[i] = new int[cols];
		G[i] = new int[cols];
		B[i] = new int[cols];
	}

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++) {
			R[i][j] = iR.at<uchar>(i, j);
			G[i][j] = iG.at<uchar>(i, j);
			B[i][j] = iB.at<uchar>(i, j);
			//	cout << R[i][j] << "/" << G[i][j] << "/" << B[i][j] << endl;
		}

	double** x = new double*[rows];
	double** y = new double*[rows];
	double** Y = new double*[rows];

	for (int i = 0; i < rows; i++) {
		x[i] = new double[cols];
		y[i] = new double[cols];
		Y[i] = new double[cols];
	}

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++) {
			//conversion to non-linear RGB
			double R1 = R[i][j] / 255.0;
			double G1 = G[i][j] / 255.0;
			double B1 = B[i][j] / 255.0;
			//	cout << R1 << "/" << G1 << "/" << B1 << endl;

			//inverse gamma correction - to linear RGB
			if (R1 < 0.03928)
			{
				R1 = R1 / 12.92;
			}
			else
			{
				R1 = pow(((R1 + 0.055) / 1.055), 2.4);
			}
			if (G1 < 0.03928)
			{
				G1 = G1 / 12.92;
			}
			else
			{
				G1 = pow(((G1 + 0.055) / 1.055), 2.4);
			}
			if (B1 < 0.03928)
			{
				B1 = B1 / 12.92;
			}
			else
			{
				B1 = pow(((B1 + 0.055) / 1.055), 2.4);
			}

			//cout << R1 << "/" << G1 << "/" << B1 << endl;

			//conversion to XYZ format
			X1 = (0.412453*R1) + (0.35758*G1) + (0.180423*B1);
			Y1 = (0.212671*R1) + (0.71516*G1) + (0.072169*B1);
			Z1 = (0.019334*R1) + (0.119193*G1) + (0.950227*B1);
			//cout << X1 <<"/" << Y1 << "/" << Z1 << endl;

			//conversion to xyY format		
			x[i][j] = (X1) / (X1 + Y1 + Z1);
			y[i][j] = (Y1) / (X1 + Y1 + Z1);
			Y[i][j] = Y1;

			//cout << x[i][j] <<"/" << y[i][j] << "/" << Y[i][j] << endl;	
		}

	//linear scaling in Y domain
	double min = Y[H1][W1];
	double max = Y[H1][W1];
	for (int i = H1; i <= H2; i++)
		for (int j = W1; j <= W2; j++) {
			if (Y[i][j] >= max)
			{
				max = Y[i][j];
			}
			if (Y[i][j] <= min)
			{
				min = Y[i][j];
			}
		}
	//cout << max << "/" << min << endl;
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++) {
			if (Y[i][j] < min)
			{
				Y[i][j] = 0;
			}
			if (Y[i][j] > max)
			{
				Y[i][j] = 1;
			}

			X1 = (x[i][j] / y[i][j])*Y[i][j];
			Y1 = Y[i][j];
			Z1 = ((1 - x[i][j] - y[i][j])*Y[i][j]) / y[i][j];

			//linear RGB
			double r = (3.240479 * X1) + (-1.53715 * Y1) + (-0.498535 * Z1);
			double g = (-0.969256 * X1) + (1.875991 * Y1) + (0.041556 * Z1);
			double b = (0.055648 * X1) + (-0.204043 * Y1) + (1.057311 * Z1);


			//linear to non-linear RGB - gamma correction

			if (r < 0.00304)
			{
				r = 12.92*r;
			}
			else
			{
				r = (1.055*pow(r, (0.4167))) - 0.055;
			}

			if (g < 0.00304)
			{
				g = 12.92*g;
			}
			else
			{
				g = (1.055*pow(g, (0.4167))) - 0.055;
			}

			if (b < 0.00304)
			{
				b = 12.92*b;
			}
			else
			{
				b = (1.055*pow(b, (0.4167))) - 0.055;
			}
			//cout << Ir << "/" << Ig << "/" << Ib << endl;

			//clipping
			if (r > 1)
			{
				r = 1;
			}
			else if (r < 0)
			{
				r = 0;
			}
			if (g > 1)
			{
				g = 1;
			}
			else if (g < 0)
			{
				g = 0;
			}
			if (b > 1)
			{
				b = 1;
			}
			else if (b < 0)
			{
				b = 0;
			}
			//	cout << r << "/" << g << "/ " << b << endl;

			//multiply by 255 - conversion to sRGB

			R[i][j] = (int)(r * 255);
			G[i][j] = (int)(g * 255);
			B[i][j] = (int)(b * 255);

			//	cout << R[i][j] << "/" << G[i][j] << "/" << B[i][j] << endl;
		}
	Mat oR(rows, cols, CV_8UC1);
	Mat oG(rows, cols, CV_8UC1);
	Mat oB(rows, cols, CV_8UC1);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++) {
			oR.at<uchar>(i, j) = R[i][j];;
			oG.at<uchar>(i, j) = G[i][j];;
			oB.at<uchar>(i, j) = B[i][j];;
		}

	Mat o_planes[] = { oB, oG, oR };
	Mat outImage;
	merge(o_planes, 3, outImage);

	namedWindow("output", CV_WINDOW_AUTOSIZE);
	imshow("output", outImage);
	imwrite(outName, outImage);
}

int main(int argc, char** argv) {
	if (argc != 7) {
		cerr << argv[0] << ": "
			<< "got " << argc - 1
			<< " arguments. Expecting six: w1 h1 w2 h2 ImageIn ImageOut."
			<< endl;
		cerr << "Example: proj1b 0.2 0.1 0.8 0.5 fruits.jpg out.bmp" << endl;
		return(-1);
	}
	double w1 = atof(argv[1]);
	double h1 = atof(argv[2]);
	double w2 = atof(argv[3]);
	double h2 = atof(argv[4]);
	char *inputName = argv[5];
	char *outputName = argv[6];

	if (w1<0 || h1<0 || w2 <= w1 || h2 <= h1 || w2>1 || h2>1) {
		cerr << " arguments must satisfy 0 <= w1 < w2 <= 1"
			<< " ,  0 <= h1 < h2 <= 1" << endl;
		return(-1);
	}

	Mat inputImage = imread(inputName, CV_LOAD_IMAGE_UNCHANGED);
	if (inputImage.empty()) {
		cout << "Could not open or find the image " << inputName << endl;
		return(-1);
	}

	string windowInput("input: ");
	windowInput += inputName;

	namedWindow(windowInput, CV_WINDOW_AUTOSIZE);
	imshow(windowInput, inputImage);

	if (inputImage.type() != CV_8UC3) {
		cout << inputName << " is not a standard color image  " << endl;
		return(-1);
	}

	int rows = inputImage.rows;
	int cols = inputImage.cols;
	int W1 = (int)(w1*(cols - 1));
	int H1 = (int)(h1*(rows - 1));
	int W2 = (int)(w2*(cols - 1));
	int H2 = (int)(h2*(rows - 1));

	runOnWindow(W1, H1, W2, H2, inputImage, outputName);

	waitKey(0); // Wait for a keystroke
	return(0);
}