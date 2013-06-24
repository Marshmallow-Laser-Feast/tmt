//
//  NoiseDistortionFilter.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/12/13.
//
//

#pragma once

#include "ofMain.h"

#include "IFilter.h"

#define PARAM_NAME_NOISE_SCALE              "Scale"
#define PARAM_NAME_NOISE_OCS_AMOUNT         "OCS Amount"
#define PARAM_NAME_NOISE_OCS_AMOUNT_AUDIO   "OCS Amount Audio"
#define PARAM_NAME_NOISE_OFFSET             "Offset"
#define PARAM_NAME_NOISE_AMP_X              "Amplitude X"
#define PARAM_NAME_NOISE_AMP_Y              "Amplitude Y"
#define PARAM_NAME_USE_NORMALS              "Use Normals"

class NoiseDistortionFilter: public IFilter
{
    
public:
    
    NoiseDistortionFilter()
    
    :IFilter( "NoiseDistortionFilter" )
    
    {
        
        params.addFloat(PARAM_NAME_NOISE_SCALE).setRange( 0.0f, 10).setClamp( true );
//        params.addFloat(PARAM_NAME_NOISE_OCS_AMOUNT).setRange( 0.0f, 10).setClamp( true );
        params.addFloat(PARAM_NAME_NOISE_OCS_AMOUNT_AUDIO).setClamp( true );
        params.addFloat(PARAM_NAME_NOISE_OFFSET).setRange(-10, 10).setClamp( true );
        params.addFloat(PARAM_NAME_NOISE_AMP_X).setRange(-2, 2).setClamp( true );
        params.addFloat(PARAM_NAME_NOISE_AMP_Y).setRange(-2, 2).setClamp( true );
       
        params.addBool(PARAM_NAME_USE_NORMALS);
        
        oscMappings[ &params.get(PARAM_NAME_ACTIVITY_VALUE) ]         = "/NoiseDistortionFilter/Amount";
        oscMappings[ &params.get(PARAM_NAME_NOISE_OCS_AMOUNT_AUDIO) ]   = "/NoiseDistortionFilter/AmountAudio";

    };
    
    virtual ~NoiseDistortionFilter()
    {};
    
public:
    
    virtual void apply( std::vector<ofPolyline> &polylines, float audioAmp )
    {
        float actv              = (float)params[PARAM_NAME_ACTIVITY_VALUE];
        float scale             = (float)params[PARAM_NAME_NOISE_SCALE];
        float offset            = (float)params[PARAM_NAME_NOISE_OFFSET];
        float ampx              = (float)params[PARAM_NAME_NOISE_AMP_X];
        float ampy              = (float)params[PARAM_NAME_NOISE_AMP_Y];
        
//        float ocsAmount         = (float)params[PARAM_NAME_NOISE_OCS_AMOUNT];
        float ocsAudioAmount    = (float)params[PARAM_NAME_NOISE_OCS_AMOUNT_AUDIO];
        
        float filteredActv      = actv;
        
        if( ocsAudioAmount > 0.0f )
        {
            filteredActv        = ofLerp( actv * ( 1 - ocsAudioAmount ), actv, audioAmp );
        }
        
        if( filteredActv == 0.0f )
        {
            return;
        }
        
        ofPoint amp( ampx, ampy );
        
        if( (bool)params[ PARAM_NAME_USE_NORMALS ] )
        {
            for( std::vector<ofPolyline>::iterator it = polylines.begin(); it != polylines.end(); ++it )
            {
                for( int i = 0; i < it->getVertices().size(); ++i )
                {
                    it->getVertices()[i]    =   it->getVertices()[i] +
                                                ofSignedNoise(      offset + it->getVertices()[i].x * scale,
                                                                    offset + it->getVertices()[i].y * scale )*
                                                it->getNormalAtIndex( i ) *
                                                amp *
                                                filteredActv;
                }
            }
        } else {
            for( std::vector<ofPolyline>::iterator it = polylines.begin(); it != polylines.end(); ++it )
            {
                for( int i = 0; i < it->getVertices().size(); ++i )
                {
                    ofVec2f n;
                    n.x = ofSignedNoise(it->getVertices()[i].x * scale, it->getVertices()[i].y * scale, offset + 134.523441);
                    n.y = ofSignedNoise(it->getVertices()[i].x * scale, it->getVertices()[i].y * scale, offset + 513.5622);

                    //                    ofVec3f n( 1, 0, 1 );
//                    n.normalize();
//                    n.rotate( ofSignedNoise( offset + it->getVertices()[i].x * scale, offset + it->getVertices()[i].y * scale ) * 360.0f , ofVec3f( 0, 0, 1 ) );
                    
                    it->getVertices()[i]    =   it->getVertices()[i] +
                                                n *
                                                amp *
                                                filteredActv;
                }
            }
        }
    };
    
};