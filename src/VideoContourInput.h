//
//  VideoContourInput.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/19/13.
//
//

#pragma once

#include "IVideoInput.h"

#include "ofxCv.h"

#include "ContourFinder2.h"

#define PARAM_NAME_THRESHOLD        "Threshold"
#define PARAM_NAME_BLUR             "Blur"
#define PARAM_NAME_DILATE           "Dilate"
#define PARAM_NAME_ERODE            "Erode"
#define PARAM_NAME_MIN_AREA         "Min Area"
#define PARAM_NAME_MAX_AREA         "Max Area"
#define PARAM_NAME_RESAMPLING       "Resampling"
#define PARAM_NAME_SMOOTHING        "Smoothing"
#define PARAM_NAME_SIMPLIFICATION   "Simplification"

class VideoContourInput: public IVideoInput
{
    
public:
    
    VideoContourInput()
    
    :IVideoInput( "Input/VideoContour", Input::SAMPLING_TYPE_POLYLINE )
    
    {
        params.addInt( PARAM_NAME_THRESHOLD ).setRange(0, 255).setClamp(true);
        params.addInt( PARAM_NAME_BLUR ).setRange(0, 50).setClamp(true);
        params.addInt( PARAM_NAME_DILATE ).setRange(0, 50).setClamp(true);;
        params.addInt( PARAM_NAME_ERODE ).setRange(0, 50).setClamp(true);;
        params.addInt( PARAM_NAME_MIN_AREA ).setRange(0, 100).setClamp(true);;
        params.addInt( PARAM_NAME_MAX_AREA ).setRange(0, 10000).setClamp(true);;
        params.addInt(PARAM_NAME_RESAMPLING).setClamp(true).setRange(0, 1000);
        params.addInt( PARAM_NAME_SMOOTHING ).setRange(0, 40).setClamp( true );
        params.addFloat( PARAM_NAME_SIMPLIFICATION ).setRange( 0.0f, 50.0f ).setClamp( true ).setIncrement( 0.01f );
    };
    
    ~VideoContourInput(){};
    
protected:
    
    virtual void processPointSamples(){};
    
    virtual void processPolylineSamples()
    {
        if( isCurrentFrameNew )
        {
            int     resampleCount   = params[PARAM_NAME_RESAMPLING];
            int     smoothAmount    = params[PARAM_NAME_SMOOTHING];
            float   simplify        = params[PARAM_NAME_SIMPLIFICATION];
            
            polylineSamples.clear();
            
            contourFinder.setThreshold(params[PARAM_NAME_THRESHOLD]);
            contourFinder.setBlur(params[PARAM_NAME_BLUR]);
            contourFinder.setDilate(params[PARAM_NAME_DILATE]);
            contourFinder.setErode(params[PARAM_NAME_ERODE]);
            contourFinder.setMinAreaRadius(params[PARAM_NAME_MIN_AREA]);
            contourFinder.setMaxAreaRadius(params[PARAM_NAME_MAX_AREA]);
            
            contourFinder.findContours( *image );
            
            std::vector<cv::Rect>   bboxes;
            
            for( int i = 0; i < contourFinder.size(); ++i )
            {
                bboxes.push_back( contourFinder.getBoundingRect(i) );
            }
            
            vector<unsigned int>    labels  = rectTracker.track( bboxes );
            
            for( int i = 0; i < labels.size(); ++i )
            {
                ofPolyline polyline = ofxCv::toOf( contourFinder.getContour(i) );
                
                if(resampleCount)
                {
                    polyline = polyline.getResampledByCount(resampleCount);
                }
                
                if(smoothAmount)
                {
                    polyline = polyline.getSmoothed(smoothAmount);
                }
                
                if(simplify > 0)
                {
                    polyline.simplify( simplify );
                }
                
                PolylineSampleT sample;
                
                sample.setSample( polyline );
                sample.setSampleID( labels[i] );
            }
        }
    };
    
private:
    
    ofxCv::RectTracker      rectTracker;
    ofxCv::ContourFinder2   contourFinder;
    
};