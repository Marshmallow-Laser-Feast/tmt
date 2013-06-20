//
//  AppParams.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/20/13.
//
//

#pragma once

#include "IControlFreakMapper.h"

#include "ofMain.h"

#define PARAM_OSC_ENABLED       "Enable OCS"
#define PARAM_MIDI_ENABLED      "Enable Midi"

class AppParams: public IControlFreakMapper
{
    
public:

    AppParams()
    
    :IControlFreakMapper( "App Params" )
    
    {
        params.addBool( PARAM_OSC_ENABLED ).set( true );
        params.addBool( PARAM_MIDI_ENABLED ).set( true );
    };
    
    ~AppParams()
    {};
    
};