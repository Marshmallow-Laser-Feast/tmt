//
//  FlockingInput.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoğlu on 6/4/13.
//
//

#pragma once

#include "ofMain.h"

#include "Flock2d.h"

#include "Input.h"

class FlockingInput : public Input
{
    
public:
    
    FlockingInput(  int     sampleCount,
                    float   width,
                    float   height,
                    int     columns,
                    int     rows,
                    float   maxSpeed_,
                    float   minForce,
                    float   maxForce,
                    float   attractorRadiusRatio  )
    :maxSpeed( maxSpeed_ )
    ,t( 0.0f )
    
    {
        flock.setup( 0, width / 2, height / 2 );
        flock.setBounds( 0, 0, width, height );
        
        samples.resize( sampleCount );
        
        int sampleIndex     = 0;
        
        for( std::vector<PointInputSampleT>::iterator it = samples.begin(); it != samples.end(); ++it )
        {
            it->setSampleID( sampleIndex );
            
            sampleIndex++;
        }
        
        for( int i = 0; i < sampleCount; i++)
        {
            flock.add( ofRandom(width), ofRandom(height) );
            flock.get( i )->setVel( ofRandom( -20, 20 ), ofRandom( -20, 20 ) );
        }
        
        float columnSize    = width / (float)columns;
        float rowSize       = height / (float)rows;
        float radius        = columnSize > rowSize ? rowSize * 0.5f : columnSize * 0.5f;
        
        for( int i = 1; i < columns - 1; ++i )
        {
            for( int j = 1; j < rows - 1; ++j )
            {
                flock.addAttractionPoint( columnSize * (float)i, rowSize * (float)j, ofRandom( minForce, maxForce ), radius * attractorRadiusRatio );
            }
        }
    };
    
    ~FlockingInput()
    {};
    
public:
    
    virtual void update()
    {
        t   += 2.0f;
        
        flock.setMaxSpeed( maxSpeed * ofNoise( t ) );
        
        flock.update();
        
        int boidIndex   = 0;
        
        for( std::vector<PointInputSampleT>::iterator it = samples.begin(); it != samples.end(); ++it )
        {
            Boid2d * boid   = flock.get( boidIndex );
            
            it->setSample( ofPoint( boid->x, boid->y, 0.0f ) );
            
            boidIndex++;
        }
    };
    
    virtual const vector<PointInputSampleT> & getSamples() const
    {
        return samples;
    };
    
private:
    
    Flock2d                         flock;
    
    float                           maxSpeed;
    float                           t;
    
    std::vector<PointInputSampleT>  samples;
    
};
