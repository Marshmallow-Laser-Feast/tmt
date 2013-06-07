//
//  MultiTouchInput.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoğlu on 6/7/13.
//
//

#pragma once

#include "ofMain.h"

#include "ofxMultiTouchPad.h"

#include "Input.h"

class MultiTouchInput : public Input
{

public:
    
    MultiTouchInput( float width_, float height_ )
    {
        width           = width_;
        height          = height_;
        multiTouchPad   = ofxMultiTouchPad();
    };
    
    ~MultiTouchInput()
    {}
    
public:
    
    virtual void update()
    {
        std::vector<MTouch> touches = multiTouchPad.getTouches();
        
        samples.clear();
        
        for (std::vector<MTouch>::iterator it = touches.begin(); it != touches.end(); ++it )
        {
            PointInputSampleT   sample;
            
            sample.setSampleID( it->ID );
            
            sample.setSample( ofPoint( it->x * width, it->y * height, 0.0f ) );
            
            samples.push_back( sample );
        }
    };
    
    virtual const vector<PointInputSampleT> & getSamples() const
    {
        return samples;
    };
    
private:
    
    float                           width;
    float                           height;
    
    ofxMultiTouchPad                multiTouchPad;
    std::vector<PointInputSampleT>  samples;
};