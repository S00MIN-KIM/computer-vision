##실행
Visual Studio 2019 ver 16.5.3
OpenCV 2.4.13.6
64bit, debug
--------------------------------------------------------------------------------------------------------------------------
##1.
hist_func.h
: header of source codes using histogram

1) float* cal_PDF(Mat& input)
: function to generate PDF for single channel image
- parameter
	Mat& input: matrix of input image
- return PDF

2) float** cal_PDF_RGB(Mat& input)
:  function to generate PDF for color image
- parameter 
	Mat& input: matrix of input image
- return PDF

3) float* cal_CDF(Mat& input)
: function to generate CDF for single channel image
- parameter 
	Mat& input: matrix of input image
- return CDF

4) float** cal_CDF_RGB(Mat& input)
: function to generate CDF for color channel image
- parameter 
	Mat& input: matrix of input image
- return CDF
	
---------------------------------------------------------------------------------------------------------------------------
##2. PDF/CDF
PDF_CDF.cpp
: program that generate PDF and CDF for input grayscale image

1) int main()
: read the input image and convert to grayscale. invoke the cal_PDF and cal_CDF function from hist_func.h
and get PDF and CDF for the image. Write the text file for PDF and CDF. Show the original image and grayscale image.
	
---------------------------------------------------------------------------------------------------------------------------
##3. Histogram Stretching
hist_stretching.cpp
: program to perform histogram stretching on the input image. 

1) int main()
: read the input image and convert to grayscale. invoke the cal_PDF and cal_CDF function from hist_func.h
and get PDF and CDF for the image. call the linear_stretching function and change the intensity ranges of the input image by transfer function.
Write the text file for PDF, stretched image PDF, and transfer function. Show the original image and stretched image.
	
2) void linear_stretching(Mat& input, Mat& stretched, G* trans_func, G x1, G x2, G y1, G y2)
: function to do linear_stretching
- parameter
	Mat& input: matrix of input image
	Mat& stretched: matrix of stretched image
	G* trans_func: transfer function to perform stretching
	G x1, G x2: intensity range to be changed
	G y1, G y2: changed intensity range

---------------------------------------------------------------------------------------------------------------------------
##4. Histogram Equalization(grayscale)
hist_eq.cpp
: program to perform histogram equalization for single channel image

1) int main()
: read the input image and convert to grayscale. invoke the cal_PDF and cal_CDF function from hist_func.h
and get PDF and CDF for the image. call the hist_eq function and change the intensity values of the input image to uniform probability by transfer function.
Write the text file for PDF, equalized image PDF, and transfer function. Show the original image and equalized image.
	
2) void hist_eq(Mat& input, Mat& equalized, G* trans_func, float *CDF)
: function to do histogram equalization
- parameter
	Mat& input: matrix of input image
	Mat& stretched: matrix of equalized image
	G* trans_func: transfer function to perform equalization
	float *CDF: CDF for the input image

---------------------------------------------------------------------------------------------------------------------------
##5. Histogram Equalization(RGB)
hist_eq_RGB.cpp
: program to perform histogram equalization for color image

1) int main()
: read the input image and invoke the cal_PDF_RGB and cal_CDF_RGB function from hist_func.h and get PDF and CDF for the image. 
call the hist_eq function and change the intensity values of the input image to uniform probability by transfer function on each channel.
Write the text file for PDF, equalized image PDF, and transfer function. Show the original image and equalized image.

2) void hist_eq(Mat& input, Mat& equalized, G* (trans_func)[3], float **CDF)
: function to do histogram equalization
- parameter
	Mat& input: matrix of input image
	Mat& stretched: matrix of equalized image
	G* (trans_func)[3]: transfer function to perform equalization
	float **CDF: CDF for the input image

---------------------------------------------------------------------------------------------------------------------------
##6. Histogram Equalization(YUV)
hist_eq_YUV.cpp
: program to perform histogram equalization for color image

1) int main()
: read the input image and convert to YUV. split each channel. 
invoke the cal_PDF_RGB for the image and cal_CDF function for the Y channel from hist_func.h.
call the hist_eq function and change the intensity values of the input image to uniform probability by transfer function on Y channel only.
merge three channels and convert equalized Y and UV to the color image (RGB). 
Write the text file for PDF, equalized image PDF, and transfer function. Show the original image and equalized image.

2) void hist_eq(Mat& input, Mat& equalized, G* trans_func, float *CDF)
: function to do histogram equalization
- parameter
	Mat& input: matrix of input image
	Mat& stretched: matrix of equalized image
	G* trans_func: transfer function to perform equalization
	float *CDF: CDF for the input image

---------------------------------------------------------------------------------------------------------------------------
##7. Histogram matching(grayscale)
hist_matching_gray.cpp
: program to perform histogram matching for grayscale image

1) int main()
: read the input image and reference image and convert them to grayscale. 
invoke the cal_PDF for the input image, and cal_CDF function for the input and reference image each.
set the transfer function T(r) and G(r). (L-1)* each CDF
find the final transfer function to apply the intensity mapping 
Perform the histogram matching by the transfer function arr[]. 
Write the text file for PDF, matched image PDF, and transfer function. Show the original image, reference image, and matched image.

---------------------------------------------------------------------------------------------------------------------------
##8. Histogram matching(color)
hist_matching_color.cpp
: program to perform histogram matching for color image

1) int main()
: read the input image and reference image and convert them to YUV. split each channel. 
invoke the cal_PDF_RGB  for the input image, and cal_CDF function for the Y channel of input and reference image each.
set the transfer function T(r) and G(r). (L-1)* each CDF
find the final transfer function to apply the intensity mapping 
Perform the histogram matching on Y channel by the transfer function arr[]. 
merge three channels and convert matched Y and UV to the color image (RGB). 
Write the text file for PDF, matched image PDF, and transfer function. Show the original image, reference image, and matched image.
