//
//  NearestDotsVisualizer.h
//  TheMeasuresTaken
//
//  Created by Memo Akten on 11/10/13.
//
//

#pragma once

#include "IVisualizer.h"

#include "ofMain.h"
#include "Params.h"


class ConvexHullVisualizer : public IVisualizer
{
    
public:
    
    ConvexHullVisualizer()
    {
        params.setName("ConvexHullVisualizer");
        
        midiMappings[ &params.get(PARAM_NAME_BRIGHTNESS) ]  = std::pair<int, int>( 8, 14 );
        midiMappings[ &params.get(PARAM_NAME_TIME_OFFSET) ] = std::pair<int, int>( 8, 15 );
    };
    
    ~ConvexHullVisualizer()
    {};
    
public:
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale, float audioAmp, float audioFFT )
    {
        int timeOffset = params[PARAM_NAME_TIME_OFFSET];
        
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        if((int)params[PARAM_NAME_BRIGHTNESS] == 0) {
            return result;
        }
        
        int count = inputAnalyser->getPathAnalysers().size();
        ofPolyline polyline;
        for(int i=0; i<count; i++) {
            polyline.addVertex(offset + inputAnalyser->getSampleWithTimeOffset(i, timeOffset) * scale);
        }
        result->push_back(ofxCv::convexHull(polyline));
        return result;
    };
    
private:
};

