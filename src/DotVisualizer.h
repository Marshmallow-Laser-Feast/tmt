//
//  DotVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/10/13.
//
//

#pragma once

#include "IVisualizer.h"

#define PARAM_NAME_DOT_VIS_RATIO                "Dots Ratio"


class DotVisualizer : public IVisualizer
{
    
public:
    
    DotVisualizer()
    {
        params.setName("DotVisualizer");
        params.addFloat(PARAM_NAME_DOT_VIS_RATIO).setClamp( true );

    
    };
    
    ~DotVisualizer()
    {};
    
public:
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale )
    {
        int dotCount    = (float)inputAnalyser->getPathAnalysers().size() * (float)params[PARAM_NAME_DOT_VIS_RATIO];
        
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
};
