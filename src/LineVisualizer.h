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

#define PARAM_NAME_LINE_VIS_COUNT               "Line Vis Count"



class LineVisualizer : public IVisualizer
{
    
public:
    
    LineVisualizer()
    {
        params.setName("LineVisualizer");
        params.addInt( PARAM_NAME_LINE_VIS_COUNT ).setRange( 0, 100 ).setClamp( true );
        
        midiMappings[ &params.get(PARAM_NAME_BRIGHTNESS) ]                      = std::pair<int, int>( 5, 14 );
        midiMappings[ &params.get(PARAM_NAME_TIME_OFFSET) ]                     = std::pair<int, int>( 5, 15 );
        midiMappings[ &params.get(PARAM_NAME_LINE_VIS_COUNT) ]                  = std::pair<int, int>( 5, 16 );
        
        oscMappings[ &params.get(PARAM_NAME_BRIGHTNESS) ]   = "/LineVisualizer Brightness";
        //oscMappings[ &params.get("Amps") ]                  = "/LineVisualizer Amp";
        
    };
    
    ~LineVisualizer()
    {};
    
public:
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale, float audioAmp, float audioFFT )
    {
        int timeOffset = params[PARAM_NAME_TIME_OFFSET];
        
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        if((int)params[PARAM_NAME_BRIGHTNESS] == 0) {
            return result;
        }

        int count = params[ PARAM_NAME_LINE_VIS_COUNT ];
        

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
    
};

