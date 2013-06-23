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

#include "InputSample.h"

#include "IControlFreakMapper.h"
#include "IControlFreakMapperOSCExt.h"
#include "IControlFreakMapperMidiExt.h"

#include "IPanelDraws.h"

typedef vector<PointSampleT>                PointSampleVectorT;
typedef vector<PointSampleVectorT>          PointSampleVectorVectorT;
typedef vector<PolylineSampleT>             PolylineSampleVectorT;

typedef ofPtr<PointSampleVectorT>           PointSampleVectorRefT;
typedef ofPtr<PointSampleVectorVectorT>     PointSampleVectorVectorRefT;
typedef ofPtr<PolylineSampleVectorT>        PolylineSampleVectorRefT;

class Input: public IControlFreakMapper, public IControlFreakMapperMidiExt, public IControlFreakMapperOSCExt, public IPanelDraws
{
    
public:
    
    Input( const std::string &name_ )
    
    :IControlFreakMapper( name_ )
    ,name( name_ )
    
    {
        IControlFreakMapperMidiExt::setParams( params );
        IControlFreakMapperOSCExt::setParams( params );
        
        setupMidi();
        setupOCS();
        
        emptyPointSampleVector          = PointSampleVectorRefT( new PointSampleVectorT() );
        emptyPointSampleVectorVector    = PointSampleVectorVectorRefT( new PointSampleVectorVectorT() );
        emptyPolylineSampleVector       = PolylineSampleVectorRefT( new PolylineSampleVectorT() );
    };
    
    virtual ~Input(){};
    
public:
    
    virtual void update(){};
    
    virtual const bool hasPointSamples( const std::string &tag ) const  { return false; };
    virtual const bool hasPointVectorSamples( const std::string &tag ) const { return false; };
    virtual const bool hasPolylineSamples( const std::string &tag ) const { return false; }
    
    virtual const std::vector<string> & getPointSampleTags() const{ return pointSampleTags; };
    virtual const std::vector<string> & getPointVectorSampleTags() const{ return pointVectorSampleTags; };
    virtual const std::vector<string> & getPolylineSampleTags() const{ return polylineSampleTags; };
    
    virtual const PointSampleVectorRefT getPointSamples( const std::string &tag ) const { return emptyPointSampleVector; };
    virtual const PointSampleVectorVectorRefT getPointVectorSamples( const std::string &tag ) const { return emptyPointSampleVectorVector; };
    virtual const PolylineSampleVectorRefT getPolylineSamples( const std::string &tag ) const { return emptyPolylineSampleVector; };
    
public:
    
    virtual const ofVec2f getSize() const { return ofVec2f(300, 300); };
    
    virtual const std::string getName() const { return name; };
    virtual const std::string getPanelName() const { return name; };
    
protected:
    
    std::vector<string>         pointSampleTags;
    std::vector<string>         pointVectorSampleTags;
    std::vector<string>         polylineSampleTags;
    
    PointSampleVectorRefT       emptyPointSampleVector;
    PointSampleVectorVectorRefT emptyPointSampleVectorVector;
    PolylineSampleVectorRefT    emptyPolylineSampleVector;
    
    std::string                 name;
    
};

