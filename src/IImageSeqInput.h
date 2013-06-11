//
//  IImageSeqInput.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/11/13.
//
//

#pragma once

#include "ofMain.h"

#include "Input.h"

class IImageSeqInput : public Input
{
    
public:
    
    IImageSeqInput()
    
    :width( 0 )
    ,height( 0 )
    ,roiX1( 0.0f )
    ,roiY1( 0.0f )
    ,roiX2( 1.0f )
    ,roiY2( 1.0f )
    
    {};
    
    ~IImageSeqInput()
    {};
    
public:
    
    void setCurrentFrameNew( bool value )
    {
        isCurrentFrameNew   = value;
    };
    
    void setDimensions( int width_, int height_ )
    {
        width               = width_;
        height              = height_;
    };
    
    void setROI( float x1, float y1, float x2, float y2 )
    {
        roiX1   = x1;
        roiY1   = y1;
        roiX2   = x2;
        roiY2   = y2;
    };
    
    virtual void init()
    {};
    
    virtual void setPixels( ofPixelsRef pixels_ )
    {
        pixels  = &pixels_;
    };
    
    virtual void update()
    {};
    
    virtual const vector<PointInputSampleT> & getSamples() const
    {};
    
    virtual void drawDebug()
    {};
    
protected:
    
    bool        isCurrentFrameNew;
    
    int         width;
    int         height;
    
    float       roiX1;
    float       roiY1;
    float       roiX2;
    float       roiY2;
    
    ofPixels    *pixels;
    
};

