//
//  FixedPointVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/10/13.
//
//

#pragma once

#include <map>

#include "IVisualizer.h"

#define PARAM_NAME_FIXED_POINT_VIS_COUNT        "Fixed Point Vis Count"
#define PARAM_NAME_FIXED_POINT_FIX              "Fixed Point Vis FIX Bang"
#define PARAM_NAME_FIXED_POINT_CLEAR            "Fixed Point Vis Clear Bang"

class FixedPointVisualizer : public IVisualizer
{
    
public:
    
    FixedPointVisualizer()
    {
        params.setName("FixedPointVisualizer");
        params.addInt( PARAM_NAME_FIXED_POINT_VIS_COUNT ).setRange( 0, 100 ).setClamp( true );
        params.addBang( PARAM_NAME_FIXED_POINT_FIX );
        params.addBang( PARAM_NAME_FIXED_POINT_CLEAR );
        
    };
    
    ~FixedPointVisualizer()
    {};
    
public:
    
    void setFixedPoints( InputAnalyser *inputAnalyser )
    {        
        for ( std::vector<PathAnalyser*>::iterator it = inputAnalyser->getPathAnalysers().begin(); it != inputAnalyser->getPathAnalysers().end(); ++it )
        {
            if( fixedPointMap.count( *it ) == 0  )
            {
                fixedPointMap[*it]  = std::vector<ofPoint>();
            }
            
            fixedPointMap[*it].push_back( (*it)->getSamples().back() );
        }
    };
    
    void clearFixedPoints()
    {
        fixedPointMap.clear();
    };
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale )
    {
        int timeOffset = params[PARAM_NAME_TIME_OFFSET];
        
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        if((int)params[PARAM_NAME_BRIGHTNESS] == 0) {
            return result;
        }

        
        int count = params[ PARAM_NAME_FIXED_POINT_VIS_COUNT ];
        
        if( (bool)params[PARAM_NAME_FIXED_POINT_FIX] )
        {
            setFixedPoints( inputAnalyser );
        }
        
        if( (bool)params[PARAM_NAME_FIXED_POINT_CLEAR] )
        {
            clearFixedPoints();
        }
        

        
        int lineCount       = MIN( inputAnalyser->getPathAnalysers().size(), count );
        
        for( int i = 0; i < lineCount; ++i )
        {
            ofPolyline  line;
            
            
            
            if( fixedPointMap.count( inputAnalyser->getPathAnalysers()[i] ) == 0 )
            {
                fixedPointMap[inputAnalyser->getPathAnalysers()[i]]  = std::vector<ofPoint>();
                
                fixedPointMap[inputAnalyser->getPathAnalysers()[i]].push_back(inputAnalyser->getPathAnalysers()[i]->getSamples().back());
            }
            
            for( std::vector<ofPoint>::iterator pit = fixedPointMap[inputAnalyser->getPathAnalysers()[i]].begin(); pit != fixedPointMap[inputAnalyser->getPathAnalysers()[i]].end(); ++pit )
            {
                line.addVertex( offset + inputAnalyser->getPathAnalysers()[i]->getSamples().back() * scale );
                line.addVertex( offset + (*pit) * scale );
            }
            
            result->push_back( line );
        }
        
        return result;
    };
    
private:
    
    std::map<PathAnalyser*, std::vector<ofPoint> >    fixedPointMap;
};
