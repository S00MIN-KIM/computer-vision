##실행
Visual Studio 2019 ver 16.5.3
OpenCV 2.4.13.6
64bit, debug
--------------------------------------------------------------------------------------------------------------------------
##1.
MeanFilterGray.cpp
: Uniform mean filter is the simplest method of smoothing images, reducing the amount of intensity variation between one pixel and the next. (often used to reduce noise in images)
This program implement the mean filter to gray scale image. 
1) Mat meanfilter(const Mat input, int n, const char* opt)
: 
- parameter
	const Mat input: input image
	int n: filter kernel size is (2n+1)(2n+1)
	char* opt: the type of boundary processing(zero-padding, mirroring, adjustkernel)
- return output
--------------------------------------------------------------------------------------------------------------------------
##2. MeanFilterRGB.cpp
: Uniform mean filter is the simplest method of smoothing images, reducing the amount of intensity variation between one pixel and the next. (often used to reduce noise in images)
This program implements the mean filter to color image for each channel. 
1) Mat meanfilter(const Mat input, int n, const char* opt)
: function to implement the mean filter
- parameter
	const Mat input: input image
	int n: filter kernel size is (2n+1)(2n+1)
	char* opt: the type of boundary processing(zero-padding, mirroring, adjustkernel)
- return output
--------------------------------------------------------------------------------------------------------------------------
##3. 
gaussian_gray.cpp
: Gaussian filter is one of the most commonly used parametric models and considers spatial distances within neghborhoods.
This program implements the gaussian filter to gray scale image. 
1) Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt)
: function to implement the gaussian filter
- parameter
	const Mat input: input image
	int n: filter kernel size is (2n+1)(2n+1)
	float sigmaT: sigma value for y coordinate (equal to sigmaS usually)
	float sigmaS: sigma value for x coordinate (equal to sigmaT usually)
	char* opt: the type of boundary processing(zero-padding, mirroring, adjustkernel)
- return output
--------------------------------------------------------------------------------------------------------------------------
##4. 
gaussian_RGB.cpp
: Gaussian filter is one of the most commonly used parametric models and considers spatial distances within neghborhoods.
This program implements the gaussian filter to color image for each channel. 
1) Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt)
: function to implement the gaussian filter
- parameter
	const Mat input: input image
	int n: filter kernel size is (2n+1)(2n+1)
	float sigmaT: sigma value for y coordinate (equal to sigmaS usually)
	float sigmaS: sigma value for x coordinate (equal to sigmaT usually)
	char* opt: the type of boundary processing(zero-padding, mirroring, adjustkernel)
- return output
--------------------------------------------------------------------------------------------------------------------------
##5. 
sobel_gray.cpp
: sobel filter creates an image emphasising edges.
This program implements the sobel filter to gray scale image. 
1) Mat sobelfilter(const Mat input)
: function to implement the sobel filter
- parameter
	const Mat input: input image
- return output
--------------------------------------------------------------------------------------------------------------------------
##6. 
sobel_RGB.cpp
: sobel filter creates an image emphasising edges.
This program implements the sobel filter to color image for each channel. 
1) Mat sobelfilter(const Mat input)
: function to implement the sobel filter
- parameter
	const Mat input: input image
- return output
--------------------------------------------------------------------------------------------------------------------------
##7. 
laplacian_gray.cpp
: laplacian filter highlights regions of rapid intensity change and is therefore often used for edge detection.
This program implements the laplacian filter to gray scale image. 
1) Mat laplacianfilter(const Mat input)
: function to implement the laplacian filter
- parameter
	const Mat input: input image
- return output
--------------------------------------------------------------------------------------------------------------------------
##8. 
laplacian_RGB.cpp
: laplacian filter highlights regions of rapid intensity change and is therefore often used for edge detection.
This program implements the laplacian filter to color image for each channel. 
1) Mat laplacianfilter(const Mat input)
: function to implement the laplacian filter
- parameter
	const Mat input: input image
- return output
--------------------------------------------------------------------------------------------------------------------------
##9. 
separable_gaussian_gray.cpp
: Gaussian filter is one of the most commonly used parametric models and considers spatial distances within neghborhoods.
This program implements the separable gaussian filter to gray scale image. It is much faster than gaussian filter.
1) Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt)
: function to implement the separable gaussian filter
- parameter
	const Mat input: input image
	int n: filter kernel size is (2n+1)(2n+1)
	float sigmaT: sigma value for y coordinate (equal to sigmaS usually)
	float sigmaS: sigma value for x coordinate (equal to sigmaT usually)
	char* opt: the type of boundary processing(zero-padding, mirroring, adjustkernel)
- return output
--------------------------------------------------------------------------------------------------------------------------
##10. 
separable_gaussian_RGB.cpp
: Gaussian filter is one of the most commonly used parametric models and considers spatial distances within neghborhoods.
This program implements the separable gaussian filter to color image for each channel. It is much faster than gaussian filter.
1) Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt)
: function to implement the separable gaussian filter
- parameter
	const Mat input: input image
	int n: filter kernel size is (2n+1)(2n+1)
	float sigmaT: sigma value for y coordinate (equal to sigmaS usually)
	float sigmaS: sigma value for x coordinate (equal to sigmaT usually)
	char* opt: the type of boundary processing(zero-padding, mirroring, adjustkernel)
- return output
--------------------------------------------------------------------------------------------------------------------------
##11. 
unsharp_gray.cpp
: Unsharp masking makes an image look sharper by boosting high-frequency components.
This program implements the unsharp masking to gray scale image. 
1) Mat unsharpMask(const Mat input, int n, float sigmaT, float sigmaS, const char* opt)
: function to implement the gaussian filter
- parameter
	const Mat input: input image
	int n: filter kernel size is (2n+1)(2n+1)
	float sigmaT: sigma value for y coordinate (equal to sigmaS usually)
	float sigmaS: sigma value for x coordinate (equal to sigmaT usually)
	char* opt: the type of boundary processing(zero-padding, mirroring, adjustkernel)
- return (input-k*ouput)/(1-k)
--------------------------------------------------------------------------------------------------------------------------
##12. 
unsharp_RGB.cpp
: Unsharp masking makes an image look sharper by boosting high-frequency components.
This program implements the unsharp masking to color image for each channel. 
1) Mat unsharpMask(const Mat input, int n, float sigmaT, float sigmaS, const char* opt)
: function to implement the gaussian filter
- parameter
	const Mat input: input image
	int n: filter kernel size is (2n+1)(2n+1)
	float sigmaT: sigma value for y coordinate (equal to sigmaS usually)
	float sigmaS: sigma value for x coordinate (equal to sigmaT usually)
	char* opt: the type of boundary processing(zero-padding, mirroring, adjustkernel)
- return (input-k*ouput)/(1-k)