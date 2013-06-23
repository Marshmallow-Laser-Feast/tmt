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
#define TARGET_POINT_SAMPLE_TAG     "TIPS_TAG"

#define PARAM_NAME_RATIO            "Ratio"
#define PARAM_NAME_TRAILS_COUNT     "Trails Count"

class DotTrailsVisualizer: public IVisualizer
{
    
public:
    
    DotTrailsVisualizer()
    
    :IVisualizer( "Visualizer/Trails" )
    
    {
        params.addFloat( PARAM_NAME_RATIO ).setClamp( true );
        params.addInt( PARAM_NAME_TRAILS_COUNT ).set( 1 );
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
        
        PointSampleVectorVectorRefT pointVectorSamples  = input->getPointVectorSamples( TARGET_POINT_SAMPLE_TAG );
        
        ofVec3f scale( 1.0f / inputSize.x, 1.0f / inputSize.y );
        
        newSampleRecievedMap.clear();
        
        for( std::map<int, std::deque<ofPoint> >::iterator it = samplesDequeMap.begin(); it != samplesDequeMap.end(); ++it )
        {
            newSampleRecievedMap[ it->first ]   = false;
        }
        
        for( PointSampleVectorVectorT::const_iterator it = pointVectorSamples->begin(); it != pointVectorSamples->end(); ++it )
        {            
            for( int i = 0; i < it->size() * ratio; ++i )
            {
                if( samplesDequeMap.count( it->at( i ).getSampleID() ) == 0 )
                {
                    samplesDequeMap[ it->at( i ).getSampleID() ]    = std::deque<ofPoint>();
                }
                
                newSampleRecievedMap[ it->at( i ).getSampleID() ]   = true;
                
                samplesDequeMap[ it->at( i ).getSampleID() ].push_back( it->at( i ).getSample() * scale );
            }
        }
        
        int trailsCount = (int)params[ PARAM_NAME_TRAILS_COUNT ];
        
        std::map<int, std::deque<ofPoint> >::iterator it = samplesDequeMap.begin();
        
        while( it != samplesDequeMap.end() )
        {
            if( !newSampleRecievedMap[ it->first ] )
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
        
        output->push_back( ofPolyline() );
        
        for( std::map<int, std::deque<ofPoint> >::iterator it = samplesDequeMap.begin(); it != samplesDequeMap.end(); ++it )
        {
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
    
    std::map<int, std::deque<ofPoint> > samplesDequeMap;
    std::map<int, bool>                 newSampleRecievedMap;
};
