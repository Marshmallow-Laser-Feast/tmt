//
//  VideoParams.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/20/13.
//
//

#pragma once

#include "IControlFreakMapper.h"

#include "ofMain.h"

#define INPUT_VIDEO             "Input Video"

class VideoParams: public IControlFreakMapper
{
    
public:
    
    enum
    {
        VIDEO_FILE      = 0,
        VIDEO_CAMERA    = 1
    };
    
public:
    
    VideoParams()
    
    :IControlFreakMapper( "Video Params" )
    
    {
        params.addNamedIndex( INPUT_VIDEO ).setLabels( 2, "File", "Camera" );
    };
    
    ~VideoParams()
    {};
    
};