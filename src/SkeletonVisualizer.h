//
//  SkeletonVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/24/13.
//
//

#pragma once

#include <vector>
#include <string>

#include "ofMain.h"

#include "MSAPhysics2D.h"

#include "IVisualizer.h"

#define INPUT_NAME                  "Input/Video Analysis"
#define SKELETON_TAG                "SKELETON_POINTS_TAG"
#define PARAM_NAME_BRIGHT_AUDIO     "Brightness/Audio"

class SkeletonVisualizer: public IVisualizer
{
    
public:
    
    SkeletonVisualizer()
    
    :IVisualizer( "Visualizer/Skeleton" )
    
    {
        params.addFloat( PARAM_NAME_BRIGHT_AUDIO ).setClamp( true );
        
        oscMappings[ &params.get( PARAM_NAME_BRIGHTNESS ) ]   = "/SkeletonViz/Brightness";
        oscMappings[ &params.get( PARAM_NAME_BRIGHT_AUDIO ) ]   = "/SkeletonViz/brightnessAudio";
        oscMappings[ &params.get( PARAM_NAME_CACHE_OFFSET ) ]   = "/SkeletonViz/timeOffset";
    };
    
    ~SkeletonVisualizer()
    {};
    
public:
    
    virtual void setup( const InputsMapT &inputsMap )
    {};
    
    virtual void visualize( const InputsMapT &inputsMap,
                            const float audioAmp,
                            const int fftSampleCount,
                            const float * fftData,
                            const float time
                           )
    {
        latestAudioAmp          = audioAmp;
        
        if( !(bool)params[ PARAM_NAME_ENABLED ] ||
            (float)params[ PARAM_NAME_BRIGHTNESS ] == 0.0f ||
            inputsMap.count( INPUT_NAME ) == 0
           )
        {
            addOutput( newOutput() );
            
            return;
        }
        
        const Input                 *input              = inputsMap.at( INPUT_NAME );
        ofVec2f                     inputSize           = input->getSize();
        
        PolylineVectorRefT          output              = newOutput();
        
        PointSampleVectorVectorRefT pointVectorSamples  = input->getPointVectorSamples( SKELETON_TAG );
        
        ofVec3f scale( 1.0f / inputSize.x, 1.0f / inputSize.y );
        
        for( PointSampleVectorVectorT::const_iterator it = pointVectorSamples->begin(); it != pointVectorSamples->end(); ++it )
        {
            output->push_back( ofPolyline() );
            output->back().resize( it->size() );
            
            for( int pit = 0; pit < it->size(); ++pit )
            {
                output->back().getVertices()[ pit ].set( it->at( pit ).getSample() * scale );
            }
        }
        
        addOutput( output );
        
    };
    
    virtual void draw( float width, float height )
    {
        if( (bool)params[ PARAM_NAME_ENABLED ] && (bool)params[ PARAM_NAME_BRIGHTNESS ] != 0.0f )
        {
            ofPushStyle();
            ofPushMatrix();
            
            ofSetColor( ofColor::white );
            
            ofScale( width, height, 1.0f );
            
            PolylineVectorRefT currentOutput    = getOutput();
            
            for( std::vector<ofPolyline>::iterator it = currentOutput->begin(); it != currentOutput->end(); ++it )
            {
                it->draw();
            }
            
            ofPopMatrix();
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
    
    virtual const float getBrightness() const
    {
        float brightness            = (float)params[ PARAM_NAME_BRIGHTNESS ] / 100.0f;
        float birghtnessAudio       = (float)params[ PARAM_NAME_BRIGHT_AUDIO ];
        
        if( birghtnessAudio > 0.0f )
        {
            brightness          = ofLerp(brightness * (1 - birghtnessAudio), brightness, latestAudioAmp);
        }
        
        return brightness;
    }
    
private:
    
    msa::physics::World2D       world;
    
    ofVec2f                     inputSize;
    ofVec3f                     scale;
    
    float                       latestAudioAmp;
};

