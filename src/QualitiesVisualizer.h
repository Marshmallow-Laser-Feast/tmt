//
//  QualitiesVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/10/13.
//
//

#pragma once

#include "IVisualizer.h"
#include "ofxMSAControlFreak.h"

class QualitiesVisualizer : public IVisualizer
{
    
public:
    
    QualitiesVisualizer() :
    polylines(new std::vector<ofPolyline>())
    
    {
        params.setName("QualitiesVisualizer");
        
        params.addInt("smoothAmount").setClamp(true).setSnap(true);
        params.addFloat("optimizeTolerance").setClamp(true).setSnap(true);
        params.addFloat("noiseTimeSpeed").setRange(0, 20).setClamp(true).setSnap(true);
        params.addFloat("noiseAmp1").setClamp(true).setSnap(true);
        params.addFloat("noisePosScale1").setRange(0, 100).setClamp(true).setSnap(true);
        params.addFloat("noiseAmp2").setClamp(true).setSnap(true);
        params.addFloat("noisePosScale2").setRange(0, 100).setClamp(true).setSnap(true);
        params.addFloat("noiseAmpX").setClamp(true).setSnap(true);
        params.addFloat("noisePosScaleX").setRange(0, 100).setClamp(true).setSnap(true);
        params.addInt("numSegments").setClamp(true).setSnap(true);
        
        midiMappings[ &params.get(PARAM_NAME_BRIGHTNESS) ]  = std::pair<int, int>( 7, 14 );
        midiMappings[ &params.get(PARAM_NAME_TIME_OFFSET) ] = std::pair<int, int>( 7, 15 );
        midiMappings[ &params.get("smoothAmount") ]         = std::pair<int, int>( 7, 14 );
        midiMappings[ &params.get("optimizeTolerance") ]    = std::pair<int, int>( 7, 15 );
        midiMappings[ &params.get("noiseTimeSpeed") ]       = std::pair<int, int>( 7, 16 );
        midiMappings[ &params.get("noiseAmp1") ]            = std::pair<int, int>( 7, 17 );
        midiMappings[ &params.get("noisePosScale1") ]       = std::pair<int, int>( 7, 18 );
        midiMappings[ &params.get("noiseAmp2") ]            = std::pair<int, int>( 7, 19 );
        midiMappings[ &params.get("noisePosScale2") ]       = std::pair<int, int>( 7, 20 );
        midiMappings[ &params.get("noiseAmpX") ]            = std::pair<int, int>( 7, 21 );
        midiMappings[ &params.get("noisePosScaleX") ]       = std::pair<int, int>( 7, 22 );
        midiMappings[ &params.get("numSegments") ]          = std::pair<int, int>( 7, 23 );
    };
    
    ~QualitiesVisualizer()
    {};
    
public:
   
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale, float audioAmp, float audioFFT )
    {
        updateTimer();
        
        if((int)params[PARAM_NAME_BRIGHTNESS] == 0) {
            return PolylineVectorRefT(new std::vector<ofPolyline>());
        }
        
        polylines->clear();
        polylines->push_back(ofPolyline());
        
        ofPolyline &poly = polylines->at(0);
        
        int numSegments = params["numSegments"];
        float noiseAmp1 = params["noiseAmp1"];
        float noisePosScale1 = params["noisePosScale1"];
        float noiseAmp2 = params["noiseAmp2"];
        float noisePosScale2 = params["noisePosScale2"];
        float noiseAmpX = params["noiseAmpX"];
        float noisePosScaleX = params["noisePosScaleX"];
        int smoothAmount = params["smoothAmount"];
        float optimizeTolerance = params["optimizeTolerance"];

        for(int i=0; i<numSegments; i++) {
            ofVec3f p;
            p.x = ofMap(i, 0, numSegments-1, 0, INPUT_WIDTH);
            float t = ofMap(i, 0, numSegments-1, -1, 1);
            p.y = 0.5 + noiseAmp1 * 0.5 * ofSignedNoise(t * noisePosScale1 + timer) + noiseAmp2 * 0.5 * ofSignedNoise(t * noisePosScale2 + timer);
            p.y *= INPUT_HEIGHT;
            p.x += noiseAmpX * 0.5 * ofSignedNoise(t * noisePosScaleX + timer);
            p = p * scale + offset;
            poly.addVertex(p);
        }

        for(int i=0; i<polylines->size(); i++) {
            if(polylines->at(i).size()) {
//                if(smoothAmount > 0) polylines->at(i) = polylines->at(i).getSmoothed(smoothAmount);
//                if(optimizeTolerance > 0) polylines->at(i).simplify(optimizeTolerance);
            }
        }
        
        return polylines;
    };
    
private:
    PolylineVectorRefT polylines;
    float timer;
    float lastFrameTime;
    
    void updateTimer() {
        float nowTime = ofGetElapsedTimef();
        float timeDiff = nowTime - lastFrameTime;
        timer += timeDiff * (float)params["noiseTimeSpeed"];
        lastFrameTime = nowTime;
    }
};
