//
//  CameraCentroidsInputs.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/10/13.
//
//

#pragma once

#include "ofMain.h"

#include "ofxCv.h"

#include "ContourFinder2.h"

#include "IImageSeqInput.h"

class CameraCentroidsInputs : public IImageSeqInput
{
    
public:
    
    CameraCentroidsInputs()
    {
        params.setName("CameraCentroidsInputs");
        params.addInt( PARAM_NAME_CONTOUR_THRESHOLD ).setRange(0, 255).setClamp(true);
        params.addInt( PARAM_NAME_CONTOUR_BLUR ).setRange(0, 50).setClamp(true);
        params.addInt( PARAM_NAME_CONTOUR_DILATE ).setRange(0, 50).setClamp(true);;
        params.addInt( PARAM_NAME_CONTOUR_MIN_CONTOUR ).setRange(0, 100).setClamp(true);;
        params.addInt( PARAM_NAME_CONTOUR_MAX_CONTOUR ).setRange(0, 10000).setClamp(true);;
    };
    
    ~CameraCentroidsInputs()
    {}
    
public:
    virtual void init()
    {};
    
    virtual void update()
    {
        if( isCurrentFrameNew )
        {
            contourFinder.setBlur(params[PARAM_NAME_CONTOUR_BLUR]);
            contourFinder.setThreshold(params[PARAM_NAME_CONTOUR_THRESHOLD]);
            contourFinder.setDilate(params[PARAM_NAME_CONTOUR_DILATE]);
            contourFinder.setMinAreaRadius(params[PARAM_NAME_CONTOUR_MIN_CONTOUR]);
            contourFinder.setMaxAreaRadius(params[PARAM_NAME_CONTOUR_MAX_CONTOUR]);
            
            samples.clear();
            
            ofImage image;
            
            float w = roiX2 - roiX1;
            float h = roiY2 - roiY1;
            
            image.setFromPixels( *pixels );
            
            image.crop( roiX1 * width , roiY1 * height, w * width, h * height );
            
            contourFinder.findContours( image );
            
            for( int i = 0; i < contourFinder.size(); ++i )
            {
                PointInputSampleT   sample;
                
                sample.setSampleID( contourFinder.getLabel( i ) );
                
                ofPoint p(ofxCv::toOf( contourFinder.getCentroid( i ) ) );
//                p-= ofPoint( roiX1 * width, roiY1 * height );
                sample.setSample(p);
                
                samples.push_back( sample );
            }
        }
    };
    
    virtual const vector<PointInputSampleT> & getSamples() const
    {
        return samples;
    };
    
    virtual void drawDebug()
    {
        contourFinder.draw();
    };
    
private:
    
    ofxCv::ContourFinder2           contourFinder;
    
    std::vector<PointInputSampleT>  samples;
};