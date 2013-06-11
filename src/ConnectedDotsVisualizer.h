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

class ConnectedDotsVisualizer : public IVisualizer
{
    
public:
    
    ConnectedDotsVisualizer()
    
    :count( 0 )
    ,neighborsOffset( 0 )
    
    {
        params.setName("ConnectedDotsVisualizer");
    };
    
    ~ConnectedDotsVisualizer()
    {};
    
public:
    
    void setCount( int value )
    {
        count           = value;
    };
    
    void setNeighborsOffset( int value )
    {
        neighborsOffset = value;
    };
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale )
    {
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        
        int lineCount   = MIN( inputAnalyser->getPathAnalysers().size() / 2, count );
        
        for( int i = 0; i < lineCount; ++i )
        {
            ofPolyline  line;
            
            line.addVertex( offset + inputAnalyser->getPathAnalysers()[i]->getSamples().back() * scale );
            line.addVertex( offset + inputAnalyser->getPathAnalysers()[ ( i + ( inputAnalyser->getPathAnalysers().size() / 2 ) + neighborsOffset ) % inputAnalyser->getPathAnalysers().size() ]->getSamples().back() * scale );
            
            result->push_back( line );
        }
        
        return result;
    };
    
private:
    
    int count;
    int neighborsOffset;
    
};

