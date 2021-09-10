##실행
Visual Studio 2019 ver 16.5.3
OpenCV 2.4.13.6
64bit, Release
--------------------------------------------------------------------------------------------------------------------------
##1. SIFT+RANSAC+affine+stitching.cpp
: image stitching using affine transform based on SIFT (and RANSAC)
#if true, apply RANSAC, #else, do not apply RANSAC

1) #define RATIO_THR 0.7 : ratio-based threshold used while running SIFT
2) #define THR 0.2 : threshold used when finding inliers 
3) #define K 3 : number of sampled points
4) Mat cal_affine(vector<float>& ptl_x, vector<float>& ptl_y, vector<float>& ptr_x, vector<float>& ptr_y, int number_of_points)
: function to get the affine matrix
- parameter
	vector<float>& ptl_x, vector<float>& ptl_y, vector<float>& ptr_x, vector<float>& ptr_y: corresponding pixels
	int number_of_points: # of corresponding points
- return affine matrix

5) void blend_stitching(const Mat I1, const Mat I2, Mat& I_f, int diff_x, int diff_y, float alpha)
: function to do image stitching with blend. 
- parameter
	const Mat I1: left image 
	const Mat I2: right image
	Mat& I_f: final image(use address to be changable)
	int bound_l: boundary for final image which is less than 0
	int bound_u: boundary for final image which is less than 0
	float alpha: given float (0~1)
6) double euclidDistance(Mat& vec1, Mat& vec2)
: calculate euclid distance between vec1 and vec2
- parameter
	Mat& vec1: point1
	Mat& vec2: point2
- return distance
7) int nearestNeighbor(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors)
: find the index of nearest neighbor from keypoints
- parameter
	Mat& vec: one point of descriptors of imageA
	vector<KeyPoint>& keypoints: keypoints of imageB
	Mat& descriptors: descriptors of imageB
- return index
8) int nearestNeighbor2(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors)
: find the index of second nearest neighbor from key points
- parameter
	Mat& vec: one point of descriptors of imageA
	vector<KeyPoint>& keypoints: keypoints of imageB
	Mat& descriptors: descriptors of imageB
- return index
9) void findPairs(vector<KeyPoint>& keypoints1, Mat& descriptors1, vector<KeyPoint>& keypoints2, Mat& descriptors2,
	bool crossCheck, bool ratio_threshold)
: Find pairs of points with the smallest distance between keypoints
- parameter
	vector<KeyPoint>& keypoints1: keypoints of imageA
	Mat& descriptors1: descriptors of imageA
	vector<KeyPoint>& keypoints2: keypoints of imageB
	Mat& descriptors2: descriptors of imageB
	bool crossCheck: whether do cross-checking or not
	bool ratio_threshold: whether do ratio based threshoing or not
--------------------------------------------------------------------------------------------------------------------------
##2. hough.cpp
: line fitting by canny edge detector and hough transform
#if true, apply Houghlines. #else, apply HoughlinesP
1) Canny(InputArray image, OutputArray edges, double threshold1, double threshold2, int apertureSize = 3, bool L2gradient = false)
: Finds edges in an image using the Canny algorithm with custom image gradient.
2) HoughLines(InputArray image, OutputArray lines, double rho, double theta, int threshold, 
double srn = 0, double stn = 0, double min_theta = 0, double max_theta = CV_PI )
: The function implements the standard or standard multi-scale Hough transform algorithm for line detection.
3) HoughLinesP(InputArray image, OutputArray lines, double rho, double theta, int threshold, double minLineLength = 0, double maxLineGap = 0)
: The function implements the probabilistic Hough transform algorithm for line detection