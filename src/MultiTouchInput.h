//
//  MultiTouchInput.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoğlu on 6/7/13.
//
//

#pragma once

#include <vector>
#include <map>

#include "ofMain.h"

#include "ofxMultiTouchPad.h"

#include "Input.h"
#include "InputSample.h"

#define DRAW_CIRCLE_RADIUS      12.0f

#define PARAM_NAME_WIDTH        "width"
#define PARAM_NAME_HEIGHT       "height"

#define PARAM_NAME_ENABLE       "Enabled"

class MultiTouchInput : public Input
{
    
protected:
    
    typedef std::map< std::string, PointSampleVectorRefT>   PointSampleVectorRefMapT;
    
public:
    
    static const std::string FINGERS_TAG;

public:
    
    MultiTouchInput()
    
    :Input( "Input/Multi Touch" )
    
    {
        multiTouchPad   = ofxMultiTouchPad();
        
        params.addBool( PARAM_NAME_ENABLE ).set( true );
        
        params.addInt( PARAM_NAME_WIDTH ).set( 1024 );
        params.addInt( PARAM_NAME_HEIGHT ).set( 768 );
        
        pointSampleVectorRefMap[ FINGERS_TAG ]  = PointSampleVectorRefT( new PointSampleVectorT() );
        
        pointSampleTags.push_back( FINGERS_TAG );
    };
    
    ~MultiTouchInput()
    {}
    
public:

    virtual void update()
    {
        if( (bool)params[ PARAM_NAME_ENABLE ] )
        {
            processPointSamples();
        }
    };
    
    virtual bool hasPointSamples( const std::string &tag )
    {
        return pointSampleVectorRefMap.count( tag ) > 0;
    };
    
    virtual const PointSampleVectorRefT getPointSamples( const std::string &tag ) const
    {
        return pointSampleVectorRefMap.at( tag );
    };
    
    virtual void draw( float width, float height )
    {
        ofPushStyle();
        
        ofEnableAlphaBlending();
        
        ofSetColor( ofColor::yellow, 125 );
        
        ofVec3f scale( width / (int)params[ PARAM_NAME_WIDTH ], height / (int)params[ PARAM_NAME_HEIGHT ] );
        
        for( vector<PointSampleT>::const_iterator it = pointSampleVectorRefMap[ FINGERS_TAG ]->begin(); it != pointSampleVectorRefMap[ FINGERS_TAG ]->end(); ++it )
        {
            ofCircle( it->getSample() * scale, DRAW_CIRCLE_RADIUS );
        }
        
        ofDisableAlphaBlending();
        
        ofSetColor( ofColor::white );
        
        for( vector<PointSampleT>::const_iterator it = pointSampleVectorRefMap[ FINGERS_TAG ]->begin(); it != pointSampleVectorRefMap[ FINGERS_TAG ]->end(); ++it )
        {
            ofDrawBitmapString( ofToString( it->getSampleID() ), it->getSample() * scale + ofPoint( DRAW_CIRCLE_RADIUS + 2, 0, 0 ) );
        }
        
        ofPopStyle();
    };
    
    virtual const ofVec2f getPanelSize() const
    {
        return ofVec2f( 200, 200 );
    };
    
    virtual const ofVec2f getSize() const
    {
        return ofVec2f( (int)params[ PARAM_NAME_WIDTH ], (int)params[ PARAM_NAME_HEIGHT ] );
    };
    
private:
    
    void processPointSamples()
    {
        pointSampleVectorRefMap[ FINGERS_TAG ]->clear();
        
        std::vector<MTouch> touches = multiTouchPad.getTouches();
                
        for (std::vector<MTouch>::iterator it = touches.begin(); it != touches.end(); ++it )
        {
            pointSampleVectorRefMap[ FINGERS_TAG ]->push_back( PointSampleT() );
                        
            pointSampleVectorRefMap[ FINGERS_TAG ]->back().setSampleID( it->ID );
            pointSampleVectorRefMap[ FINGERS_TAG ]->back().setSample( ofPoint( it->x * (int)params[ PARAM_NAME_WIDTH ], it->y * (int)params[ PARAM_NAME_HEIGHT ], 0.0f ) );
        }
    }
    
    ofxMultiTouchPad            multiTouchPad;
    
    PointSampleVectorRefMapT    pointSampleVectorRefMap;
};