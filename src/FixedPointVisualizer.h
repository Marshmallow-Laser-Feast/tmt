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

class FixedPointVisualizer : public IVisualizer
{
    
public:
    
    FixedPointVisualizer()
    
    :count( 0 )
    
    {
        params.setName("FixedPointVisualizer");
    };
    
    ~FixedPointVisualizer()
    {};
    
public:
    
    void setCount( int value )
    {
        count               = value;
    };
    
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
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        
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
    
    int count;
    
};
