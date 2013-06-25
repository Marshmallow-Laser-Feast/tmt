//
//  RectangleSmoothing.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/25/13.
//
//

#pragma once

#include "ofMain.h"

#include "ofxCv.h"

#include "InputSample.h"

class RectangleSmoother
{
    
public:
    
    RectangleSmoother()
    {
        firstSampleSet      = false;
        newSampleReceived   = false;
    };
    
    ~RectangleSmoother(){};
    
public:
    
    void setNewSampleReceived( bool value )
    {
        newSampleReceived   = value;
    }
    
    bool getNewSampleReceived()
    {
        return newSampleReceived;
    }
    
    cv::Rect getSmoothed( const cv::Rect & sample, float smoothing )
    {
        if( !firstSampleSet )
        {
            firstSampleSet      = true;
            
            oldSample   = sample;
            
            return sample;
        } else {
            
            ofPoint currentTL( sample.tl().x, sample.tl().y );
            ofPoint currentBR( sample.br().x, sample.br().y );
            
            ofPoint oldTL( oldSample.tl().x, oldSample.tl().y );
            ofPoint oldBR( oldSample.br().x, oldSample.br().y );
            
            oldTL       = currentTL.getInterpolated( oldTL , smoothing );
            oldBR       = currentBR.getInterpolated( oldBR , smoothing );
            
            oldSample   = cv::Rect( cv::Point( oldTL.x, oldTL.y ), cv::Point( oldBR.x, oldBR.y ) );
            
            return oldSample;
        }
        
        return cv::Rect();
    };
    
private:
    
    bool                firstSampleSet;
    bool                newSampleReceived;
    
    cv::Rect            oldSample;
};


