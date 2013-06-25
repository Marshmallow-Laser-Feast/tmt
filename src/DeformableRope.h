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
        params.addFloat("drag").setRange(0, 1).setClamp(true);
        params.addInt("iterations").setRange(0, 50).setClamp(true);
        
        params.addInt("numParticles").setRange(0, 200).setClamp(true).setSnap(true);
        params.addFloat("stiffness").setRange(0, 1).setClamp(true).setSnap(true);
        params.addFloat("stiffnessHome").setRange(0, 1).setClamp(true).setSnap(true);
        params.addFloat("restLengthMult").setRange(0, 1).setClamp(true).setSnap(true);
        params.addFloat("restLengthSpeed").setRange(0, 1).setClamp(true).setSnap(true);
        params.addFloat("radiusBase").setRange(0, 50).setClamp(true).setSnap(true);
        params.addFloat("radiusMult").setRange(0, 50).setClamp(true).setSnap(true);
        params.endGroup();
        
        params.startGroup("Deform");
        params.addFloat("amp").setRange(0, 5).setClamp(true);
        params.addInt("centerOffset").setRange(-INPUT_HEIGHT, INPUT_HEIGHT).setClamp(true);
        params.addInt("curvature").setRange(-INPUT_HEIGHT, INPUT_HEIGHT).setClamp(true);
        params.endGroup();
        
        params.startGroup("Noise");
        params.addInt("resample").setRange(0, 1000).setClamp(true);
        //        params.addFloat("noiseTimeSpeed").setRange(0, 20).setClamp(true).setSnap(true);
        params.addFloat("noiseAmp1").setRange(0, INPUT_HEIGHT).setClamp(true).setSnap(true);
        params.addFloat("noisePosScale1").setRange(0, 50).setClamp(true).setSnap(true);
        params.addFloat("noiseAmp2").setRange(0, INPUT_HEIGHT).setClamp(true).setSnap(true);
        params.addFloat("noisePosScale2").setRange(0, 640).setClamp(true).setSnap(true);
        params.addFloat("noiseAmpX").setRange(0, INPUT_WIDTH).setClamp(true).setSnap(true);
        params.addFloat("noisePosScaleX").setRange(0, 50).setClamp(true).setSnap(true);
        params.endGroup();
        
        params.startGroup("Post");
        params.addInt("smoothPoly").setClamp(true);
        params.addFloat("easeAmount").setClamp(true);
        params.endGroup();
        
        
        params.endGroup();
        
        physics.disableCollision();
        physics.setDrag(1);
    }
    
    void update(const vector<ofVec2f>& shape) {
        if(shape.size() < 2) return;
        
        physics.setDrag(params["Rope.Physics.drag"]);
        physics.setNumIterations(params["Rope.Physics.iterations"]);
        
        rope.numParticles = params["Rope.Physics.numParticles"];
        rope.stiffness = params["Rope.Physics.stiffness"];
        rope.stiffnessHome = params["Rope.Physics.stiffnessHome"];
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
        
        int smoothPoly = params["Rope.Post.smoothPoly"];
        float easeAmount = params["Rope.Post.easeAmount"];
        
        
        ofVec2f a(shape.front());
        ofVec2f b(shape.back());
        
        rope.set(a, b, physics);
        rope.update();
        
        if(curvature != 0 || centerOffset != 0) {
            for(int i=0; i<rope.particles.size(); i++) {
                msa::physics::Particle2D &p = *rope.particles[i];
                
                //            ofVec2f pos(p.getPosition());
                //            pos.x /= INPUT_WIDTH;
                //            pos.y /= INPUT_HEIGHT;
                float t = ofMap(i, 0, rope.particles.size()-1, 0, 1);
                //            float ts = ofMap(i, 0, rope.particles.size()-1, -1, 1);
                
                // bell curve (kind of)
                //            float ta = t < 0.5 ? t * 2 : (1 - t) * 2;
                //            ta = 3 * ta * ta - 2 * ta * ta * ta;
                
                ofVec2f o;
                if(curvature != 0) o.y += curvature * sin(t * PI);
                if(centerOffset != 0) o.y += centerOffset;
                //            if(noiseAmp1) o.y += ta * noiseAmp1 * ofSignedNoise(ts * noisePosScale1);
                //            if(noiseAmp2) o.y += ta * noiseAmp2 * ofSignedNoise(ts * noisePosScale2);
                //            if(noiseAmpX) o.x += ta * noiseAmpX * ofSignedNoise(ts * noisePosScaleX);
                //            if(amp && shape.size() > 1) {
                //                float homeY = ofLerp(a.y, b.y, t);
                //                float deformedY = shape.getPointAtPercent(t).y;
                //                o.y += (deformedY - homeY) * amp;
                //            }
                p.moveBy(o);
            }
        }
        
        if(amp) {
            for(int i=0; i<shape.size(); i++) {
                // shoot a ray up, if there is another point on the same contour, then don't use that point
                ofVec2f p(shape[i]);
                
                bool bIsTop = true;
                // find a segment which crosses this point
                for(int j=0; j<shape.size()-1; j++) {
                    
                    // don't check segments including this point
                    if(j < i || j > i+1) {
                        ofVec2f p1(shape[j]);
                        ofVec2f p2(shape[j+1]);
                        // found a segment
                        if(ofInRange(p.x, MIN(p1.x, p2.x), MAX(p1.x, p2.x))) {
                            float py = ofMap(p.x, p1.x, p2.x, p1.y, p2.y);
                            if(py > p.y) bIsTop = false;
                        }
                    }
                }
                if(bIsTop) {
                    int particleIndex = round(ofMap(p.x, a.x, b.x, 0, rope.particles.size()-1));
                    float homeY = ofMap(particleIndex, 0, rope.particles.size()-1, a.y, b.y);
                    rope.particles[particleIndex]->moveBy(ofVec2f(0, (p.y - homeY) * amp));
                }
            }
        }
        
        physics.update();
        
        
        poly = resample ? rope.drawVector(easeAmount).getResampledByCount(resample) : rope.drawVector(easeAmount);
        //
        // post noise
        if(noiseAmp1 > 0 || noiseAmp2 > 0 || noiseAmpX > 0) {
            for(int j=0; j<poly.size(); j++) {
                float t = ofMap(j, 0, poly.size()-1, -1, 1);
                ofPoint &p = poly[j];
                
                p.y += (noiseAmp1 * ofSignedNoise(t * noisePosScale1) + noiseAmp2 * ofSignedNoise(t * noisePosScale2));
                p.x += noiseAmpX * ofSignedNoise(t * noisePosScaleX);
            }
        }
        
        if(smoothPoly) poly = poly.getSmoothed(smoothPoly);
    }
};