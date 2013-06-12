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
        params.addBool("maintainConnections");
    };
    
    ~NearestDotsVisualizer()
    {};
    
public:
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale )
    {
        int timeOffset = params[PARAM_NAME_TIME_OFFSET];
        
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        if((int)params[PARAM_NAME_BRIGHTNESS] == 0) {
            return result;
        }

        int count = params[ PARAM_NAME_NEAREST_DOT_VIS_COUNT ];
        bool doMaintainConnections = params["maintainConnections"];
        
        int lineCount   = MIN( inputAnalyser->getPathAnalysers().size(), count );
        
        std::map<int, int>  connectionMap;
        
        for( int i = 0; i < lineCount; ++i )
        {
            ofPolyline  line;
            
            ofPoint     currentPoint    = inputAnalyser->getSampleWithTimeOffset(i, timeOffset);
            
            float       closestDistance = 100000000.0;
            int         closestIndex    = 0;
            
            for( int j = i+1; j < inputAnalyser->getPathAnalysers().size(); ++j )
            {
                float currentDistance   = currentPoint.distanceSquared( inputAnalyser->getSampleWithTimeOffset(j, timeOffset) ) ;
                
                if(currentDistance < closestDistance) {
                    if(!doMaintainConnections || !(j != i && connectionMap[i] != j && connectionMap[j] != i)) {
                        
                        closestIndex        = j;
                        closestDistance     = currentDistance;
                    }
                }
            }
            
            connectionMap[i]            = closestIndex;
            connectionMap[closestIndex] = i;
            
            line.addVertex( offset + currentPoint * scale );
            line.addVertex( offset + inputAnalyser->getSampleWithTimeOffset(closestIndex, timeOffset) * scale );
            
            result->push_back( line );
        }
        
        return result;
    };
    
private:
};

