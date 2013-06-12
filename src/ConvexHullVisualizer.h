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
    };
    
    ~ConvexHullVisualizer()
    {};
    
public:
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale )
    {
        int timeOffset = params[PARAM_NAME_TIME_OFFSET];
        
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        if((int)params[PARAM_NAME_BRIGHTNESS] == 0) {
            return result;
        }
        
        int count = inputAnalyser->getPathAnalysers().size();
        ofPolyline polyline;
        for(int i=0; i<count; i++) {
            int index = ofClamp(inputAnalyser->getPathAnalysers()[i]->getSamples().size()-1-timeOffset, 0, inputAnalyser->getPathAnalysers()[i]->getSamples().size()-1);
            polyline.addVertex(offset + inputAnalyser->getPathAnalysers()[i]->getSamples()[index] * scale);
        }
        result->push_back(ofxCv::convexHull(polyline));
        return result;
    };
    
private:
};

