//
//  CameraConvexHullInput.h
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

class CameraConvexHullInput : public IImageSeqInput
{
    
public:
    
    CameraConvexHullInput()
    {
        params.setName("CameraConvexHullInput");
        params.addInt( PARAM_NAME_CONTOUR_THRESHOLD ).setRange(0, 255).setClamp(true);
        params.addInt( PARAM_NAME_CONTOUR_BLUR ).setRange(0, 50).setClamp(true);
        params.addInt( PARAM_NAME_CONTOUR_DILATE ).setRange(0, 50).setClamp(true);;
        params.addInt( PARAM_NAME_CONTOUR_MIN_CONTOUR ).setRange(0, 100).setClamp(true);;
        params.addInt( PARAM_NAME_CONTOUR_MAX_CONTOUR ).setRange(0, 10000).setClamp(true);;
        params.addFloat( PARAM_NAME_CONTOUR_SIMPLIFY ).setRange( 0.0f, 50.0f ).setClamp( true ).setIncrement( 0.01f );
        params.addFloat(PARAM_NAME_CONTOUR_STRETCH).setClamp(true);
        params.addFloat(PARAM_NAME_SMOOTHING).setClamp(true);
        params.addFloat(PARAM_NAME_TRACKING_DISTANCE).setClamp(true).setRange(0, 640);
        params.addInt(PARAM_NAME_TRACKING_PERSISTENCE).setClamp(true).setRange(0, 100);
        params.addInt(PARAM_NAME_RESAMPLE_COUNT).setClamp(true).setRange(0, 1000);
    };
    
    ~CameraConvexHullInput()
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
            float smoothing = params[PARAM_NAME_SMOOTHING];
            int resampleCount = params[PARAM_NAME_RESAMPLE_COUNT];
            
            samples.clear();
            
            ofImage image;
            
            float w = roiX2 - roiX1;
            float h = roiY2 - roiY1;
            
            image.setFromPixels( *pixels );
            
            image.crop( roiX1 * width , roiY1 * height, w * width, h * height );
            
            contourFinder.findContours( image );
            
            vector<cv::Point>       allConvexHullPoints;
            vector<cv::Point2f>     allConvexHullFloatPoints;
            
            ofPolyline              line;
            
            for( int i = 0; i < contourFinder.size(); ++i )
            {
                vector<cv::Point> convexHullPoints  = contourFinder.getConvexHull( i );
                
                line.clear();
                cv::Rect r(contourFinder.getBoundingRect(i));
                
                for( vector<cv::Point>::iterator it = convexHullPoints.begin(); it != convexHullPoints.end(); ++it )
                {
                    ofVec2f p( ofxCv::toOf( *it ));
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
                
                for( int j = 0; j < line.getVertices().size(); ++j )
                {
                    allConvexHullPoints.push_back( cv::Point2f( line.getVertices()[j].x, line.getVertices()[j].y ) );
                }
            }
            
            for( vector<cv::Point>::iterator it = allConvexHullPoints.begin(); it != allConvexHullPoints.end(); ++it )
            {
                allConvexHullFloatPoints.push_back( cv::Point2f( it->x, it->y ) );
            }
            
            pointTracker.setMaximumDistance(params[PARAM_NAME_TRACKING_DISTANCE]);
            pointTracker.setPersistence(params[PARAM_NAME_TRACKING_PERSISTENCE]);
            pointTracker.track( allConvexHullFloatPoints );
            
            for( int i  = 0; i < allConvexHullFloatPoints.size(); ++i )
            {
                PointInputSampleT   sample;
                
                int label = pointTracker.getLabelFromIndex(i);
                sample.setSampleID(label);
                
                ofPoint p(ofxCv::toOf( allConvexHullFloatPoints[i] ) );
//                if(smoothing > 0 && pointTracker.existsPrevious(label)) {
//                    ofPoint prev(ofxCv::toOf(pointTracker.getPrevious(label)));
//                    p = p/2;//.interpolated(p, smoothing);
//                }
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
        ofPolyline  line;
        float simplify = params[PARAM_NAME_CONTOUR_SIMPLIFY];
        
        for( int i = 0; i < contourFinder.size(); ++i )
        {
            vector<cv::Point> convexHullPoints  = contourFinder.getConvexHull( i );
            
            line.clear();
            
            for( vector<cv::Point>::iterator it = convexHullPoints.begin(); it != convexHullPoints.end(); ++it )
            {
                line.lineTo( ofxCv::toOf( *it ) );
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