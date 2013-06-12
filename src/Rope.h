#pragma once

#include "MSAPhysics2D.h"

class Rope {
public:
    vector<msa::physics::Particle2D*> particles;
    vector<msa::physics::Spring2D*> springs;
    
    msa::physics::World2D physics;
//    static msa::physics::World2D *physics;
    static int numParticles;
    static float stiffness;
    static float restLengthMult;
    static float restLengthSpeed;
    static float radiusBase;
    static float radiusMult;
    
    //--------------------------------------------------------------
    Rope() {
    }
    
    
    //--------------------------------------------------------------
    ~Rope() {
        clear();
    }
    
    //--------------------------------------------------------------
    void clear() {
        for(int i=0; i<particles.size(); i++) particles[i]->kill();
        for(int i=0; i<springs.size(); i++) springs[i]->kill();
        
        particles.clear();
        springs.clear();
    }
    
    //--------------------------------------------------------------
    void set(ofPoint a, ofPoint b) {
        if(a == b) b.y += 1;
        this->a = a;
        this->b = b;
        if(particles.size() != numParticles) {
            clear();
            for(int i=0; i<numParticles; i++) {
                msa::physics::Particle2D *p = physics.makeParticle(a.getInterpolated(b, i/(float)(numParticles-1)));
                p->enablePassiveCollision();
                p->enableCollision();
                particles.push_back(p);
            }
            particles.front()->makeFixed();
            particles.back()->makeFixed();
            
            for(int i=0; i<numParticles-1; i++) {
                springs.push_back(physics.makeSpring(particles[i], particles[i+1], stiffness, 0));
            }
        }
        particles.front()->moveTo(a);
        particles.back()->moveTo(b);
        
        active();
    }
    
    //--------------------------------------------------------------
    void active() {
        life = 3;
    }
    
    //--------------------------------------------------------------
    bool isDead() {
        return life <= 0;
    }
    
    
    //--------------------------------------------------------------
    void update() {
        life--;
        
        if(springs.size() > 0 && particles.size() > 0) {
            float segmentLength = (b - a).length() / springs.size();
            float targetRestLength = restLengthMult * segmentLength;
            
            for(int i=0; i<springs.size(); i++) {
                msa::physics::Spring2D &spring = *springs[i];
                spring.setStrength(stiffness);
                spring.setRestLength(ofLerp(spring.getRestLength(), targetRestLength, restLengthSpeed));
            }
            
            float radius = radiusBase + segmentLength * radiusMult;
            for(int i=0; i<particles.size(); i++) {
                particles[i]->setRadius(radius);
            }
            physics.update();
        }
    }
    
    
    //--------------------------------------------------------------
    ofPolyline drawVector() {
        if(particles.empty()) return ofPolyline();

        ofPolyline poly;
        poly.curveTo(particles.front()->getPosition(), 8);
        for(int i=1; i<particles.size()-1; i++) {
            poly.curveTo(particles[i]->getPosition(), 8);
        }
        poly.curveTo(particles.back()->getPosition(), 8);
        return poly;
    }
    
    //--------------------------------------------------------------
    void drawRaster(float w, float h) {
        if(particles.empty()) return ofPolyline();

        ofSetColor(255);
        ofPushMatrix();
        ofScale(w, h);
        drawVector().draw();
        for(int i=0; i<particles.size(); i++) {
            ofCircle(particles[i]->getPosition(), particles[i]->getRadius());
        }
        ofPopMatrix();
    }

    
private:
    ofPoint a;
    ofPoint b;
    int life;
};