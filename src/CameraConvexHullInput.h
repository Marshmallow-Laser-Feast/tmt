//
//  CameraConvexHullInput.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/11/13.
//
//

#pragma once

#include "ofMain.h"

#include "ofxCv.h"

#include "ContourFinder2.h"

#include "IImageSeqInput.h"

class CameraConvexHullInput : public IImageSeqInput
{
    
public:
    
    CameraConvexHullInput()
    {
        contourFinder.setMinAreaRadius( 20 );
        contourFinder.setMaxAreaRadius( 200 );
        contourFinder.setThreshold( 125 );
        contourFinder.setBlur( 4 );
        contourFinder.setDilate( 3 );
    };
    
    ~CameraConvexHullInput()
    {}
    
public:
    
    void setBlurAmount( int value )
    {
        contourFinder.setBlur( value );
    };
    
    void setThreshold( int value )
    {
        contourFinder.setThreshold( value );
    };
    
    void setDilateAmount( int value )
    {
        contourFinder.setDilate( value );
    };
    
    void setMinContourAreaRadius( int value )
    {
        contourFinder.setMinAreaRadius( value );
    };
    
    void setMaxContourAreaRadius( int value )
    {
        contourFinder.setMaxAreaRadius( value );
    };
    
    virtual void init()
    {};
    
    virtual void update()
    {
        if( isCurrentFrameNew )
        {
            samples.clear();
            
            ofImage image;
            
            float w = roiX2 - roiX1;
            float h = roiY2 - roiY1;
            
            image.setFromPixels( *pixels );
            
            image.crop( roiX1 * width , roiY1 * height, w * width, h * height );
            
            contourFinder.findContours( image );
            
            vector<cv::Point>       allConvexHullPoints;
            vector<cv::Point2f>     allConvexHullFloatPoints;
            
            ofPolyline              line;
            
            for( int i = 0; i < contourFinder.size(); ++i )
            {
                vector<cv::Point> convexHullPoints  = contourFinder.getConvexHull( i );
                
                line.clear();
                
                for( vector<cv::Point>::iterator it = convexHullPoints.begin(); it != convexHullPoints.end(); ++it )
                {
                    line.lineTo( ofxCv::toOf( *it ) );
                }
                
                line.close();
                
                line.simplify( 0.5f );
                
                for( int j = 0; j < line.getVertices().size(); ++j )
                {
                    allConvexHullPoints.push_back( cv::Point2f( line.getVertices()[j].x, line.getVertices()[j].y ) );
                }
            }
            
            for( vector<cv::Point>::iterator it = allConvexHullPoints.begin(); it != allConvexHullPoints.end(); ++it )
            {
                allConvexHullFloatPoints.push_back( cv::Point2f( it->x, it->y ) );
            }
            
            pointTracker.track( allConvexHullFloatPoints );
            
            for( int i  = 0; i < allConvexHullFloatPoints.size(); ++i )
            {
                PointInputSampleT   sample;
                
                sample.setSampleID( pointTracker.getLabelFromIndex( i ) );
                
                sample.setSample( ofPoint( ofxCv::toOf( allConvexHullFloatPoints[i] ) ) + ofPoint( roiX1 * width, roiY1 * height ) );
                
                samples.push_back( sample );
            }
        }
    };
    
    virtual const vector<PointInputSampleT> & getSamples() const
    {
        return samples;
    };
    
    virtual void drawDebug()
    {
        contourFinder.draw();
    };
    
private:
    
    ofxCv::ContourFinder2           contourFinder;
    ofxCv::PointTracker             pointTracker;
    
    std::vector<PointInputSampleT>  samples;
};