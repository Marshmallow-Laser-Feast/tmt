//
//  IVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/10/13.
//
//

#pragma once

#include <vector>

#include "ofMain.h"
#include "TheMeasuresTaken.h"

#include "InputAnalyser.h"

typedef ofPtr< std::vector<ofPolyline> > PolylineVectorRefT;

class IVisualizer
{
    
public:
    msa::controlfreak::ParameterGroup params;
    
    IVisualizer()
    {
        params.addInt(PARAM_NAME_BRIGHTNESS).setClamp(true);
    };
    
    virtual ~IVisualizer()
    {};
    
    
    float getBrightness() const {
        return (float)params[PARAM_NAME_BRIGHTNESS] / 100.0f;
    }
    
public:
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale )
    {};
};