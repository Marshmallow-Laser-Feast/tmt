//
//  InputSample.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/18/13.
//
//

#pragma once

#include "ofMain.h"


template<typename T>
class InputSample
{
    
public:
    
    InputSample()
    {};
    
    ~InputSample(){};
    
public:
    
    void setSampleID( const int sampleID_ )
    {
        sampleID    = sampleID_;
    };
    
    void setSample( const T & sample_ )
    {
        sample  = sample_;
    };
    
    const int & getSampleID() const
    {
        return sampleID;
    };
    
    const T & getSample() const
    {
        return sample;
    };
    
private:
    
    int     sampleID;
    T       sample;
};

typedef InputSample<ofPoint>        PointSampleT;
typedef InputSample<ofPolyline>     PolylineSampleT;