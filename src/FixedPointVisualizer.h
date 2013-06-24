//
//  FixedPointVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/23/13.
//
//

#pragma once

#pragma once

#include <vector>
#include <string>
#include <deque>
#include <map>

#include "IVisualizer.h"

#define INPUT_NAME                  "Input/Video Analysis"
#define TARGET_POINT_SAMPLE_TAG     "TIPS_TAG"

#define PARAM_NAME_RATIO            "Ratio"

#define PARAM_NAME_FIX              "FIX"
#define PARAM_NAME_CLEAR            "Clear"

class FixedPointVisualizer: public IVisualizer
{
    
public:
    
    FixedPointVisualizer()
    
    :IVisualizer( "Visualizer/FixedPoint" )
    
    {
        params.addFloat( PARAM_NAME_RATIO ).setClamp( true );
        
        params.addBool( PARAM_NAME_FIX );
        params.addBang( PARAM_NAME_CLEAR );
    };
    
    ~FixedPointVisualizer()
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
        
        newSampleReceivedMap.clear();
        
        const Input *input  = inputsMap.at( INPUT_NAME );
        PointSampleVectorVectorRefT pointVectorSamples  = input->getPointVectorSamples( TARGET_POINT_SAMPLE_TAG );
        
        for( std::map<int, bool>::iterator it = newSampleReceivedMap.begin(); it != newSampleReceivedMap.end(); ++it )
        {
            newSampleReceivedMap[ it->first ]   = false;
        }
        
        if( (bool)params[PARAM_NAME_FIX] )
        {
            float   ratio   = (float)params[ PARAM_NAME_RATIO ];
            
            for( PointSampleVectorVectorT::const_iterator it = pointVectorSamples->begin(); it != pointVectorSamples->end(); ++it )
            {
                for( int i = 0; i < it->size() * ratio; ++i )
                {
                    if( fixedPointMap.count( it->at( i ).getSampleID() ) == 0 )
                    {
                        fixedPointMap[ it->at( i ).getSampleID() ]  = std::vector<ofPoint>();
                    }
                    
                    newSampleReceivedMap[ it->at( i ).getSampleID() ]   = true;
                    
                    fixedPointMap[ it->at( i ).getSampleID() ].push_back(  it->at( i ).getSample() );
                }
            }            
        }
        
        if( (bool)params[PARAM_NAME_CLEAR] )
        {
            fixedPointMap.clear();
        }
        
        std::map<int, bool>::iterator it = newSampleReceivedMap.begin();
        
        while( it != newSampleReceivedMap.end() )
        {
            if( !it->second )
            {
                fixedPointMap.erase( it->first );
                newSampleReceivedMap.erase( it++ );
            } else {
                ++it;
            }
        }
        
        ofVec2f inputSize   = input->getSize();
        
        ofVec3f scale( 1.0f / inputSize.x, 1.0f / inputSize.y );
        
         PolylineVectorRefT  output = newOutput();
        
        for( PointSampleVectorVectorT::const_iterator it = pointVectorSamples->begin(); it != pointVectorSamples->end(); ++it )
        {
            output->push_back( ofPolyline() );
            
            for( int i = 0; i < it->size(); ++i )
            {
                if( fixedPointMap.count( it->at( i ).getSampleID() ) > 0 )
                {
                    for( std::vector<ofPoint>::iterator pit = fixedPointMap[it->at( i ).getSampleID()].begin(); pit != fixedPointMap[it->at( i ).getSampleID()].end(); ++pit )
                    {
                        output->back().addVertex( it->at( i ).getSample() * scale );
                        output->back().lineTo( *pit * scale );
                    }
                }
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
    
private:
    
    std::map<int, std::vector<ofPoint> >    fixedPointMap;
    std::map<int, bool>                     newSampleReceivedMap;
};
