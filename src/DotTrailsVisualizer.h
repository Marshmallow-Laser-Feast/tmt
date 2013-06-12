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


#define PARAM_NAME_DOT_TRAILS_VIS_RATIO         "Dots Trails Ratio"
#define PARAM_NAME_DOT_TRAILS_VIS_TRAILS_COUNT  "Dots Trails Count"


class DotTrailsVisualizer : public IVisualizer
{
    
public:
    
    DotTrailsVisualizer()
    {
        params.setName("DotTrailsVisualizer");
        params.addFloat( PARAM_NAME_DOT_TRAILS_VIS_RATIO ).setClamp( true );
        params.addInt( PARAM_NAME_DOT_TRAILS_VIS_TRAILS_COUNT ).setRange( 0, 100 ).setClamp( true );
        
        midiMappings[ &params.get(PARAM_NAME_BRIGHTNESS) ]                      = std::pair<int, int>( 2, 14 );
        midiMappings[ &params.get(PARAM_NAME_TIME_OFFSET) ]                     = std::pair<int, int>( 2, 15 );
        midiMappings[ &params.get(PARAM_NAME_DOT_TRAILS_VIS_RATIO) ]            = std::pair<int, int>( 2, 16 );
        midiMappings[ &params.get(PARAM_NAME_DOT_TRAILS_VIS_TRAILS_COUNT) ]     = std::pair<int, int>( 2, 17 );
        
    };
    
    ~DotTrailsVisualizer()
    {
    };
    
public:

    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale )
    {
        int timeOffset = params[PARAM_NAME_TIME_OFFSET];
        
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        if((int)params[PARAM_NAME_BRIGHTNESS] == 0) {
            return result;
        }

        int dotCount    = (float)inputAnalyser->getPathAnalysers().size() * (float)params[PARAM_NAME_DOT_TRAILS_VIS_RATIO];
        int trailCount = params[PARAM_NAME_DOT_TRAILS_VIS_TRAILS_COUNT];
        
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
};
