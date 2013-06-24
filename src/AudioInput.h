//
//  AudioInput.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/18/13.
//
//

#pragma once

#define OF_SOUND_PLAYER_FMOD

#include "ofMain.h"

#include "IControlFreakMapper.h"
#include "IControlFreakMapperOSCExt.h"
#include "IControlFreakMapperMidiExt.h"

#include "IPanelDraws.h"

#define AUDIO_FILE_NAME                     "sound/measures-total-test16.aif"

#define AUDIO_FOLDER                        "sound"
        
#define FFT_SAMPLE_COUNT                    128
    
#define PARAM_NAME_AUDIO_SOURCE             "Audio Source"

#define PARAM_NAME_ENABLE_PLAYBACK          "Playback"

#define PARAM_NAME_AUDIO_SCALE              "Scale"
#define PARAM_NAME_AUDIO_SMOOTHING_LOW      "Smoothing Low"
#define PARAM_NAME_AUDIO_SMOOTHING_HIGH     "Smoothing High"

#define PARAM_NAME_AUDIO_AMP                "Amplitude"
#define PARAM_NAME_EXTERNAL_AUDIO           "Ext Audio Amp"
#define PARAM_NAME_EXTERNAL_AUDIO_SCALED    "Scaled Ext Audio Amp"
#define PARAM_NAME_POSITON                  "Position"

class AudioInput: public IControlFreakMapper, public IControlFreakMapperMidiExt, public IControlFreakMapperOSCExt, public IPanelDraws
{
    
public:
    
    AudioInput()
    
    :IControlFreakMapper( "Audio Input" )
    
    {
        IControlFreakMapperMidiExt::setParams( params );
        IControlFreakMapperOSCExt::setParams( params );
        
        setupMidi();
        setupOCS();
        
        updateAudioFiles();
        
        if( audioFileNames.size() > 0 )
        {
            loadAudioAtIndex( 0 );
        }
        
        params.addBool( PARAM_NAME_ENABLE_PLAYBACK ).set( true );
        
        params.addNamedIndex( PARAM_NAME_AUDIO_SOURCE ).setLabels( audioFileNames );
        
        params.addFloat( PARAM_NAME_AUDIO_SCALE ).set( 1.0f );
        params.addFloat( PARAM_NAME_AUDIO_SMOOTHING_LOW ).setClamp( true ).set( 1.0f );
        params.addFloat( PARAM_NAME_AUDIO_SMOOTHING_HIGH ).setClamp( true ).set( 1.0f );
        
        params.addFloat( PARAM_NAME_AUDIO_AMP ).setClamp( true );
        params.addFloat( PARAM_NAME_EXTERNAL_AUDIO ).setClamp( true );
        params.addFloat( PARAM_NAME_EXTERNAL_AUDIO_SCALED ).setClamp( true );
        params.addFloat( PARAM_NAME_POSITON ).setClamp( true );
        
        oscMappings[ &params.get( PARAM_NAME_EXTERNAL_AUDIO ) ]     = "/AudioFFT";
        
        fftData     = new float[ FFT_SAMPLE_COUNT ];
        
        for (int i = 0; i < FFT_SAMPLE_COUNT; i++)
        {
            fftData[i] = 0;
        }
    
    };
    
    ~AudioInput()
    {};
    
public:
    
    void update()
    {
        if( params[ PARAM_NAME_AUDIO_SOURCE ].hasChanged() )
        {
            loadAudioAtIndex( params[ PARAM_NAME_AUDIO_SOURCE ] );
        }
        
        ofSoundUpdate();
        
        fftData         = ofSoundGetSpectrum( FFT_SAMPLE_COUNT );
        
        float newAmp    = 0.0f;
        
        for (int i = 0; i < FFT_SAMPLE_COUNT; i++)
        {
            newAmp      += fftData[ i ];
        }
        
        newAmp          /= (float)FFT_SAMPLE_COUNT;
        
        if( amp < newAmp )
        {
            amp         = ofLerp( amp, newAmp, (float)params[PARAM_NAME_AUDIO_SMOOTHING_HIGH]);
        } else {
            amp         = ofLerp( amp, newAmp, (float)params[PARAM_NAME_AUDIO_SMOOTHING_LOW]);
        }
        
        if( params[ PARAM_NAME_ENABLE_PLAYBACK ].hasChanged() )
        {
            soundPlayer.setPaused( !(bool)params[ PARAM_NAME_ENABLE_PLAYBACK ] );
        }
        
        params[ PARAM_NAME_AUDIO_AMP ].set( amp * (float)params[ PARAM_NAME_AUDIO_SCALE ] );
        params[ PARAM_NAME_EXTERNAL_AUDIO_SCALED ].set( (float)params[ PARAM_NAME_EXTERNAL_AUDIO ] * (float)params[ PARAM_NAME_AUDIO_SCALE ] );
        
        if( (bool)params[ PARAM_NAME_ENABLE_PLAYBACK ] )
        {
            params[ PARAM_NAME_POSITON ].set( soundPlayer.getPosition() );
        } else {
            soundPlayer.setPosition( (float)params[ PARAM_NAME_POSITON ] );
        }
    }
    
    virtual void draw( float width, float height )
    {
        float scale             = (float)params[ PARAM_NAME_AUDIO_SCALE ];
        float cellWidth         = width / (float)FFT_SAMPLE_COUNT;
        
        ofPushStyle();
        
        for (int i = 0; i < FFT_SAMPLE_COUNT; i++)
        {
            float value         = ofClamp(fftData[i] * scale, 0.0f, 1.0f);
            float cellHeight    = value * height;
            float y             = (height - cellHeight) * 0.5f;
            
            ofSetColor( ofColor::beige.getLerped( ofColor::chocolate, value ) );
            
            ofRect( i * cellWidth, y, cellWidth - 1.0f, cellHeight );
        }
        
        ofPopStyle();
    };
    
    virtual const std::string getPanelName() const { return "Audio Input"; };
    
    virtual const ofVec2f getPanelSize() const
    {
        return ofVec2f( 300, 80 );
    };
    
    const int getFFTSampleCount() const
    {
        return FFT_SAMPLE_COUNT;
    }
    
    const float * getFFTData() const
    {
        return fftData;
    }
    
    const float getAmp() const
    {
        return (float)params[ PARAM_NAME_AUDIO_AMP ];
    }
    
    const float getAmpExternal() const
    {
        return (float)params[ PARAM_NAME_EXTERNAL_AUDIO ] * (float)params[ PARAM_NAME_AUDIO_SCALE ];
    }
    
private:
    
    ofSoundPlayer   soundPlayer;
    
    float           *fftData;
    float           amp;
    
private:
    
    void updateAudioFiles()
    {
        ofDirectory audioDirectory;
        
        audioDirectory.listDir( AUDIO_FOLDER );
        
        audioFileNames.clear();
        
        for( int i = 0; i < audioDirectory.getFiles().size(); i++ )
        {
            std::string fileName    = audioDirectory.getName( i );
            
            if( fileName.length() > 17 )
            {
                fileName            = fileName.substr( 0, 17 );
                fileName            = fileName + "...";
            }
            
            audioFileNames.push_back( fileName );
            
            audioFileNamePathMap[ fileName ] = audioDirectory.getPath( i );
        }
    };
    
    void loadAudioAtIndex( int index )
    {
        soundPlayer.loadSound( audioFileNamePathMap[ audioFileNames[index] ] );
        soundPlayer.play();
    }
    
    
private:
    
    std::vector<std::string>            audioFileNames;
    std::map<std::string, std::string>  audioFileNamePathMap;
};
