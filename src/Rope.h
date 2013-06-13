#pragma once

#include "MSAPhysics2D.h"

class Rope {
public:
    vector<msa::physics::Particle2D*> particles;
    vector<msa::physics::Spring2D*> springs;
    
    vector<msa::physics::Particle2D*> homeParticles;
    vector<msa::physics::Spring2D*> homeSprings;
    
    int numParticles;
    float stiffness;
    float stiffnessHome;
    float restLengthMult;
    float restLengthSpeed;
    float radiusBase;
    float radiusMult;
    
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

        for(int i=0; i<homeParticles.size(); i++) homeParticles[i]->kill();
        for(int i=0; i<homeSprings.size(); i++) homeSprings[i]->kill();
        
        particles.clear();
        springs.clear();
        
        homeParticles.clear();
        homeSprings.clear();
    }
    
    //--------------------------------------------------------------
    void set(ofPoint _a, ofPoint _b, msa::physics::World2D &physics, float smoothing) {
        a += (_a - a) * (1-smoothing);
        b += (_b - b) * (1-smoothing);

        if(numParticles<2) return;
        
        if(particles.size() != numParticles) {
            a = _a;
            b = _b;
            clear();
            for(int i=0; i<numParticles; i++) {
                msa::physics::Particle2D *p1 = physics.makeParticle(a.getInterpolated(b, i/(float)(numParticles-1)));
                p1->enablePassiveCollision();
                p1->enableCollision();
                p1->makeFree();
                particles.push_back(p1);
                
                msa::physics::Particle2D *p2 = physics.makeParticle(p1->getPosition());
                p2->disablePassiveCollision();
                p2->disableCollision();
                p2->makeFixed();
                homeParticles.push_back(p2);
                
                homeSprings.push_back(physics.makeSpring(p1, p2, stiffnessHome, 0));
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
            // update particles
            float segmentLength = (b - a).length() / springs.size();
            float radius = radiusBase + segmentLength * radiusMult;
            for(int i=0; i<particles.size(); i++) particles[i]->setRadius(radius);

            // update springs
            float targetRestLength = restLengthMult * segmentLength;
            for(int i=0; i<springs.size(); i++) {
                msa::physics::Spring2D &spring = *springs[i];
                spring.setStrength(stiffness);
                spring.setRestLength(ofLerp(spring.getRestLength(), targetRestLength, restLengthSpeed));
            }
            
            // update home particles
            for(int i=0; i<homeParticles.size(); i++) homeParticles[i]->moveTo(a.getInterpolated(b, i/(float)(homeParticles.size()-1)));
            
            // update home springs
            for(int i=0; i<homeSprings.size(); i++) homeSprings[i]->setStrength(stiffnessHome);
            
            
            

        }
    }
    
    
    //--------------------------------------------------------------
    ofPolyline drawVector() {
        if(particles.empty()) return ofPolyline();

        ofPolyline poly;
//        poly.curveTo(particles.front()->getPosition(), 8);
        for(int i=0; i<particles.size(); i++) {
//            poly.curveTo(particles[i]->getPosition(), 8);
            poly.lineTo(particles[i]->getPosition());
        }
//        poly.curveTo(particles.back()->getPosition(), 8);
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