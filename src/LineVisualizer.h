//
//  LineVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/10/13.
//
//

#pragma once

#include "IVisualizer.h"

#include "ofMain.h"

struct comparePointX
{
    bool operator() (const ofPoint & i, const ofPoint & j)
    {
        return (i.x<j.x);
    };
}comparePointX__;

class LineVisualizer : public IVisualizer
{
    
public:
    
    LineVisualizer()
    
    :count( 0 )
    
    {};
    
    ~LineVisualizer()
    {};
    
public:
    
    void setCount( int value )
    {
        count           = value;
    };
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale )
    {
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        
        int lineCount   = MIN( inputAnalyser->getPathAnalysers().size(), count );
        
        std::vector<ofPoint>    orderedVector;
        
        for( int i = 0; i < inputAnalyser->getPathAnalysers().size(); ++i )
        {
            orderedVector.push_back( inputAnalyser->getPathAnalysers()[i]->getSamples().back() );
        }
        
        std::sort ( orderedVector.begin(), orderedVector.end(), comparePointX__ );
        
        for( int i = 1; i < lineCount; ++i )
        {
            ofPolyline  line;
            
            line.addVertex( offset + orderedVector[i - 1] * scale );
            line.addVertex( offset + orderedVector[i] * scale );
            
            result->push_back( line );
        }
        
        return result;
    };
    
private:
    
    int count;
    
};

