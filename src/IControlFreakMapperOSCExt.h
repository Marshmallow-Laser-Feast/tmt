//
//  IControlFreakMapperOSCExt.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/17/13.
//
//

#pragma once

#include <map>
#include <string>

#include "ofxMSAControlFreak.h"

#define PARAM_NAME_ENABLE_OCS_INPUT "OCS"

class IControlFreakMapperOSCExt
{
    
protected:
    
    typedef std::map< msa::controlfreak::Parameter*, std::string >  OSCMappingsT;
    
public:
    
    IControlFreakMapperOSCExt()
    
    :paramsPtr( NULL )
    
    {};
    
    virtual ~IControlFreakMapperOSCExt()
    {};
    
public:
    
    void setParams( msa::controlfreak::ParameterGroup &param )
    {
        paramsPtr    = &param;
    };
    
    void setupOCS()
    {
        if( paramsPtr != NULL )
        {
            paramsPtr->addBool( PARAM_NAME_ENABLE_OCS_INPUT ).set( true );
        }
    };
    
    template<typename T>
    void applyOCS( std::map< string, vector<T> > &ocsData )
    {
        if( paramsPtr != NULL )
        {
            if( (bool)(*paramsPtr)[ PARAM_NAME_ENABLE_OCS_INPUT ] )
            {
                applyOCSInternal( ocsData );
            }
        } else {
            applyOCSInternal( ocsData );
        }
    };
    
    
public:
    
    template<typename T>
    void applyOCSInternal( std::map< string, vector<T> > &ocsData )
    {
        for ( OSCMappingsT::iterator pIt = oscMappings.begin() ; pIt != oscMappings.end(); ++pIt )
        {
            if( ocsData.count( pIt->second ) > 0 )
            {
                pIt->first->set( ocsData[ pIt->second ][0] );
            }
        }
    };
    
    msa::controlfreak::ParameterGroup   *paramsPtr;
    
    OSCMappingsT                        oscMappings;
    
};