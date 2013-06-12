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
        params.addInt( PARAM_NAME_CONTOUR_MIN_CONTOUR ).setRange(0, 100).setClamp(true);;
        params.addInt( PARAM_NAME_CONTOUR_MAX_CONTOUR ).setRange(0, 10000).setClamp(true);;
        params.addFloat( PARAM_NAME_CONTOUR_SIMPLIFY ).setRange( 0.0f, 50.0f ).setClamp( true ).setIncrement( 0.01f );
        params.addFloat(PARAM_NAME_CONTOUR_STRETCH).setClamp(true);
        //        params.addFloat(PARAM_NAME_SMOOTHING).setClamp(true);
        params.addFloat(PARAM_NAME_TRACKING_DISTANCE).setClamp(true).setRange(0, 640);
        params.addInt(PARAM_NAME_TRACKING_PERSISTENCE).setClamp(true).setRange(0, 100);
        params.addInt(PARAM_NAME_RESAMPLE_COUNT).setClamp(true).setRange(0, 1000);
        params.addFloat(PARAM_NAME_TIP_THRESHOLD).setRange(-1, 1).setClamp(true);
        params.addBool("Do Convex Hull");
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
            contourFinder.setMinAreaRadius(params[PARAM_NAME_CONTOUR_MIN_CONTOUR]);
            contourFinder.setMaxAreaRadius(params[PARAM_NAME_CONTOUR_MAX_CONTOUR]);
            float simplify = params[PARAM_NAME_CONTOUR_SIMPLIFY];
            float stretch = params[PARAM_NAME_CONTOUR_STRETCH];
            //            float smoothing = params[PARAM_NAME_SMOOTHING];
            int resampleCount = params[PARAM_NAME_RESAMPLE_COUNT];
            float tipThreshold = params[PARAM_NAME_TIP_THRESHOLD];
            bool doConvexHull = params["Do Convex Hull"];
            
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
                cv::Rect r(contourFinder.getBoundingRect(i));
                
                vector<cv::Point> contour = doConvexHull ? contourFinder.getConvexHull( i ) : contourFinder.getContour( i );
                
                for ( int j = 0; j < contour.size(); ++j )
                {
                    ofPoint p;
                    p.set(contour[j].x, contour[j].y );
                    //                    p += ofPoint( roiX1 * width, roiY1 * height );
                    if(stretch > 0) {
                        ofVec2f pNorm;
                        pNorm.x = ofMap(p.x, r.x, r.x + r.width, 0, image.width);
                        pNorm.y = ofMap(p.y, r.y, r.y + r.height, 0, image.height);
                        p.interpolate(pNorm, stretch);
                    }
                    line.lineTo(p);
                    
                }
                
                line.close();
                
                if(resampleCount) line = line.getResampledByCount(resampleCount);
                if(simplify > 0) line.simplify( simplify );
                
                for ( int k = 0; k < line.getVertices().size(); ++k )
                {
                    allFloatPoints.push_back( cv::Point2f( line.getVertices()[k].x, line.getVertices()[k].y ) );
                }
            }
            
            pointTracker.setMaximumDistance(params[PARAM_NAME_TRACKING_DISTANCE]);
            pointTracker.setPersistence(params[PARAM_NAME_TRACKING_PERSISTENCE]);
            pointTracker.track( allFloatPoints );
            
            for( int i  = 0; i < allFloatPoints.size(); ++i )
            {
                PointInputSampleT   sample;
                int label = pointTracker.getLabelFromIndex(i);
                sample.setSampleID(label);
                
                ofPoint p(ofxCv::toOf( allFloatPoints[i] ) );
                //                p-= ofPoint( roiX1 * width, roiY1 * height );
                sample.setSample(p);
                //                sample.setSample( ofPoint( ofxCv::toOf( allConvexHullFloatPoints[i] ) ) + ofPoint( roiX1 * width, roiY1 * height ) );
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