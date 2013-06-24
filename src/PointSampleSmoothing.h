//
//  PointSampleSmoothing.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/24/13.
//
//

#pragma once

#include "ofMain.h"

#include "ofxCv.h"

#include "InputSample.h"

class PointSampleSmoother
{
    
public:
    
    PointSampleSmoother()
    {        
        firstSampleSet      = false;
        newSampleReceived   = false;
    };
    
    ~PointSampleSmoother(){};
    
public:
    
    void setNewSampleReceived( bool value )
    {
        newSampleReceived   = value;
    }
    
    bool getNewSampleReceived()
    {
        return newSampleReceived;
    }
    
    ofPoint getSmoothed( const ofPoint & sample, float smoothing )
    {
        if( !firstSampleSet )
        {
            firstSampleSet      = true;
            
            oldSample.set( sample );
            
            return sample;
        } else {
                        
            oldSample   = sample.getInterpolated( oldSample , smoothing );
            
            return oldSample;
        }
        
        return ofPoint();
    };
    
private:
    
    bool                firstSampleSet;
    bool                newSampleReceived;
    ofPoint             oldSample;
};

