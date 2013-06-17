//
//  IControlFreakMapper.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/17/13.
//
//

#pragma

#include <string>

#include "ofxMSAControlFreak.h"

class IControlFreakMapper
{
    
public:
    
    IControlFreakMapper( const std::string &paramName )
    {
        param.setName( paramName );
    };
    
    virtual ~IControlFreakMapper(){};
    
public:
    
    msa::controlfreak::ParameterGroup param;
    
};
