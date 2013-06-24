//
//  IFilter.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/12/13.
//
//

#pragma once

#include "ofMain.h"

#include "IControlFreakMapper.h"
#include "IControlFreakMapperOSCExt.h"
#include "IControlFreakMapperMidiExt.h"

#define PARAM_NAME_ACTIVITY_VALUE   "Activity Value"

class IFilter: public IControlFreakMapper, public IControlFreakMapperMidiExt, public IControlFreakMapperOSCExt
{
    
public:
    
    IFilter( const std::string &name_ )
    
    :IControlFreakMapper( name_ )
    
    {
        IControlFreakMapperMidiExt::setParams( params );
        IControlFreakMapperOSCExt::setParams( params );
        
        setupMidi();
        setupOCS();
        
        params.addFloat(PARAM_NAME_ACTIVITY_VALUE).setClamp(true);
    };
    
    virtual ~IFilter(){};
    
public:
    
    virtual void apply( std::vector<ofPolyline> &polylines, float audioAmp ){};
        
};