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
    output = gaussianfilter(input_gray, 3, 3, 3, "adjustkernel"); //Boundary process: zero-paddle, mirroring, adjustkernel

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


    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {

            float sum1 = 0.0;

            if (!strcmp(opt, "zero-paddle")) {
                for (int a = -n; a <= n; a++) {
                    for (int b = -n; b <= n; b++) {

                        /* Gaussian filter with Zero-paddle boundary process:

                        Fill the code:
                        */
                        if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
                            //if the pixel is not a border pixel

                            sum1 += kernels.at<float>(a + n, 0) * kernelt.at<float>(0, b + n) * (float)(input.at<G>(i + a, j + b));
                        }
                    }
                }
                output.at<G>(i, j) = (G)sum1;
            }

            else if (!strcmp(opt, "mirroring")) {
                for (int a = -n; a <= n; a++) {
                    for (int b = -n; b <= n; b++) {

                        /* Gaussian filter with "mirroring" process:

                        Fill the code:
                        */
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
                        sum1 += kernels.at<float>(a + n, 0) * kernelt.at<float>(0, b + n) * (float)(input.at<G>(tempa, tempb));

                    }
                }
                output.at<G>(i, j) = (G)sum1;
            }




            else if (!strcmp(opt, "adjustkernel")) {
                float sum1 = 0.0;
                float sum2 = 0.0;
                for (int a = -n; a <= n; a++) {
                    for (int b = -n; b <= n; b++) {

                        /* Gaussian filter with "adjustkernel" process:

                        Fill the code:
                        */
                        if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
                            sum1 += kernels.at<float>(a + n, 0) * kernelt.at<float>(0, b + n) * (float)(input.at<G>(i + a, j + b));
                            sum2 += kernels.at<float>(a + n, 0) * kernelt.at<float>(0, b + n);
                        }
                    }
                }
                output.at<G>(i, j) = (G)(sum1 / sum2);
            }
        }
    }
    return output;
}