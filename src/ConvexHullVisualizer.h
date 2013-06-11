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
        params.setName("NearestDotsVisualizer");
    };
    
    ~ConvexHullVisualizer()
    {};
    
public:
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale )
    {
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        
        return result;
    };
    
private:
};

