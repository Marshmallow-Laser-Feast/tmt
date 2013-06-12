//
//  DeformableRope.h
//  TheMeasuresTaken
//
//  Created by Memo Akten on 12/06/2013.
//
//

#pragma once 

#include "Rope.h"
#include "ofxMSAControlFreak.h"

class DeformableRope {
public:
    Rope rope;
    ofPolyline poly;
    msa::physics::World2D physics;
    msa::controlfreak::ParameterGroup &params;
    
    DeformableRope(msa::controlfreak::ParameterGroup &params) : params(params) {
        params.startGroup("Rope");
        params.startGroup("Physics");
        params.addInt("numParticles").setRange(0, 50).setClamp(true).setSnap(true);
        params.addFloat("stiffness").setRange(0, 1).setClamp(true).setSnap(true);
        params.addFloat("restLengthMult").setRange(0, 1).setClamp(true).setSnap(true);
        params.addFloat("restLengthSpeed").setRange(0, 1).setClamp(true).setSnap(true);
        params.addFloat("radiusBase").setRange(0, 50).setClamp(true).setSnap(true);
        params.addFloat("radiusMult").setRange(0, 50).setClamp(true).setSnap(true);
        params.endGroup();
        
        params.startGroup("Deform");
        params.addFloat("amp").setRange(0, 5).setClamp(true);
        params.addFloat("centerOffset").setRange(-200, 200).setClamp(true);
        params.addFloat("curvature").setRange(-1, 1).setClamp(true);
        params.endGroup();
        
        params.startGroup("Noise");
        params.addInt("resample").setRange(0, 1000).setClamp(true);
//        params.addFloat("noiseTimeSpeed").setRange(0, 20).setClamp(true).setSnap(true);
        params.addFloat("noiseAmp1").setClamp(true).setSnap(true);
        params.addFloat("noisePosScale1").setRange(0, 50).setClamp(true).setSnap(true);
        params.addFloat("noiseAmp2").setClamp(true).setSnap(true);
        params.addFloat("noisePosScale2").setRange(0, 640).setClamp(true).setSnap(true);
        params.addFloat("noiseAmpX").setClamp(true).setSnap(true);
        params.addFloat("noisePosScaleX").setRange(0, 50).setClamp(true).setSnap(true);
        params.endGroup();
        
        params.startGroup("Post");
        params.addInt("smoothAmount").setClamp(true);
        params.addFloat("easeAmount").setClamp(true);
        params.endGroup();


        params.endGroup();
        
        physics.disableCollision();
        physics.setDrag(1);
    }
    
    void update(ofPoint a, ofPoint b, const ofPolyline &deformer) {
        rope.numParticles = params["Rope.Physics.numParticles"];
        rope.stiffness = params["Rope.Physics.stiffness"];
        rope.restLengthMult = params["Rope.Physics.restLengthMult"];
        rope.restLengthSpeed = params["Rope.Physics.restLengthSpeed"];
        rope.radiusBase = params["Rope.Physics.stiffness"];
        rope.radiusMult = params["Rope.Physics.radiusMult"];
        
        float amp = params["Rope.Deform.amp"];
        float centerOffset = params["Rope.Deform.centerOffset"];
        float curvature = params["Rope.Deform.curvature"];
        
        int resample = params["Rope.Noise.resample"];
        float noiseAmp1 = params["Rope.Noise.noiseAmp1"];
        float noisePosScale1 = params["Rope.Noise.noisePosScale1"];
        float noiseAmp2 = params["Rope.Noise.noiseAmp2"];
        float noisePosScale2 = params["Rope.Noise.noisePosScale2"];
        float noiseAmpX = params["Rope.Noise.noiseAmpX"];
        float noisePosScaleX = params["Rope.Noise.noisePosScaleX"];

        int smoothAmount = params["Rope.Post.smoothAmount"];


        
        rope.set(a, b, physics);
        rope.update();
        
        physics.update();
        
        for(int i=0; i<rope.particles.size(); i++) {
            msa::physics::Particle2D &p = *rope.particles[i];
            float t = ofMap(i, 0, rope.particles.size()-1, 0, 1);
            float ts = ofMap(i, 0, rope.particles.size()-1, -1, 1);

            ofVec2f o;
            if(curvature != 0) o.y += curvature * sin(t * PI);
            if(centerOffset != 0) o.y += centerOffset;
            if(noiseAmp1) o.y += noiseAmp1 * ofSignedNoise(ts * noisePosScale1);
            if(noiseAmp2) o.y += noiseAmp2 * ofSignedNoise(ts * noisePosScale2);
            if(noiseAmpX) o.x += noiseAmpX * ofSignedNoise(ts * noisePosScaleX);
            if(amp && deformer.size() > 2) {
                float homeY = ofLerp(a.y, b.y, t);
                float deformedY = deformer.getPointAtPercent(t).y;
                float y = ofLerp(homeY, deformedY, amp);
            }
            p.moveBy(o);
        }
        
        poly = resample ? rope.drawVector().getResampledByCount(resample) : rope.drawVector();
//
//        for(int j=0; j<poly.size(); j++) {
//            float t = ofMap(j, 0, poly.size()-1, -1, 1);
//            ofPoint &p = poly[j];
//            p.y += (noiseAmp1 * ofSignedNoise(t * noisePosScale1) + noiseAmp2 * ofSignedNoise(t * noisePosScale2));
//            p.x += noiseAmpX * ofSignedNoise(t * noisePosScaleX);
//        }
        
        if(smoothAmount) poly = poly.getSmoothed(smoothAmount);
    }
};