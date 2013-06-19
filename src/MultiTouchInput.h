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

#define DRAW_CIRCLE_RADIUS      2.0f

#define PARAM_NAME_WIDTH        "width"
#define PARAM_NAME_HEIGHT       "height"

class MultiTouchInput : public Input
{

public:
    
    MultiTouchInput()
    
    :Input( "Input/Multi Touch", Input::SAMPLING_TYPE_POINT )
    
    {
        multiTouchPad   = ofxMultiTouchPad();
        
        params.addInt( PARAM_NAME_WIDTH ).set( 1024 );
        params.addInt( PARAM_NAME_HEIGHT ).set( 768 );
    };
    
    ~MultiTouchInput()
    {}
    
public:
    
    virtual std::string getName(){ return "Input/Multi Touch"; };
    
    virtual void draw( float width, float height )
    {
        ofPushStyle();
        
        ofSetColor( ofColor::yellow );
        
        ofVec3f scale( width / (int)params[ PARAM_NAME_WIDTH ], height / (int)params[ PARAM_NAME_HEIGHT ] );
        
        for( vector<PointSampleT>::const_iterator it = pointSamples.begin(); it != pointSamples.end(); ++it )
        {
            ofCircle( it->getSample() * scale, DRAW_CIRCLE_RADIUS );
        }
        
        ofPopStyle();
    };
    
protected:
    
    virtual void processPointSamples()
    {
        pointSamples.clear();
        
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