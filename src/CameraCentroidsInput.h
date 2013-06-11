//
//  CameraCentroidsInputs.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/10/13.
//
//

#pragma once

#include "ofMain.h"

#include "IImageSeqInput.h"

class CameraCentroidsInputs : public IImageSeqInput
{
    
public:
    
    CameraCentroidsInputs()
    {};
    
    ~CameraCentroidsInputs()
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