//
//  Thinner.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/21/13.
//
//

// Zhang&Suen http://www-prima.inrialpes.fr/perso/Tran/Draft/gateway.cfm.pdf
// Impl : http://answers.opencv.org/question/3207/what-is-a-good-thinning-algorithm-for-getting-the/

#pragma once

#include "opencv2/opencv.hpp"

namespace ofxCv {
 
    using namespace cv;
    
    class Thinner
    {
        
    public:
        
        Thinner()
        {};
        
        ~Thinner()
        {};
        
    public:
        
        void static process(Mat & inputarray, Mat & outputarray)
        {
            bool bDone = false;
            int rows = inputarray.rows;
            int cols = inputarray.cols;
            
            inputarray.convertTo(inputarray,CV_32FC1);
            
            inputarray.copyTo(outputarray);
            
            outputarray.convertTo(outputarray,CV_32FC1);
            
            /// pad source
            Mat p_enlarged_src = Mat(rows + 2, cols + 2, CV_32FC1);
            for(int i = 0; i < (rows+2); i++) {
                p_enlarged_src.at<float>(i, 0) = 0.0f;
                p_enlarged_src.at<float>( i, cols+1) = 0.0f;
            }
            for(int j = 0; j < (cols+2); j++) {
                p_enlarged_src.at<float>(0, j) = 0.0f;
                p_enlarged_src.at<float>(rows+1, j) = 0.0f;
            }
            for(int i = 0; i < rows; i++) {
                for(int j = 0; j < cols; j++) {
                    if (inputarray.at<float>(i, j) >= 20.0f) {
                        p_enlarged_src.at<float>( i+1, j+1) = 1.0f;
                    }
                    else
                        p_enlarged_src.at<float>( i+1, j+1) = 0.0f;
                }
            }
            
            /// start to thin
            Mat p_thinMat1 = Mat::zeros(rows + 2, cols + 2, CV_32FC1);
            Mat p_thinMat2 = Mat::zeros(rows + 2, cols + 2, CV_32FC1);
            Mat p_cmp = Mat::zeros(rows + 2, cols + 2, CV_8UC1);
            
            while (bDone != true) {
                /// sub-iteration 1
                ThinSubiteration1(p_enlarged_src, p_thinMat1);
                /// sub-iteration 2
                ThinSubiteration2(p_thinMat1, p_thinMat2);
                /// compare
                compare(p_enlarged_src, p_thinMat2, p_cmp, CV_CMP_EQ);
                /// check
                int num_non_zero = countNonZero(p_cmp);
                if(num_non_zero == (rows + 2) * (cols + 2)) {
                    bDone = true;
                }
                /// copy
                p_thinMat2.copyTo(p_enlarged_src);
            }
            // copy result
            for(int i = 0; i < rows; i++) {
                for(int j = 0; j < cols; j++) {
                    outputarray.at<float>( i, j) = p_enlarged_src.at<float>( i+1, j+1);
                }
            }
        }
        
    private:
        
        void static ThinSubiteration1(Mat & pSrc, Mat & pDst) {
            int rows = pSrc.rows;
            int cols = pSrc.cols;
            pSrc.copyTo(pDst);
            for(int i = 0; i < rows; i++) {
                for(int j = 0; j < cols; j++) {
                    if(pSrc.at<float>(i, j) == 1.0f) {
                        /// get 8 neighbors
                        /// calculate C(p)
                        int neighbor0 = (int) pSrc.at<float>( i-1, j-1);
                        int neighbor1 = (int) pSrc.at<float>( i-1, j);
                        int neighbor2 = (int) pSrc.at<float>( i-1, j+1);
                        int neighbor3 = (int) pSrc.at<float>( i, j+1);
                        int neighbor4 = (int) pSrc.at<float>( i+1, j+1);
                        int neighbor5 = (int) pSrc.at<float>( i+1, j);
                        int neighbor6 = (int) pSrc.at<float>( i+1, j-1);
                        int neighbor7 = (int) pSrc.at<float>( i, j-1);
                        int C = int(~neighbor1 & ( neighbor2 | neighbor3)) +
                        int(~neighbor3 & ( neighbor4 | neighbor5)) +
                        int(~neighbor5 & ( neighbor6 | neighbor7)) +
                        int(~neighbor7 & ( neighbor0 | neighbor1));
                        if(C == 1) {
                            /// calculate N
                            int N1 = int(neighbor0 | neighbor1) +
                            int(neighbor2 | neighbor3) +
                            int(neighbor4 | neighbor5) +
                            int(neighbor6 | neighbor7);
                            int N2 = int(neighbor1 | neighbor2) +
                            int(neighbor3 | neighbor4) +
                            int(neighbor5 | neighbor6) +
                            int(neighbor7 | neighbor0);
                            int N = MIN(N1,N2);
                            if ((N == 2) || (N == 3)) {
                                /// calculate criteria 3
                                int c3 = ( neighbor1 | neighbor2 | ~neighbor4) & neighbor3;
                                if(c3 == 0) {
                                    pDst.at<float>( i, j) = 0.0f;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        
        void static ThinSubiteration2(Mat & pSrc, Mat & pDst) {
            int rows = pSrc.rows;
            int cols = pSrc.cols;
            pSrc.copyTo( pDst);
            for(int i = 0; i < rows; i++) {
                for(int j = 0; j < cols; j++) {
                    if (pSrc.at<float>( i, j) == 1.0f) {
                        /// get 8 neighbors
                        /// calculate C(p)
                        int neighbor0 = (int) pSrc.at<float>( i-1, j-1);
                        int neighbor1 = (int) pSrc.at<float>( i-1, j);
                        int neighbor2 = (int) pSrc.at<float>( i-1, j+1);
                        int neighbor3 = (int) pSrc.at<float>( i, j+1);
                        int neighbor4 = (int) pSrc.at<float>( i+1, j+1);
                        int neighbor5 = (int) pSrc.at<float>( i+1, j);
                        int neighbor6 = (int) pSrc.at<float>( i+1, j-1);
                        int neighbor7 = (int) pSrc.at<float>( i, j-1);
                        int C = int(~neighbor1 & ( neighbor2 | neighbor3)) +
                        int(~neighbor3 & ( neighbor4 | neighbor5)) +
                        int(~neighbor5 & ( neighbor6 | neighbor7)) +
                        int(~neighbor7 & ( neighbor0 | neighbor1));
                        if(C == 1) {
                            /// calculate N
                            int N1 = int(neighbor0 | neighbor1) +
                            int(neighbor2 | neighbor3) +
                            int(neighbor4 | neighbor5) +
                            int(neighbor6 | neighbor7);
                            int N2 = int(neighbor1 | neighbor2) +
                            int(neighbor3 | neighbor4) +
                            int(neighbor5 | neighbor6) +
                            int(neighbor7 | neighbor0);
                            int N = MIN(N1,N2);
                            if((N == 2) || (N == 3)) {
                                int E = (neighbor5 | neighbor6 | ~neighbor0) & neighbor7;
                                if(E == 0) {
                                    pDst.at<float>(i, j) = 0.0f;
                                }
                            }
                        }
                    }
                }
            }
        }
        
    };
    
}