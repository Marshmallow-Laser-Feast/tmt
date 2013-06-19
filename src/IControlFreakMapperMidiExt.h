//
//  IControlFreakMapperMidiExt.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/17/13.
//
//

#pragma once

#include <map>

#include "ofxMSAControlFreak.h"

#define PARAM_NAME_ENABLE_MIDI_INPUT    "Midi Input"

class IControlFreakMapperMidiExt
{
    
protected:
    
    typedef std::map< msa::controlfreak::Parameter*, std::pair<int, int> > MidiMappingsT;
    
public:
    
    IControlFreakMapperMidiExt()
    
    :paramsPtr( NULL )
    
    {};
    
    virtual ~IControlFreakMapperMidiExt()
    {};
    
public:
    
    void setParams( msa::controlfreak::ParameterGroup &param )
    {
        paramsPtr    = &param;
    };
    
    void setupMidi()
    {
        if( paramsPtr != NULL )
        {
            paramsPtr->addBool( PARAM_NAME_ENABLE_MIDI_INPUT );
        }
    };
    
    template<typename T>
    void applyMidi( std::map<std::pair<int,int>, T> &midiData )
    {
        if( paramsPtr != NULL )
        {
            if( (bool)(*paramsPtr)[ PARAM_NAME_ENABLE_MIDI_INPUT ] )
            {
                applyMidiInternal( midiData );
            }
        } else {
            applyMidiInternal( midiData );
        }
    };
    
public:
    
    template<typename T>
    void applyMidiInternal( std::map<std::pair<int,int>, T> &midiData )
    {
        for ( MidiMappingsT::iterator pIt = midiMappings.begin() ; pIt != midiMappings.end(); ++pIt )
        {
            if( midiData.count( pIt->second ) > 0 )
            {
                pIt->first->set( midiData[ pIt->second ] );
            }
        }
    };
    
    msa::controlfreak::ParameterGroup   *paramsPtr;
    
    MidiMappingsT                       midiMappings;
    
};