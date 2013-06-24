//
//  NearestDotsVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/23/13.
//
//

#pragma once

#include <vector>
#include <string>

#include "IVisualizer.h"

#define INPUT_NAME          "Input/Video Analysis"

#define PARAM_NAME_BRIGHT_AUDIO     "Brightness/Audio"
#define PARAM_NAME_COUNT            "Count"
#define PARAM_NAME_PICK_SHORTEST    "Pick Shortest"


class NearestDotsVisualizer: public IVisualizer
{
    
public:
    
    NearestDotsVisualizer()
    
    :IVisualizer( "Visualizer/NearestDot" )
    
    {
        params.addFloat( PARAM_NAME_BRIGHT_AUDIO ).setClamp( true );
        params.addInt( PARAM_NAME_COUNT ).setClamp(true);
        params.addBool( PARAM_NAME_PICK_SHORTEST ).setClamp(true);
        
        oscMappings[ &params.get( PARAM_NAME_BRIGHTNESS ) ]     = "/NearestDotsViz/Brightness";
        oscMappings[ &params.get( PARAM_NAME_BRIGHT_AUDIO ) ]   = "/NearestDotsViz/brightnessAudio";
        oscMappings[ &params.get( PARAM_NAME_CACHE_OFFSET ) ]   = "/NearestDotsViz/timeOffset";

    };
    
    ~NearestDotsVisualizer()
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
        latestAudioAmp  = audioAmp;
        
        if( !(bool)params[ PARAM_NAME_ENABLED ] || (float)params[ PARAM_NAME_BRIGHTNESS ] == 0.0f )
        {
            addOutput( newOutput() );
            
            return;
        }
        
        std::vector<ofPoint> points;
        
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
        
        PolylineVectorRefT  output  = newOutput();
        
        int     count           = params[ PARAM_NAME_COUNT ];
        bool    doPickShortest  = params[ PARAM_NAME_PICK_SHORTEST ];
        
        std::map<int, int>  connectionMap;
        vector<ofVec3f> distances;  //x, y: indices, z: distanceSquared. HACK
        
        int lineCount   = MIN( points.size(), count );
        
        for( int i = 0; i < points.size(); ++i )
        {
            ofPoint     currentPoint    = points[i];
            
            float       closestDistance = 100000000.0;
            int         closestIndex    = 0;
            
            for( int j = i+1; j < points.size(); ++j )
            {
                float currentDistance   = currentPoint.distanceSquared( points[j] ) ;
                if(doPickShortest) {
                    distances.push_back(ofVec3f(i, j, currentDistance));
                } else {
                    if(currentDistance < closestDistance && j != i && connectionMap[i] != j && connectionMap[j] != i) {
                        closestIndex        = j;
                        closestDistance     = currentDistance;
                    }
                }
            }
            
            if(!doPickShortest) {
                connectionMap[i]            = closestIndex;
                connectionMap[closestIndex] = i;
                
                ofPolyline  line;
                line.addVertex( currentPoint * scale );
                line.addVertex( points[closestIndex] * scale );
                
                output->push_back( line );
                
                if(output->size() >= lineCount) break;
            }
        }
        
        if(doPickShortest) {
            
            int lineCount   = MIN( distances.size(), count );
            
            sort(distances.begin(), distances.end(), mysort);
            for(int i=0; i<lineCount; i++) {
                ofPolyline line;
                ofPoint indices = distances[i];
                line.addVertex( points[indices.x] * scale );
                line.addVertex( points[indices.y] * scale );
                output->push_back( line );
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
        float brightness        = (float)params[ PARAM_NAME_BRIGHTNESS ];
        float birghtnessAudio   = (float)params[ PARAM_NAME_BRIGHT_AUDIO ];
        
        if( birghtnessAudio > 0 )
        {
            brightness          = ofLerp(brightness * (1 - birghtnessAudio), brightness, latestAudioAmp);
        }
        
        return (float)params[ PARAM_NAME_BRIGHTNESS ] / 100.0f;
    }
    
private:
    
    static bool mysort(const ofVec3f &a, const ofVec3f &b) { return (a.z < b.z); }
    
private:
    
    float   latestAudioAmp;
};
