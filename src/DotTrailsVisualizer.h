//
//  DotTrailsVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/23/13.
//
//

#pragma once

#include <vector>
#include <string>
#include <deque>
#include <map>

#include "IVisualizer.h"

#define INPUT_NAME                  "Input/Video Analysis"
#define TIPS_TAG                    "TIPS_TAG"

#define PARAM_NAME_BRIGHT_AUDIO     "Brightness/Audio"
#define PARAM_NAME_RATIO            "Ratio"
#define PARAM_NAME_TRAILS_COUNT     "Trails Count"

class DotTrailsVisualizer: public IVisualizer
{
    
public:
    
    DotTrailsVisualizer()
    
    :IVisualizer( "Visualizer/Trails" )
    
    {
        params.addFloat( PARAM_NAME_BRIGHT_AUDIO ).setClamp( true );
        params.addFloat( PARAM_NAME_RATIO ).setClamp( true );
        params.addInt( PARAM_NAME_TRAILS_COUNT ).set( 1 );
        
        oscMappings[ &params.get( PARAM_NAME_BRIGHTNESS ) ]     = "/DotTrailsViz/Brightness";
        oscMappings[ &params.get( PARAM_NAME_BRIGHT_AUDIO ) ]   = "/DotTrailsViz/brightnessAudio";
        oscMappings[ &params.get( PARAM_NAME_CACHE_OFFSET ) ]   = "/DotTrailsViz/timeOffset";
        oscMappings[ &params.get( PARAM_NAME_TRAILS_COUNT ) ]   = "/DotTrailsViz/TrailCount";
    };
    
    ~DotTrailsVisualizer()
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
            inputsMap.count( INPUT_NAME ) ==  0
           )
        {
            addOutput( newOutput() );
            
            return;
        }
        
        const Input *input      = inputsMap.at( INPUT_NAME );

        ofVec2f     inputSize   = input->getSize();
        
        float       ratio       = (float)params[ PARAM_NAME_RATIO ];
        
        PointSampleVectorVectorRefT pointVectorSamples  = input->getPointVectorSamples( TIPS_TAG );
        
        ofVec3f scale( 1.0f / inputSize.x, 1.0f / inputSize.y );
        
        newSampleReceivedMap.clear();
        
        for( std::map<int, std::deque<ofPoint> >::iterator it = samplesDequeMap.begin(); it != samplesDequeMap.end(); ++it )
        {
            newSampleReceivedMap[ it->first ]   = false;
        }
        
        for( PointSampleVectorVectorT::const_iterator it = pointVectorSamples->begin(); it != pointVectorSamples->end(); ++it )
        {            
            for( int i = 0; i < it->size() * ratio; ++i )
            {
                if( samplesDequeMap.count( it->at( i ).getSampleID() ) == 0 )
                {
                    samplesDequeMap[ it->at( i ).getSampleID() ]    = std::deque<ofPoint>();
                }
                
                newSampleReceivedMap[ it->at( i ).getSampleID() ]   = true;
                
                samplesDequeMap[ it->at( i ).getSampleID() ].push_back( it->at( i ).getSample() * scale );
            }
        }
        
        int trailsCount = (int)params[ PARAM_NAME_TRAILS_COUNT ];
        
        std::map<int, std::deque<ofPoint> >::iterator it = samplesDequeMap.begin();
        
        while( it != samplesDequeMap.end() )
        {
            if( !newSampleReceivedMap[ it->first ] )
            {
                if( it->second.size() > 0 )
                {
                    it->second.pop_front();
                    
                    ++it;
                } else {
                    samplesDequeMap.erase( it++ );
                }
            } else {
                
                if( it->second.size() > trailsCount )
                {
                    it->second.pop_front();
                }
                
                ++it;
            }
        }
        
        PolylineVectorRefT  output  = newOutput();
        
        for( std::map<int, std::deque<ofPoint> >::iterator it = samplesDequeMap.begin(); it != samplesDequeMap.end(); ++it )
        {
            output->push_back( ofPolyline() );
            
            for( std::deque<ofPoint>::iterator pit = it->second.begin(); pit != it->second.end(); ++pit )
            {
                output->back().addVertex( *pit );
            }
        }
        
        addOutput( output );
    };
    
    virtual void draw( float width, float height )
    {
        if( (bool)params[ PARAM_NAME_ENABLED ] )
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
        float brightness        = (float)params[ PARAM_NAME_BRIGHTNESS ];
        float birghtnessAudio    = (float)params[ PARAM_NAME_BRIGHT_AUDIO ];
        
        if( birghtnessAudio > 0 )
        {
            brightness          = ofLerp(brightness * (1 - birghtnessAudio), brightness, latestAudioAmp);
        }
        
        return (float)params[ PARAM_NAME_BRIGHTNESS ] / 100.0f;
    }
    
private:
    
    std::map<int, std::deque<ofPoint> > samplesDequeMap;
    std::map<int, bool>                 newSampleReceivedMap;
    
    float                               latestAudioAmp;
};
