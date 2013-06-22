//
//  IVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/22/13.
//
//

#pragma once

#include <string>
#include <map>
#include <deque>

#include "ofMain.h"

#include "IControlFreakMapper.h"
#include "IControlFreakMapperOSCExt.h"
#include "IControlFreakMapperMidiExt.h"

#include "IPanelDraws.h"

#include "Input.h"

#define PARAM_NAME_ENABLED      "Enabled"

#define PARAM_NAME_CACHE_SIZE   "Cache Size"
#define PARAM_NAME_CACHE_OFFSET "Cache Offset"

#define PARAM_NAME_BRIGHTNESS   "Brightness"

typedef std::map<std::string, Input *>      InputsMapT;
typedef std::vector<ofPolyline>             PolylineVectorT;
typedef ofPtr< std::vector<ofPolyline> >    PolylineVectorRefT;

class IVisualizer: public IControlFreakMapper, public IControlFreakMapperMidiExt, public IControlFreakMapperOSCExt, public IPanelDraws
{
    
public:
    
    typedef deque<PolylineVectorRefT>   PolylineVectorRefDequeT;
    
public:
    
    IVisualizer( const std::string &name_ )
    
    :IControlFreakMapper( name_ )
    ,name( name_ )
    
    {
        IControlFreakMapperMidiExt::setParams( params );
        IControlFreakMapperOSCExt::setParams( params );
        
        setupMidi();
        setupOCS();
        
        params.addBool( PARAM_NAME_ENABLED );
        
        params.addInt( PARAM_NAME_CACHE_SIZE ).set( 1 );
        params.addInt( PARAM_NAME_CACHE_OFFSET ).setRange(0, 1).setClamp(true);
        
        params.addFloat( PARAM_NAME_BRIGHTNESS ).setRange(0, 1).setClamp(true);        
    };
    
    virtual ~IVisualizer()
    {};
    
public:
    
    void update()
    {
        if( params[ PARAM_NAME_CACHE_SIZE ].hasChanged() )
        {
            params[ PARAM_NAME_CACHE_OFFSET ].setRange(0, (int)params[ PARAM_NAME_CACHE_SIZE ] ).setClamp(true);
        }
        
        while( polylineVectorRefDeque.size() > (int)params[ PARAM_NAME_CACHE_SIZE ] )
        {
            polylineVectorRefDeque.pop_front();
        }
    }
    
    const PolylineVectorRefT getOutput() const
    {
        if( polylineVectorRefDeque.size() > 0 )
        {
            int offset  = ofClamp( (int)polylineVectorRefDeque.size() - (int)params[ PARAM_NAME_CACHE_OFFSET ], 0, polylineVectorRefDeque.size() - 1 );
            
            return polylineVectorRefDeque[ offset ];
        }
        
        return PolylineVectorRefT( new PolylineVectorT() );
        
    };
    
    PolylineVectorRefT getOutput()
    {
        if( polylineVectorRefDeque.size() > 0 )
        {
            int offset  = ofClamp( (int)polylineVectorRefDeque.size() - (int)params[ PARAM_NAME_CACHE_OFFSET ], 0, polylineVectorRefDeque.size() - 1 );
            
            return polylineVectorRefDeque[ offset ];
        }
        
        return PolylineVectorRefT( new PolylineVectorT() );
        
    };
    
    virtual void setup( const InputsMapT &inputsMap )
    {};
    
    virtual void visualize( const InputsMapT &inputsMap,
                            const float audioAmp,
                            const int fftSampleCount,
                            const float * fftData,
                            const float time
                           )
    {};
    
    const float getBrightness() const
    {
        return (float)params[ PARAM_NAME_BRIGHTNESS ];
    }
    
public:
    
    virtual const std::string getPanelName() const { return name; };
    
    virtual const ofVec2f getPanelSize() const { return ofVec2f(300, 300); };
    
    virtual void draw( float width, float height ){};
    
protected:
    
    PolylineVectorRefT newOutput()
    {
        return PolylineVectorRefT( new PolylineVectorT() );
    };
    
    void addOutput( PolylineVectorRefT output )
    {
        polylineVectorRefDeque.push_back( output );
    };
    
private:
    
    std::string             name;
    
    PolylineVectorRefDequeT polylineVectorRefDeque;
};