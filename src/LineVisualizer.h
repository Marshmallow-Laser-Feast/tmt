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
        params.addInt( PARAM_NAME_LINE_VIS_COUNT ).setRange( 0, 100 ).setClamp( true );
        
        params.addFloat("Amp").setRange(0, 2).setClamp(true);
        params.addInt("Resample").setRange(0, 1000).setClamp(true);
        
        rope = new DeformableRope(params);
        
        
        
        midiMappings[ &params.get(PARAM_NAME_BRIGHTNESS) ]                      = std::pair<int, int>( 5, 14 );
        midiMappings[ &params.get(PARAM_NAME_TIME_OFFSET) ]                     = std::pair<int, int>( 5, 15 );
        midiMappings[ &params.get(PARAM_NAME_LINE_VIS_COUNT) ]                  = std::pair<int, int>( 5, 16 );

    };
    
    ~LineVisualizer()
    {};
    
public:
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale, float audioAmp )
    {
        int timeOffset = params[PARAM_NAME_TIME_OFFSET];
//        float amp = params["Amp"];
//        int resample = params["Resample"];
        
//        float noiseAmp1 = params["noiseAmp1"];
//        float noisePosScale1 = params["noisePosScale1"];
//        float noiseAmp2 = params["noiseAmp2"];
//        float noisePosScale2 = params["noisePosScale2"];
//        float noiseAmpX = params["noiseAmpX"];
//        float noisePosScaleX = params["noisePosScaleX"];
//        
//        int smoothAmount = params["smoothAmount"];
        
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
        
        int numPoints   = MIN(orderedVector.size(), (int)params[ PARAM_NAME_LINE_VIS_COUNT ] );
        

        ofPoint p1(orderedVector.front() * scale + offset);
        ofPoint p2(orderedVector.back() * scale + offset);
        
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
        
        rope->update(p1, p2);
        

        result->push_back( rope->poly);
        
        return result;
    };
    
private:
    DeformableRope *rope;
};

