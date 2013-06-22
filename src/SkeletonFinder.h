//
//  SkeletonFinder.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/21/13.
//
//

#pragma once

#include "ofMain.h"

#include "ofxCv.h"

#include "Thinner.h"

namespace ofxCv {
    
    typedef vector<vector<cv::Point> > ContourVectorT;
    
    class SkeletonFinder
    {
        
    public:
        
        SkeletonFinder()
        {};
        
        ~SkeletonFinder()
        {};
        
    public:
        
        template <class T>
        void findSkeletons( T& img, const ContourVectorT &contours, const vector<cv::Rect> &boundingRects, bool preciseProcess )
        {
            findSkeletons( toCv(img), contours, boundingRects, preciseProcess );
        }
        
        const vector<ofPolyline> & getSkeletons() const
        {
            return skeletons;
        }
        
    private:
        
        void findSkeletons( cv::Mat img, const ContourVectorT &contours, const vector<cv::Rect> &boundingRects, bool preciseProcess )
        {
            cv::Mat contoursImage(img.size(), CV_8UC1, cv::Scalar(0));
            cv::Mat skeletonImage(img.size(), CV_8UC1, cv::Scalar(0));
            
            cv::drawContours( contoursImage, contours, -1, cv::Scalar(255), CV_FILLED );
            
            if( preciseProcess )
            {
                // Slower + No gaps in skeleton
                
                Thinner::process( contoursImage, skeletonImage );
            } else {
                // Faster + Gaps in skeleton
                // http://felix.abecassis.me/2011/09/opencv-morphological-skeleton/
                
                cv::Mat temp;
                cv::Mat eroded;
                
                cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
                
                bool done;
                do
                {
                    cv::erode(contoursImage, eroded, element);
                    cv::dilate(eroded, temp, element);
                    cv::subtract(contoursImage, temp, temp);
                    cv::bitwise_or(skeletonImage, temp, skeletonImage);
                    eroded.copyTo(contoursImage);
                    
                    done = (cv::countNonZero(contoursImage) == 0);
                } while (!done);
            }
            
            skeletonImage.convertTo(skeletonImage,CV_8UC1);
            
            ContourVectorT skeletonContours;
            
            skeletons.clear();
            
            for( int c = 0; c < contours.size(); ++c )
            {
                cv::Mat roiSkeletonImage    = skeletonImage( boundingRects[c] );
                
                skeletonContours.clear();
                
                cv::findContours( roiSkeletonImage, skeletonContours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE );
                
                for(int i = 0; i < skeletonContours.size(); i++)
                {
                    skeletons.push_back( ofPolyline() );
                    
                    skeletons.back().resize( skeletonContours[i].size() / 2 );
                    
                    for(int j = 0; j < skeletonContours[i].size() / 2; ++j )
                    {
                        skeletons.back()[j].x = skeletonContours[i][j].x + boundingRects[c].x;
                        skeletons.back()[j].y = skeletonContours[i][j].y + boundingRects[c].y;
                    }
                }
            }
        }
        
    private:
        
        vector<ofPolyline>  skeletons;
    };
    
}
