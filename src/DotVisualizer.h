//
//  DotVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/10/13.
//
//

#pragma once

#include "IVisualizer.h"

class DotVisualizer : public IVisualizer
{
    
public:
    
    DotVisualizer()
    
    :renderRatio( 0.0f )
    
    {
        params.setName("DotVisualizer");
    
    };
    
    ~DotVisualizer()
    {};
    
public:
    
    void setRenderRatio( float value )
    {
        renderRatio = ofClamp( value, 0.0f, 1.0f );
    };
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale )
    {
        int dotCount    = (float)inputAnalyser->getPathAnalysers().size() * renderRatio;
        
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        
        for( int i = 0; i < dotCount; ++i )
        {
            ofPolyline  line;
            
            line.addVertex( offset + inputAnalyser->getPathAnalysers()[i]->getSamples().back() * scale );
            
            result->push_back( line );
        }
        
        return result;
    };
    
private:
    
    float   renderRatio;
    
};
