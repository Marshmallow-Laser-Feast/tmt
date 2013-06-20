//
//  HelpTextWidget.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/19/13.
//
//

#pragma once

#include <string>

#include "IPanelDraws.h"

class HelpTextWidget: public IPanelDraws
{
    
public:
    
    HelpTextWidget()
    {
        std::stringstream stream;
        
        stream << "  <f>    Toggles fullscreen" << endl;
        stream << "  <s>    Saves all parameters and ui state" << endl;
        stream << "  <r>    Resets panels from (via saved file)" << endl;
        stream << "  <p>    Toggles pannel visibility" << endl;
        stream << " <tab>   Opens contextual panel menu" << endl;
        stream << "<space>  Toggles controlfreak visibility" << endl;
        
        helpString  = stream.str();
    };
    
    ~HelpTextWidget()
    {};
    
public:
    
    virtual std::string getName(){ return "Help"; };
    
    virtual void draw( float width, float height )
    {
        ofPushStyle();
        
        ofDrawBitmapString( helpString, 10, 20 );
        
        ofPopStyle();
    };
    
    virtual ofVec2f getSize(){ return ofVec2f(360, 105); };
    
private:
    
    std::string helpString;
    
};