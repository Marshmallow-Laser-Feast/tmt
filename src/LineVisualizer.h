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
        
        midiMappings[ &params.get(PARAM_NAME_BRIGHTNESS) ]                      = std::pair<int, int>( 5, 14 );
        midiMappings[ &params.get(PARAM_NAME_TIME_OFFSET) ]                     = std::pair<int, int>( 5, 15 );
        midiMappings[ &params.get(PARAM_NAME_LINE_VIS_COUNT) ]                  = std::pair<int, int>( 5, 16 );
        params.addFloat("Amp").setRange(0, 2).setClamp(true);
        params.addInt("Resample").setRange(0, 1000).setClamp(true);
        
        params.addFloat("noiseTimeSpeed").setRange(0, 20).setClamp(true).setSnap(true);
        params.addFloat("noiseAmp1").setClamp(true).setSnap(true);
        params.addFloat("noisePosScale1").setRange(0, 50).setClamp(true).setSnap(true);
        params.addFloat("noiseAmp2").setClamp(true).setSnap(true);
        params.addFloat("noisePosScale2").setRange(0, 640).setClamp(true).setSnap(true);
        params.addFloat("noiseAmpX").setClamp(true).setSnap(true);
        params.addFloat("noisePosScaleX").setRange(0, 50).setClamp(true).setSnap(true);
        params.addInt("smoothAmount").setClamp(true);

    };
    
    ~LineVisualizer()
    {};
    
public:
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale, float audioAmp )
    {
        int timeOffset = params[PARAM_NAME_TIME_OFFSET];
        float amp = params["Amp"];
        int resample = params["Resample"];
        
        float noiseAmp1 = params["noiseAmp1"];
        float noisePosScale1 = params["noisePosScale1"];
        float noiseAmp2 = params["noiseAmp2"];
        float noisePosScale2 = params["noisePosScale2"];
        float noiseAmpX = params["noiseAmpX"];
        float noisePosScaleX = params["noisePosScaleX"];
        
        int smoothAmount = params["smoothAmount"];
        
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        if((int)params[PARAM_NAME_BRIGHTNESS] == 0) {
            return result;
        }
        
        int count = params[ PARAM_NAME_LINE_VIS_COUNT ];
        
        
        int lineCount   = MIN( inputAnalyser->getPathAnalysers().size(), count );
        
        std::vector<ofPoint>    orderedVector;
        
        for( int i = 0; i < inputAnalyser->getPathAnalysers().size(); ++i )
        {
            orderedVector.push_back( inputAnalyser->getPathAnalysers()[i]->getSamples().back() );
        }
        
        std::sort ( orderedVector.begin(), orderedVector.end(), comparePointX__ );
        
        if(orderedVector.size() > 2) {
            ofPoint p1(orderedVector.front());
            ofPoint p2(orderedVector.back());
            
            ofPolyline  line;
            for( int i = 0; i < lineCount; ++i )
            {
                ofPoint p(orderedVector[i]);
                float t = ofMap(p.x, p1.x, p2.x, 0.0, 1.0, true);
                float destY = ofLerp(p1.y, p2.y, t);
                p.y = ofLerp(destY, p.y, amp);
                p = p * scale + offset;
                line.addVertex(p);
            }
            
            // add noise
            if(resample) line = line.getResampledByCount(resample);
            int numPoints = line.size();
            for(int j=0; j<numPoints; j++) {
                float t = ofMap(j, 0, numPoints-1, -1, 1);
                ofPoint &p = line[j];
                p.y += (noiseAmp1 * ofSignedNoise(t * noisePosScale1) + noiseAmp2 * ofSignedNoise(t * noisePosScale2));
                p.x += noiseAmpX * ofSignedNoise(t * noisePosScaleX);
            }
            if(smoothAmount) line = line.getSmoothed(smoothAmount);

            
            result->push_back( line );
        }
        
        return result;
    };
    
private:
    
};

