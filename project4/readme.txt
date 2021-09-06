##실행
Visual Studio 2019 ver 16.5.3
OpenCV 2.4.13.6
64bit, debug
--------------------------------------------------------------------------------------------------------------------------
##1.
Salt_and_Pepper_Noise_Removal.cpp
: Program that generates an image (both gray scale and color) with the salt-and-pepper noise, 
and removes the salt-and-pepper noise with the median filtering
1) Mat Add_salt_pepper_Noise(const Mat input, float ps, float pp)
: function to generate an image (both gray scale and color) with the salt-and-pepper noise
- parameter
	const Mat input: input image
	float ps: density of salt noise (0~1)
	float pp: density of pepper noise (0~1)
- return output

2) Mat Salt_pepper_noise_removal_Gray(const Mat input, int n, const char* opt)
: function to remove the salt-and-pepper noise with the median filtering on gray scale image
- parameter
	const Mat input: input image
	int n: filter kernel size is (2n+1)(2n+1)
	char* opt: the type of boundary processing(zero-padding, mirroring, adjustkernel)
- return output

3) Mat Salt_pepper_noise_removal_RGB(const Mat input, int n, const char* opt)
: function to remove the salt-and-pepper noise with the median filtering on color image
- parameter
	const Mat input: input image
	int n: filter kernel size is (2n+1)(2n+1)
	char* opt: the type of boundary processing(zero-padding, mirroring, adjustkernel)
- return output
--------------------------------------------------------------------------------------------------------------------------
##2. 
Gaussian_Noise_Removal_Gaussian_filtering.cpp
: Program that generates an image with the Gaussian noise,
and removes the Gaussian noise with the Gaussian filtering
1) Mat Add_Gaussian_noise(const Mat input, double mean, double sigma)
: function to generate an image with the Gaussian noise
- parameter
	const Mat input: input image
	double mean: mean
	double sigma: standard deviation
- return output

2) Mat Gaussianfilter_Gray(const Mat input, int n, double sigma_t, double sigma_s, const char* opt)
: function to remove the Gaussian noise with the Gaussian filtering on single channel image
- parameter
	const Mat input: input image
	int n: filter kernel size is (2n+1)(2n+1)
	float sigma_t: Standard deviation  for y coordinate 
	float sigma_s: Standard deviation  for x coordinate 
	char* opt: the type of boundary processing(zero-padding, mirroring, adjustkernel)
- return output

3) Mat Gaussianfilter_RGB(const Mat input, int n, double sigma_t, double sigma_s, const char* opt)
: function to remove the Gaussian noise with the Gaussian filtering on each RGB channel in color image
- parameter
const Mat input: input image
	int n: filter kernel size is (2n+1)(2n+1)
	float sigma_t: Standard deviation  for y coordinate 
	float sigma_s: Standard deviation  for x coordinate 
	char* opt: the type of boundary processing(zero-padding, mirroring, adjustkernel)
- return output
--------------------------------------------------------------------------------------------------------------------------
##3. 
Gaussian_Noise_Removal_Bilateral_filtering.cpp
: program that generates an image with the Gaussian noise,
and removes the Gaussian noise with the bilateral filtering
1) Mat Add_Gaussian_noise(const Mat input, double mean, double sigma)
: function to generate an image with the Gaussian noise
- parameter
	const Mat input: input image
	double mean: mean
	double sigma: standard deviation
- return output

2) Mat Bilateralfilter_Gray(const Mat input, int n, double sigma_t, double sigma_s, double sigma_r, const char* opt)
: function to remove the Gaussian noise with the bilateral filtering on single channel image
- parameter
	const Mat input: input image
	int n: filter kernel size is (2n+1)(2n+1)
	float sigma_t: Standard deviation for y coordinate 
	float sigma_s: Standard deviation for x coordinate
	float sigma_r: Standard deviation for intensity
	char* opt: the type of boundary processing(zero-padding, mirroring, adjustkernel)
- return output

3) Mat Bilateralfilter_RGB(const Mat input, int n, double sigma_t, double sigma_s, double sigma_r, const char* opt)
: function to remove the Gaussian noise with the bilateral filtering on each RGB channel in color image
- parameter
	const Mat input: input image
	int n: filter kernel size is (2n+1)(2n+1)
	float sigma_t: Standard deviation  for y coordinate 
	float sigma_s: Standard deviation  for x coordinate 
	float sigma_r: Standard deviation  for color
	char* opt: the type of boundary processing(zero-padding, mirroring, adjustkernel)
- return output
