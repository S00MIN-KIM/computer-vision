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

Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt);

int main() {

    Mat input = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);
    Mat output;

    if (!input.data)
    {
        std::cout << "Could not open" << std::endl;
        return -1;
    }

    namedWindow("Grayscale", WINDOW_AUTOSIZE);
    imshow("Grayscale", input);
    output = gaussianfilter(input, 3, 3, 3, "mirroring"); //Boundary process: zero-paddle, mirroring, adjustkernel

    namedWindow("Gaussian Filter", WINDOW_AUTOSIZE);
    imshow("Gaussian Filter", output);


    waitKey(0);

    return 0;
}


Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt) {

    Mat kernels, kernelt;

    int row = input.rows;
    int col = input.cols;
    int kernel_size = (2 * n + 1);
    int tempa;
    int tempb;
    float denoms, denomt;
    //float kernelvalue;

    // Initialiazing Kernel Matrix 
    kernels = Mat::zeros(kernel_size, 1, CV_32F);
    kernelt = Mat::zeros(1, kernel_size, CV_32F);

    denoms = 0.0, denomt = 0.0;
    for (int a = -n; a <= n; a++) {
        float value1 = exp(-(pow(a, 2) / (2 * pow(sigmaS, 2))));
        kernels.at<float>(a + n, 0) = value1;
        denoms += value1;
    }

    for (int b = -n; b <= n; b++) {
        float value2 = exp(-(pow(b, 2) / (2 * pow(sigmaT, 2))));
        kernelt.at<float>(0, b + n) = value2;
        denomt += value2;
    }

    for (int a = -n; a <= n; a++) { 
        kernels.at<float>(a + n, 0) /= denoms;
    }

    for (int b = -n; b <= n; b++) {
        kernelt.at<float>(0, b + n) /= denomt;
    }


    Mat output = Mat::zeros(row, col, input.type());
    float kernelvalue = kernels.at<float>(0, 0) * kernelt.at<float>(0, 0);



    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {

            float sum1 = 0.0;

            if (!strcmp(opt, "zero-paddle")) {
                float sum1_r = 0.0;
                float sum1_g = 0.0;
                float sum1_b = 0.0;
                for (int a = -n; a <= n; a++) { // for each kernel window
                    for (int b = -n; b <= n; b++) {

                        if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) { //if the pixel is not a border pixel
                            sum1_r += kernels.at<float>(a + n, 0) * kernelt.at<float>(0, b + n) * (float)(input.at<C>(i + a, j + b)[0]);
                            sum1_g += kernels.at<float>(a + n, 0) * kernelt.at<float>(0, b + n) * (float)(input.at<C>(i + a, j + b)[1]);
                            sum1_b += kernels.at<float>(a + n, 0) * kernelt.at<float>(0, b + n) * (float)(input.at<C>(i + a, j + b)[2]);
                        }
                    }
                }
                output.at<C>(i, j)[0] = (G)sum1_r;
                output.at<C>(i, j)[1] = (G)sum1_g;
                output.at<C>(i, j)[2] = (G)sum1_b;
            }

            else if (!strcmp(opt, "mirroring")) {
                float sum1_r = 0.0;
                float sum1_g = 0.0;
                float sum1_b = 0.0;
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
                        sum1_r += kernels.at<float>(a + n, 0) * kernelt.at<float>(0, b + n) * (float)(input.at<C>(tempa, tempb)[0]);
                        sum1_g += kernels.at<float>(a + n, 0) * kernelt.at<float>(0, b + n) * (float)(input.at<C>(tempa, tempb)[1]);
                        sum1_b += kernels.at<float>(a + n, 0) * kernelt.at<float>(0, b + n) * (float)(input.at<C>(tempa, tempb)[2]);
                    }
                }
                output.at<C>(i, j)[0] = (G)sum1_r;
                output.at<C>(i, j)[1] = (G)sum1_g;
                output.at<C>(i, j)[2] = (G)sum1_b;
            }




            else if (!strcmp(opt, "adjustkernel")) {
                float sum1_r = 0.0;
                float sum1_g = 0.0;
                float sum1_b = 0.0;
                float sum2 = 0.0;
                for (int a = -n; a <= n; a++) { // for each kernel window
                    for (int b = -n; b <= n; b++) {
                        if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
                            sum1_r += kernels.at<float>(a + n, 0) * kernelt.at<float>(0, b + n) * (float)(input.at<C>(i + a, j + b)[0]);
                            sum1_g += kernels.at<float>(a + n, 0) * kernelt.at<float>(0, b + n) * (float)(input.at<C>(i + a, j + b)[1]);
                            sum1_b += kernels.at<float>(a + n, 0) * kernelt.at<float>(0, b + n) * (float)(input.at<C>(i + a, j + b)[2]);
                            sum2 += kernels.at<float>(a + n, 0) * kernelt.at<float>(0, b + n);
                        }
                    }
                }
                output.at<C>(i, j)[0] = (G)(sum1_r / sum2);
                output.at<C>(i, j)[1] = (G)(sum1_g / sum2);
                output.at<C>(i, j)[2] = (G)(sum1_b / sum2);
            }
        }
    }
    return output;
}