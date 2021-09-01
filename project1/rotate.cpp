#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

template <typename T>
Mat myrotate(const Mat input, float angle, const char* opt);

int main()
{
	Mat input, rotated;

	// Read each image
	input = imread("lena.jpg");

	// Check for invalid input
	if (!input.data) {
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	// original image
	namedWindow("image");
	imshow("image", input);

	rotated = myrotate<Vec3b>(input, 45, "bilinear");

	// rotated image
	namedWindow("rotated");
	imshow("rotated", rotated);

	waitKey(0);

	return 0;
}

//rotate input by given angle in given interpolation type and return output
template <typename T>
Mat myrotate(const Mat input, float angle, const char* opt) {
	//row and column of the input image
	int row = input.rows;
	int col = input.cols;

	//theta
	float radian = angle * CV_PI / 180;

	//row and column of output image (extern big square of rotated image)
	float sq_row = ceil(row * sin(radian) + col * cos(radian));
	float sq_col = ceil(col * sin(radian) + row * cos(radian));

	//rotated image
	Mat output = Mat::zeros(sq_row, sq_col, input.type());

	//for every pixel of output image
	for (int i = 0; i < sq_row; i++) {
		for (int j = 0; j < sq_col; j++) {
			
			//calculate x and y (which is the input coordinate) by i, j(which is ouput coordinate) ...use inverse warping
			float x = (j - sq_col / 2) * cos(radian) - (i - sq_row / 2) * sin(radian) + col / 2;
			float y = (j - sq_col / 2) * sin(radian) + (i - sq_row / 2) * cos(radian) + row / 2;

			//if x and y is in the range
			if ((y >= 0) && (y <= (row - 1)) && (x >= 0) && (x <= (col - 1))) {
				//nearest neighbor
				if (!strcmp(opt, "nearest")) {
					//copy and paste
					output.at<Vec3b>(i, j) = input.at<Vec3b>(y, x);

				}
				//bilinear
				else if (!strcmp(opt, "bilinear")) {
										
					float y1 = floor(y);
					float y2 = ceil(y);
					float x1 = floor(x);
					float x2 = ceil(x);

					float mu = y - y1;
					float lambda = x - x1;

					//use proportional formula in 2D image
					output.at<Vec3b>(i, j) = lambda * (mu * input.at<Vec3b>(y2, x2) + (1 - mu) * input.at<Vec3b>(y1, x2)) +
						(1 - lambda) * (mu * input.at<Vec3b>(y2, x1) + (1 - mu) * input.at<Vec3b>(y1, x1));
				}
			}
		}
	}

	return output;
}