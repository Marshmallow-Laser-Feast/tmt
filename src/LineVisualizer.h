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
        
        oscMappings[ &params.get(PARAM_NAME_BRIGHTNESS) ]   = "/LineViz/Brightness";
        oscMappings[ &params.get("edgeFixAmount") ]         = "/LineViz/edgeFixAmount";
        oscMappings[ &params.get("Rope.Deform.amp") ]         = "/LineViz/Amp";
        
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
        std::vector<ofPoint> points;
        for( int i = 0; i < inputAnalyser->getPathAnalysers().size(); ++i ) {
            points.push_back( inputAnalyser->getSampleWithTimeOffset(i, timeOffset));
        }
        
        // add point if empty
        if(points.size() == 0) {
            points.push_back(ofPoint(INPUT_WIDTH * 0.4, INPUT_HEIGHT/2));
        }
        
        // add second point if empty
        if(points.size() == 1) {
            points.push_back(points.front() + ofPoint(INPUT_WIDTH * 0.1, 0));
        }
        
        // lerp to edges
        if(edgeFixAmount) {
            points.push_back(points.front().interpolated(ofVec3f(0, edgeFixHeight, 0), edgeFixAmount));
            points.push_back(points.back().interpolated(ofVec3f(INPUT_WIDTH, edgeFixHeight, 0), edgeFixAmount));
        }

        // sort
        std::sort ( points.begin(), points.end(), comparePointX__ );

        // update rope
        rope->update(points);
        
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

