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

typedef vector<PointSampleT>            PointSampleVectorT;
typedef vector<PointSampleVectorT>      PointSampleVectorVectorT;
typedef vector<PolylineSampleT>         PolylineSampleVectorT;

typedef ofPtr<PointSampleVectorT>       PointSampleVectorRefT;
typedef ofPtr<PointSampleVectorVectorT> PointSampleVectorVectorRefT;
typedef ofPtr<PolylineSampleVectorT>    PolylineSampleVectorRefT;

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
        
        pointSamplesMap[ DEFAULT_TAG ]          = PointSampleVectorRefT( new PointSampleVectorT );
        pointVectorSamplesMap[ DEFAULT_TAG ]    = PointSampleVectorVectorRefT( new PointSampleVectorVectorT() );
        polylineSamplesMap[ DEFAULT_TAG ]       = PolylineSampleVectorRefT( new PolylineSampleVectorT() );
        
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
    
    bool hasPointVectorSamples( const std::string &tag )
    {
        return pointVectorSamplesMap.count( tag ) > 0;
    };
    
    bool hasPolylineSamples( const std::string &tag )
    {
        return polylineSamplesMap.count( tag ) > 0;
    }
    
    const PointSampleVectorRefT getPointSamples( const std::string &tag ) const { return pointSamplesMap.at( tag ); };
    const PointSampleVectorVectorRefT getPointVectorSamples( const std::string &tag ) const { return pointVectorSamplesMap.at( tag ); };
    const PolylineSampleVectorRefT getPolylineSamples( const std::string &tag ) const { return polylineSamplesMap.at( tag ); };

    virtual std::string getName(){ return "Unnamed Input"; };
    
    virtual void draw( float width, float height ){};
    
protected:
    
    virtual void processPointSamples(){};
    virtual void processPointVectorSamples(){};
    virtual void processPolylineSamples(){};
    
    void drawPointSamples( PointSampleVectorRefT & samples, float circleR )
    {
        for( PointSampleVectorT::iterator it = samples->begin(); it != samples->end(); ++it )
        {
            ofCircle( it->getSample(), circleR );
        }
    }
    
    void drawPointVectorSamples( PointSampleVectorVectorRefT & samples, float circleR )
    {
        for( PointSampleVectorVectorT::iterator it = samples->begin(); it != samples->end(); ++it )
        {
            for( PointSampleVectorT::iterator pit = it->begin(); pit != it->end(); ++pit )
            {
                ofCircle( pit->getSample(), circleR );
            }
        }
    }
    
    void drawPolylineSamples( PolylineSampleVectorRefT & samples )
    {
        for( PolylineSampleVectorT::iterator it = samples->begin(); it != samples->end(); ++it )
        {
            it->getSample().draw();
        }
    }
    
    std::map< std::string, PointSampleVectorRefT>       pointSamplesMap;
    std::map< std::string, PointSampleVectorVectorRefT> pointVectorSamplesMap;
    std::map< std::string, PolylineSampleVectorRefT>    polylineSamplesMap;
    
};
