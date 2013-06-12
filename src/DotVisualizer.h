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
        
        midiMappings[ &params.get(PARAM_NAME_BRIGHTNESS) ]                      = std::pair<int, int>( 1, 14 );
        midiMappings[ &params.get(PARAM_NAME_TIME_OFFSET) ]                     = std::pair<int, int>( 1, 15 );
        midiMappings[ &params.get(PARAM_NAME_DOT_VIS_RATIO) ]                   = std::pair<int, int>( 1, 16 );
    };
    
    ~DotVisualizer()
    {};
    
public:
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale, float audioAmp, vector<float> & audioFFT, float avgFFT )
    {
        int timeOffset = params[PARAM_NAME_TIME_OFFSET];
        
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        if((int)params[PARAM_NAME_BRIGHTNESS] == 0) {
            return result;
        }

        int dotCount    = (float)inputAnalyser->getPathAnalysers().size() * (float)params[PARAM_NAME_DOT_VIS_RATIO];
        
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
