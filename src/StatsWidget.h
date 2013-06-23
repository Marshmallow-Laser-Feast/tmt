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
    ,oscMessageRecieved( false )
    ,midiMessageRecieved( false )
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
        stream << "OSC Message Recieved     : " << ofToString( oscMessageRecieved ) << endl;
        stream << "Midi Message Recieved    : " << ofToString( midiMessageRecieved ) << endl;
        
        ofPushStyle();
        
        ofDrawBitmapString( stream.str(), 10, 20 );
        
        ofPopStyle();
    };
    
    void setOSC( bool recieved )
    {
        if( oscMessageRecieved != recieved )
        {
            if( !recieved )
            {
                if( oscTimeout == TIMEOUT )
                {
                    oscMessageRecieved  = recieved;
                    oscTimeout          = 0;
                } else {
                    oscTimeout++;
                }
            } else {
                oscMessageRecieved      = recieved;
                oscTimeout              = 0;
            }
        } else {
            oscTimeout                  = 0;
        }
    }
    
    void setMidi( bool recieved )
    {
        if( midiMessageRecieved != recieved )
        {
            if( !recieved )
            {
                if( midiTimeout == TIMEOUT )
                {
                    midiMessageRecieved  = recieved;
                    midiTimeout          = 0;
                } else {
                    midiTimeout++;
                }
            } else {
                midiMessageRecieved     = recieved;
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
    bool        oscMessageRecieved;
    bool        midiMessageRecieved;
    
    int         oscTimeout;
    int         midiTimeout;
};