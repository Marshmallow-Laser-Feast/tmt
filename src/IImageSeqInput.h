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

typedef ofPtr<ofPixels> ofPixelsSharedPtrT;

class IImageSeqInput : public Input
{
    
public:
    
    IImageSeqInput()
    
    {};
    
    ~IImageSeqInput()
    {};
    
public:
    
    virtual void setPixels( ofPixelsSharedPtrT pixels_ )
    {
        pixels  = pixels_;
    };
    
    virtual void update()
    {};
    
    virtual const vector<PointInputSampleT> & getSamples() const
    {};
    
protected:
    
    ofPixelsSharedPtrT pixels;
    
};

