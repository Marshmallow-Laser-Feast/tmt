//
//  Input.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoğlu on 6/4/13.
//
//

#pragma once

#include "ofMain.h"

#include "IControlFreakMapper.h"
#include "IControlFreakMapperOSCExt.h"
#include "IControlFreakMapperMidiExt.h"

#include "InputSample.h"

#define PARAM_NAME_ENABLE_POINT_SAMPLING    "Point Sampling"
#define PARAM_NAME_ENABLE_POLYLINE_SAMPLING "Polyline Sampling"

class Input: public IControlFreakMapper, public IControlFreakMapperMidiExt, public IControlFreakMapperOSCExt
{
    
public:
    
    Input( const std::string &name )
    
    :IControlFreakMapper( name )
    
    {
        IControlFreakMapperMidiExt::setParams( params );
        IControlFreakMapperOSCExt::setParams( params );
        
        setupMidi();
        setupOCS();
        
        params.addBool( PARAM_NAME_ENABLE_POINT_SAMPLING ).set( true );
        params.addBool( PARAM_NAME_ENABLE_POLYLINE_SAMPLING ).set( true );
    };
    
    virtual ~Input(){};
    
public:
    
    virtual void update()
    {
        pointSamples.clear();
        polylineSamples.clear();
        
        if( (bool)params[ PARAM_NAME_ENABLE_POINT_SAMPLING ] )
        {
            processPointSamples();
        }
        
        if( (bool)params[ PARAM_NAME_ENABLE_POLYLINE_SAMPLING ] )
        {
            processPolylineSamples();
        }
    };
    
    const vector<PointSampleT> & getPointSamples() const { return pointSamples; };
    const vector<PolylineSampleT> & getPolylineSamples() const { return polylineSamples; };
    
protected:
    
    virtual void processPointSamples(){};
    virtual void processPolylineSamples(){};
    
    vector<PointSampleT>    pointSamples;
    vector<PolylineSampleT> polylineSamples;
};
