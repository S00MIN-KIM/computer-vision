#include <iostream>
#include <opencv2/opencv.hpp>

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

// Note that this code is for the case when an input data is a color value.
int main() {
    Mat input = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);
    Mat output;
    Mat input_gray;
    cvtColor(input, input_gray, CV_RGB2GRAY);   // convert RGB to Grayscale

    if (!input.data)
    {
        std::cout << "Could not open" << std::endl;
        return -1;
    }

    namedWindow("Grayscale", WINDOW_AUTOSIZE);
    imshow("Grayscale", input_gray);

    Mat samples(input_gray.rows * input_gray.cols, 3, CV_32F);
    for (int y = 0; y < input_gray.rows; y++)
        for (int x = 0; x < input_gray.cols; x++) {

            samples.at<float>(y + x * input_gray.rows, 0) = (float)(input_gray.at<G>(y, x)) / 255;
            samples.at<float>(y + x * input_gray.rows, 1) = (float)(y) / input_gray.rows / 2;
            samples.at<float>(y + x * input_gray.rows, 2) = (float)(x) / input_gray.cols / 2;
        }

    int clusterCount = 10;
    Mat labels;
    int attempts = 5;
    Mat centers;
    kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);

    Mat new_image(input_gray.size(), input_gray.type());
    for (int y = 0; y < input_gray.rows; y++)
        for (int x = 0; x < input_gray.cols; x++)
        {
            int cluster_idx = labels.at<int>(y + x * input.rows, 0);
            //Fill code that finds for each pixel of each channel of the output image the intensity of the cluster center.
            new_image.at<G>(y, x) = (centers.at<float>(cluster_idx, 0)) * 255;
        }
    imshow("clustered image", new_image);
    waitKey(0);
    return 0;
}
