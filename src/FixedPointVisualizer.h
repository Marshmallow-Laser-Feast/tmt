//
//  FixedPointVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/10/13.
//
//

#pragma once

#include "IVisualizer.h"

class FixedPointVisualizer : public IVisualizer
{
    
public:
    
    FixedPointVisualizer()
    
    :count( 0 )
    ,fixedPointOffset( 0 )
    
    {};
    
    ~FixedPointVisualizer()
    {};
    
public:
    
    void setCount( int value )
    {
        count               = value;
    };
    
    void setFixedPointOffset( int value )
    {
        fixedPointOffset    = value;
    };
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale )
    {
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        
        int lineCount       = MIN( inputAnalyser->getPathAnalysers().size() / 2, count );
        
        for( int i = 0; i < lineCount; ++i )
        {
            ofPolyline  line;
            
            int newOffset   = fixedPointOffset;
            
            if( newOffset > inputAnalyser->getPathAnalysers()[i]->getSamples().size() )
            {
                newOffset   = inputAnalyser->getPathAnalysers()[i]->getSamples().size();
            }
            
            line.addVertex( offset + inputAnalyser->getPathAnalysers()[i]->getSamples().back() * scale );
            line.addVertex( offset + inputAnalyser->getPathAnalysers()[i]->getSamples()[ inputAnalyser->getPathAnalysers()[i]->getSamples().size() - (newOffset+1) ] * scale );
            
            result->push_back( line );
        }
        
        return result;
    };
    
private:
    
    int count;
    int fixedPointOffset;
    
};
