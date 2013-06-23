//
//  ConvexHullVisualizer.h
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
#define TARGET_POLYLINE_SAMPLE_TAG          "CONVEXHULL_TAG"

class ConvexHullVisualizer: public IVisualizer
{
    
public:
    
    ConvexHullVisualizer()
    
    :IVisualizer( "Visualizer/ConvexHull" )
    
    {
    };
    
    ~ConvexHullVisualizer()
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
           inputsMap.count( INPUT_NAME ) == 0
           )
        {
            addOutput( newOutput() );
            
            return;
        }
        
        const Input                 *input          = inputsMap.at( INPUT_NAME );
        ofVec2f                     inputSize       = input->getSize();
        
        PolylineVectorRefT          output          = newOutput();
        
        PolylineSampleVectorRefT    polylineSamples = input->getPolylineSamples( TARGET_POLYLINE_SAMPLE_TAG );
        
        ofVec3f scale( 1.0f / inputSize.x, 1.0f / inputSize.y );
        
        for( PolylineSampleVectorT::iterator pit = polylineSamples->begin(); pit != polylineSamples->end(); ++pit )
        {
            output->push_back( ofPolyline() );
            output->back().resize( pit->getSample().getVertices().size() );
            
            for ( int i = 0; i < pit->getSample().getVertices().size(); ++i )
            {
                output->back().getVertices()[ i ].set( pit->getSample().getVertices()[ i ] * scale );
            }
            
            output->back().close();
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
    
private:
    
    msa::physics::World2D       world;
    
    ofVec2f                     inputSize;
    ofVec3f                     scale;
};

