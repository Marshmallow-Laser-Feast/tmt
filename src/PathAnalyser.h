//
//  PathAnalyser.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoğlu on 6/2/13.
//
//

#pragma once

#include <deque>

#include "ofMain.h"

class PathAnalyser
{
    
public:
    
    PathAnalyser()
    
    :length( 0.0f )
    ,amplitude( 0.0f )
    ,curvature( 0.0f )
    ,acceleration( 0.0f )
    ,velocity( 0.0f )
    ,maxLength( 0.0f )
    ,maxSamples( 0 )
    ,maxHistory( 0 )
    
    {
    };
    
    
    ~PathAnalyser()
    {
    };
    
    void setMaxSamples( int value )
    {
        maxSamples              = value;
    }
    
    void setMaxHistory( int value )
    {
        maxHistory              = value;
    }
    
    void setMaxLength( float value )
    {
        maxLength               = value;
    }
    
    void addSample( const ofPoint &sample )
    {
        samples.push_back( sample );
        times.push_back( ofGetElapsedTimef() );
        
        updateAnalysis();
    };
    
    void addSample( float x, float y, float z )
    {
        samples.push_back( ofPoint( x, y, z ) );
        times.push_back( ofGetElapsedTimef() );
        
        updateAnalysis();
    };
    
    void update()
    {
        while( samples.size() > maxSamples )
        {
            samples.pop_front();
            
            times.pop_front();
        }
        
        while( lengths.size() > maxHistory )
        {
            lengths.pop_front();
            amplitudes.pop_front();
            curvatures.pop_front();
            accelerations.pop_front();
            velocities.pop_front();
        }
    }
    
    void clear()
    {
        samples.clear();
        times.clear();
        
        updateAnalysis();
    };
    
    void clearHistory()
    {
        lengths.clear();
        amplitudes.clear();
        curvatures.clear();
        accelerations.clear();
        velocities.clear();
    }
    
    const deque<ofPoint> & getSamples() const
    {
        return samples;
    }
    
    const deque<float> & getLengths() const
    {
        return lengths;
    }
    
    const deque<float> & getAmplitudes() const
    {
        return amplitudes;
    }
    
    const deque<float> & getCurvatures() const
    {
        return curvatures;
    }
    
    const deque<float> & getAccelerations() const
    {
        return accelerations;
    }
    
    const deque<float> & getVelocities() const
    {
        return velocities;
    }
    
    const float getLength() const
    {
        return length;
    }
    
    const float getAmplitude() const
    {
        return amplitude;
    };
    
    const float getCurvature() const
    {
        return curvature;
    };
    
    const float getAcceleration() const
    {
        return acceleration;
    };
    
    const float getVelocity() const
    {
        return velocity;
    };
    
    void draw()
    {
        if( samples.size() > 1 )
        {
            for( int i = 1; i < samples.size(); ++i )
            {
                ofPoint prevPoint       = samples[ i - 1 ];
                ofPoint currentPoint    = samples[ i ];
                
                ofLine( prevPoint, currentPoint );
            }
        }
    };
    
    void drawSamples( float circleSize )
    {
        for ( deque<ofPoint>::iterator it = samples.begin(); it != samples.end(); ++it )
        {
            ofCircle( *it, circleSize );
        }
    };
    
    void drawCurvature( float lineLength )
    {
        if( samples.size() > 1 )
        {            
            for( int i = 1; i < samples.size() - 1; ++i )
            {
                float   rot             = ofClamp(abs(getSampleRotation( i ).z) / 0.15f, 0.0f, 1.0f);
            
                ofLine( samples[ i ], samples[ i ] + getSampleNormal( i ).rotated(0, 0, 90.0f) * rot * lineLength );
            }
        }
    };
    
    void drawAcceleration( float lineLength )
    {
        if( samples.size() > 1 )
        {
            float avgDistance           = length / ((float)samples.size() - 1);
            
            for( int i = 1; i < samples.size() - 1; ++i )
            {
                ofPoint prevPoint       = samples[ i - 1 ];
                ofPoint currentPoint    = samples[ i ];
                
                float distance          = currentPoint.distance( prevPoint );
                
                ofLine( currentPoint, currentPoint + getSampleNormal( i ).rotated(0, 0, 270.0f) * (distance / avgDistance) * lineLength );
            }
        }
    };
    
private:
    
    ofPoint getSampleRotation( int index )
    {
        ofPoint p1( samples[ MAX( index - 1, 0 ) ] );
        ofPoint p2( samples[ index ] );
        ofPoint p3( samples[ MIN( index + 1, samples.size() - 1 ) ] );
        
        ofVec3f v1(p1 - p2);
        ofVec3f v2(p3 - p2);
        
        v1.normalize();
        v2.normalize();
        
        return v1.crossed(v2);
    };
    
    ofPoint getSampleNormal( int index )
    {
        ofPoint p1( samples[ MAX( index - 1, 0 ) ] );
        ofPoint p2( samples[ index ] );
        ofPoint p3( samples[ MIN( index + 1, samples.size() - 1 ) ] );
        
        ofVec3f v1(p1 - p2);
        ofVec3f v2(p3 - p2);
        
        v1.normalize();
        v2.normalize();
        
        ofVec3f tangent(v2 - v1);
        tangent.normalize();
        
        return tangent.normalized();
    };
    
    void updateAnalysis()
    {
        length                          = 0.0f;
        amplitude                       = 0.0f;
        curvature                       = 0.0f;
        acceleration                    = 0.0f;
        velocity                        = 0.0f;
        
        if( samples.size() > 1 )
        {
            ofPoint prevPoint;
            ofPoint currentPoint;
        
            for( int i = 1; i < samples.size(); ++i )
            {
                prevPoint.set( samples[ i - 1 ] );
                currentPoint.set( samples[ i ] );
                
                length                  += prevPoint.distance( currentPoint );
            }
            
            int   sampleCount           = 0;
            float avgDistance           = length / (float)(samples.size() - 1);
            
            for( int i = 1; i < samples.size() - 1; ++i )
            {
                prevPoint.set( samples[ i - 1 ] );
                currentPoint.set( samples[ i ] );
                
                curvature               += ofClamp(abs(getSampleRotation( i ).z) / 0.15f, 0.0f, 1.0f);
                acceleration            += prevPoint.distance( currentPoint ) / avgDistance;
                
                sampleCount++;
            }
            
            amplitude                   = length / maxLength;
            curvature                   /= (float)sampleCount;
            acceleration                /= (float)sampleCount;
            velocity                    = length / ( times.back() - times.front() );
        }

        lengths.push_back( length );
        amplitudes.push_back( amplitude );
        curvatures.push_back( curvature );
        accelerations.push_back( acceleration );
        velocities.push_back( velocity );
        
    };
    
private:
    
    float           maxLength;
    
    int             maxSamples;
    int             maxHistory;
    
    float           length;
    float           amplitude;
    float           curvature;
    float           acceleration;
    float           velocity;
    
    deque<ofPoint>  samples;
    
    deque<float>    lengths;
    deque<float>    amplitudes;
    deque<float>    curvatures;
    deque<float>    accelerations;
    deque<float>    velocities;
    
    deque<float>    times;
    
};
