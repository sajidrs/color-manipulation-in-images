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

	double** L = new double*[rows];
	double** u = new double*[rows];
	double** v = new double*[rows];

	for (int i = 0; i < rows; i++) {
		L[i] = new double[cols];
		u[i] = new double[cols];
		v[i] = new double[cols];
	}

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++) {

			//conversion to non-linear RGB
			double R1 = R[i][j] / 255.0;
			double G1 = G[i][j] / 255.0;
			double B1 = B[i][j] / 255.0;
			//	cout << R1 << "/" << G1 << "/" << B1 << endl;

			//inverse gamma correction - conversion to linear RGB
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

			double X, Y, Z;
			//conversion to XYZ format
			X = (0.412453*R1) + (0.35758*G1) + (0.180423*B1);
			Y = (0.212671*R1) + (0.71516*G1) + (0.072169*B1);
			Z = (0.019334*R1) + (0.119193*G1) + (0.950227*B1);
			//	cout << X <<"/" << Y << "/" << Z << endl;

			double Xw = 0.95;
			double Yw = 1.0;
			double Zw = 1.09;
			double Uw = (4 * Xw) / (Xw + (15 * Yw) + (3 * Zw));
			double Vw = (9 * Yw) / (Xw + (15 * Yw) + (3 * Zw));
			//cout << Uw << "/" << Vw <<endl;

			//conversion to Luv
			if (Y > 0.008856)
			{
				L[i][j] = (116 * pow(Y, (1 / 3.0))) - 16;
			}
			else
			{
				L[i][j] = 903.3*Y;
			}

			//clipping L between 0 to 100
			/*		if (L[i][j] > 100)
			{
			L[i][j] = 100;
			}
			else if (L[i][j] < 0)
			{
			L[i][j] = 0;
			}
			*/
			double d = X + (15 * Y) + (3 * Z);
			double u1, v1;
			if (d == 0)
			{
				u1 = 0;
				v1 = 0;
			}
			else
			{
				u1 = (4 * X) / d;
				v1 = (9 * Y) / d;
			}
			u[i][j] = 13 * L[i][j] * (u1 - Uw);
			v[i][j] = 13 * L[i][j] * (v1 - Vw);
			// cout << L[i][j] << "/" << u[i][j] << "/" << v[i][j] << endl;

		}

	//Finding the Lmax and Lmin in the given window
	double min = L[H1][W1];
	double max = L[H1][W1];
	for (int i = H1; i <= H2; i++)
		for (int j = W1; j <= W2; j++) {
			if (L[i][j]>max)
			{
				max = L[i][j];
			}
			if (L[i][j] < min)
			{
				min = L[i][j];
			}
		}
	//	cout << max << "/" << min << endl;

	//Linear scaling on L
	//For the L values above max, in the whole image, the L value is equated to 100 and for values below min, it is changed to 0
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++) {
			if (L[i][j] >= max)
			{
				L[i][j] = 100;
			}
			if (L[i][j] <= min)
			{
				L[i][j] = 0;
			}
			double x, y, z;
			//	 cout << L[i][j]<< "/" << u[i][j] << "/" << v[i][j]<< endl;

			double Xw = 0.95;
			double Yw = 1.0;
			double Zw = 1.09;
			double u1, v1;
			double Uw = (4 * Xw) / (Xw + (15 * Yw) + (3 * Zw));
			double Vw = (9 * Yw) / (Xw + (15 * Yw) + (3 * Zw));
			if (L[i][j] == 0) {
				u1 = 0;
				v1 = 0;
			}
			else {
				u1 = (u[i][j] + (13 * Uw * L[i][j])) / (13 * L[i][j]);
				v1 = (v[i][j] + (13 * Vw * L[i][j])) / (13 * L[i][j]);
			}
			// cout << L[1][2]<< "/" << u[1][2] << "/" << v[1][2]<< endl;

			//Luv to XYZ conversion
			if (L[i][j] > 7.9996)
			{
				y = (pow(((L[i][j] + 16) / 116.0), 3));
			}
			else
			{
				y = (L[i][j] / 903.3);
			}
			//	 cout << y << endl;
			if (v1 == 0)
			{
				x = 0;
				z = 0;
			}
			else
			{
				x = y*(2.25)*(u1 / v1);
				z = (y*(3 - (0.75 * u1) - (5 * v1))) / v1;
			}
			//		cout << x << "/" << y << "/" << z << endl;

			//linear RGB
			double r = (3.240479 * x) + (-1.53715 * y) + (-0.498535 * z);
			double g = (-0.969256 * x) + (1.875991 * y) + (0.041556 * z);
			double b = (0.055648 * x) + (-0.204043 * y) + (1.057311 * z);
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