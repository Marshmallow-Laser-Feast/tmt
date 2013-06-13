//
//  LineVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/10/13.
//
//

#pragma once

#include "IVisualizer.h"

#include "ofMain.h"
#include "DeformableRope.h"

struct comparePointX
{
    bool operator() (const ofPoint & i, const ofPoint & j)
    {
        return (i.x<j.x);
    };
}comparePointX__;

#define PARAM_NAME_LINE_VIS_COUNT               "Line Vis Count"



class LineVisualizer : public IVisualizer
{
    
public:
    
    LineVisualizer()
    {
        params.setName("LineVisualizer");
        params.addFloat("edgeFixAmount").setClamp(true);    // fix to edges
        params.addFloat("edgeFixHeight").setRange(0, INPUT_HEIGHT).setClamp(true);

        rope = new DeformableRope(params);
        
        midiMappings[ &params.get(PARAM_NAME_BRIGHTNESS) ]                      = std::pair<int, int>( 5, 14 );
        midiMappings[ &params.get(PARAM_NAME_TIME_OFFSET) ]                     = std::pair<int, int>( 5, 15 );
        
        oscMappings[ &params.get(PARAM_NAME_BRIGHTNESS) ]   = "/LineVisualizer Brightness";
    };
    
    ~LineVisualizer()
    {};
    
public:
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale, float audioAmp, vector<float> & audioFFT, float avgFFT )
    {
        int timeOffset = params[PARAM_NAME_TIME_OFFSET];
        float edgeFixAmount = params["edgeFixAmount"];
        float edgeFixHeight = params["edgeFixHeight"];

        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        if((int)params[PARAM_NAME_BRIGHTNESS] == 0) {
            return result;
        }
        
        // add to vector
        std::vector<ofPoint>    orderedVector;
        for( int i = 0; i < inputAnalyser->getPathAnalysers().size(); ++i ) {
            orderedVector.push_back( inputAnalyser->getPathAnalysers()[i]->getSamples().back() );
        }
        
        // add point if empty
        if(orderedVector.size() == 0) {
            orderedVector.push_back(ofPoint(INPUT_WIDTH * 0.4, INPUT_HEIGHT/2));
        }
        
        // add second point if empty
        if(orderedVector.size() == 1) {
            orderedVector.push_back(orderedVector.front() + ofPoint(INPUT_WIDTH * 0.1, 0));
        }
        
        // lerp to edges
        if(edgeFixAmount) {
            orderedVector.push_back(orderedVector.front().interpolated(ofVec3f(0, edgeFixHeight, 0), edgeFixAmount));
            orderedVector.push_back(orderedVector.back().interpolated(ofVec3f(INPUT_WIDTH, edgeFixHeight, 0), edgeFixAmount));
        }

        // sort
        std::sort ( orderedVector.begin(), orderedVector.end(), comparePointX__ );

        // update rope
        rope->update(orderedVector);
        
        // offset and scale
        for(int i=0; i<rope->poly.size(); i++) {
            ofPoint &p = rope->poly[i];
            p = p * scale + offset;
        }

        result->push_back( rope->poly);
        
        return result;
    };
    
private:
    DeformableRope *rope;
};

