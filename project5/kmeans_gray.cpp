#include <iostream>
#include <opencv2/opencv.hpp>

#define IM_TYPE   CV_8UC3

using namespace cv;
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

int main() {

    Mat input = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);
    Mat output;

    Mat input_gray;
    cvtColor(input, input_gray, CV_RGB2GRAY);

    if (!input.data)
    {
        std::cout << "Could not open" << std::endl;
        return -1;
    }

    namedWindow("Grayscale", WINDOW_AUTOSIZE);
    imshow("Grayscale", input_gray);

    Mat samples(input_gray.rows * input_gray.cols, 1, CV_32F);
    for (int y = 0; y < input_gray.rows; y++)
        for (int x = 0; x < input_gray.cols; x++)
            samples.at<float>(y + x * input_gray.rows, 0) = input_gray.at<G>(y, x);

    // Note that the intensity value is not normalized here (0~1). You should normalize both intensity and position when using them simultaneously.
    int clusterCount = 10;
    Mat labels;
    int attempts = 5;
    Mat centers;
    kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);

    Mat new_image(input_gray.size(), input_gray.type());

    for (int y = 0; y < input_gray.rows; y++)
        for (int x = 0; x < input_gray.cols; x++)
        {
            int cluster_idx = labels.at<int>(y + x * input_gray.rows, 0);
            //Fill code that finds for each pixel of each channel of the output image the intensity of the cluster center.
            new_image.at <G>(y, x) = centers.at<float>(cluster_idx, 0);

        }

    imshow("clustered image", new_image);
    waitKey(0);

    return 0;
}