//
//  IPanelDraws.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/19/13.
//
//

#pragma once

#include "ofMain.h"

class IPanelDraws
{
    
public:
    
    IPanelDraws(){};
    
    virtual ~IPanelDraws(){};
    
public:
    
    virtual const std::string getPanelName() const { return "Empty Panel Draws"; };
    
    virtual void draw( float width, float height ){};
    
    virtual const ofVec2f getPanelSize() const { return ofVec2f(300, 300); };
    
};