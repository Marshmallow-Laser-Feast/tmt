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

#include "IControlFreakMapper.h"
#include "IControlFreakMapperOSCExt.h"
#include "IControlFreakMapperMidiExt.h"

#include "IPanelDraws.h"

#include "Input.h"
#include "InputSample.h"

#define DRAW_CIRCLE_RADIUS      2.0f

#define PARAM_NAME_WIDTH        "width"
#define PARAM_NAME_HEIGHT       "height"

#define PARAM_NAME_ENABLE       "Enabled"

class MultiTouchInput : public Input, public IControlFreakMapper, public IControlFreakMapperMidiExt, public IControlFreakMapperOSCExt, public IPanelDraws
{
    
protected:
    
    typedef std::map< std::string, PointSampleVectorRefT>   PointSampleVectorRefMapT;
    
public:
    
    static const std::string DEFAULT_TAG;

public:
    
    MultiTouchInput()
    
    :IControlFreakMapper( "Input/Video Analysis" )
    
    {
        IControlFreakMapperMidiExt::setParams( params );
        IControlFreakMapperOSCExt::setParams( params );
        
        setupMidi();
        setupOCS();
        
        multiTouchPad   = ofxMultiTouchPad();
        
        params.addBool( PARAM_NAME_ENABLE ).set( true );
        
        params.addInt( PARAM_NAME_WIDTH ).set( 1024 );
        params.addInt( PARAM_NAME_HEIGHT ).set( 768 );
        
        pointSampleVectorRefMap[ DEFAULT_TAG ]  = PointSampleVectorRefT( new PointSampleVectorT() );
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
        
        ofSetColor( ofColor::yellow );
        
        ofVec3f scale( width / (int)params[ PARAM_NAME_WIDTH ], height / (int)params[ PARAM_NAME_HEIGHT ] );
        
        for( vector<PointSampleT>::const_iterator it = pointSampleVectorRefMap[ DEFAULT_TAG ]->begin(); it != pointSampleVectorRefMap[ DEFAULT_TAG ]->end(); ++it )
        {
            ofCircle( it->getSample() * scale, DRAW_CIRCLE_RADIUS );
        }
        
        ofPopStyle();
    };
    
    virtual std::string getName(){ return "Input/Multi Touch"; };
    
    virtual ofVec2f getSize()
    {
        return ofVec2f( 200, 200 );
    };
    
private:
    
    void processPointSamples()
    {
        pointSampleVectorRefMap[ DEFAULT_TAG ]->clear();
        
        std::vector<MTouch> touches = multiTouchPad.getTouches();
                
        for (std::vector<MTouch>::iterator it = touches.begin(); it != touches.end(); ++it )
        {
            pointSampleVectorRefMap[ DEFAULT_TAG ]->push_back( PointSampleT() );
                        
            pointSampleVectorRefMap[ DEFAULT_TAG ]->back().setSampleID( it->ID );
            pointSampleVectorRefMap[ DEFAULT_TAG ]->back().setSample( ofPoint( it->x * (int)params[ PARAM_NAME_WIDTH ], it->y * (int)params[ PARAM_NAME_HEIGHT ], 0.0f ) );
        }
    }
    
    ofxMultiTouchPad            multiTouchPad;
    
    PointSampleVectorRefMapT    pointSampleVectorRefMap;
};