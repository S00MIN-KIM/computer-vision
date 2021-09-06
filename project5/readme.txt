##실행
Visual Studio 2019 ver 16.5.3
OpenCV 2.4.13.6
64bit, debug
--------------------------------------------------------------------------------------------------------------------------
##1.
adaptive.cpp
: Implement Adaptive Thresholding Using Moving Averages
1) Mat adaptive_thres(const Mat input, int n, float bnumber)
: segment the input grayscale image. each pixel in output image is 0 or 255
- parameter
	const Mat input: input image
	int n: filter kernel size = (2n+1)(2n+1)
	float bnumber: weight
- return output
--------------------------------------------------------------------------------------------------------------------------
##2. 
kmeans_gray.cpp
: program that performs the image segmentation on grayscale image using the K-means clustering function, considering only intensity
1) kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers)
: k-means clustering function provided by OpenCV
- parameter
	samples: 1D data (intensity)
	clusterCount: # of cluster = k
	labels: cluster number
	TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001): criteria
	attempts: # of attemps
	KMEANS_PP_CENTERS: flag(setting initial center)
	centers: data of center in cluster
--------------------------------------------------------------------------------------------------------------------------
##3. 
kmeans_rgb.cpp
: program that performs the image segmentation on color image using the K-means clustering function, considering only r,g,b value
1) kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers)
: k-means clustering function provided by OpenCV
- parameter
	samples: 3D data (rgb)
	clusterCount: # of cluster = k
	labels: cluster number
	TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001): criteria
	attempts: # of attemps
	KMEANS_PP_CENTERS: flag(setting initial center)
	centers: data of center in cluster
--------------------------------------------------------------------------------------------------------------------------
##4. 
kmeans_position_gray.cpp
: program that performs the image segmentation on gray scale image using the K-means clustering function, considering both intensity value and position
1) kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers)
: k-means clustering function provided by OpenCV
- parameter
	samples: 3D data (normalized intensity, normalized x/sigma, normalized y/sigma)
	clusterCount: # of cluster = k
	labels: cluster number
	TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001): criteria
	attempts: # of attemps
	KMEANS_PP_CENTERS: flag(setting initial center)
	centers: data of center in cluster
--------------------------------------------------------------------------------------------------------------------------
##5. 
kmeans_position_rgb.cpp
: program that performs the image segmentation on color image using the K-means clustering function, considering both r,g,b value and position
1) kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers)
: k-means clustering function provided by OpenCV
- parameter
	samples: 3D data (normalized r, normalized g, normalized b, normalized x/sigma, normalized y/sigma)
	clusterCount: # of cluster = k
	labels: cluster number
	TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001): criteria
	attempts: # of attemps
	KMEANS_PP_CENTERS: flag(setting initial center)
	centers: data of center in cluster
