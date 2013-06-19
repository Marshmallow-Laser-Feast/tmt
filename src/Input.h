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

#include "IPanelDraws.h"

#include "InputSample.h"

#define PARAM_NAME_ENABLE   "Enabled"

class Input: public IControlFreakMapper, public IControlFreakMapperMidiExt, public IControlFreakMapperOSCExt, public IPanelDraws
{
    
public:
    
    typedef enum
    {
        SAMPLING_TYPE_POINT,
        SAMPLING_TYPE_POLYLINE,
        SAMPLING_TYPE_BOTH
    } SamplingType;
    
public:
    
    Input( const std::string &name, SamplingType availableSamplingType_ )
    
    :IControlFreakMapper( name )
    ,availableSamplingType( availableSamplingType_ )
    
    {
        IControlFreakMapperMidiExt::setParams( params );
        IControlFreakMapperOSCExt::setParams( params );
        
        setupMidi();
        setupOCS();
        
        params.addBool( PARAM_NAME_ENABLE ).set( true );
    };
    
    virtual ~Input(){};
    
public:
    
    virtual void update()
    {
        if( (bool)params[ PARAM_NAME_ENABLE ] )
        {
            processPointSamples();
            processPolylineSamples();
        }
    };

    const SamplingType getAvailableSamplingType() const
    {
        return availableSamplingType;
    };
    
    const vector<PointSampleT> & getPointSamples() const { return pointSamples; };
    const vector<PolylineSampleT> & getPolylineSamples() const { return polylineSamples; };
    
    virtual std::string getName(){ return "Empty Input"; };
    
    virtual void draw( float width, float height ){};
    
protected:
    
    virtual void processPointSamples(){};
    virtual void processPolylineSamples(){};
    
    vector<PointSampleT>    pointSamples;
    vector<PolylineSampleT> polylineSamples;
    
private:
    
    SamplingType    availableSamplingType;
};
