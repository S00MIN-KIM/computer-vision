#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>

#define RATIO_THR 0.4
#define THR 0.01
#define K 3 //3 or 4

using namespace std;
using namespace cv;


template <typename T>
Mat cal_affine(vector<float>& ptl_x, vector<float>& ptl_y, vector<float>& ptr_x, vector<float>& ptr_y, int number_of_points);

void blend_stitching(const Mat I1, const Mat I2, Mat& I_f, int diff_x, int diff_y, float alpha);

double euclidDistance(Mat& vec1, Mat& vec2);
int nearestNeighbor(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors);
void findPairs(vector<KeyPoint>& keypoints1, Mat& descriptors1,
    vector<KeyPoint>& keypoints2, Mat& descriptors2,
    bool crossCheck, bool ratio_threshold);

vector<float> srcPoints_x;
vector<float> dstPoints_x;
vector<float> srcPoints_y;
vector<float> dstPoints_y;

int main() {
    Mat I1, I2, I1_gray, I2_gray;

    // Read each image
    I1 = imread("input1.jpg");
    I2 = imread("input2.jpg");

    if (!I1.data || !I2.data)
    {
        std::cout << "Could not open" << std::endl;
        return -1;
    }

    //resize(input1, input1, Size(input1.cols / 2, input1.rows / 2));
    //resize(input2, input2, Size(input2.cols / 2, input2.rows / 2));

    cvtColor(I1, I1_gray, CV_RGB2GRAY);
    cvtColor(I2, I2_gray, CV_RGB2GRAY);

    FeatureDetector* detector = new SiftFeatureDetector(
        0,      // nFeatures
        4,      // nOctaveLayers
        0.04,   // contrastThreshold
        10,      // edgeThreshold
        1.6      // sigma
    );

    DescriptorExtractor* extractor = new SiftDescriptorExtractor();

    // Create a image for displaying mathing keypoints
    Size size = I2.size();
    Size sz = Size(size.width + I1_gray.size().width, max(size.height, I1_gray.size().height));
    Mat matchingImage = Mat::zeros(sz, CV_8UC3);

    I1.copyTo(matchingImage(Rect(size.width, 0, I1_gray.size().width, I1_gray.size().height)));
    I2.copyTo(matchingImage(Rect(0, 0, size.width, size.height)));

    // Compute keypoints and descriptor from the source image in advance
    vector<KeyPoint> keypoints1;
    Mat descriptors1;

    detector->detect(I1_gray, keypoints1);
    extractor->compute(I1_gray, keypoints1, descriptors1);
    printf("input1 : %d keypoints are found.\n", (int)keypoints1.size());

    vector<KeyPoint> keypoints2;
    Mat descriptors2;

    // Detect keypoints
    detector->detect(I2_gray, keypoints2);
    extractor->compute(I2_gray, keypoints2, descriptors2);

    printf("input2 : %zd keypoints are found.\n", keypoints2.size());


    // Find nearest neighbor pairs

    bool crossCheck = true;
    bool ratio_threshold = true;
    findPairs(keypoints2, descriptors2, keypoints1, descriptors1, crossCheck, ratio_threshold);
    printf("%zd keypoints are matched.\n", srcPoints_x.size());

    // height(row), width(col) of each image
    const float I1_row = I1.rows;
    const float I1_col = I1.cols;
    const float I2_row = I2.rows;
    const float I2_col = I2.cols;

    I1.convertTo(I1, CV_32FC3, 1.0 / 255);
    I2.convertTo(I2, CV_32FC3, 1.0 / 255);


#if 1 //if true,  apply RAMSAC (case2)

    ////////////RANSAC////////////
    Mat max_affine;
    int s = 10;
    int max_count = 0;
    srand(time(NULL));
    //random data
    vector<float> lpoint_x;
    vector<float> lpoint_y;
    vector<float> rpoint_x;
    vector<float> rpoint_y;
    for (int m = 0; m < s; m++) {

        int index[K] = { 0 };
        for (int i = 0; i < K; i++) {
            index[i] = rand() % dstPoints_x.size();
            for (int j = 0; j < i; j++)
                if (index[i] == index[j])
                    i--;
        }

        for (int i = 0; i < K; i++) {
            lpoint_x.push_back(srcPoints_x[index[i]]);
            lpoint_y.push_back(srcPoints_y[index[i]]);
            rpoint_x.push_back(dstPoints_x[index[i]]);
            rpoint_y.push_back(dstPoints_y[index[i]]);
        }

        Mat affineM = cal_affine<float>(lpoint_x, lpoint_y, rpoint_x, rpoint_y, K);

        //find the best affine matrix
        int count = 0;
        for (int p = 0; p < srcPoints_x.size(); p++) {
            float result = 0;
            float aff_ptr_x = affineM.at<float>(0) * (srcPoints_x[p]) + affineM.at<float>(1) * (srcPoints_x[p]) + affineM.at<float>(2);
            float aff_ptr_y = affineM.at<float>(3) * (srcPoints_y[p]) + affineM.at<float>(4) * (srcPoints_y[p]) + affineM.at<float>(5);
            result = pow(((aff_ptr_x - dstPoints_x[p]) / I2_row), 2) + pow(((aff_ptr_y - dstPoints_y[p]) / I2_col), 2);
            //printf("%f\n", result);
            if (result < pow(THR, 2))
                count++;
        }
        if (max_count < count) {
            max_count = count;
            max_affine = affineM;
        }

    }
    printf("max count: %d\n", max_count);
    if (max_count == 0) {
        printf("fail\n");
        return 0;
    }

    //find inlier
    vector<float> lin_x;
    vector<float> lin_y;
    vector<float> rin_x;
    vector<float> rin_y;
    for (int p = 0; p < srcPoints_x.size(); p++) {
        float aff_ptr_x = max_affine.at<float>(0) * srcPoints_x[p] + max_affine.at<float>(1) * srcPoints_x[p] + max_affine.at<float>(2);
        float aff_ptr_y = max_affine.at<float>(3) * srcPoints_y[p] + max_affine.at<float>(4) * srcPoints_y[p] + max_affine.at<float>(5);
        float result2 = pow(((aff_ptr_x - dstPoints_x[p]) / I2_row), 2) + pow(((aff_ptr_y - dstPoints_y[p]) / I2_col), 2);
        if (result2 < pow(THR, 2)) {
            lin_x.push_back(srcPoints_x[p]);
            lin_y.push_back(srcPoints_y[p]);
            rin_x.push_back(dstPoints_x[p]);
            rin_y.push_back(dstPoints_y[p]);
        }
    }
    // calculate affine Matrix A12, A21
    Mat A21 = cal_affine<float>(lin_x, lin_y, rin_x, rin_y, max_count);
    Mat A12 = cal_affine<float>(rin_x, rin_y, lin_x, lin_y, max_count);

#else //do not apply RANSAC (case1)

    // calculate affine Matrix A12, A21
    Mat A21 = cal_affine<float>(srcPoints_x, srcPoints_y, dstPoints_x, dstPoints_y, srcPoints_x.size());
    Mat A12 = cal_affine<float>(dstPoints_x, dstPoints_y, srcPoints_x, srcPoints_y, srcPoints_x.size());

#endif
    // compute corners (p1, p2, p3, p4)
    // p1: (0,0)
    // p2: (row, 0)
    // p3: (row, col)
    // p4: (0, col)
    Point2f p1(A21.at<float>(0) * 0 + A21.at<float>(1) * 0 + A21.at<float>(2), A21.at<float>(3) * 0 + A21.at<float>(4) * 0 + A21.at<float>(5));
    Point2f p2(A21.at<float>(0) * 0 + A21.at<float>(1) * I2_row + A21.at<float>(2), A21.at<float>(3) * 0 + A21.at<float>(4) * I2_row + A21.at<float>(5));
    Point2f p3(A21.at<float>(0) * I2_col + A21.at<float>(1) * I2_row + A21.at<float>(2), A21.at<float>(3) * I2_col + A21.at<float>(4) * I2_row + A21.at<float>(5));
    Point2f p4(A21.at<float>(0) * I2_col + A21.at<float>(1) * 0 + A21.at<float>(2), A21.at<float>(3) * I2_col + A21.at<float>(4) * 0 + A21.at<float>(5));

    // compute boundary for merged image(I_f)
    // bound_u <= 0
    // bound_b >= I1_row-1
    // bound_l <= 0
    // bound_r >= I1_col-1
    int bound_u = (int)round(min(0.0f, min(p1.y, p4.y)));
    int bound_b = (int)round(max(I1_row - 1, max(p2.y, p3.y)));
    int bound_l = (int)round(min(0.0f, min(p1.x, p2.x)));
    int bound_r = (int)round(max(I1_col - 1, max(p3.x, p4.x)));

    // initialize merged image
    //calculate the size of a final merged image by p1, p2, p3, p4 using A12
    //#of row, # of col
    Mat I_f(bound_b - bound_u + 1, bound_r - bound_l + 1, CV_32FC3, Scalar(0));

    // inverse warping with bilinear interplolation
    for (int i = bound_u; i <= bound_b; i++) {
        for (int j = bound_l; j <= bound_r; j++) {
            float x = A12.at<float>(0) * j + A12.at<float>(1) * i + A12.at<float>(2) - bound_l;
            float y = A12.at<float>(3) * j + A12.at<float>(4) * i + A12.at<float>(5) - bound_u;

            float y1 = floor(y);
            float y2 = ceil(y);
            float x1 = floor(x);
            float x2 = ceil(x);

            float mu = y - y1;
            float lambda = x - x1;

            if (x1 >= 0 && x2 < I2_col && y1 >= 0 && y2 < I2_row)
                I_f.at<Vec3f>(i - bound_u, j - bound_l) = lambda * (mu * I2.at<Vec3f>(y2, x2) + (1 - mu) * I2.at<Vec3f>(y1, x2)) +
                (1 - lambda) * (mu * I2.at<Vec3f>(y2, x1) + (1 - mu) * I2.at<Vec3f>(y1, x1));
        }
    }

    // image stitching with blend
    blend_stitching(I1, I2, I_f, bound_l, bound_u, 0.5);

    namedWindow("Left Image");
    imshow("Left Image", I1);

    namedWindow("Right Image");
    imshow("Right Image", I2);

    namedWindow("result");
    imshow("result", I_f);

    I_f.convertTo(I_f, CV_8UC3, 255.0);
    imwrite("result.png", I_f);

    waitKey(0);

    return 0;

}

/**
* Calculate euclid distance
*/
double euclidDistance(Mat& vec1, Mat& vec2) {
    double sum = 0.0;
    int dim = vec1.cols;
    for (int i = 0; i < dim; i++) {
        sum += (vec1.at<uchar>(0, i) - vec2.at<uchar>(0, i)) * (vec1.at<uchar>(0, i) - vec2.at<uchar>(0, i));
    }

    return sqrt(sum);
}

/**
* Find the index of nearest neighbor point from keypoints.
*/
int nearestNeighbor(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors) {
    int neighbor = -1;
    double minDist = 1e6;

    for (int i = 0; i < descriptors.rows; i++) {
        Mat v = descriptors.row(i);      // each row of descriptor

        //
        //   Fill the code
        //
        if (euclidDistance(vec, v) < minDist) {
            minDist = euclidDistance(vec, v);
            neighbor = i;
        }
    }

    return neighbor;
}

int nearestNeighbor2(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors) {
    int neighbor = -1;
    double minDist = 1e6;
    double neighbor2 = 0;

    for (int i = 0; i < descriptors.rows; i++) {
        Mat v = descriptors.row(i);   // each row of descriptor
        double dist = euclidDistance(vec, v);
        if (minDist > dist) {
            neighbor2 = neighbor;
            minDist = dist;
            neighbor = i;
        }

    }
    return neighbor2;
}

/**
* Find pairs of points with the smallest distace between them
*/
void findPairs(vector<KeyPoint>& keypoints1, Mat& descriptors1,
    vector<KeyPoint>& keypoints2, Mat& descriptors2,
    bool crossCheck, bool ratio_threshold) {
    for (int i = 0; i < descriptors1.rows; i++) {
        KeyPoint pt1 = keypoints1[i];
        Mat desc1 = descriptors1.row(i);

        int nn = nearestNeighbor(desc1, keypoints2, descriptors2);
        Mat desc2 = descriptors2.row(nn);

        // Refine matching points using ratio_based thresholding
        if (ratio_threshold) {
            //
            //   Fill the code
            //
            int nnnn = nearestNeighbor2(desc1, keypoints2, descriptors2);
            Mat desc3 = descriptors2.row(nnnn);
            double ratio;

            ratio = euclidDistance(desc1, desc2) / euclidDistance(desc1, desc3);
            if (ratio >= RATIO_THR) continue;
        }
        // Refine matching points using cross-checking
        if (crossCheck) {
            //
            //   Fill the code
            //
            int nnn = nearestNeighbor(desc2, keypoints1, descriptors1);
            if (nnn != i) continue;
        }

        KeyPoint pt2 = keypoints2[nn];
        srcPoints_x.push_back(pt1.pt.x);
        srcPoints_y.push_back(pt1.pt.y);
        dstPoints_x.push_back(pt2.pt.x);
        dstPoints_y.push_back(pt2.pt.y);
    }
}


template <typename T>
Mat cal_affine(vector<float>& ptl_x, vector<float>& ptl_y, vector<float>& ptr_x, vector<float>& ptr_y, int number_of_points) {

    Mat M(2 * number_of_points, 6, CV_32F, Scalar(0));
    Mat b(2 * number_of_points, 1, CV_32F);

    Mat M_trans, temp, affineM;

    // initialize matrix
    for (int i = 0; i < number_of_points; i++) {
        M.at<T>(2 * i, 0) = ptl_x[i];      M.at<T>(2 * i, 1) = ptl_y[i];      M.at<T>(2 * i, 2) = 1;
        M.at<T>(2 * i + 1, 3) = ptl_x[i];      M.at<T>(2 * i + 1, 4) = ptl_y[i];      M.at<T>(2 * i + 1, 5) = 1;
        b.at<T>(2 * i) = ptr_x[i];      b.at<T>(2 * i + 1) = ptr_y[i];
    }

    // (M^T * M)^(−1) * M^T * b ( * : Matrix multiplication)
    transpose(M, M_trans);
    invert(M_trans * M, temp);
    affineM = temp * M_trans * b;

    return affineM;
}

void blend_stitching(const Mat I1, const Mat I2, Mat& I_f, int bound_l, int bound_u, float alpha) {

    int col = I_f.cols;
    int row = I_f.rows;

    // I2 is already in I_f by inverse warping
    for (int i = 0; i < I1.rows; i++) {
        for (int j = 0; j < I1.cols; j++) {
            bool cond_I2 = I_f.at<Vec3f>(i - bound_u, j - bound_l) != Vec3f(0, 0, 0) ? true : false;

            if (cond_I2)
                I_f.at<Vec3f>(i - bound_u, j - bound_l) = alpha * I1.at<Vec3f>(i, j) + (1 - alpha) * I_f.at<Vec3f>(i - bound_u, j - bound_l);
            else
                I_f.at<Vec3f>(i - bound_u, j - bound_l) = I1.at<Vec3f>(i, j);

        }
    }
}