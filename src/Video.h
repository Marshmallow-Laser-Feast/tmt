//
//  Video.h
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

#include "IPanelDraws.h"

#define PARAM_NAME_ROI_X1   "ROI X1"
#define PARAM_NAME_ROI_Y1   "ROI Y1"
#define PARAM_NAME_ROI_X2   "ROI X2"
#define PARAM_NAME_ROI_Y2   "ROI Y2"

class Video: public IControlFreakMapper, public IControlFreakMapperMidiExt, public IControlFreakMapperOSCExt, public IPanelDraws
{
    
public:
    
    Video( const std::string &name )
    
    :IControlFreakMapper( name )
    
    {
        IControlFreakMapperMidiExt::setParams( params );
        IControlFreakMapperOSCExt::setParams( params );
        
        setupMidi();
        setupOCS();
    };
    
    ~Video(){};
    
public:
    
    virtual void update()
    {};
    
    virtual bool isFrameNew()
    {
        return false;
    };
    
    ofImage & getOriginalFrameImage()
    {
        return originalFrameImage;
    };
    
    ofImage & getRoiImage()
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
        
        ofSetColor( 100 );
        
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
    
    virtual const std::string getPanelName() const { return "Empty Video"; };
    
    virtual void draw( float width, float height )
    {        
        float scale = 0.0f;
        
        if( width / originalFrameImage.getWidth() > height / originalFrameImage.getHeight() )
        {
            scale   = height / originalFrameImage.getHeight();
        } else {
            scale   = width / originalFrameImage.getWidth();
        }
        
        ofPushMatrix();
        
        ofScale(scale, scale);
        
        drawROIImage( 0, 0 );
        
        ofPopMatrix();
    };
    
protected:
    
    ofImage     originalFrameImage;
    ofImage     roiImage;
    
};