//
//  NearestDotsVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/10/13.
//
//

#pragma once

#include <map>

#include "IVisualizer.h"

#include "ofMain.h"

#define PARAM_NAME_NEAREST_DOT_VIS_COUNT        "Nearest Dots Count"



class NearestDotsVisualizer : public IVisualizer
{
    
public:
    
    NearestDotsVisualizer()
    {
        params.setName("NearestDotsVisualizer");
        params.addInt( PARAM_NAME_NEAREST_DOT_VIS_COUNT ).setRange( 0, 100 ).setClamp( true );
        params.addBool("doPickShortest");
        
        midiMappings[ &params.get(PARAM_NAME_BRIGHTNESS) ]                      = std::pair<int, int>( 4, 14 );
        midiMappings[ &params.get(PARAM_NAME_TIME_OFFSET) ]                     = std::pair<int, int>( 4, 15 );
        midiMappings[ &params.get(PARAM_NAME_NEAREST_DOT_VIS_COUNT) ]           = std::pair<int, int>( 4, 16 );
        midiMappings[ &params.get("doPickShortest") ]                           = std::pair<int, int>( 4, 17 );
    };
    
    ~NearestDotsVisualizer()
    {};
    
public:
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale, float audioAmp )
    {
        int timeOffset = params[PARAM_NAME_TIME_OFFSET];
        
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        if((int)params[PARAM_NAME_BRIGHTNESS] == 0) {
            return result;
        }

        int count = params[ PARAM_NAME_NEAREST_DOT_VIS_COUNT ];
        bool doPickShortest = params["doPickShortest"];
        
        std::map<int, int>  connectionMap;
        vector<ofVec3f> distances;  //x, y: indices, z: distanceSquared. HACK

        int lineCount   = MIN( inputAnalyser->getPathAnalysers().size(), count );

        for( int i = 0; i < inputAnalyser->getPathAnalysers().size(); ++i )
        {
            ofPoint     currentPoint    = inputAnalyser->getSampleWithTimeOffset(i, timeOffset);
            
            float       closestDistance = 100000000.0;
            int         closestIndex    = 0;
            
            for( int j = i+1; j < inputAnalyser->getPathAnalysers().size(); ++j )
            {
                float currentDistance   = currentPoint.distanceSquared( inputAnalyser->getSampleWithTimeOffset(j, timeOffset) ) ;
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
                line.addVertex( offset + currentPoint * scale );
                line.addVertex( offset + inputAnalyser->getSampleWithTimeOffset(closestIndex, timeOffset) * scale );
                result->push_back( line );
                if(result->size() >= lineCount) break;
            }
        }
        
        if(doPickShortest) {
            int lineCount   = MIN( distances.size(), count );
            
            sort(distances.begin(), distances.end(), mysort);
            for(int i=0; i<lineCount; i++) {
                ofPolyline line;
                ofPoint indices = distances[i];
                line.addVertex( offset + inputAnalyser->getSampleWithTimeOffset(indices.x, timeOffset) * scale );
                line.addVertex( offset + inputAnalyser->getSampleWithTimeOffset(indices.y, timeOffset) * scale );
                result->push_back( line );
            }
            
        }
        
        return result;
    };
    
private:
    
    static bool mysort(const ofVec3f &a, const ofVec3f &b) { return (a.z < b.z); }
};

