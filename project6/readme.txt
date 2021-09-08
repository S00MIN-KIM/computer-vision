##실행
Visual Studio 2019 ver 16.5.3
OpenCV 2.4.13.6
64bit, debug
--------------------------------------------------------------------------------------------------------------------------
##1. LoG.cpp
: Implement the Laplacian of Gaussian (grayscale image)

1) Mat get_Gaussian_Kernel(int n, double sigma_t, double sigma_s, bool normalize)
: set the gaussian filter
- parameter
	int n: Filter kernel size n -> (2n+1)(2n+1)
	double sigma_t: standard deviation for x coordinate
	double sigma_s: standard deviation for y coordinate
	bool normalize: boolean for choosing normalize or not
- return gaussian filter

2) Mat get_Laplacian_Kernel()
: set the laplacian filter
- return laplacian filter

3) Mat Gaussianfilter(const Mat input, int n, double sigma_t, double sigma_s)
: apply the gaussian filter into input image
- parameter
	int n: Filter kernel size n -> (2n+1)(2n+1)
	double sigma_t: standard deviation for x coordinate
	double sigma_s: standard deviation for y coordinate
- return image applied gaussian filter

4) Mat Laplacianfilter(const Mat input)
: apply the laplacian filter into input image
- parameter
	Mat input: input image
- return image applied laplacian filter

5) Mat Mirroring(const Mat input, int n)
: implement the boundary process mirroring
- parameter
	Mat input: input image
	int n: kernel size
- return  doundary processed image with (row+2n) * (low+2n) size
--------------------------------------------------------------------------------------------------------------------------
##2. LoG_rgb.cpp
: implement the laplacian of gaussian (color image)

1) Mat get_Gaussian_Kernel(int n, double sigma_t, double sigma_s, bool normalize)
: set the gaussian filter
- parameter
	int n: Filter kernel size n -> (2n+1)(2n+1)
	double sigma_t: standard deviation for x coordinate
	double sigma_s: standard deviation for y coordinate
	bool normalize: boolean for choosing normalize or not
- return gaussian filter

2) Mat get_Laplacian_Kernel()
: set the laplacian filter
- return laplacian filter

3) Mat Gaussianfilter(const Mat input, int n, double sigma_t, double sigma_s)
: apply the gaussian filter into input image
- parameter
	int n: Filter kernel size n -> (2n+1)(2n+1)
	double sigma_t: standard deviation for x coordinate
	double sigma_s: standard deviation for y coordinate
- return image applied gaussian filter

4) Mat Laplacianfilter(const Mat input)
: apply the laplacian filter into input image
- parameter
	Mat input: input image
- return image applied laplacian filter

5) Mat Mirroring(const Mat input, int n)
: implement the boundary process mirroring
- parameter
	Mat input: input image
	int n: kernel size
- return  doundary processed image with (row+2n) * (low+2n) size

--------------------------------------------------------------------------------------------------------------------------
##3. Canny.cpp
: implement canny edge detector

Canny(InputArray image, OutputArray edges, double threshold1, double threshold2, int apertureSize = 3, bool L2gradient = false)
: provided by opencv
--------------------------------------------------------------------------------------------------------------------------
##4. Harris_corner.cpp
: implement harris corner detector

1) cornerHarris(InputArray src, OutputArray dst, int blockSize, int ksize, double k, int borderType = BORDER_DEFAULT );
: provided by opencv

2) vector<Point2f> MatToVec(const Mat input);
: change matrix to vector
- parameter
	const Mat input: input matrix
- return vector

3) Mat NonMaximum_Suppression(const Mat input, Mat corner_mat, int radius);
: implement the non maximum suppression
- parameter
	const Mat input: input image
	Mat corner_mat: matrix of detected corner
	int radius: range of finding maximum value
- return input

4) Mat Mirroring(const Mat input, int n)
: implement the boundary process mirroring
- parameter
	const Mat input: input image
	int n: kernel size
- return doundary processed image with (row+2n) * (low+2n) size

5) void type2str(int type)
: print the type
