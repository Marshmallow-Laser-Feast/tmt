//
//  StatsWidget.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/19/13.
//
//

#pragma once

#include <string>

#include "ofMain.h"

#include "IPanelDraws.h"

#define TIMEOUT 2

class StatsWidget: public IPanelDraws
{
    
public:
    
    StatsWidget()
    
    :oscEnabled( false )
    ,midiEnbled( false )
    ,oscMessageReceived( false )
    ,midiMessageReceived( false )
    ,oscTimeout( 0 )
    ,midiTimeout( 0 )
    
    {
    };
    
    ~StatsWidget()
    {};
    
public:
    
    virtual void draw( float width, float height )
    {
        std::stringstream stream;
        
        stream << "Framerate                : " << ofToString( ofGetFrameRate() ) << endl;
        stream << "OSC Enabled              : " << ofToString( oscEnabled ) << endl;
        stream << "Midi Enabled             : " << ofToString( midiEnbled ) << endl;
        stream << "OSC Message Received     : " << ofToString( oscMessageReceived ) << endl;
        stream << "Midi Message Received    : " << ofToString( midiMessageReceived ) << endl;
        
        ofPushStyle();
        
        ofDrawBitmapString( stream.str(), 10, 20 );
        
        ofPopStyle();
    };
    
    void setOSC( bool received )
    {
        if( oscMessageReceived != received )
        {
            if( !received )
            {
                if( oscTimeout == TIMEOUT )
                {
                    oscMessageReceived  = received;
                    oscTimeout          = 0;
                } else {
                    oscTimeout++;
                }
            } else {
                oscMessageReceived      = received;
                oscTimeout              = 0;
            }
        } else {
            oscTimeout                  = 0;
        }
    }
    
    void setMidi( bool received )
    {
        if( midiMessageReceived != received )
        {
            if( !received )
            {
                if( midiTimeout == TIMEOUT )
                {
                    midiMessageReceived  = received;
                    midiTimeout          = 0;
                } else {
                    midiTimeout++;
                }
            } else {
                midiMessageReceived     = received;
                midiTimeout             = 0;
            }
            
        } else {    
            midiTimeout                 = 0;
        }
    }
    
    virtual const std::string getPanelName() const { return "Stats"; };
    
    virtual const ofVec2f getPanelSize() const { return ofVec2f(360, 90); };
    
public:
    
    bool        oscEnabled;
    bool        midiEnbled;
    bool        oscMessageReceived;
    bool        midiMessageReceived;
    
    int         oscTimeout;
    int         midiTimeout;
};