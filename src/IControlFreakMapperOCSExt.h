//
//  IControlFreakMapperOCSExt.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/17/13.
//
//

#pragma once

#include <map>
#include <string>

#include "ofxMSAControlFreak.h"

class IControlFreakMapperOCSExt
{
    
protected:
    
    typedef std::map< msa::controlfreak::Parameter*, std::string >  OCSMappingsT;
    
public:
    
    IControlFreakMapperOCSExt()
    {};
    
    virtual ~IControlFreakMapperOCSExt()
    {};
    
public:
    
    template<typename T>
    void applyOCS( const std::map< string, vector<T> > &ocsData )
    {
        for ( OCSMappingsT::iterator pIt = ocsMappings.begin() ; pIt != ocsMappings.end(); ++pIt )
        {
            if( ocsData.count( pIt->second ) > 0 )
            {
                pIt->first->set( ocsData[ pIt->second ][0] );
            }
        }
    };
    
    
public:
    
    OCSMappingsT ocsMappings;
    
};