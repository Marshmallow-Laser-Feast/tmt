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
    msa::controlfreak::ParameterGroup params;
    
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
        params.setName("Qualities");
        params.addInt("smoothAmount").setClamp(true).setSnap(true);
        params.addFloat("optimizeTolerance").setClamp(true).setSnap(true);
        params.addFloat("noisePosScale").setClamp(true).setSnap(true);
        params.addFloat("noiseTimeSpeed").setClamp(true).setSnap(true);
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
        float noisePosScale = params["noisePosScale"];
//        float noiseTimeSpeed = params["noiseTimeSpeed"];
        int smoothAmount = params["smoothAmount"];
        float optimizeTolerance = params["optimizeTolerance"];

        for(int i=0; i<numSegments; i++) {
            ofVec3f p;
            p.x = ofMap(i, 0, numSegments-1, 0, 1);
            p.y = ofSignedNoise(p.x * noisePosScale + timer);
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
        timer += nowTime * (float)params["noiseTimeSpeed"];
        lastFrameTime = nowTime;
    }
};
