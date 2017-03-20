#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;

double linear_to_nonlinear_gamma(double D);

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

		/*
		Convert xyY to XYZ

		X = (x/y)Y, Y=Y, Z=(1-x-y)Y/y

		Converting XYZ to sRGB


		*/

		//Getting the XYZ values
		double X = (x / y)*Y;

		double Z = (1 - x - y)*Y / y;

		double xyz[3] = { X, Y, Z };
		//Getting the sRGB values
		double mxr[3][3] = {
			{ 3.240479, -1.53715, -0.498535 },
			{ -0.969256, 1.875991, 0.041556 },
			{ 0.055648, -0.204043, 1.057311 }
		};

		double sRGB[3];
		double SR1, SG1, SB1, sum = 0.0;
		for (int m = 0; m < 3; m++){
			sum = 0.0;
			for (int n = 0; n < 3; n++){
				sum = sum + mxr[i][j] * xyz[i]; // Multiply by the corresponding values to get the R G B values
			}

			// Condition check for the RGB values should be between 0 and 1.
			if (sum < 0.0){
				sRGB[i] = 0.0;
			}
			else if (sum > 1.0){
				sRGB[i] = 1.0;
			}
			else{
				sRGB[i] = sum;
			}
		}

		//Getting the non-linear sRGB values - Gamma Correction
		double nsRGB[3];
		double val;
		for (int m = 0; m < 3; m++){
			/*
			double linear_to_nonlinear_gamma(double D)
			*/
			val = linear_to_nonlinear_gamma(sRGB[i]);
			if (val<0.0)
				nsRGB[i] = 0.0;
			else if (val>1.0)
				nsRGB[i] = 1.0;
			else
				nsRGB[i] = val;
		}

		/* Your code should be placed here
		It should translate xyY to byte sRGB
		and Luv to byte sRGB
		*/
		r1 = (int)(nsRGB[0] * 255);
		g1 = (int)(nsRGB[1] * 255);
		b1 = (int)(nsRGB[2] * 255);

		r2 = (int)(1.0 * 255);
		g2 = (int)(x * 255);
		b2 = (int)(y * 255);

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

double linear_to_nonlinear_gamma(double D){
	if (D < 0.00304)
		return 12.92*D;
	else
	{
		double x = 1 / 2.4;
		return 1.055*pow(D, x) - 0.055;
	}
}
