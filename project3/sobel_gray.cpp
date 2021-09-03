﻿#include <iostream>
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
    Mat input_gray;
    Mat output;


    cvtColor(input, input_gray, CV_RGB2GRAY);



    if (!input.data)
    {
        std::cout << "Could not open" << std::endl;
        return -1;
    }

    namedWindow("Grayscale", WINDOW_AUTOSIZE);
    imshow("Grayscale", input_gray);
    output = sobelfilter(input_gray); //Boundary process: zero-paddle, mirroring, adjustkernel

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

            float sum1 = 0.0;
            float sum2 = 0.0;
            float sum3 = 0.0;

            for (int a = -n; a <= n; a++) {
                for (int b = -n; b <= n; b++) {

                    // Use mirroring boundary process
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

                    // Find output M(x,y) = sqrt( input.at<G>(x, y)*Sx + input.at<G>(x, y)*Sy ) 
                    //sum1 += kernelvalue * (float)(input.at<G>(tempa, tempb));
                    sum1 += (float)(input.at<G>(tempa, tempb) * Sx.at<float>(a + n, b + n));
                    sum2 += (float)(input.at<G>(tempa, tempb) * Sy.at<float>((a + n), (b + n)));
                }

            }
            if (sum1 < 0)
                sum1 = -sum1;
            if (sum2 < 0)
                sum2 = -sum2;
            sum3 = sum1 + sum2;
            //sum1 = sqrt(sum1);
            if (255 <= sum3)
                output.at<G>(i, j) = 255;
            else output.at<G>(i, j) = sum3;

        }
    }
    return output;
}