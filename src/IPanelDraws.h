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
    
    virtual std::string getName(){ return "Empty Panel Draws"; };
    
    virtual void draw( float width, float height ){};
    
};