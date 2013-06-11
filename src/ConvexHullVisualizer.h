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
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        
        int count = inputAnalyser->getPathAnalysers().size();
        ofPolyline polyline;
        for(int i=0; i<count; i++) {
            polyline.addVertex(offset + inputAnalyser->getPathAnalysers()[i]->getSamples().back() * scale );
        }
        
        result->push_back(polyline);
        return result;
    };
    
private:
};

