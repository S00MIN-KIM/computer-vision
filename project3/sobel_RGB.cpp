#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>       /* exp */
#define IM_TYPE   CV_8UC3

using namespace cv;

// Image Type
// "G" for GrayScale Image, "C" for Color Image
#if (IM_TYPE == CV_8UC3)
typedef uchar G;
typedef cv::Vec3b C;
#elif (IM_TYPE == CV_16SC3)
typedef short G;
typedef Vec3s C;
#elif (IM_TYPE == CV_32SC3)
typedef int G;
typedef Vec3i C;
#elif (IM_TYPE == CV_32FC3)
typedef float G;
typedef Vec3f C;
#elif (IM_TYPE == CV_64FC3)
typedef double G;
typedef Vec3d C;
#endif

Mat sobelfilter(const Mat input);

int main() {

    Mat input = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);
    Mat output;

    if (!input.data)
    {
        std::cout << "Could not open" << std::endl;
        return -1;
    }

    namedWindow("Original", WINDOW_AUTOSIZE);
    imshow("Original", input);
    output = sobelfilter(input); //Boundary process: zero-paddle, mirroring, adjustkernel

    namedWindow("Sobel Filter", WINDOW_AUTOSIZE);
    imshow("Sobel Filter", output);


    waitKey(0);

    return 0;
}


Mat sobelfilter(const Mat input) {

    Mat kernel;

    int row = input.rows;
    int col = input.cols;
    int n = 1; // Sobel Filter Kernel N

    int tempa;
    int tempb;

    // Initialiazing 2 Kernel Matrix with 3x3 size for Sx and Sy
    //Fill code to initialize Sobel filter kernel matrix for Sx and Sy (Given in the lecture notes)
    Mat Sx = *(Mat_<float>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
    Mat Sy = *(Mat_<float>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);

    Mat output = Mat::zeros(row, col, input.type());

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            float sum1_r_x = 0.0;   float sum1_r_y = 0.0;   float sum1_r = 0.0;
            float sum1_g_x = 0.0;   float sum1_g_y = 0.0;   float sum1_g = 0.0;
            float sum1_b_x = 0.0;   float sum1_b_y = 0.0;   float sum1_b = 0.0;

            for (int a = -n; a <= n; a++) { // for each kernel window
                for (int b = -n; b <= n; b++) {

                    if (i + a > row - 1) {  //mirroring for the border pixels
                        tempa = i - a;
                    }
                    else if (i + a < 0) {
                        tempa = -(i + a);
                    }
                    else {
                        tempa = i + a;
                    }
                    if (j + b > col - 1) {
                        tempb = j - b;
                    }
                    else if (j + b < 0) {
                        tempb = -(j + b);
                    }
                    else {
                        tempb = j + b;
                    }

                    sum1_r_x += (float)(input.at<C>(tempa, tempb)[0] * Sx.at<float>(a + n, b + n));
                    sum1_r_y += (float)(input.at<C>(tempa, tempb)[0] * Sy.at<float>(a + n, b + n));
                    sum1_g_x += (float)(input.at<C>(tempa, tempb)[1] * Sx.at<float>(a + n, b + n));
                    sum1_g_y += (float)(input.at<C>(tempa, tempb)[1] * Sy.at<float>(a + n, b + n));
                    sum1_b_x += (float)(input.at<C>(tempa, tempb)[2] * Sx.at<float>(a + n, b + n));
                    sum1_b_y += (float)(input.at<C>(tempa, tempb)[2] * Sy.at<float>(a + n, b + n));
                }
            }
            if (sum1_r_x < 0) sum1_r_x = -sum1_r_x;
            if (sum1_r_y < 0) sum1_r_y = -sum1_r_y;
            if (sum1_g_x < 0) sum1_g_x = -sum1_g_x;
            if (sum1_g_y < 0) sum1_g_y = -sum1_g_y;
            if (sum1_b_x < 0) sum1_b_x = -sum1_b_x;
            if (sum1_b_y < 0) sum1_b_y = -sum1_b_y;

            sum1_r = sum1_r_x + sum1_r_y;
            sum1_g = sum1_g_x + sum1_g_y;
            sum1_b = sum1_b_x + sum1_b_y;

            if (sum1_r > 255) sum1_r = 255;
            if (sum1_g > 255) sum1_g = 255;
            if (sum1_b > 255) sum1_b = 255;

            output.at<C>(i, j)[0] = (G)(sum1_r);
            output.at<C>(i, j)[1] = (G)(sum1_g);
            output.at<C>(i, j)[2] = (G)(sum1_b);

        }
    }
    return output;
}