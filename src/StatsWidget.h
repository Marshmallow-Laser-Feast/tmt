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

class StatsWidget: public IPanelDraws
{
    
public:
    
    StatsWidget()
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
    
    virtual const std::string getPanelName() const { return "Stats"; };
    
    virtual const ofVec2f getPanelSize() const { return ofVec2f(360, 90); };
    
public:
    
    bool        oscEnabled;
    bool        midiEnbled;
    bool        oscMessageRecieved;
    bool        midiMessageRecieved;
};