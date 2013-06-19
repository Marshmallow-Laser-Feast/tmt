//
//  IVideoInput.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/19/13.
//
//

#pragma once

#include "Input.h"

class IVideoInput: public Input
{
    
public:
    
    IVideoInput( const std::string &name, SamplingType availableSamplingType_ )
    
    :Input( name, availableSamplingType_ )
    ,image( NULL )
    ,isCurrentFrameNew( false )
    
    {};
    
    ~IVideoInput(){};
    
public:
    
    void setImage( ofImage & image_ )
    {
        image  = &image_;
    };
    
    void setCurrentFrameNew( bool value )
    {
        isCurrentFrameNew   = value;
    };
    
protected:
    
    ofImage     *image;
    bool        isCurrentFrameNew;
    
};