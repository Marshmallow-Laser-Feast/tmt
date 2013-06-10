//
//  QualitiesVisualizer.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/10/13.
//
//

#pragma once

#include "IVisualizer.h"

class QualitiesVisualizer : public IVisualizer
{
    
public:
    
    QualitiesVisualizer()
    
    :uSamples( 0 )
    ,ucount( 0 )
    ,vcount( 0 )
    ,width( 0.0f)
    ,height( 0.0f )
    ,noiseHeight( 0.0f )
    ,noiseOffset( 0.0f )
    
    {};
    
    ~QualitiesVisualizer()
    {};
    
public:
    
    void setUSamples( int uSamples_ )
    {
        uSamples              = uSamples_;
    };
    
    void setUCount( int ucount_ )
    {
        ucount              = ucount_;
    };
    
    void setVCount( int vcount_ )
    {
        vcount              = vcount_;
    };
    
    void setSize( float width_, float height_ )
    {
        width               = width_;
        height              = height_;
    };
    
    void setNoiseHeight( float noiseHeight_ )
    {
        noiseHeight          = noiseHeight_;
    };
    
    void setNoiseOffset( float noiseOffset_ )
    {
        noiseOffset         = noiseOffset_;
    }
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale )
    {
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        
        
        
        return result;
    };
    
private:
    
    int     uSamples;
    int     ucount;
    int     vcount;
    
    float   width;
    float   height;
    float   noiseHeight;
    float   noiseOffset;
    
};
