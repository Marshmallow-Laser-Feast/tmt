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
#include "Params.h"
#include "ofxMSAControlFreak.h"

#include "InputAnalyser.h"

typedef ofPtr< std::vector<ofPolyline> > PolylineVectorRefT;

class IVisualizer
{
    
public:
    
    msa::controlfreak::ParameterGroup params;
    
    std::map< msa::controlfreak::Parameter*, std::pair<int, int> >  midiMappings;
    std::map< msa::controlfreak::Parameter*, std::string >          oscMappings;
    
    IVisualizer()
    {
        params.addInt(PARAM_NAME_BRIGHTNESS).setClamp(true);
        params.addInt(PARAM_NAME_TIME_OFFSET).setRange(0, MAX_PATH_ANALYSER_HISTORY).setClamp(true);
    };
    
    virtual ~IVisualizer()
    {};
    
    
    virtual float getBrightness() const {
        return (float)params[PARAM_NAME_BRIGHTNESS] / 100.0f;
    }
    
public:
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale, float audioAmp, vector<float> & audioFFT, float avgFFT )
    {};
    
};