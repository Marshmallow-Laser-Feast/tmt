//
//  CameraContourInput.h
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

class CameraContourInput : public IImageSeqInput
{
    
public:
    
    CameraContourInput()
    {
        contourFinder.setMinAreaRadius( 20 );
        contourFinder.setMaxAreaRadius( 200 );
        contourFinder.setThreshold( 125 );
        contourFinder.setBlur( 4 );
        contourFinder.setDilate( 3 );
        
        simplification  = 0.3f;
        params.setName("CameraContourInput");

    };
    
    ~CameraContourInput()
    {}
    
public:
    
    void setSimplification( float value )
    {
        simplification  = value;
    };
    
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
            
            ofPolyline              line;
            
            vector<cv::Point2f>     allFloatPoints;
            
            for( int i = 0; i < contourFinder.size(); ++i )
            {
                line.clear();
                
                for ( int j = 0; j < contourFinder.getContour( i ).size(); ++j )
                {
                    line.lineTo( ofPoint( contourFinder.getContour( i )[j].x, contourFinder.getContour( i )[j].y ) + ofPoint( roiX1 * width, roiY1 * height ) );
                }
                
                line.close();
                line.simplify( simplification );
                
                for ( int k = 0; k < line.getVertices().size(); ++k )
                {
                    allFloatPoints.push_back( cv::Point2f( line.getVertices()[k].x, line.getVertices()[k].y ) );
                }
            }
            
            pointTracker.track( allFloatPoints );
            
            for( int i  = 0; i < allFloatPoints.size(); ++i )
            {
                PointInputSampleT   sample;
                
                sample.setSampleID( pointTracker.getLabelFromIndex( i ) );
                
                
                ofPoint p(ofxCv::toOf( allFloatPoints[i] ) );
                p-= ofPoint( roiX1 * width, roiY1 * height );
                sample.setSample(p);
                
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
        ofPolyline              line;
                
        for( int i = 0; i < contourFinder.size(); ++i )
        {
            line.clear();
            
            for ( int j = 0; j < contourFinder.getContour( i ).size(); ++j )
            {
                line.lineTo( ofPoint( contourFinder.getContour( i )[j].x, contourFinder.getContour( i )[j].y ) + ofPoint( roiX1 * width, roiY1 * height ) );
            }
            
            line.close();
            line.simplify( simplification );
            line.draw();
        }
    };
    
private:
    
    float                           simplification;
    
    ofxCv::ContourFinder2           contourFinder;
    ofxCv::PointTracker             pointTracker;
    
    std::vector<PointInputSampleT>  samples;
};