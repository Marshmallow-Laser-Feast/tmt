//
//  DotVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/22/13.
//
//

#pragma once

#include <vector>
#include <string>

#include "IVisualizer.h"

#define PARAM_NAME_RATIO    "Ratio"
#define PARAM_NAME_BRIGHT_AUDIO     "Brightness/Audio"

class DotVisualizer: public IVisualizer
{
    
public:
    
    DotVisualizer()
    
    :IVisualizer( "Visualizer/Dot" )
    
    {
        params.addFloat( PARAM_NAME_BRIGHT_AUDIO ).setClamp( true );
        params.addFloat( PARAM_NAME_RATIO ).setClamp( true );
        
        oscMappings[ &params.get( PARAM_NAME_BRIGHTNESS ) ]     = "/DotsViz/Brightness";
        oscMappings[ &params.get( PARAM_NAME_BRIGHT_AUDIO ) ]   = "/DotsViz/brightnessAudio";
        oscMappings[ &params.get( PARAM_NAME_CACHE_OFFSET ) ]   = "/DotsViz/timeOffset";

    };
    
    ~DotVisualizer()
    {};
    
public:
    
    virtual void setup( const InputsMapT &inputsMap )
    {
        for (InputsMapT::const_iterator it = inputsMap.begin(); it != inputsMap.end(); ++it )
        {
            for( std::vector<std::string>::const_iterator tit = it->second->getPointSampleTags().begin(); tit != it->second->getPointSampleTags().end(); ++tit )
            {
                params.addBool( (*tit) ).set( false );
            }
            
            for( std::vector<std::string>::const_iterator tit = it->second->getPointVectorSampleTags().begin(); tit != it->second->getPointVectorSampleTags().end(); ++tit )
            {
                params.addBool( (*tit) ).set( false );
            }
            
            for( std::vector<std::string>::const_iterator tit = it->second->getPolylineSampleTags().begin(); tit != it->second->getPolylineSampleTags().end(); ++tit )
            {
                params.addBool( (*tit) ).set( false );
            }
        }
    };
    
    virtual void visualize( const InputsMapT &inputsMap,
                            const float audioAmp,
                            const int fftSampleCount,
                            const float * fftData,
                            const float time
                           )
    {
        latestAudioAmp  = audioAmp;

        if( !(bool)params[ PARAM_NAME_ENABLED ] || (float)params[ PARAM_NAME_BRIGHTNESS ] == 0.0f )
        {
            addOutput( newOutput() );
            
            return;
        }
        
        PolylineVectorRefT  output  = newOutput();
        
        float ratio                 = (float)params[ PARAM_NAME_RATIO ];
        
        output->push_back( ofPolyline() );
        
        for (InputsMapT::const_iterator it = inputsMap.begin(); it != inputsMap.end(); ++it )
        {
            ofVec2f inputSize = it->second->getSize();
            
            ofVec3f scale( 1.0f / inputSize.x, 1.0f / inputSize.y );
            
            for( std::vector<std::string>::const_iterator tit = it->second->getPointSampleTags().begin(); tit != it->second->getPointSampleTags().end(); ++tit )
            {
                if( (bool)params[ (*tit) ] )
                {
                    PointSampleVectorRefT   pointSamples    = it->second->getPointSamples( (*tit) );
                    
                    for( int i = 0; i < pointSamples->size() * ratio; ++i )
                    {
                        output->back().addVertex( pointSamples->at( i ).getSample() * scale );
                    }
                }
            }
            
            for( std::vector<std::string>::const_iterator tit = it->second->getPointVectorSampleTags().begin(); tit != it->second->getPointVectorSampleTags().end(); ++tit )
            {
                if( (bool)params[ (*tit) ] )
                {
                    PointSampleVectorVectorRefT pointVectorSamples  = it->second->getPointVectorSamples( (*tit) );
                    
                    for( PointSampleVectorVectorT::const_iterator it = pointVectorSamples->begin(); it != pointVectorSamples->end(); ++it )
                    {
                        for( int i = 0; i < it->size() * ratio; ++i )
                        {
                            output->back().addVertex( it->at( i ).getSample() * scale );
                        }
                    }
                }
            }
            
            for( std::vector<std::string>::const_iterator tit = it->second->getPolylineSampleTags().begin(); tit != it->second->getPolylineSampleTags().end(); ++tit )
            {
                if( (bool)params[ (*tit) ] )
                {
                    PolylineSampleVectorRefT polylineSamples    = it->second->getPolylineSamples( (*tit) );
                    
                    for( PolylineSampleVectorT::iterator pit = polylineSamples->begin(); pit != polylineSamples->end(); ++pit )
                    {
                        for( int i = 0; i < pit->getSample().getVertices().size() * ratio; ++i )
                        {
                            output->back().addVertex( pit->getSample().getVertices()[ i ] * scale );
                        }
                    }
                }
            }
        }
        
        addOutput( output );
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
    
    virtual void draw( float width, float height )
    {
        if( (bool)params[ PARAM_NAME_ENABLED ] && (bool)params[ PARAM_NAME_BRIGHTNESS ] != 0.0f )
        {
            ofPushStyle();
            
            ofSetColor( ofColor::white );
            
            ofVec3f scale( width, height, 1.0f );
            
            PolylineVectorRefT currentOutput    = getOutput();
            
            for( std::vector<ofPolyline>::iterator it = currentOutput->begin(); it != currentOutput->end(); ++it )
            {
                for( std::vector<ofPoint>::const_iterator pit = it->getVertices().begin(); pit != it->getVertices().end(); ++pit )
                {
                    ofCircle( *pit * scale, 2.0f );
                }
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
    float   latestAudioAmp;

};
