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

#define PARAM_NAME_NOISE_SCALE  "Scale"
#define PARAM_NAME_NOISE_OFFSET "Offset"
#define PARAM_NAME_NOISE_AMP_X  "Amplitude X"
#define PARAM_NAME_NOISE_AMP_Y  "Amplitude Y"
#define PARAM_NAME_USE_NORMALS  "Use Normals"

class NoiseDistortionFilter: public IFilter
{
    
public:
    
    NoiseDistortionFilter()
    {
        params.setName("NoiseDistortionFilter");
        
        params.addFloat(PARAM_NAME_NOISE_SCALE).setRange( 0.0f, 10).setClamp( true );
        params.addFloat(PARAM_NAME_NOISE_OFFSET).setRange(-10, 10).setClamp( true );
        params.addFloat(PARAM_NAME_NOISE_AMP_X).setRange(-2, 2).setClamp( true );
        params.addFloat(PARAM_NAME_NOISE_AMP_Y).setRange(-2, 2).setClamp( true );
        params.addBool(PARAM_NAME_USE_NORMALS);
    };
    
    virtual ~NoiseDistortionFilter()
    {};
    
public:
    
    virtual void apply( std::vector<ofPolyline> &polylines )
    {
        float actv      = (float)params[PARAM_NAME_ACTIVITY_VALUE];
        float scale     = (float)params[PARAM_NAME_NOISE_SCALE];
        float offset    = (float)params[PARAM_NAME_NOISE_OFFSET];
        float ampx      = (float)params[PARAM_NAME_NOISE_AMP_X];
        float ampy      = (float)params[PARAM_NAME_NOISE_AMP_Y];
        
        if( actv == 0.0f )
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
                                                actv;
                }
            }
        } else {
            for( std::vector<ofPolyline>::iterator it = polylines.begin(); it != polylines.end(); ++it )
            {
                for( int i = 0; i < it->getVertices().size(); ++i )
                {
                    ofVec3f n( 1, 0, 1 );
                    
                    n.normalize();
                    n.rotate( ofSignedNoise( offset + it->getVertices()[i].x * scale, offset + it->getVertices()[i].y * scale ) * 360.0f , ofVec3f( 0, 0, 1 ) );
                    
                    it->getVertices()[i]    =   it->getVertices()[i] +
                                                n *
                                                amp *
                                                actv;
                }
            }
        }
    };
    
};