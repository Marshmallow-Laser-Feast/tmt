//
//  VideoFile.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/18/13.
//
//

#pragma once

#include <vector>
#include <map>

#include "ofMain.h"

#include "Video.h"

#define VIDEO_FOLDER                    "inputVideo"

#define PARAM_NAME_ENABLE_PLAYBACK      "Playback"

#define PARAM_NAME_FLIP_X               "Flip X"
#define PARAM_NAME_FLIP_Y               "Flip Y"

#define PARAM_NAME_VIDEO_SOURCE         "Video Source"
#define PARAM_NAME_VIDEO_FRAME          "Video Frame"

class VideoFile: public Video
{
    
public:
    
    VideoFile()
    
    :Video( "Video/File" )
    ,videoWidth( 0 )
    ,videoHeight( 0 )
    
    {
        updateVideoFiles();
        
        if( videoFileNames.size() > 0 )
        {
            loadVideoAtIndex( 0 );
        }
        
        params.addBool( PARAM_NAME_ENABLE_PLAYBACK ).set( true );
        
        params.addBool( PARAM_NAME_FLIP_X );
        params.addBool( PARAM_NAME_FLIP_Y );
        
        params.addNamedIndex( PARAM_NAME_VIDEO_SOURCE ).setLabels( videoFileNames );
        
        params.addFloat( PARAM_NAME_ROI_X1 ).setClamp( true );
        params.addFloat( PARAM_NAME_ROI_Y1 ).setClamp( true );
        params.addFloat( PARAM_NAME_ROI_X2 ).setClamp( true ).set( 1.0f );
        params.addFloat( PARAM_NAME_ROI_Y2 ).setClamp( true ).set( 1.0f );
        
        params.addInt( PARAM_NAME_VIDEO_FRAME ).setRange(0, videoPlayer.getTotalNumFrames());
    };
    
    ~VideoFile(){};
    
public:
    
    virtual void update()
    {
        if( params[ PARAM_NAME_VIDEO_SOURCE ].hasChanged() )
        {
            loadVideoAtIndex( params[ PARAM_NAME_VIDEO_SOURCE ] );
        }
        
        if( params[ PARAM_NAME_ENABLE_PLAYBACK ].hasChanged() )
        {
            if( (bool)params[ PARAM_NAME_ENABLE_PLAYBACK ] )
            {
                videoPlayer.play();
            } else {
                videoPlayer.stop();
            }
        }
        
        if( videoPlayer.isPlaying() )
        {
            params[ PARAM_NAME_VIDEO_FRAME ].set( videoPlayer.getCurrentFrame() );
        } else {
            videoPlayer.setFrame( params[ PARAM_NAME_VIDEO_FRAME ] );
        }
        
        videoPlayer.update();
        
        originalFrameImage.setFromPixels( videoPlayer.getPixelsRef() );

        bool doFlipX = params[PARAM_NAME_FLIP_X];
        bool doFlipY = params[PARAM_NAME_FLIP_Y];
        
        if( doFlipX || doFlipY )
        {
            originalFrameImage.mirror( doFlipY, doFlipX  );
        }
        
        float w = ofClamp( (float)params[ PARAM_NAME_ROI_X2 ] - (float)params[ PARAM_NAME_ROI_X1 ], 0.0f, 1.0f );
        float h = ofClamp( (float)params[ PARAM_NAME_ROI_Y2 ] - (float)params[ PARAM_NAME_ROI_Y1 ], 0.0f, 1.0f );
        
        if( params[ PARAM_NAME_ROI_X1 ].hasChanged() ||
            params[ PARAM_NAME_ROI_Y1 ].hasChanged() ||
            params[ PARAM_NAME_ROI_X2 ].hasChanged() ||
            params[ PARAM_NAME_ROI_Y2 ].hasChanged()
           )
        {
            roiImage.allocate(  w * originalFrameImage.getWidth(),
                                h * originalFrameImage.getHeight(),
                                originalFrameImage.getPixelsRef().getImageType()
                              );
        }
        
        roiImage.cropFrom(  originalFrameImage,
                            (float)params[ PARAM_NAME_ROI_X1 ] * originalFrameImage.getWidth(),
                            (float)params[ PARAM_NAME_ROI_Y1 ] * originalFrameImage.getHeight(),
                            w * originalFrameImage.getWidth(),
                            h * originalFrameImage.getHeight()
                          );
    };
    
    virtual bool isFrameNew()
    {
        return videoPlayer.isFrameNew();
    };
    
    virtual const std::string getPanelName() const { return "Video/File"; };
    
    virtual const ofVec2f getPanelSize() const
    {
        return ofVec2f( videoWidth, videoHeight );
    };
    
private:
    
    void updateVideoFiles()
    {
        ofDirectory videoDirectory;
        
        videoDirectory.listDir( VIDEO_FOLDER );
        
        videoFileNames.clear();
        
        for( int i = 0; i < videoDirectory.getFiles().size(); i++ )
        {
            std::string fileName    = videoDirectory.getName( i );
            
            if( fileName.length() > 17 )
            {
                fileName            = fileName.substr( 0, 17 );
                fileName            = fileName + "...";
            }
            
            videoFileNames.push_back( fileName );
            
            videoFileNamePathMap[ fileName ] = videoDirectory.getPath( i );
        }
    };
    
    void loadVideoAtIndex( int index )
    {
        videoPlayer.loadMovie( videoFileNamePathMap[ videoFileNames[index] ] );
        videoPlayer.setLoopState( OF_LOOP_NORMAL );
        videoPlayer.setVolume( 0 );
        
        videoWidth  = videoPlayer.getWidth();
        videoHeight = videoPlayer.getHeight();
    };
    
private:
    
    std::vector<std::string>            videoFileNames;
    std::map<std::string, std::string>  videoFileNamePathMap;
    ofVideoPlayer                       videoPlayer;
    
    int                                 videoWidth;
    int                                 videoHeight;
    
};