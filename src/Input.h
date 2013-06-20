//
//  Input.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoğlu on 6/4/13.
//
//

#pragma once

#include <map>
#include <vector>

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
    
    static const std::string DEFAULT_TAG;
    
public:
    
    Input( const std::string &name )
    
    :IControlFreakMapper( name )
    
    {
        IControlFreakMapperMidiExt::setParams( params );
        IControlFreakMapperOSCExt::setParams( params );
        
        setupMidi();
        setupOCS();
        
        params.addBool( PARAM_NAME_ENABLE ).set( true );
        
        pointSamplesMap[ DEFAULT_TAG ]      = vector<PointSampleT>();
        polylineSamplesMap[ DEFAULT_TAG ]   = vector<PolylineSampleT>();
        
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
    
    bool hasPointSamples( const std::string &tag )
    {
        return pointSamplesMap.count( tag ) > 0;
    };
    
    bool hasPolylineSamples( const std::string &tag )
    {
        return polylineSamplesMap.count( tag ) > 0;
    }
    
    const vector<PointSampleT> & getPointSamples( const std::string &tag ) const { return pointSamplesMap.at( tag ); };
    const vector<PolylineSampleT> & getPolylineSamples( const std::string &tag ) const { return polylineSamplesMap.at( tag ); };
    
    virtual std::string getName(){ return "Unnamed Input"; };
    
    virtual void draw( float width, float height ){};
    
protected:
    
    virtual void processPointSamples(){};
    virtual void processPolylineSamples(){};
    
    void drawPointSamples( std::vector<PointSampleT> &samples, float circleR )
    {
        for( std::vector<PointSampleT>::iterator it = samples.begin(); it != samples.end(); ++it )
        {
            ofCircle( it->getSample(), circleR );
        }
    }
    
    void drawPolylineSamples( std::vector<PolylineSampleT> &samples )
    {
        for( std::vector<PolylineSampleT>::iterator it = samples.begin(); it != samples.end(); ++it )
        {
            it->getSample().draw();
        }
    }
    
    std::map< std::string, vector<PointSampleT> >       pointSamplesMap;
    std::map< std::string, vector<PolylineSampleT> >    polylineSamplesMap;
    
};
