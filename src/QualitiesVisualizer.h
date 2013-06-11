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
    
//    int smoothAmount;   // how much to smooth the path (zero to ignore)
//    float optimizeTolerance;    // howmuch to optimize the path, based on curvature (zero to ignore)
//    
//    float noisePosScale;
//    float noiseTimeSpeed;
//    
//    int numSegments;
//
    QualitiesVisualizer() :
    polylines(new std::vector<ofPolyline>())
    
//    :uSamples( 0 )
//    ,ucount( 0 )
//    ,vcount( 0 )
//    ,width( 0.0f)
//    ,height( 0.0f )
//    ,noiseHeight( 0.0f )
//    ,noiseOffset( 0.0f )
    
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
    };
    
    ~QualitiesVisualizer()
    {};
    
public:
    
//    void setUSamples( int uSamples_ )
//    {
//        uSamples              = uSamples_;
//    };
//    
//    void setUCount( int ucount_ )
//    {
//        ucount              = ucount_;
//    };
//    
//    void setVCount( int vcount_ )
//    {
//        vcount              = vcount_;
//    };
//    
//    void setSize( float width_, float height_ )
//    {
//        width               = width_;
//        height              = height_;
//    };
//    
//    void setNoiseHeight( float noiseHeight_ )
//    {
//        noiseHeight          = noiseHeight_;
//    };
//    
//    void setNoiseOffset( float noiseOffset_ )
//    {
//        noiseOffset         = noiseOffset_;
//    }

    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale )
    {
        updateTimer();
        
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
            p.x = ofMap(i, 0, numSegments-1, 0, 1);
            float t = ofMap(i, 0, numSegments-1, -1, 1);
            p.y = 0.5 + noiseAmp1 * 0.5 * ofSignedNoise(t * noisePosScale1 + timer) + noiseAmp2 * 0.5 * ofSignedNoise(t * noisePosScale2 + timer);
            p.x += noiseAmpX * 0.5 * ofSignedNoise(t * noisePosScaleX + timer);
            poly.addVertex(p);
        }

        for(int i=0; i<polylines->size(); i++) {
            if(polylines->at(i).size()) {
                if(smoothAmount > 0) polylines->at(i) = polylines->at(i).getSmoothed(smoothAmount);
                if(optimizeTolerance > 0) polylines->at(i).simplify(optimizeTolerance);
            }
        }
        
        return polylines;
    };
    
private:
    PolylineVectorRefT polylines;
    float timer;
    float lastFrameTime;
//    int     uSamples;
//    int     ucount;
//    int     vcount;
//    
//    float   width;
//    float   height;
//    float   noiseHeight;
//    float   noiseOffset;
    
    
    void updateTimer() {
        float nowTime = ofGetElapsedTimef();
        float timeDiff = nowTime - lastFrameTime;
        timer += timeDiff * (float)params["noiseTimeSpeed"];
        lastFrameTime = nowTime;
    }
};
