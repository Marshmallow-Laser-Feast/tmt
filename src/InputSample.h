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
    
    void setGroupID( const int groupID_ )
    {
        groupID     = groupID_;
    };
    
    void setSample( const T & sample_ )
    {
        sample  = sample_;
    };
    
    void setVelocity( const ofPoint & velocity_ )
    {
        velocity    = velocity_;
    };
    
    const int & getSampleID() const
    {
        return sampleID;
    };
    
    const int & getGroupID() const
    {
        return groupID;
    };
    
    const T & getSample() const
    {
        return sample;
    };
    
    const ofPoint & getVelocity() const
    {
        return velocity;
    };
    
    T & getSample()
    {
        return sample;
    };
    
private:
    
    int     sampleID;
    int     groupID;
    T       sample;
    
    ofPoint velocity;
};

typedef InputSample<ofPoint>        PointSampleT;
typedef InputSample<ofPolyline>     PolylineSampleT;