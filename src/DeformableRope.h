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
        params.addInt("numParticles").setRange(0, 50).setClamp(true).setSnap(true);
        params.addFloat("stiffness").setRange(0, 1).setClamp(true).setSnap(true);
        params.addFloat("restLengthMult").setRange(0, 1).setClamp(true).setSnap(true);
        params.addFloat("restLengthSpeed").setRange(0, 1).setClamp(true).setSnap(true);
        params.addFloat("radiusBase").setRange(0, 50).setClamp(true).setSnap(true);
        params.addFloat("radiusMult").setRange(0, 50).setClamp(true).setSnap(true);

        
        params.addFloat("noiseTimeSpeed").setRange(0, 20).setClamp(true).setSnap(true);
        params.addFloat("noiseAmp1").setClamp(true).setSnap(true);
        params.addFloat("noisePosScale1").setRange(0, 50).setClamp(true).setSnap(true);
        params.addFloat("noiseAmp2").setClamp(true).setSnap(true);
        params.addFloat("noisePosScale2").setRange(0, 640).setClamp(true).setSnap(true);
        params.addFloat("noiseAmpX").setClamp(true).setSnap(true);
        params.addFloat("noisePosScaleX").setRange(0, 50).setClamp(true).setSnap(true);
        params.addInt("smoothAmount").setClamp(true);
        
        physics.disableCollision();
        physics.setDrag(1);
    }
    
    void update(ofPoint a, ofPoint b) {
        rope.numParticles = params["numParticles"];
        rope.stiffness = params["stiffness"];
        rope.restLengthMult = params["restLengthMult"];
        rope.restLengthSpeed = params["restLengthSpeed"];
        rope.radiusBase = params["stiffness"];
        rope.radiusMult = params["radiusMult"];
        
        rope.set(a, b, physics);
        rope.update();
        physics.update();
    }
};