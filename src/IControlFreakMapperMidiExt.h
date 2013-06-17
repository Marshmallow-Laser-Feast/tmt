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

class IControlFreakMapperMidiExt
{
    
protected:
    
    typedef std::map< msa::controlfreak::Parameter*, std::pair<int, int> > MidiMappingsT;
    
public:
    
    IControlFreakMapperMidiExt()
    {};
    
    virtual ~IControlFreakMapperMidiExt()
    {};
    
public:
    
    template<typename T>
    void applyMidi( const std::map<std::pair<int,int>, T> &midiData )
    {
        for ( MidiMappingsT::iterator pIt = midiMappings.begin() ; pIt != midiMappings.end(); ++pIt )
        {
            if( midiData.count( pIt->second ) > 0 )
            {
                pIt->first->set( midiData[ pIt->second ] );
            }
        }
    };
    
public:
    
      MidiMappingsT midiMappings;
    
};