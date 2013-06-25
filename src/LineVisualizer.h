//
//  LineVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/23/13.
//
//

#pragma once

#include <vector>
#include <string>

#include "IVisualizer.h"

#include "ofMain.h"
#include "DeformableRope.h"

#define INPUT_NAME                      "Input/Video Analysis"

#define PARAM_NAME_EDGE_FIX_AMOUNT      "Edge fix Amount"
#define PARAM_NAME_EDGE_FIX_HEIGHT      "Edge fix Height"
#define PARAM_NAME_AUDIO_NOISE_AMOUNT   "Audio Noise Amount"
#define PARAM_NAME_SORT                 "Sort Points"

struct comparePointX
{
    bool operator() (const ofPoint & i, const ofPoint & j)
    {
        return (i.x<j.x);
    };
}comparePointX__;

class LineVisualizer: public IVisualizer
{
    
public:
    
    LineVisualizer()
    
    :IVisualizer( "Visualizer/Line" )
    
    {
        params.addFloat( PARAM_NAME_EDGE_FIX_AMOUNT ).setClamp(true);
        params.addFloat( PARAM_NAME_EDGE_FIX_HEIGHT ).setClamp(true);
        params.addFloat( PARAM_NAME_AUDIO_NOISE_AMOUNT ).setRange(0, 20).setClamp(true);
        params.addBool(PARAM_NAME_SORT);
     
        rope = new DeformableRope( params );
        
        oscMappings[ &params.get( "Rope.Deform.amp" ) ]         = "/LineViz/Amp";
        oscMappings[ &params.get( "Rope.Deform.curvature" ) ]     = "/LineViz/curvature";
        oscMappings[ &params.get( "Rope.Physics.drag" ) ]         = "/LineViz/drag";
        oscMappings[ &params.get( "Rope.Physics.stiffness" ) ]         = "/LineViz/stiffness";
        oscMappings[ &params.get( "Rope.Post.smoothPoly" ) ]         = "/LineViz/smoothPoly";
        oscMappings[ &params.get( "Rope.Post.easeAmount" ) ]         = "/LineViz/easeAmount";
//        oscMappings[ &params.get( "Rope.Noise.noiseAmp2" ) ]   = "/LineViz/noiseAmount2";
        
        oscMappings[ &params.get( PARAM_NAME_BRIGHTNESS ) ]         = "/LineViz/Brightness";
        oscMappings[ &params.get( PARAM_NAME_EDGE_FIX_AMOUNT ) ]    = "/LineViz/edgeFixAmount";
        oscMappings[ &params.get( PARAM_NAME_CACHE_OFFSET ) ]       = "/LineViz/timeOffset";
        
    };
    
    ~LineVisualizer()
    {};
    
public:
    
    virtual void setup( const InputsMapT &inputsMap )
    {
        for( std::vector<std::string>::const_iterator tit = inputsMap.at(INPUT_NAME)->getPointSampleTags().begin(); tit != inputsMap.at(INPUT_NAME)->getPointSampleTags().end(); ++tit )
        {
            params.addBool( (*tit) ).set( false );
        }
        
        for( std::vector<std::string>::const_iterator tit = inputsMap.at(INPUT_NAME)->getPointVectorSampleTags().begin(); tit != inputsMap.at(INPUT_NAME)->getPointVectorSampleTags().end(); ++tit )
        {
            params.addBool( (*tit) ).set( false );
        }
        
        for( std::vector<std::string>::const_iterator tit = inputsMap.at(INPUT_NAME)->getPolylineSampleTags().begin(); tit != inputsMap.at(INPUT_NAME)->getPolylineSampleTags().end(); ++tit )
        {
            params.addBool( (*tit) ).set( false );
        }
        
        
    };
    
    virtual void visualize( const InputsMapT &inputsMap,
                            const float audioAmp,
                            const int fftSampleCount,
                            const float * fftData,
                            const float time
                           )
    {
        if( !(bool)params[ PARAM_NAME_ENABLED ] || (float)params[ PARAM_NAME_BRIGHTNESS ] == 0.0f )
        {
            addOutput( newOutput() );
            
            return;
        }
        
        bool bSortPoints = params[PARAM_NAME_SORT];
        
        std::vector<ofVec2f> points;
        
        ofVec2f inputSize = inputsMap.at(INPUT_NAME)->getSize();
        
        ofVec3f scale( 1.0f / inputSize.x, 1.0f / inputSize.y );
        
        for( std::vector<std::string>::const_iterator tit = inputsMap.at(INPUT_NAME)->getPointSampleTags().begin(); tit != inputsMap.at(INPUT_NAME)->getPointSampleTags().end(); ++tit )
        {
            if( (bool)params[ (*tit) ] )
            {
                PointSampleVectorRefT   pointSamples    = inputsMap.at(INPUT_NAME)->getPointSamples( (*tit) );
                
                for( int i = 0; i < pointSamples->size(); ++i )
                {
                    points.push_back( pointSamples->at( i ).getSample() );
                }
            }
        }
        
        for( std::vector<std::string>::const_iterator tit = inputsMap.at(INPUT_NAME)->getPointVectorSampleTags().begin(); tit != inputsMap.at(INPUT_NAME)->getPointVectorSampleTags().end(); ++tit )
        {
            if( (bool)params[ (*tit) ] )
            {
                PointSampleVectorVectorRefT pointVectorSamples  = inputsMap.at(INPUT_NAME)->getPointVectorSamples( (*tit) );
                
                for( PointSampleVectorVectorT::const_iterator it = pointVectorSamples->begin(); it != pointVectorSamples->end(); ++it )
                {
                    for( int i = 0; i < it->size(); ++i )
                    {
                        points.push_back( it->at( i ).getSample() );
                    }
                }
            }
        }
        
        for( std::vector<std::string>::const_iterator tit = inputsMap.at(INPUT_NAME)->getPolylineSampleTags().begin(); tit != inputsMap.at(INPUT_NAME)->getPolylineSampleTags().end(); ++tit )
        {
            if( (bool)params[ (*tit) ] )
            {
                PolylineSampleVectorRefT polylineSamples    = inputsMap.at(INPUT_NAME)->getPolylineSamples( (*tit) );
                
                for( PolylineSampleVectorT::iterator pit = polylineSamples->begin(); pit != polylineSamples->end(); ++pit )
                {
                    for( int i = 0; i < pit->getSample().getVertices().size(); ++i )
                    {
                        points.push_back( pit->getSample().getVertices()[ i ] );
                    }
                }
            }
        }
        
        if(points.size() == 0)
        {
            points.push_back( ofPoint( inputSize.x * 0.4, inputSize.y * 0.5f ) );
        }
        
        if(points.size() == 1)
        {
            points.push_back( points.front() + ofPoint( inputSize.x * 0.1f, 0 ) );
        }
        
        float edgeFixAmount = params[ PARAM_NAME_EDGE_FIX_AMOUNT ];
        float edgeFixHeight = params[ PARAM_NAME_EDGE_FIX_HEIGHT ];

        vector<ofVec2f> sortedPoints(points);
        std::sort ( sortedPoints.begin(), sortedPoints.end(), comparePointX__ );
        
        if(bSortPoints) {
//            std::sort ( points.begin(), points.end(), comparePointX__ );
            points = sortedPoints;
        }

//        if( edgeFixAmount )
        {
//            points.push_back(points.front().interpolated(ofVec3f(0, edgeFixHeight * inputSize.y, 0), edgeFixAmount));
            points.insert(points.begin(), sortedPoints.front().interpolated(ofVec3f(0, edgeFixHeight * inputSize.y, 0), edgeFixAmount));
            points.push_back(sortedPoints.back().interpolated(ofVec3f(inputSize.x, edgeFixHeight * inputSize.y, 0), edgeFixAmount));
        }
        
        
        rope->update(points);
                
        for(int i=0; i<rope->poly.size(); i++)
        {
            ofPoint &p = rope->poly[i];
            p = p * scale;
        }
        
        PolylineVectorRefT  output  = newOutput();
        
        output->push_back( rope->poly );
        
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
    
    DeformableRope *rope;
};
