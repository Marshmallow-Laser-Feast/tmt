//
//  VideoAnalysisInput.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/19/13.
//
//

#pragma once

#include "ofMain.h"

#include "ofxCv.h"

#include "ContourFinder2.h"

#include "Input.h"

#define PARAM_NAME_VIS_CENTROID                     "Draw Centroids"
#define PARAM_NAME_VIS_CONTOUR                      "Draw Contours"
#define PARAM_NAME_VIS_CONVEXHULL                   "Draw Convex Hull"
#define PARAM_NAME_VIS_TIPS                         "Draw Tips"

#define PARAM_NAME_THRESHOLD                        "Threshold"
#define PARAM_NAME_BLUR                             "Blur"
#define PARAM_NAME_DILATE                           "Dilate"
#define PARAM_NAME_ERODE                            "Erode"
#define PARAM_NAME_MIN_AREA                         "Min Area"
#define PARAM_NAME_MAX_AREA                         "Max Area"
#define PARAM_NAME_RESAMPLING                       "Resampling"
#define PARAM_NAME_SMOOTHING                        "Smoothing"
#define PARAM_NAME_SIMPLIFICATION                   "Simplification"

#define PARAM_NAME_TIP_THRESHOLD                    "Tip Threshold"
#define PARAM_NAME_CENT_TRACK_DIST                  "Centroid Dist"
#define PARAM_NAME_CENT_TRACK_PERS                  "Centroid Pers"

class VideoAnalysisInput: public Input
{
    
public:
    
    static const std::string CONTOUR_TAG;
    static const std::string CONVEXHULL_TAG;
    static const std::string TIPS_TAG;
    static const std::string CENTROID_TAG;
    
public:
    
    VideoAnalysisInput()
    
    :Input( "Input/Video Analysis" )
    ,image( NULL )
    ,isCurrentFrameNew( false )
    
    {
        params.addBool( PARAM_NAME_VIS_CENTROID ).set( true );
        params.addBool( PARAM_NAME_VIS_CONTOUR ).set( true );
        params.addBool( PARAM_NAME_VIS_CONVEXHULL ).set( true );
        params.addBool( PARAM_NAME_VIS_TIPS ).set( true );
        
        params.addInt( PARAM_NAME_THRESHOLD ).setRange(0, 255).setClamp(true);
        params.addInt( PARAM_NAME_BLUR ).setRange(0, 50).setClamp(true);
        params.addInt( PARAM_NAME_DILATE ).setRange(0, 50).setClamp(true);;
        params.addInt( PARAM_NAME_ERODE ).setRange(0, 50).setClamp(true);;
        params.addInt( PARAM_NAME_MIN_AREA ).setRange(0, 100).setClamp(true);;
        params.addInt( PARAM_NAME_MAX_AREA ).setRange(0, 10000).setClamp(true);;
        params.addInt(PARAM_NAME_RESAMPLING).setClamp(true).setRange(0, 1000);
        params.addInt( PARAM_NAME_SMOOTHING ).setRange(0, 40).setClamp( true );
        params.addFloat( PARAM_NAME_SIMPLIFICATION ).setRange( 0.0f, 50.0f ).setClamp( true ).setIncrement( 0.01f );
        
        params.addFloat(PARAM_NAME_TIP_THRESHOLD).setRange(-180, 180).setClamp(true);
        
        params.addFloat(PARAM_NAME_CENT_TRACK_DIST);
        params.addFloat(PARAM_NAME_CENT_TRACK_PERS);
                
        pointSamplesMap[ CENTROID_TAG ]         = vector<PointSampleT>();
        
        polylineSamplesMap[ CONTOUR_TAG ]       = vector<PolylineSampleT>();
        polylineSamplesMap[ CONVEXHULL_TAG ]    = vector<PolylineSampleT>();
        polylineSamplesMap[ TIPS_TAG ]          = vector<PolylineSampleT>();
    };
    
    ~VideoAnalysisInput(){};
    
public:
    
    virtual void draw( float width, float height )
    {        
        ofPushStyle();
        
        ofSetColor( 150 );
        
        image->draw(0, 0);
        
        float yOffset   = 0.0f;
        
        if( (bool)params[ PARAM_NAME_VIS_CONTOUR ] )
        {
            ofSetColor( ofColor::red );
            ofDrawBitmapString( "Contour", 20.0f, 40.0f );
            drawPolylineSamples( polylineSamplesMap[ CONTOUR_TAG ] );
            
            yOffset += 40.0f;
        }
        
        if( (bool)params[ PARAM_NAME_VIS_CONVEXHULL ] )
        {
            ofSetColor( ofColor::blue );
            ofDrawBitmapString( "Convex", 20.0f, yOffset + 20.0f );
            drawPolylineSamples( polylineSamplesMap[ CONVEXHULL_TAG ] );
            
            yOffset += 20.0f;
        }
        
        if( (bool)params[ PARAM_NAME_VIS_TIPS ] )
        {
            ofSetColor( ofColor::yellow );
            ofDrawBitmapString( "Tips", 20.0f, yOffset + 20.0f );
            drawPolylineSamples( polylineSamplesMap[ TIPS_TAG ] );
            
            yOffset += 20.0f;
        }
        
        if( (bool)params[ PARAM_NAME_VIS_CENTROID ] )
        {
            ofSetColor( ofColor::green );
            ofDrawBitmapString( "Centroids", 20.0f, yOffset + 20.0f );
            drawPointSamples( pointSamplesMap[ CENTROID_TAG ], 4.0f );
        }
        
        ofPopStyle();
    };
    
    void setImage( ofImage & image_ )
    {
        image  = &image_;
    };
    
    void setCurrentFrameNew( bool value )
    {
        isCurrentFrameNew   = value;
    };
    
    virtual std::string getName(){ return "Input/Video Analysis"; };
    
    virtual ofVec2f getSize()
    {
        if( image != NULL )
        {
            return ofVec2f( image->getWidth(), image->getHeight() );
        }
        
        return ofVec2f( 150, 100 );
    };
    
    virtual void update()
    {
        if( (bool)params[ PARAM_NAME_ENABLE ] )
        {
            if( isCurrentFrameNew && image != NULL )
            {
                contourFinder.setThreshold(params[PARAM_NAME_THRESHOLD]);
                contourFinder.setBlur(params[PARAM_NAME_BLUR]);
                contourFinder.setDilate(params[PARAM_NAME_DILATE]);
                contourFinder.setErode(params[PARAM_NAME_ERODE]);
                contourFinder.setMinAreaRadius(params[PARAM_NAME_MIN_AREA]);
                contourFinder.setMaxAreaRadius(params[PARAM_NAME_MAX_AREA]);
                
                contourFinder.findContours( *image );
                
                processPointSamples();
                processPolylineSamples();
                
                pointSamplesMap[ Input::DEFAULT_TAG ]       = pointSamplesMap[ CENTROID_TAG ];
                polylineSamplesMap[ Input::DEFAULT_TAG ]    = polylineSamplesMap[ CONTOUR_TAG ];
            }
        }
    };
    
protected:
    
    virtual void processPointSamples()
    {
        processCentroids( (float)params[PARAM_NAME_CENT_TRACK_DIST], (float)params[PARAM_NAME_CENT_TRACK_PERS]  );
    };
    
    virtual void processPolylineSamples()
    {
        int     resampleCount   = params[PARAM_NAME_RESAMPLING];
        int     smoothAmount    = params[PARAM_NAME_SMOOTHING];
        float   simplify        = params[PARAM_NAME_SIMPLIFICATION];
        float   tipThreshold    = params[PARAM_NAME_TIP_THRESHOLD];
        
        std::vector<cv::Rect>   bboxes;
        
        for( int i = 0; i < contourFinder.size(); ++i )
        {
            bboxes.push_back( contourFinder.getBoundingRect(i) );
        }
        
        vector<unsigned int>    labels  = boundingBoxTracker.track( bboxes );
                
        processContour( labels, resampleCount, smoothAmount, simplify );
        processConvexHull( labels, resampleCount, smoothAmount, simplify );
        processTips( labels, resampleCount, smoothAmount, simplify, tipThreshold );
    };
    
private:
    
    void processContour(  vector<unsigned int> & labels, int resampleCount, int smoothAmount, float simplify )
    {
        polylineSamplesMap[ CONTOUR_TAG ].clear();
        
        for( int i = 0; i < labels.size(); ++i )
        {
            ofPolyline polyline = ofxCv::toOf( contourFinder.getContour(i) );
            
            if( resampleCount )
            {
                polyline    = polyline.getResampledByCount(resampleCount);
            }
            
            if( smoothAmount )
            {
                polyline    = polyline.getSmoothed(smoothAmount);
            }
            
            if( simplify > 0 )
            {
                polyline.simplify( simplify );
            }
            
            polylineSamplesMap[ CONTOUR_TAG ].push_back( PolylineSampleT() );
            
            polylineSamplesMap[ CONTOUR_TAG ].back().setSample( polyline );
            polylineSamplesMap[ CONTOUR_TAG ].back().setSampleID( labels[i] );
        }
    }
    
    void processConvexHull(  vector<unsigned int> & labels, int resampleCount, int smoothAmount, float simplify )
    {
        polylineSamplesMap[ CONVEXHULL_TAG ].clear();
        
        for( int i = 0; i < labels.size(); ++i )
        {
            ofPolyline polyline = ofxCv::toOf( contourFinder.getConvexHull(i) );
            
            if( resampleCount )
            {
                polyline    = polyline.getResampledByCount(resampleCount);
            }
            
            if( smoothAmount )
            {
                polyline    = polyline.getSmoothed(smoothAmount);
            }
            
            if( simplify > 0 )
            {
                polyline.simplify( simplify );
            }
            
            polylineSamplesMap[ CONVEXHULL_TAG ].push_back( PolylineSampleT() );
            
            polylineSamplesMap[ CONVEXHULL_TAG ].back().setSample( polyline );
            polylineSamplesMap[ CONVEXHULL_TAG ].back().setSampleID( labels[i] );
        }
    }
    
    void processTips(  vector<unsigned int> & labels, int resampleCount, int smoothAmount, float simplify, float tipThreshold )
    {
        polylineSamplesMap[ TIPS_TAG ].clear();
        
        for( int i = 0; i < labels.size(); ++i )
        {
            ofPolyline polyline = ofxCv::toOf( contourFinder.getContour(i) );
            
            ofPolyline temporaryPolyline;
            
            if( resampleCount )
            {
                polyline        = polyline.getResampledByCount(resampleCount);
            }
            
            if( smoothAmount )
            {
                polyline        = polyline.getSmoothed(smoothAmount);
            }
            
            for( int i = 0; i < polyline.size(); ++i )
            {
                if( polyline.getAngleAtIndex(i) > tipThreshold )
                {
                    temporaryPolyline.addVertex( polyline[i] );
                }
            }
            
            temporaryPolyline.close();
            
            polyline            = temporaryPolyline;
            
            if( simplify > 0 )
            {
                polyline.simplify( simplify );
            }
            
            polylineSamplesMap[ TIPS_TAG ].push_back( PolylineSampleT() );
            
            polylineSamplesMap[ TIPS_TAG ].back().setSample( polyline );
            polylineSamplesMap[ TIPS_TAG ].back().setSampleID( labels[i] );
        }
    }
    
    void processCentroids( float trackingDistance, float trackingPersistance )
    {
        pointSamplesMap[ CENTROID_TAG ].clear();
        
        vector<cv::Point2f> allFloatPoints;
        
        for( int i = 0; i < contourFinder.size(); ++i )
        {
            allFloatPoints.push_back( contourFinder.getCentroid( i ) );
        }
        
        centroidTracker.setMaximumDistance(trackingDistance);
        centroidTracker.setPersistence(trackingPersistance);
        centroidTracker.track( allFloatPoints );
        
        for( int i = 0; i < allFloatPoints.size(); ++i )
        {
            pointSamplesMap[ CENTROID_TAG ].push_back( PointSampleT() );
            
            pointSamplesMap[ CENTROID_TAG ].back().setSample( ofPoint(ofxCv::toOf( allFloatPoints[i] ) ) );
            pointSamplesMap[ CENTROID_TAG ].back().setSampleID( centroidTracker.getLabelFromIndex( i ) );
        }
    }
    
    bool                    isCurrentFrameNew;
    
    ofImage                 *image;
    
    ofxCv::RectTracker      boundingBoxTracker;
    ofxCv::PointTracker     centroidTracker;
    ofxCv::ContourFinder2   contourFinder;
    
};