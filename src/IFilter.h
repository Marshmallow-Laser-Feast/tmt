//
//  IFilter.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/12/13.
//
//

#pragma once

#include "ofMain.h"

#include "ofxMSAControlFreak.h"

#define PARAM_NAME_ACTIVITY_VALUE   "Activity Value"

class IFilter
{
    
public:
    
    msa::controlfreak::ParameterGroup params;
    
    std::map< msa::controlfreak::Parameter*, std::pair<int, int> > midiMappings;
    
    IFilter()
    {
        params.addFloat(PARAM_NAME_ACTIVITY_VALUE).setClamp(true);
    };
    
    virtual ~IFilter(){};
    
public:
    
    virtual void apply( std::vector<ofPolyline> &polylines ){};
        
};