//
//  ParticleVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/23/13.
//
//

#pragma once

#include <vector>
#include <string>

#include "ofMain.h"

#include "MSAPhysics2D.h"

#include "IVisualizer.h"

#define INPUT_NAME                          "Input/Video Analysis"
#define PARTICLE_SOURCE_TIPS_TAG            "TIPS_TAG"
#define ATTRACTOR_SOURCE_CENTROID_TAG       "CENTROID_TAG"

#define PARTICLE_RATIO_TO_KILL              0.02f

class ParticleVisualizer: public IVisualizer
{
    
public:
    
    ParticleVisualizer()
    
    :IVisualizer( "Visualizer/Particle" )
    
    {        
    };
    
    ~ParticleVisualizer()
    {};
    
public:
    
    virtual void setup( const InputsMapT &inputsMap )
    {
        if( inputsMap.count( INPUT_NAME ) > 0 )
        {
            inputSize   = inputsMap.at( INPUT_NAME )->getSize();
            scale       = ofVec3f( 1.0f / inputSize.x, 1.0f / inputSize.y );
        }
    };
    
    virtual void update()
    {
        IVisualizer::update();
        
        for( int i = 0; i < world.numberOfParticles() * PARTICLE_RATIO_TO_KILL; ++i )
        {
            world.getParticle( floor(ofRandom(0, world.numberOfParticles() ) ) )->kill();
        }
        
        world.update();
    }
    
    virtual void visualize( const InputsMapT &inputsMap,
                            const float audioAmp,
                            const int fftSampleCount,
                            const float * fftData,
                            const float time
                           )
    {
        if( !(bool)params[ PARAM_NAME_ENABLED ] ||
            (float)params[ PARAM_NAME_BRIGHTNESS ] == 0.0f ||
            inputsMap.count( INPUT_NAME ) ==  0
           )
        {
            addOutput( newOutput() );
            
            return;
        }
        
        const Input     *input      = inputsMap.at( INPUT_NAME );
        
        int groupIndex              = 0;
        
        PointSampleVectorVectorRefT particleSourceVectorSamples = input->getPointVectorSamples( PARTICLE_SOURCE_TIPS_TAG );
        PointSampleVectorRefT       attractorSourceSamples      = input->getPointSamples( ATTRACTOR_SOURCE_CENTROID_TAG );
        
        for( PointSampleVectorVectorT::const_iterator it = particleSourceVectorSamples->begin(); it != particleSourceVectorSamples->end(); ++it )
        {
            msa::physics::Particle2D *attractorParticle   = new msa::physics::Particle2D( ( ofVec2f( attractorSourceSamples->at( groupIndex ).getSample() ) ) , 1.0f, 1.0f );
            
            for( int i = 0; i < it->size(); ++i )
            {
                msa::physics::Particle2D *particle= world.makeParticle( ofVec2f( it->at( i ).getSample() ) );
                
                world.makeAttraction( attractorParticle , particle, -0.01f );
            }
        }
        
        PolylineVectorRefT  output  = newOutput();
        
        output->push_back( ofPolyline() );
        
        for (long i = 0; i < world.numberOfParticles(); ++i )
        {
            output->back().addVertex(  world.getParticle( i )->getPosition() * scale );
        }
        
        addOutput( output );
    };
    
    virtual void draw( float width, float height )
    {
        if( (bool)params[ PARAM_NAME_ENABLED ] )
        {
            ofPushStyle();
            
            ofSetColor( ofColor::white );
            
            ofVec3f scale( width, height, 1.0f );
            
            for (long i = 0; i < world.numberOfParticles(); ++i )
            {
                ofCircle( ofPoint( world.getParticle( i )->getPosition() / inputSize * ofVec2f( width, height ) ), 2.0f );
            }
            
            ofPopStyle();
        } else {
            ofPushStyle();
            
            ofEnableAlphaBlending();
            ofSetColor( ofColor::red, 20 );
            
            ofRect( 0, 0, width, height );
            
            ofDisableAlphaBlending();
            
            ofSetColor( ofColor::red );
            
            ofDrawBitmapString( "Disabled", 10, 20 );
            
            ofPopStyle();
        }
    };
    
private:
    
    msa::physics::World2D       world;
    
    ofVec2f                     inputSize;
    ofVec3f                     scale;
};

