//
//  MultiTouchInput.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoğlu on 6/7/13.
//
//

#pragma once

#include "ofMain.h"

#include "ofxMultiTouchPad.h"

#include "Input.h"
#include "InputSample.h"

#define PARAM_NAME_WIDTH        "width"
#define PARAM_NAME_HEIGHT       "height"

class MultiTouchInput : public Input
{

public:
    
    MultiTouchInput()
    
    :Input( "MultiTouch Input" )
    
    {
        multiTouchPad   = ofxMultiTouchPad();
        
        params.addInt( PARAM_NAME_WIDTH ).set( 1024 );
        params.addInt( PARAM_NAME_HEIGHT ).set( 768 );
    };
    
    ~MultiTouchInput()
    {}
    
public:
    
    virtual void processPointSamples()
    {
        std::vector<MTouch> touches = multiTouchPad.getTouches();
                
        for (std::vector<MTouch>::iterator it = touches.begin(); it != touches.end(); ++it )
        {
            PointSampleT    sample;
            
            sample.setSampleID( it->ID );
            
            sample.setSample( ofPoint( it->x * (int)params[ PARAM_NAME_WIDTH ], it->y * (int)params[ PARAM_NAME_HEIGHT ], 0.0f ) );
            
            pointSamples.push_back( sample );
        }
    }
    
private:
    
    ofxMultiTouchPad                multiTouchPad;
};