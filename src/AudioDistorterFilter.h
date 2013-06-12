//
//  AudioDistorterFilter.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/12/13.
//
//

#pragma once

#include "ofMain.h"

#include "IFilter.h"

class AudioDistorterFilter: public IFilter
{
    
public:
    
    AudioDistorterFilter()
    {
        params.setName("AudioDistorterFilter");
    };
    
    virtual ~AudioDistorterFilter()
    {};
    
public:
    
    virtual void apply( std::vector<ofPolyline> &polylines )
    {};
    
};