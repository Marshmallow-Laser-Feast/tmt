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
    
    :Input( "Input/Multi Touch" )
    
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
        
        for( vector<PointSampleT>::const_iterator it = pointSamplesMap[ Input::DEFAULT_TAG ].begin(); it != pointSamplesMap[ Input::DEFAULT_TAG ].end(); ++it )
        {
            ofCircle( it->getSample() * scale, DRAW_CIRCLE_RADIUS );
        }
        
        ofPopStyle();
    };
    
protected:
    
    virtual void processPointSamples()
    {
        pointSamplesMap[ Input::DEFAULT_TAG ].clear();
        
        std::vector<MTouch> touches = multiTouchPad.getTouches();
                
        for (std::vector<MTouch>::iterator it = touches.begin(); it != touches.end(); ++it )
        {
            PointSampleT    sample;
            
            sample.setSampleID( it->ID );
            
            sample.setSample( ofPoint( it->x * (int)params[ PARAM_NAME_WIDTH ], it->y * (int)params[ PARAM_NAME_HEIGHT ], 0.0f ) );
            
            pointSamplesMap[ Input::DEFAULT_TAG ].push_back( sample );
        }
    }
    
    virtual void processPolylineSamples()
    {}
    
    virtual ofVec2f getSize()
    {
        return ofVec2f( 200, 200 );
    };
    
private:
    
    ofxMultiTouchPad                multiTouchPad;
};