##실행
Visual Studio 2019 ver 16.5.3
OpenCV 2.4.13.6
64bit, Release
--------------------------------------------------------------------------------------------------------------------------
##1. SIFT.cpp
: SIFT descriptor that match two images by feature
1) double euclidDistance(Mat& vec1, Mat& vec2)
: calculate euclid distance between vec1 and vec2
- parameter
	Mat& vec1: point1
	Mat& vec2: point2
- return distance

2) int nearestNeighbor(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors)
: find the index of nearest neighbor from keypoints
- parameter
	Mat& vec: one point of descriptors of imageA
	vector<KeyPoint>& keypoints: keypoints of imageB
	Mat& descriptors: descriptors of imageB
- return index

3) int nearestNeighbor2(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors)
: find the index of second nearest neighbor from key points
- parameter
	Mat& vec: one point of descriptors of imageA
	vector<KeyPoint>& keypoints: keypoints of imageB
	Mat& descriptors: descriptors of imageB
- return index

4) void findPairs(vector<KeyPoint>& keypoints1, Mat& descriptors1,
	vector<KeyPoint>& keypoints2, Mat& descriptors2,
	vector<Point2f>& srcPoints, vector<Point2f>& dstPoints, bool crossCheck, bool ratio_threshold)
: Find pairs of points with the smallest distance between keypoints
- parameter
	vector<KeyPoint>& keypoints1: keypoints of imageA
	Mat& descriptors1: descriptors of imageA
	vector<KeyPoint>& keypoints2: keypoints of imageB
	Mat& descriptors2: descriptors of imageB
	vector<Point2f>& srcPoints: output (pairs of imageA)
	vector<Point2f>& dstPoints: output (pairs of imageB)
	bool crossCheck: whether do cross-checking or not
	bool ratio_threshold: whether do ratio based threshoing or not