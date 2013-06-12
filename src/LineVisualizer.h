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

        rope = new DeformableRope(params);
        
        midiMappings[ &params.get(PARAM_NAME_BRIGHTNESS) ]                      = std::pair<int, int>( 5, 14 );
        midiMappings[ &params.get(PARAM_NAME_TIME_OFFSET) ]                     = std::pair<int, int>( 5, 15 );
        
        oscMappings[ &params.get(PARAM_NAME_BRIGHTNESS) ]   = "/LineVisualizer Brightness";
        //oscMappings[ &params.get("Amps") ]                  = "/LineVisualizer Amp";

    };
    
    ~LineVisualizer()
    {};
    
public:
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale, float audioAmp, vector<float> & audioFFT, float avgFFT )
    {
        int timeOffset = params[PARAM_NAME_TIME_OFFSET];

        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        if((int)params[PARAM_NAME_BRIGHTNESS] == 0) {
            return result;
        }
        
        std::vector<ofPoint>    orderedVector;
        for( int i = 0; i < inputAnalyser->getPathAnalysers().size(); ++i )
        {
            orderedVector.push_back( inputAnalyser->getPathAnalysers()[i]->getSamples().back() );
        }
        
        std::sort ( orderedVector.begin(), orderedVector.end(), comparePointX__ );

        if(orderedVector.size() == 0) {
            orderedVector.push_back(ofPoint(INPUT_WIDTH * 0.4, INPUT_HEIGHT/2));
        }
        
        if(orderedVector.size() == 1) {
            orderedVector.push_back(orderedVector.front() + ofPoint(INPUT_WIDTH * 0.1, 0));
        }
        
        int numPoints   = orderedVector.size();

        ofPoint p1(orderedVector.front());
        ofPoint p2(orderedVector.back());
        
//        ofPolyline  line;
//        for( int i = 0; i < numPoints; ++i )
//        {
//            ofPoint p(orderedVector[i]);
//            float t = ofMap(p.x, p1.x, p2.x, 0.0, 1.0, true);
//            float destY = ofLerp(p1.y, p2.y, t);
//            p.y = ofLerp(destY, p.y, amp);
//            p = p * scale + offset;
//            line.addVertex(p);
//        }
        
        rope->update(p1, p2, ofPolyline(orderedVector));
        
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

