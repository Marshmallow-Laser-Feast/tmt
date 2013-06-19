//
//  VideoInput.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/18/13.
//
//

#pragma once

#include "ofMain.h"

#include "IControlFreakMapper.h"
#include "IControlFreakMapperOSCExt.h"
#include "IControlFreakMapperMidiExt.h"

#define PARAM_NAME_ROI_X1   "ROI X1"
#define PARAM_NAME_ROI_Y1   "ROI Y1"
#define PARAM_NAME_ROI_X2   "ROI X2"
#define PARAM_NAME_ROI_Y2   "ROI Y2"

class VideoInput: public IControlFreakMapper, public IControlFreakMapperMidiExt, public IControlFreakMapperOSCExt
{
    
public:
    
    VideoInput( const std::string &name )
    
    :IControlFreakMapper( name )
    
    {
        IControlFreakMapperMidiExt::setParams( params );
        IControlFreakMapperOSCExt::setParams( params );
        
        setupMidi();
        setupOCS();
    };
    
    ~VideoInput(){};
    
public:
    
    virtual void update()
    {};
    
    virtual bool isFrameNew()
    {
        return false;
    };
    
    const ofImage & getOriginalFrameImage() const
    {
        return originalFrameImage;
    };
    
    const ofImage & getRoiImage() const
    {
        return roiImage;
    };
    
    void drawOriginalFrameImage( float x, float y )
    {
        originalFrameImage.draw( x, y );
    };
    
    void drawROIImage( float x, float y )
    {
        ofPushStyle();
        ofPushMatrix();
        
        ofSetColor( 50 );
        
        drawOriginalFrameImage( x, y );
        
        ofTranslate(    x + (float)params[ PARAM_NAME_ROI_X1 ] * originalFrameImage.getWidth(),
                        y + (float)params[ PARAM_NAME_ROI_Y1 ] * originalFrameImage.getHeight()
                    );
        
        ofNoFill();
        ofSetLineWidth( 2.0f );
        
        ofSetColor( ofColor::red );
        
        ofRect( 0, 0, roiImage.getWidth(), roiImage.getHeight() );
        
        ofFill();
        
        ofSetColor( ofColor::white );
        
        roiImage.draw( 0, 0 );
        
        ofPopMatrix();
        ofPopStyle();
    };
    
protected:
    
    ofImage     originalFrameImage;
    ofImage     roiImage;
    
};