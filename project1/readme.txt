##실행
Visual Studio 2019 ver 16.5.3
OpenCV 2.4.13.6
64bit, debug
--------------------------------------------------------------------------------------------------------------------------
##Project 1.
rotate.cpp
: program that loads an image and rotates it.

1) int main
: read an image and show it. rotate the image by invoking myrotate function and show the rotated one. 
two window will appear when debug the program; the original one and rotated one

2) Mat myrotate(const Mat input, float angle, const char* opt)
: function that rotates the input image and returns the result
- parameter
	const Mat input: original image to rotate
	float angle: angle to ratate
	const char* opt: chosen interpolation type
- return the rotated image
---------------------------------------------------------------------------------------------------------------------------
##Project 2.
stitching.cpp
: program that combines two images with overlapping

1) int main
: read an image set corresponding pixels. Estimate affine transformation by invokig cal_affine and compute boundary for merged image.
calculate the size of a final merged image and initialize it. do inverse warping with bilinear interpolation. 
finally, do image stitching with blend. 

2) Mat cal_affine(int ptl_x[], int ptl_y[], int ptr_x[], int ptr_y[], int number_of_points)
: function to get the affine matrix
- parameter
	int ptl_x[], ptl_y[], ptr_x[], ptr_y[]: corresponding pixels
	int number_of_points: # of corresponding points
- return affine matrix

3) blend_stitching(const Mat I1, const Mat I2, Mat& I_f, int bound_l, int bound_u, float alpha)
: function to do image stitching with blend. 
- parameter
	const Mat I1: left image 
	const Mat I2: right image
	Mat& I_f: final image(use address to be changable)
	int bound_l: boundary for final image which is less than 0
	int bound_u: boundary for final image which is less than 0
	float alpha: given float (0~1)
	


