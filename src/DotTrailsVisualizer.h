//
//  DotTrailsVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/10/13.
//
//

#pragma once

#include "IVisualizer.h"

#include "ofMain.h"

class DotTrailsVisualizer : public IVisualizer
{
    
public:
    
    DotTrailsVisualizer()
    
    :renderRatio( 0.0f )
    ,trailCount( 0 )
    
    {
        params.setName("DotTrailsVisualizer");
    };
    
    ~DotTrailsVisualizer()
    {
    };
    
public:
    
    void setRenderRatio( float value )
    {
        renderRatio = ofClamp( value, 0.0f, 1.0f );
    };
    
    void setTrailCount( int value )
    {
        trailCount  = value;
    };
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale )
    {
        int dotCount    = (float)inputAnalyser->getPathAnalysers().size() * renderRatio;
        
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        
        for( int i = 0; i < dotCount; ++i )
        {
            ofPolyline  line;
            
            for( int j = 0; j < MIN( trailCount, inputAnalyser->getPathAnalysers()[i]->getSamples().size() ); ++j )
            {
                line.addVertex( offset + inputAnalyser->getPathAnalysers()[i]->getSamples()[ inputAnalyser->getPathAnalysers()[i]->getSamples().size() - (j+1) ] * scale );
            }
            
            result->push_back( line );
        }
        
        return result;
    };
    
private:
    
    float   renderRatio;
    int     trailCount;
    
};
