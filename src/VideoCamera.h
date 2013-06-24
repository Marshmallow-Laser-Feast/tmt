//
//  VideoCamera.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/18/13.
//
//

#pragma once

#include "ofMain.h"

#include "Video.h"

#include "Grabber.h"

#define INPUT_WIDTH                     640
#define INPUT_HEIGHT                    480

#define PARAM_NAME_ENABLE_UPDATE        "Update"

#define PARAM_NAME_FLIP_X               "Flip X"
#define PARAM_NAME_FLIP_Y               "Flip Y"

#define PARAM_NAME_CAMERA_BRIGHTNESS    "Camera Brigtness"
#define PARAM_NAME_CAMERA_GAMMA         "Camera Gamma"
#define PARAM_NAME_CAMERA_SHUTTER       "Camera Shutter"
#define PARAM_NAME_CAMERA_GAIN          "Camera Gain"

class VideoCamera: public Video
{
    
public:
    
    VideoCamera()
    
    :Video( "Video/Camera" )
    
    {
        params.addBool( PARAM_NAME_ENABLE_UPDATE ).set( grabber.getCameraCount() > 0 );
        
        params.addBool( PARAM_NAME_FLIP_X );
        params.addBool( PARAM_NAME_FLIP_Y );
        
        params.addFloat( PARAM_NAME_ROI_X1 ).setClamp( true );
        params.addFloat( PARAM_NAME_ROI_Y1 ).setClamp( true );
        params.addFloat( PARAM_NAME_ROI_X2 ).setClamp( true ).set( 1.0f );
        params.addFloat( PARAM_NAME_ROI_Y2 ).setClamp( true ).set( 1.0f );
        
        params.addFloat( PARAM_NAME_CAMERA_BRIGHTNESS ).setClamp( true ).setSnap( true ).set( 1 );
        params.addFloat( PARAM_NAME_CAMERA_GAMMA ).setClamp( true ).setSnap( true ).set( 1 );
        params.addFloat( PARAM_NAME_CAMERA_SHUTTER ).setClamp( true ).setSnap( true ).set( 1 );
        params.addFloat( PARAM_NAME_CAMERA_GAIN ).setClamp( true ).setSnap( true ).set( 1 );
        
        grabber.setSize( INPUT_WIDTH , INPUT_HEIGHT );
        grabber.setImageType( OF_IMAGE_GRAYSCALE );
        grabber.setup();
        
        
    };
    
    ~VideoCamera(){};
    
public:
    
    virtual void update()
    {
        if( params[PARAM_NAME_CAMERA_BRIGHTNESS].hasChanged() )
        {
            grabber.setBrightness( params[PARAM_NAME_CAMERA_BRIGHTNESS] );
        }
        
        if( params[PARAM_NAME_CAMERA_GAMMA].hasChanged() )
        {
            grabber.setGamma( params[PARAM_NAME_CAMERA_GAMMA] );
        }
        
        if( params[PARAM_NAME_CAMERA_SHUTTER].hasChanged() )
        {
            grabber.setShutter( params[PARAM_NAME_CAMERA_SHUTTER] );
        }
        
        if( params[PARAM_NAME_CAMERA_GAIN].hasChanged() )
        {
            grabber.setGain( params[PARAM_NAME_CAMERA_GAIN] );
        }
        
        if( (bool)params[PARAM_NAME_ENABLE_UPDATE] )
        {
            grabber.update();
            
            originalFrameImage.setFromPixels( grabber.getPixelsRef() );
            
            bool doFlipX = params[PARAM_NAME_FLIP_X];
            bool doFlipY = params[PARAM_NAME_FLIP_Y];
            
            if( doFlipX || doFlipY )
            {
                originalFrameImage.mirror( doFlipY, doFlipX );
            }
            
            float w = ofClamp( (float)params[ PARAM_NAME_ROI_X2 ] - (float)params[ PARAM_NAME_ROI_X1 ], 0.0f, 1.0f );
            float h = ofClamp( (float)params[ PARAM_NAME_ROI_Y2 ] - (float)params[ PARAM_NAME_ROI_Y1 ], 0.0f, 1.0f );
            
            roiImage.cropFrom(  originalFrameImage,
                                (float)params[ PARAM_NAME_ROI_X1 ] * originalFrameImage.getWidth(),
                                (float)params[ PARAM_NAME_ROI_Y1 ] * originalFrameImage.getHeight(),
                                w * originalFrameImage.getWidth(),
                                h * originalFrameImage.getHeight()
                              );
        }
    };
    
    virtual bool isFrameNew()
    {
        return true;
    };
    
    virtual const std::string getPanelName() const { return "Video/Camera"; };
    
    virtual const ofVec2f getPanelSize() const
    {
        return ofVec2f( INPUT_WIDTH, INPUT_HEIGHT );
    };
    
private:
        
    ofxLibdc::Grabber           grabber;
    
};