//
//  CameraCentroidsInputs.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/10/13.
//
//

#pragma once

#include "ofMain.h"

#include "ofxCv.h"

#include "ContourFinder2.h"

#include "IImageSeqInput.h"

class CameraCentroidsInputs : public IImageSeqInput
{
    
public:
    
    CameraCentroidsInputs()
    {
        contourFinder.setMinAreaRadius( 20 );
        contourFinder.setMaxAreaRadius( 200 );
        contourFinder.setThreshold( 125 );
        contourFinder.setBlur( 4 );
        contourFinder.setDilate( 3 );
    };
    
    ~CameraCentroidsInputs()
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
            
            for( int i = 0; i < contourFinder.size(); ++i )
            {
                PointInputSampleT   sample;
                
                sample.setSampleID( contourFinder.getLabel( i ) );
                
                ofPoint p(ofxCv::toOf( contourFinder.getCentroid( i ) ) );
//                p-= ofPoint( roiX1 * width, roiY1 * height );
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
        contourFinder.draw();
    };
    
private:
    
    ofxCv::ContourFinder2           contourFinder;
    
    std::vector<PointInputSampleT>  samples;
};