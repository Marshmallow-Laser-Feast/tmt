//
//  CameraContourInput.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/11/13.
//
//

#pragma once

#include "ofMain.h"

#include "ofxCv.h"

#include "ContourFinder2.h"

#include "IImageSeqInput.h"

class CameraContourInput : public IImageSeqInput
{
    
public:
    
    CameraContourInput()
    {
        params.setName("CameraContourInput");
        params.addInt( PARAM_NAME_CONTOUR_THRESHOLD ).setRange(0, 255).setClamp(true);
        params.addInt( PARAM_NAME_CONTOUR_BLUR ).setRange(0, 50).setClamp(true);
        params.addInt( PARAM_NAME_CONTOUR_DILATE ).setRange(0, 50).setClamp(true);;
        params.addInt( PARAM_NAME_CONTOUR_ERODE ).setRange(0, 50).setClamp(true);;
        params.addInt( PARAM_NAME_CONTOUR_MIN_CONTOUR ).setRange(0, 100).setClamp(true);;
        params.addInt( PARAM_NAME_CONTOUR_MAX_CONTOUR ).setRange(0, 10000).setClamp(true);;
        params.addFloat( PARAM_NAME_CONTOUR_SIMPLIFY ).setRange( 0.0f, 50.0f ).setClamp( true ).setIncrement( 0.01f );
    };
    
    ~CameraContourInput()
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
            contourFinder.setErode(params[PARAM_NAME_CONTOUR_ERODE]);
            contourFinder.setMinAreaRadius(params[PARAM_NAME_CONTOUR_MIN_CONTOUR]);
            contourFinder.setMaxAreaRadius(params[PARAM_NAME_CONTOUR_MAX_CONTOUR]);
            float simplify = params[PARAM_NAME_CONTOUR_SIMPLIFY];
            
            samples.clear();
            
            ofImage image;
            
            float w = roiX2 - roiX1;
            float h = roiY2 - roiY1;
            
            image.setFromPixels( *pixels );
            
            image.crop( roiX1 * width , roiY1 * height, w * width, h * height );
            
            contourFinder.findContours( image );
            
            ofPolyline              line;
            
            vector<cv::Point2f>     allFloatPoints;
            
            for( int i = 0; i < contourFinder.size(); ++i )
            {
                line.clear();
                
                for ( int j = 0; j < contourFinder.getContour( i ).size(); ++j )
                {
                    line.lineTo( ofPoint( contourFinder.getContour( i )[j].x, contourFinder.getContour( i )[j].y ) + ofPoint( roiX1 * width, roiY1 * height ) );
                }
                
                line.close();
                line.simplify( simplify );
                
                for ( int k = 0; k < line.getVertices().size(); ++k )
                {
                    allFloatPoints.push_back( cv::Point2f( line.getVertices()[k].x, line.getVertices()[k].y ) );
                }
            }
            
            pointTracker.track( allFloatPoints );
            
            for( int i  = 0; i < allFloatPoints.size(); ++i )
            {
                PointInputSampleT   sample;
                
                sample.setSampleID( pointTracker.getLabelFromIndex( i ) );
                
                
                ofPoint p(ofxCv::toOf( allFloatPoints[i] ) );
                p-= ofPoint( roiX1 * width, roiY1 * height );
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
        ofPolyline              line;
        float simplify = params[PARAM_NAME_CONTOUR_SIMPLIFY];
        for( int i = 0; i < contourFinder.size(); ++i )
        {
            line.clear();
            
            for ( int j = 0; j < contourFinder.getContour( i ).size(); ++j )
            {
                line.lineTo( ofPoint( contourFinder.getContour( i )[j].x, contourFinder.getContour( i )[j].y ) + ofPoint( roiX1 * width, roiY1 * height ) );
            }
            
            line.close();
            line.simplify( simplify );
            line.draw();
        }
    };
    
private:
    
    ofxCv::ContourFinder2           contourFinder;
    ofxCv::PointTracker             pointTracker;
    
    std::vector<PointInputSampleT>  samples;
};