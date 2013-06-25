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
        void findSkeletons( T& img, const ContourVectorT &contours, const vector<cv::Rect> &boundingRects, bool preciseProcess, float connectionThreshold = 0.0f )
        {
            findSkeletons( toCv(img), contours, boundingRects, preciseProcess, connectionThreshold );
        }
        
        const vector<ofPolyline> & getSkeletons() const
        {
            return skeletons;
        }
        
    private:
        
        void findSkeletons( cv::Mat img, const ContourVectorT &contours, const vector<cv::Rect> &boundingRects, bool preciseProcess, float connectionThreshold )
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
            
            Vector<ofPoint> allTipVertices;
            vector<ofVec3f> distances;
            
            skeletons.clear();
            
            for( int c = 0; c < contours.size(); ++c )
            {
                cv::Mat roiSkeletonImage    = skeletonImage( boundingRects[c] );
                
                skeletonContours.clear();
                allTipVertices.clear();
                
                cv::findContours( roiSkeletonImage, skeletonContours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE );
                
                for(int i = 0; i < skeletonContours.size(); i++)
                {
                    skeletons.push_back( ofPolyline() );
                    
                    skeletons.back().resize( skeletonContours[i].size() / 2 );
                    
                    allTipVertices.push_back( ofPoint( skeletonContours[i].front().x + boundingRects[c].x, skeletonContours[i].front().y + boundingRects[c].y ) );
                    allTipVertices.push_back( ofPoint( skeletonContours[i].back().x + boundingRects[c].x, skeletonContours[i].back().y + boundingRects[c].y ) );
                    
                    for(int j = 0; j < skeletonContours[i].size() / 2; ++j )
                    {
                        skeletons.back()[j].x = skeletonContours[i][j].x + boundingRects[c].x;
                        skeletons.back()[j].y = skeletonContours[i][j].y + boundingRects[c].y;
                    }
                }
                
                if( connectionThreshold > 0.0f )
                {                    
                    for( int k = 0; k < allTipVertices.size(); ++k )
                    {
                        ofPoint     currentPoint    = allTipVertices[k];
                        
                        float       closestDistance = 100000000.0;
                        int         closestIndex    = 0;
                        
                        distances.clear();
                        
                        for( int p = k + 1; p < allTipVertices.size(); ++p )
                        {
                            float currentDistance   = currentPoint.distanceSquared( allTipVertices[p] ) ;
                            
                            if( currentDistance < connectionThreshold )
                            {
                                distances.push_back( ofVec3f( k, p, currentDistance ) );
                            }
                        }
                        
                        if( distances.size() > 0 )
                        {
                            sort(distances.begin(), distances.end(), mysort);
                            
                             skeletons.push_back( ofPolyline() );
                            
                            ofPoint indices = distances.back();
                            
                            skeletons.back().addVertex( allTipVertices[indices.x] );
                            skeletons.back().addVertex( allTipVertices[indices.y] );
                        }
                    }
                }
            }

            
        }
        
    private:
        
        static bool mysort(const ofVec3f &a, const ofVec3f &b) { return (a.z < b.z); }
        
    private:
        
        vector<ofPolyline>  skeletons;
    };
    
}
