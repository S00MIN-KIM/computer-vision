#include "hist_func.h"


void hist_eq(Mat& input, Mat& equalized, G* trans_func, float* CDF);

int main() {

	Mat input = imread("input.jpg", CV_LOAD_IMAGE_COLOR); //read input image
	Mat reference = imread("b.jpg", CV_LOAD_IMAGE_COLOR); //read reference image
	Mat input_gray;
	Mat reference_gray;

	cvtColor(input, input_gray, CV_RGB2GRAY);	// convert RGB to Grayscale
	cvtColor(reference, reference_gray, CV_RGB2GRAY);	// convert RGB to Grayscale

	//output files
	FILE* f_PDF;
	FILE* f_matched_PDF_gray;
	FILE* f_trans_func_match;

	fopen_s(&f_PDF, "PDF.txt", "w+");
	fopen_s(&f_matched_PDF_gray, "matched_PDF_gray.txt", "w+");
	fopen_s(&f_trans_func_match, "trans_func_match.txt", "w+");

	float* PDF = cal_PDF(input_gray);   // PDF of Input image(Grayscale) : [L]
	float* CDF = cal_CDF(input_gray);	// CDF of Input image(Grayscale) : [L]
	float* reference_CDF = cal_CDF(reference_gray);	// CDF of refernce image(Grayscale) : [L]

	G trans_func_eq[L] = { 0 };			// transfer function of input
	G trans_func_eq_reference[L] = { 0 };   // transfer function of reference

	//set transfer functions
	for (int i = 0; i < L; i++)
		trans_func_eq[i] = (G)((L - 1) * (CDF[i])); //s=T(r)

	for (int i = 0; i < L; i++)
		trans_func_eq_reference[i] = (G)((L - 1) * (reference_CDF[i])); //s=G(z)

	//Apply the intensity mapping
	G arr[L] = { 0 }; //transfer function z=G^(-1)(T(r))

	for (int i = 0; i < L; i++) {
		int k = L;
		int jj = 0;
		for (int j = 0; j < L; j++) {

			if ((trans_func_eq[i] >= trans_func_eq_reference[j]) && (trans_func_eq[i] - trans_func_eq_reference[j]) < k) {
				k = trans_func_eq[i] - trans_func_eq_reference[j];
				jj = j;
			}
		}
		arr[i] = jj;
	}

	//do matching
	Mat matched = input_gray.clone();
	for (int i = 0; i < matched.rows; i++)
		for (int j = 0; j < matched.cols; j++)
			matched.at<G>(i, j) = (G)arr[matched.at<G>(i, j)];

	float* matched_PDF_gray = cal_PDF(matched);									// matched PDF (grayscale)

	for (int i = 0; i < L; i++) {
		// write PDF
		fprintf(f_PDF, "%d\t%f\n", i, PDF[i]);
		fprintf(f_matched_PDF_gray, "%d\t%f\n", i, matched_PDF_gray[i]);

		// write transfer functions
		fprintf(f_trans_func_match, "%d\t%d\n", i, arr[i]);
	}

	free(PDF);
	free(reference_CDF);
	free(CDF);
	fclose(f_PDF);
	fclose(f_matched_PDF_gray);
	fclose(f_trans_func_match);

	////////////////////// Show each image ///////////////////////

	namedWindow("Grayscale", WINDOW_AUTOSIZE);
	imshow("Grayscale", input_gray);

	namedWindow("Reference", WINDOW_AUTOSIZE);
	imshow("Reference", reference);

	namedWindow("matched", WINDOW_AUTOSIZE);
	imshow("matched", matched);

	//////////////////////////////////////////////////////////////

	waitKey(0);

	return 0;
}