#include <opencv2/opencv.hpp>
#include <stdio.h>

#define IM_TYPE   CV_64FC3

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

Mat Add_Gaussian_noise(const Mat input, double mean, double sigma);
Mat Bilateralfilter_Gray(const Mat input, int n, double sigma_t, double sigma_s, double sigma_r, const char* opt);
Mat Bilateralfilter_RGB(const Mat input, int n, double sigma_t, double sigma_s, double sigma_r, const char* opt);

int main() {

    Mat input = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);
    Mat input_gray;

    // check for validation
    if (!input.data) {
        printf("Could not open\n");
        return -1;
    }

    cvtColor(input, input_gray, CV_RGB2GRAY);   // convert RGB to Grayscale

    // 8-bit unsigned char -> 64-bit floating point
    input.convertTo(input, CV_64FC3, 1.0 / 255);
    input_gray.convertTo(input_gray, CV_64F, 1.0 / 255);

    // Add noise to original image
    Mat noise_Gray = Add_Gaussian_noise(input_gray, 0, 0.1);
    Mat noise_RGB = Add_Gaussian_noise(input, 0, 0.1);

    // Denoise, using gaussian filter
    Mat Denoised_Gray = Bilateralfilter_Gray(noise_Gray, 3, 10, 10, 0.2, "adjustkernel");
    Mat Denoised_RGB = Bilateralfilter_RGB(noise_RGB, 3, 10, 10, 0.2, "mirroring");

    namedWindow("Grayscale", WINDOW_AUTOSIZE);
    imshow("Grayscale", input_gray);

    namedWindow("RGB", WINDOW_AUTOSIZE);
    imshow("RGB", input);

    namedWindow("Gaussian Noise (Grayscale)", WINDOW_AUTOSIZE);
    imshow("Gaussian Noise (Grayscale)", noise_Gray);

    namedWindow("Gaussian Noise (RGB)", WINDOW_AUTOSIZE);
    imshow("Gaussian Noise (RGB)", noise_RGB);

    namedWindow("Denoised (Grayscale)", WINDOW_AUTOSIZE);
    imshow("Denoised (Grayscale)", Denoised_Gray);

    namedWindow("Denoised (RGB)", WINDOW_AUTOSIZE);
    imshow("Denoised (RGB)", Denoised_RGB);

    waitKey(0);

    return 0;
}

Mat Add_Gaussian_noise(const Mat input, double mean, double sigma) {

    Mat NoiseArr = Mat::zeros(input.rows, input.cols, input.type());
    RNG rng;
    rng.fill(NoiseArr, RNG::NORMAL, mean, sigma);

    add(input, NoiseArr, NoiseArr);

    return NoiseArr;
}

Mat Bilateralfilter_Gray(const Mat input, int n, double sigma_t, double sigma_s, double sigma_r, const char* opt) {

    int row = input.rows;
    int col = input.cols;
    int kernel_size = (2 * n + 1);
    int tempa;
    int tempb;
    float denom = 0.0;

    // Initialiazing Gaussian Kernel Matrix
    // Fill code to initialize Gaussian filter kernel matrix

    Mat output = Mat::zeros(row, col, input.type());

    Mat kernel;
    kernel = Mat::zeros(kernel_size, kernel_size, CV_32F);

    Mat mat;
    mat = Mat::zeros(row, col, CV_32F);


    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            denom = 0.0;
            for (int a = -n; a <= n; a++) {
                for (int b = -n; b <= n; b++) {
                    if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {

                        float value = exp(-(pow(a, 2) / (2 * pow(sigma_s, 2))) - (pow(b, 2) / (2 * pow(sigma_t, 2)))) * exp((-(pow((input.at<G>(i, j) - input.at<G>(i + a, j + b)), 2)) / (2 * pow(sigma_r, 2))));

                        denom += value;
                    }

                    else {
                        float value1 = (float)(exp(-(pow(a, 2) / (2 * pow(sigma_s, 2))) - (pow(b, 2) / (2 * pow(sigma_t, 2)))));
                        //kernel.at<float>(a + n, b + n) = value1;
                        denom += value1;
                    }
                }
            }

            mat.at<float>(i, j) = denom;

        }
    }



    // convolution
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {

            if (!strcmp(opt, "zero-padding")) {
                float sum1 = 0.0;
                for (int a = -n; a <= n; a++) {
                    for (int b = -n; b <= n; b++) {

                        if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
                            float value1 = exp(-(pow(a, 2) / (2 * pow(sigma_s, 2))) - (pow(b, 2) / (2 * pow(sigma_t, 2)))) * exp((-(pow((input.at<G>(i, j) - input.at<G>(i + a, j + b)), 2) / (2 * pow(sigma_r, 2)))));
                            value1 = value1 / mat.at<float>(i, j);
                            kernel.at<float>(a + n, b + n) = value1;
                            sum1 += kernel.at<float>(a + n, b + n) * (float)(input.at<G>(i + a, j + b));
                        }
                    }
                }
                output.at<G>(i, j) = sum1;

            }

            else if (!strcmp(opt, "mirroring")) {
                float sum1 = 0.0;
                for (int a = -n; a <= n; a++) {
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
                        float value1 = exp(-(pow(a, 2) / (2 * pow(sigma_s, 2))) - (pow(b, 2) / (2 * pow(sigma_t, 2)))) * exp((-(pow((input.at<G>(i, j) - input.at<G>(tempa, tempb)), 2) / (2 * pow(sigma_r, 2)))));
                        value1 = value1 / mat.at<float>(i, j);
                        kernel.at<float>(a + n, b + n) = value1;
                        sum1 += kernel.at<float>(a + n, b + n) * (float)(input.at<G>(tempa, tempb));
                    }
                }
                output.at<G>(i, j) = (G)sum1;

            }

            else if (!strcmp(opt, "adjustkernel")) {
                float sum1 = 0.0;
                float sum2 = 0.0;
                for (int a = -n; a <= n; a++) {
                    for (int b = -n; b <= n; b++) {


                        if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
                            float value1 = exp(-(pow(a, 2) / (2 * pow(sigma_s, 2))) - (pow(b, 2) / (2 * pow(sigma_t, 2)))) * exp((-(pow((input.at<G>(i, j) - input.at<G>(i + a, j + b)), 2) / (2 * pow(sigma_r, 2)))));
                            value1 = value1 / mat.at<float>(i, j);
                            kernel.at<float>(a + n, b + n) = value1;
                            sum1 += kernel.at<float>(a + n, b + n) * (float)(input.at<G>(i + a, j + b));
                            sum2 += kernel.at<float>(a + n, b + n);
                        }
                    }
                }
                output.at<G>(i, j) = (G)(sum1 / sum2);
            }

        }
    }

    return output;
}

Mat Bilateralfilter_RGB(const Mat input, int n, double sigma_t, double sigma_s, double sigma_r, const char* opt) {

    Mat kernel;

    int row = input.rows;
    int col = input.cols;
    int kernel_size = (2 * n + 1);
    int tempa;
    int tempb;
    float denom;

    // Initialiazing Gaussian Kernel Matrix
    // Fill code to initialize Gaussian filter kernel matrix
    //kernel = Mat::ones(kernel_size, kernel_size, CV_32F) / (float)(kernel_size * kernel_size);

    kernel = Mat::zeros(kernel_size, kernel_size, CV_32F);

    Mat mat;
    mat = Mat::zeros(row, col, CV_32F);



    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            denom = 0.0;
            for (int a = -n; a <= n; a++) {
                for (int b = -n; b <= n; b++) {
                    if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {

                        float value = exp(-(pow(a, 2) / (2 * pow(sigma_s, 2))) - (pow(b, 2) / (2 * pow(sigma_t, 2)))) * exp((-(pow((input.at<C>(i, j)[1] - input.at<C>(i + a, j + b)[1]), 2)) / (2 * pow(sigma_r, 2))));

                        denom += value;
                    }

                    else {
                        float value1 = (float)(exp(-(pow(a, 2) / (2 * pow(sigma_s, 2))) - (pow(b, 2) / (2 * pow(sigma_t, 2)))));
                        //kernel.at<float>(a + n, b + n) = value1;
                        denom += value1;
                    }

                }
            }

            mat.at<float>(i, j) = denom;

        }
    }
    Mat output = Mat::zeros(row, col, input.type());

    // convolution
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {

            if (!strcmp(opt, "zero-padding")) {
                float sum1_r = 0.0;
                float sum1_g = 0.0;
                float sum1_b = 0.0;
                for (int a = -n; a <= n; a++) { // for each kernel window
                    for (int b = -n; b <= n; b++) {

                        if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
                            float value1 = exp(-(pow(a, 2) / (2 * pow(sigma_s, 2))) - (pow(b, 2) / (2 * pow(sigma_t, 2)))) * exp((-(pow((input.at<C>(i, j)[1] - input.at<C>(i + a, j + b)[1]), 2) / (2 * pow(sigma_r, 2)))));
                            value1 = value1 / mat.at<float>(i, j);
                            kernel.at<float>(a + n, b + n) = value1;
                            sum1_r += kernel.at<float>(a + n, b + n) * (float)(input.at<C>(i + a, j + b)[0]);
                            sum1_g += kernel.at<float>(a + n, b + n) * (float)(input.at<C>(i + a, j + b)[1]);
                            sum1_b += kernel.at<float>(a + n, b + n) * (float)(input.at<C>(i + a, j + b)[2]);
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
                        float value1 = exp(-(pow(a, 2) / (2 * pow(sigma_s, 2))) - (pow(b, 2) / (2 * pow(sigma_t, 2)))) * exp((-(pow((input.at<C>(i, j)[1] - input.at<C>(tempa, tempb)[1]), 2) / (2 * pow(sigma_r, 2)))));
                        value1 = value1 / mat.at<float>(i, j);
                        kernel.at<float>(a + n, b + n) = value1;
                        //  sum1 += small_w.at<float>(a + n, b + n) * (float)(input.at<G>(i + a, j + b));

                        sum1_r += kernel.at<float>(a + n, b + n) * (float)(input.at<C>(tempa, tempb)[0]);
                        sum1_g += kernel.at<float>(a + n, b + n) * (float)(input.at<C>(tempa, tempb)[1]);
                        sum1_b += kernel.at<float>(a + n, b + n) * (float)(input.at<C>(tempa, tempb)[2]);
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
                            float value1 = exp(-(pow(a, 2) / (2 * pow(sigma_s, 2))) - (pow(b, 2) / (2 * pow(sigma_t, 2)))) * exp((-(pow((input.at<C>(i, j)[1] - input.at<C>(i + a, j + b)[1]), 2) / (2 * pow(sigma_r, 2)))));
                            value1 = value1 / mat.at<float>(i, j);
                            kernel.at<float>(a + n, b + n) = value1;

                            sum1_r += kernel.at<float>(a + n, b + n) * (float)(input.at<C>(i + a, j + b)[0]);
                            sum1_g += kernel.at<float>(a + n, b + n) * (float)(input.at<C>(i + a, j + b)[1]);
                            sum1_b += kernel.at<float>(a + n, b + n) * (float)(input.at<C>(i + a, j + b)[2]);
                            sum2 += kernel.at<float>(a + n, b + n);
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