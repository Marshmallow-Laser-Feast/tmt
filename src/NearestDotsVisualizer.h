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
    };
    
    ~NearestDotsVisualizer()
    {};
    
public:
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale )
    {
        int count = params[ PARAM_NAME_NEAREST_DOT_VIS_COUNT ];
        
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        
        int lineCount   = MIN( inputAnalyser->getPathAnalysers().size(), count );
        
        std::map<int, int>  connectionMap;
        
        for( int i = 0; i < lineCount; ++i )
        {
            ofPolyline  line;
            
            ofPoint     currentPoint    = inputAnalyser->getPathAnalysers()[i]->getSamples().back();
            
            float       closestDistance = 1000000.0;
            int         closestIndex    = 0;
            
            for( int j = 0; j < inputAnalyser->getPathAnalysers().size(); ++j )
            {
                float currentDistance   = currentPoint.distance( inputAnalyser->getPathAnalysers()[j]->getSamples().back() ) ;
                
                if( j != i && connectionMap[i] != j && connectionMap[j] != i && currentDistance < closestDistance )
                {
                    closestIndex        = j;
                    closestDistance     = currentDistance;
                }
            }
            
            connectionMap[i]            = closestIndex;
            connectionMap[closestIndex] = i;
            
            line.addVertex( offset + currentPoint * scale );
            line.addVertex( offset + inputAnalyser->getPathAnalysers()[closestIndex]->getSamples().back() * scale );
            
            result->push_back( line );
        }
        
        return result;
    };
    
private:
};

