//
//  ConnectedDotsVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/10/13.
//
//

#pragma once

#include "IVisualizer.h"

#include "ofMain.h"

#define PARAM_NAME_CONNECTED_DOT_VIS_COUNT      "Connected Dots Count"
#define PARAM_NAME_CONNECTED_DOT_VIS_N_OFFSET   "Connected Neighbor Offset"



class ConnectedDotsVisualizer : public IVisualizer
{
    
public:
    
    ConnectedDotsVisualizer()
    {
        params.setName("ConnectedDotsVisualizer");
        params.addInt( PARAM_NAME_CONNECTED_DOT_VIS_COUNT ).setRange( 0, 100 ).setClamp( true );
        params.addInt( PARAM_NAME_CONNECTED_DOT_VIS_N_OFFSET ).setRange( 0, 1000 ).setClamp( true );
    };
    
    ~ConnectedDotsVisualizer()
    {};
    
public:

    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale )
    {
        int count = params[ PARAM_NAME_CONNECTED_DOT_VIS_COUNT ];
        int neighborOffset = params[ PARAM_NAME_CONNECTED_DOT_VIS_N_OFFSET ];

        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        
        int lineCount   = MIN( inputAnalyser->getPathAnalysers().size() / 2, count );
        
        for( int i = 0; i < lineCount; ++i )
        {
            ofPolyline  line;
            
            line.addVertex( offset + inputAnalyser->getPathAnalysers()[i]->getSamples().back() * scale );
            line.addVertex( offset + inputAnalyser->getPathAnalysers()[ ( i + ( inputAnalyser->getPathAnalysers().size() / 2 ) + neighborOffset ) % inputAnalyser->getPathAnalysers().size() ]->getSamples().back() * scale );
            
            result->push_back( line );
        }
        
        return result;
    };
    
private:
};

