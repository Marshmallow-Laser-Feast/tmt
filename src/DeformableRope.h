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
        params.addFloat("noiseTimeSpeed").setRange(0, 20).setClamp(true).setSnap(true);
        params.addFloat("noiseAmp1").setClamp(true).setSnap(true);
        params.addFloat("noisePosScale1").setRange(0, 50).setClamp(true).setSnap(true);
        params.addFloat("noiseAmp2").setClamp(true).setSnap(true);
        params.addFloat("noisePosScale2").setRange(0, 640).setClamp(true).setSnap(true);
        params.addFloat("noiseAmpX").setClamp(true).setSnap(true);
        params.addFloat("noisePosScaleX").setRange(0, 50).setClamp(true).setSnap(true);
        params.endGroup();
        
        params.startGroup("Post FX");
        params.addInt("smoothAmount").setClamp(true);
        params.addFloat("easeAmount").setClamp(true);
        params.endGroup();


        params.endGroup();
        
        physics.disableCollision();
        physics.setDrag(1);
    }
    
    void update(ofPoint a, ofPoint b){ //, const ofPolyline &deformer) {
        rope.numParticles = params["Rope.Physics.numParticles"];
        rope.stiffness = params["Rope.Physics.stiffness"];
        rope.restLengthMult = params["Rope.Physics.restLengthMult"];
        rope.restLengthSpeed = params["Rope.Physics.restLengthSpeed"];
        rope.radiusBase = params["Rope.Physics.stiffness"];
        rope.radiusMult = params["Rope.Physics.radiusMult"];
        
        float amp = params["Rope.Deform.amp"];
        float centerOffset = params["Rope.Deform.centerOffset"];
        float curvature = params["Rope.Deform.curvature"];

        
        rope.set(a, b, physics);
        rope.update();
        
        physics.update();
        for(int i=0; i<rope.particles.size(); i++) {
            msa::physics::Particle2D &p = *rope.particles[i];
            float t = ofMap(i, 0, rope.particles.size()-1, 0, 1);
            
            if(curvature != 0) p.moveBy(ofVec2f(0, curvature * sin(t * PI)));
            if(centerOffset != 0) p.moveBy(ofVec2f(0, centerOffset));
        }
        
    }
};