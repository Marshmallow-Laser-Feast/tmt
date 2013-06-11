//
//  CameraInput.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/10/13.
//
//

#pragma once

#include "ofMain.h"

#include "IImageSeqInput.h"

class CameraInput : public IImageSeqInput
{
    
public:
    
    CameraInput()
    {};
    
    ~CameraInput()
    {}
    
public:
    
    virtual void update()
    {
        
    };
    
    virtual const vector<PointInputSampleT> & getSamples() const
    {
        return samples;
    };
    
private:
    
    std::vector<PointInputSampleT>  samples;
};