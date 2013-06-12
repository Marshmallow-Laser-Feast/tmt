//
//  LinesInDistanceVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/11/13.
//
//

#pragma once

#include <map>
#include <set>

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
    
    /*
     
     x-------x------x
             I
             I
             I
             x
     
     */
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale )
    {
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        
        for( int i = 0; i < inputAnalyser->getPathAnalysers().size(); ++i )
        {
            for( int j = 0; j < inputAnalyser->getPathAnalysers().size(); ++j )
            {
                if( i != j )
                {
                    ofPoint p1          = inputAnalyser->getPathAnalysers()[ i ]->getSamples().back();
                    ofPoint p2          = inputAnalyser->getPathAnalysers()[ j ]->getSamples().back();
                    
                    pair<int,int> connection1( inputAnalyser->getPathAnalysers()[ i ]->getID(), inputAnalyser->getPathAnalysers()[ j ]->getID() );
                    pair<int,int> connection2( inputAnalyser->getPathAnalysers()[ j ]->getID(), inputAnalyser->getPathAnalysers()[ i ]->getID() );
                    
                    int lineCountSource = connectionCounts[ inputAnalyser->getPathAnalysers()[i]->getID()];
                    int lineCountTarget = connectionCounts[ inputAnalyser->getPathAnalysers()[j]->getID()];
                    
                    float distance      =  p1.distance( p2 );
                    
                    if(     distance >= (float)params[ PARAM_NAME_MIN_DISTANCE ] &&
                            distance <= (float)params[ PARAM_NAME_MAX_DISTANCE ] &&
                            connections.count( connection1 ) == 0 &&
                            connections.count( connection2 ) == 0 &&
                            lineCountSource < (int)params[PARAM_NAME_MAX_CONNECTIONS] &&
                            lineCountTarget < (int)params[PARAM_NAME_MAX_CONNECTIONS]
                       )
                    {   
                        ofPolyline  line;
                        
                        line.lineTo( offset + p1 * scale );
                        line.lineTo( offset + p2 * scale );
                        
                        result->push_back( line );
                        
                        if( connectionCounts.count( inputAnalyser->getPathAnalysers()[ i ]->getID() ) == 0 )
                        {
                            connectionCounts[ inputAnalyser->getPathAnalysers()[ i ]->getID() ] = 0;
                        }
                        
                        if( connectionCounts.count( inputAnalyser->getPathAnalysers()[ j ]->getID() ) == 0 )
                        {
                            connectionCounts[ inputAnalyser->getPathAnalysers()[ j ]->getID() ]         = 0;
                        }
                        
                        connectionCounts[ inputAnalyser->getPathAnalysers()[i]->getID()]++;
                        connectionCounts[ inputAnalyser->getPathAnalysers()[j]->getID()]++;
                                         
                        connections.insert( connection1 );
                        connections.insert( connection2 );
                    }
                }
            }
        }
        
        return result;
    };
    
private:
    
    map<int, int>           connectionCounts;
    set< pair<int, int> >   connections;
    
};

