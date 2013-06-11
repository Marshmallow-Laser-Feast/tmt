//
//  LinesInDistanceVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/11/13.
//
//

#pragma once

#include <map>

#include "IVisualizer.h"

#include "ofMain.h"

#define PARAM_NAME_VIS_RATIO        "Vis Ratio"
#define PARAM_NAME_MIN_DISTANCE     "Min Distance"
#define PARAM_NAME_MAX_DISTANCE     "Max Distance"
#define PARAM_NAME_MAX_CONNECTIONS  "Max Connections"

class LinesInDistanceVisualizer : public IVisualizer
{
    
public:
    
    LinesInDistanceVisualizer()
    {
        params.setName("LinesInDistanceVisualizer");
        
        params.addFloat( PARAM_NAME_VIS_RATIO ).setRange( 0.0f, 1.0f ).setClamp( true ).setIncrement( 0.01f );
        params.addFloat( PARAM_NAME_MIN_DISTANCE ).setIncrement( 10 );
        params.addFloat( PARAM_NAME_MAX_DISTANCE ).setIncrement( 10 );
        params.addInt( PARAM_NAME_MAX_CONNECTIONS ).setRange( 0, 10000000 ).setClamp( true ).setIncrement( 1 );
    };
    
    ~LinesInDistanceVisualizer()
    {};
    
public:
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale )
    {
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        
        std::map< std::pair<int, int>, int >   connectionsMap;
        
        for( int i = 0; i < inputAnalyser->getPathAnalysers().size(); ++i )
        {
            for( int j = 0; j < inputAnalyser->getPathAnalysers().size(); ++j )
            {
                if( i != j )
                {
                    ofPoint p1      = inputAnalyser->getPathAnalysers()[ i ]->getSamples().back();
                    ofPoint p2      = inputAnalyser->getPathAnalysers()[ j ]->getSamples().back();
                    
                    std::pair<int,int>  pair1( inputAnalyser->getPathAnalysers()[ i ]->getID(), inputAnalyser->getPathAnalysers()[ j ]->getID() );
                    std::pair<int,int>  pair2( inputAnalyser->getPathAnalysers()[ i ]->getID(), inputAnalyser->getPathAnalysers()[ j ]->getID() );
                    
                    float distance  =  p1.distance( p2 );
                    
                    if(     distance >= (float)params[ PARAM_NAME_MIN_DISTANCE ] &&
                            distance <= (float)params[ PARAM_NAME_MAX_DISTANCE ] &&
                            connectionsMap.count( pair1 ) == 0 &&
                            connectionsMap.count( pair2 ) == 0
                       )
                    {   
                        ofPolyline  line;
                        
                        line.lineTo( offset + p1 * scale );
                        line.lineTo( offset + p2 * scale );
                        
                        result->push_back( line );
                        
                        connectionsMap[ pair1 ] = 1;
                        connectionsMap[ pair2 ] = 1;
                    }
                }
            }
        }
        
        return result;
    };
    
private:
    
};

