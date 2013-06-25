//
//  ContourVisualizer.h
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

#define INPUT_NAME                  "Input/Video Analysis"
#define CONTOUR_TAG                 "CONTOUR_TAG"

#define PARAM_NAME_STRETCH_POINTS_X "Stretch Points X"
#define PARAM_NAME_STRETCH_POINTS_Y "Stretch Points Y"

class ContourVisualizer: public IVisualizer
{
    
public:
    
    ContourVisualizer()
    
    :IVisualizer( "Visualizer/Contour" )
    
    {
        params.addFloat( PARAM_NAME_STRETCH_POINTS_X ).setClamp( true );
        params.addFloat( PARAM_NAME_STRETCH_POINTS_Y ).setClamp( true );
        
        oscMappings[ &params.get( PARAM_NAME_BRIGHTNESS ) ]     = "/ContourViz/Brightness";
        oscMappings[ &params.get( PARAM_NAME_STRETCH_POINTS_X ) ] = "/CameraContourInput/StretchPointsX";
        oscMappings[ &params.get( PARAM_NAME_STRETCH_POINTS_Y ) ] = "/CameraContourInput/StretchPointsY";
    };
    
    ~ContourVisualizer()
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
        
        float                       stretchPoints_x = params[ PARAM_NAME_STRETCH_POINTS_X ];
        float                       stretchPoints_y = params[ PARAM_NAME_STRETCH_POINTS_Y ];
        
        PolylineSampleVectorRefT    polylineSamples = input->getPolylineSamples( CONTOUR_TAG );
        
        ofVec3f scale( 1.0f / inputSize.x, 1.0f / inputSize.y );
        
        for( PolylineSampleVectorT::iterator pit = polylineSamples->begin(); pit != polylineSamples->end(); ++pit )
        {            
            output->push_back( ofPolyline() );
            output->back().resize( pit->getSample().getVertices().size() );
            
            for ( int i = 0; i < pit->getSample().getVertices().size(); ++i )
            {
                ofRectangle r = pit->getSample().getBoundingBox();
                
                ofPoint &p  = pit->getSample().getVertices()[i];
                
                if( stretchPoints_x > 0 || stretchPoints_y > 0 )
                {
                    ofPoint pNormX;
                    ofPoint pNormY;
                    
                    pNormX.set( p );
                    pNormY.set( p );
                    
                    pNormX.x = ofMap( p.x, r.x, r.x + r.width, 0, inputSize.x);
                    pNormY.y = ofMap( p.y, r.y, r.y + r.height, 0, inputSize.y);
                    
                    p.interpolate(pNormX, stretchPoints_x);
                    p.interpolate(pNormY, stretchPoints_y);
                }
                
                output->back().getVertices()[ i ].set( p * scale );
            }
            
            output->back().close();
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
    
    msa::physics::World2D       world;
    
    ofVec2f                     inputSize;
    ofVec3f                     scale;
};

