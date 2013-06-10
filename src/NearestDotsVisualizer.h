//
//  NearestDotsVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/10/13.
//
//

#pragma once

#include "IVisualizer.h"

#include "ofMain.h"

class NearestDotsVisualizer : public IVisualizer
{
    
public:
    
    NearestDotsVisualizer()
    
    :count( 0 )
    
    {};
    
    ~NearestDotsVisualizer()
    {};
    
public:
    
    void setCount( int value )
    {
        count           = value;
    };
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale )
    {
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        
        int lineCount   = MIN( inputAnalyser->getPathAnalysers().size() / 2, count );
        
        for( int i = 0; i < lineCount; ++i )
        {
            ofPolyline  line;
            
            ofPoint     currentPoint    = inputAnalyser->getPathAnalysers()[i]->getSamples().back();
            
            float       closestDistance = 1000000.0;
            int         closestIndex    = 0;
            
            for( int j = 0; j < inputAnalyser->getPathAnalysers().size(); ++j )
            {
                float currentDistance   = currentPoint.distance( inputAnalyser->getPathAnalysers()[j]->getSamples().back() ) ;
                
                if( j != i && currentDistance < closestDistance )
                {
                    closestIndex        = j;
                    closestDistance     = currentDistance;
                }
            }
            
            line.addVertex( offset + currentPoint * scale );
            line.addVertex( offset + inputAnalyser->getPathAnalysers()[closestIndex]->getSamples().back() * scale );
            
            result->push_back( line );
        }
        
        return result;
    };
    
private:
    
    int count;
    
};

