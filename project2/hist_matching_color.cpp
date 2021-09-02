#include "hist_func.h"

void hist_eq(Mat& input, Mat& equalized, G* trans_func, float* CDF);

int main() {

    Mat input = imread("input.jpg", CV_LOAD_IMAGE_COLOR);
    Mat reference = imread("i.jpg", CV_LOAD_IMAGE_COLOR);
    Mat matched_YUV;
    Mat reference_YUV;

    cvtColor(input, matched_YUV, CV_RGB2YUV);   // convert RGB to YUV
    cvtColor(reference, reference_YUV, CV_RGB2GRAY);   // convert RGB to YUV

    // split each channel(Y, U, V)
    Mat channels[3];
    split(matched_YUV, channels);   // U = channels[1], V = channels[2]
    Mat Y = channels[0];

    //same work on reference image
    Mat channels_r[3];
    split(reference_YUV, channels_r);
    Mat Y_r = channels_r[0];

    // PDF or transfer function txt files
    FILE* f_matched_PDF_YUV, * f_PDF_RGB;
    FILE* f_trans_func_match_YUV;

    float** PDF_RGB = cal_PDF_RGB(input); // PDF of Input image(RGB) : [L][3]
    float* CDF = cal_CDF(Y);   // CDF of Input image : [L]
    float* reference_CDF = cal_CDF(Y_r);   // CDF of Input image : [L]

    fopen_s(&f_PDF_RGB, "PDF_RGB.txt", "w+");
    fopen_s(&f_matched_PDF_YUV, "matched_PDF_YUV.txt", "w+");
    fopen_s(&f_trans_func_match_YUV, "trans_func_match_YUV.txt", "w+");

    G trans_func_eq[L] = { 0 };         // transfer function
    G trans_func_eq_reference[L] = { 0 };  // transfer function
    
    //set transfer function
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

    //do matching on Y channel
    for (int i = 0; i < Y.rows; i++)
        for (int j = 0; j < Y.cols; j++)
            Y.at<G>(i, j) = (G)arr[Y.at<G>(i, j)];

    //Convert the histogram - matched Y and UV to the color image(RGB).
    merge(channels, 3, matched_YUV);
    cvtColor(matched_YUV, matched_YUV, CV_YUV2RGB);


    // matched PDF (YUV)
    float** matched_PDF_YUV = cal_PDF_RGB(matched_YUV);

    for (int k = 0; k < 3; k++) {
        for (int i = 0; i < L; i++) {
            // write PDF
            fprintf(f_PDF_RGB, "%d %d\t%f\n", i, k, PDF_RGB[i][k]);
            fprintf(f_matched_PDF_YUV, "%d\t%f\n", i, matched_PDF_YUV[i][k]);
            // write transfer functions
            fprintf(f_trans_func_match_YUV, "%d\t%d\n", i, arr[i]);
        }
    }

    free(reference_CDF);
    free(CDF);
    free(PDF_RGB);
    fclose(f_PDF_RGB);
    fclose(f_matched_PDF_YUV);
    fclose(f_trans_func_match_YUV);

    ////////////////////// Show each image ///////////////////////

    namedWindow("input", WINDOW_AUTOSIZE);
    imshow("input", input);

    namedWindow("Reference", WINDOW_AUTOSIZE);
    imshow("Reference", reference);

    namedWindow("matched", WINDOW_AUTOSIZE);
    imshow("matched", matched_YUV);

    //////////////////////////////////////////////////////////////

    waitKey(0);

    return 0;
}