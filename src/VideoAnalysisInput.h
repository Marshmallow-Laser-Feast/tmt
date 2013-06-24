//
//  VideoAnalysisInput.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/19/13.
//
//

#pragma once

#include <vector>
#include <deque>
#include <map>

#include "ofMain.h"

#include "ofxCv.h"

#include "ContourFinder2.h"
#include "SkeletonFinder.h"

#include "Input.h"
#include "PointSampleSmoothing.h"

#define MIN_AVERAGE_RADIUS                          0.01f

#define PARAM_NAME_PROC_CENTROID                    "Process Centroids"
#define PARAM_NAME_PROC_TIPS                        "Process Tips"
#define PARAM_NAME_PROC_CONTOUR                     "Process Contours"
#define PARAM_NAME_PROC_CONVEXHULL                  "Process Convex Hull"
#define PARAM_NAME_PROC_SKLELETON                   "Process Skeleton"

#define PARAM_NAME_VIS_CENTROID                     "Draw Centroids"
#define PARAM_NAME_VIS_CENTROID_IDS                 "Draw Centroids IDs"
#define PARAM_NAME_VIS_TIPS                         "Draw Tips"
#define PARAM_NAME_VIS_TIPS_IDS                     "Draw Tips IDs"
#define PARAM_NAME_VIS_CONTOUR                      "Draw Contours"
#define PARAM_NAME_VIS_CONVEXHULL                   "Draw Convex Hull"
#define PARAM_NAME_VIS_SKLELETON                    "Draw Skeleton"
#define PARAM_NAME_VIS_SKLELETON_POINTS             "Draw Skeleton Points"
#define PARAM_NAME_VIS_SKLELETON_POINTS_IDS         "Draw Skeleton Point IDs"

#define PARAM_NAME_CACHE_SIZE                       "Cache Size"
#define PARAM_NAME_CACHE_OFFSET                     "Cache Offset"

#define PARAM_NAME_THRESHOLD                        "Threshold"
#define PARAM_NAME_BLUR                             "Blur"
#define PARAM_NAME_DILATE                           "Dilate"
#define PARAM_NAME_ERODE                            "Erode"
#define PARAM_NAME_MIN_AREA                         "Min Area"
#define PARAM_NAME_MAX_AREA                         "Max Area"

#define PARAM_NAME_CENT_TRACK_DIST                  "Centroid Tracking Dist"
#define PARAM_NAME_CENT_TRACK_PERS                  "Centroid Tracking Pers"
#define PARAM_NAME_CENT_SAMPLE_SMOOTHING            "Centroid Sample Smoothing"

#define PARAM_NAME_TIPS_RESAMPLING                  "Tips Resampling"
#define PARAM_NAME_TIPS_SMOOTHING                   "Tips Smoothing"
#define PARAM_NAME_TIPS_SIMPLIFICATION              "Tips Simplification"
#define PARAM_NAME_TIPS_AVERAGE_RADIUS              "Tips Avarage Radius"
#define PARAM_NAME_TIPS_THRESHOLD                   "Tips Threshold"
#define PARAM_NAME_TIPS_TRACK_DIST                  "Tips Tracking Dist"
#define PARAM_NAME_TIPS_TRACK_PERS                  "Tips Tracking Pers"
#define PARAM_NAME_TIPS_SAMPLE_SMOOTHING            "Tips Sample Smoothing"

#define PARAM_NAME_CONTOUR_RESAMPLING               "Contour Resampling"
#define PARAM_NAME_CONTOUR_SMOOTHING                "Contour Smoothing"
#define PARAM_NAME_CONTOUR_SIMPLIFICATION           "Contour Simplification"
#define PARAM_NAME_CONTOUR_AVERAGE_RADIUS           "Contour Avarage Radius"

#define PARAM_NAME_CONVEXHULL_RESAMPLING            "Convex Hull Resampling"
#define PARAM_NAME_CONVEXHULL_SMOOTHING             "Convex Hull Smoothing"
#define PARAM_NAME_CONVEXHULL_SIMPLIFICATION        "Convex Hull Simplification"
#define PARAM_NAME_CONVEXHULL_AVERAGE_RADIUS        "Convex Hull Avarage Radius"
#define PARAM_NAME_CONVEXHULL_TRACK_DIST            "Convex Tracking Dist"
#define PARAM_NAME_CONVEXHULL_TRACK_PERS            "Convex Tracking Pers"
#define PARAM_NAME_CONVEXHULL_SAMPLE_SMOOTHING      "Convex Sample Smoothing"

#define PARAM_NAME_SKELETON_PRECISE_PROCESS         "Skeleton Precise Processing"
#define PARAM_NAME_SKELETON_RESAMPLING              "Skeleton Resampling"
#define PARAM_NAME_SKELETON_SMOOTHING               "Skeleton Smoothing"
#define PARAM_NAME_SKELETON_SIMPLIFICATION          "Skeleton Simplification"
#define PARAM_NAME_SKELETON_TRACK_DIST              "Skeleton Tracking Dist"
#define PARAM_NAME_SKELETON_TRACK_PERS              "Skeleton Tracking Pers"
#define PARAM_NAME_SKELETON_SAMPLE_SMOOTHING        "Skeleton Sample Smoothing"

class VideoAnalysisInput: public Input
{
    
protected:
    
    typedef std::map< std::string, PointSampleVectorRefT>       PointSampleVectorRefMapT;
    typedef std::map< std::string, PointSampleVectorVectorRefT> PointSampleVectorVectorRefMapT;
    typedef std::map< std::string, PolylineSampleVectorRefT>    PolylineSampleVectorRefMapT;
    
    typedef deque<PointSampleVectorRefMapT>                     PointSampleVectorRefMapDequeT;
    typedef deque<PointSampleVectorVectorRefMapT>               PointSampleVectorVectorRefMapDequeT;
    typedef deque<PolylineSampleVectorRefMapT>                  PolylineSampleVectorRefMapDequeT;
    
public:
    
    static const std::string CENTROID_TAG;
    static const std::string TIPS_TAG;
    static const std::string CONTOUR_TAG;
    static const std::string CONVEXHULL_TAG;
    static const std::string SKELETON_LINES_TAG;
    static const std::string SKELETON_POINTS_TAG;
    
public:
    
    VideoAnalysisInput()
    
    :Input( "Input/Video Analysis" )
    ,image( NULL )
    ,isCurrentFrameNew( false )
    
    {
        params.addBool( PARAM_NAME_PROC_CENTROID ).set( true );
        params.addBool( PARAM_NAME_PROC_TIPS ).set( true );
        params.addBool( PARAM_NAME_PROC_CONTOUR ).set( true );
        params.addBool( PARAM_NAME_PROC_CONVEXHULL ).set( true );
        params.addBool( PARAM_NAME_PROC_SKLELETON ).set( true );
        
        params.addBool( PARAM_NAME_VIS_CENTROID );
        params.addBool( PARAM_NAME_VIS_CENTROID_IDS );
        params.addBool( PARAM_NAME_VIS_TIPS ).set( true );
        params.addBool( PARAM_NAME_VIS_TIPS_IDS ).set( true );
        params.addBool( PARAM_NAME_VIS_CONTOUR ).set( true );
        params.addBool( PARAM_NAME_VIS_CONVEXHULL ).set( true );
        params.addBool( PARAM_NAME_VIS_SKLELETON ).set( true );
        params.addBool( PARAM_NAME_VIS_SKLELETON_POINTS ).set( true );
        params.addBool( PARAM_NAME_VIS_SKLELETON_POINTS_IDS ).set( true );
        
        params.addInt( PARAM_NAME_CACHE_SIZE ).set( 1 );
        params.addInt( PARAM_NAME_CACHE_OFFSET ).setRange(0, 1).setClamp(true);
        
        params.addInt( PARAM_NAME_THRESHOLD ).setRange(0, 255).setClamp(true);
        params.addInt( PARAM_NAME_BLUR ).setRange(0, 50).setClamp(true);
        params.addInt( PARAM_NAME_DILATE ).setRange(0, 50).setClamp(true);
        params.addInt( PARAM_NAME_ERODE ).setRange(0, 50).setClamp(true);
        params.addInt( PARAM_NAME_MIN_AREA ).setRange(0, 100).setClamp(true);
        params.addInt( PARAM_NAME_MAX_AREA ).setRange(0, 10000).setClamp(true);
        
        params.addFloat( PARAM_NAME_CENT_TRACK_DIST ).set( true ).set( 10.0f );
        params.addFloat( PARAM_NAME_CENT_TRACK_PERS );
        params.addFloat( PARAM_NAME_CENT_SAMPLE_SMOOTHING ).setClamp( true );
        
        params.addInt( PARAM_NAME_TIPS_RESAMPLING ).setClamp(true).setRange(0, 1000);
        params.addInt( PARAM_NAME_TIPS_SMOOTHING ).setRange(0, 40).setClamp( true );
        params.addFloat( PARAM_NAME_TIPS_SIMPLIFICATION ).setRange( 0.0f, 50.0f ).setClamp( true ).setIncrement( 0.01f );
        params.addFloat( PARAM_NAME_TIPS_AVERAGE_RADIUS ).setClamp( true );
        params.addFloat( PARAM_NAME_TIPS_THRESHOLD ).setRange( -180, 180 ).setClamp(true);
        params.addFloat( PARAM_NAME_TIPS_TRACK_DIST ).set( true ).set( 10.0f );
        params.addFloat( PARAM_NAME_TIPS_TRACK_PERS );
        params.addFloat( PARAM_NAME_TIPS_SAMPLE_SMOOTHING ).setClamp( true );
        
        params.addInt( PARAM_NAME_CONTOUR_RESAMPLING ).setClamp(true).setRange(0, 1000);
        params.addInt( PARAM_NAME_CONTOUR_SMOOTHING ).setRange(0, 40).setClamp( true );
        params.addFloat( PARAM_NAME_CONTOUR_SIMPLIFICATION ).setRange( 0.0f, 50.0f ).setClamp( true ).setIncrement( 0.01f );
        params.addFloat( PARAM_NAME_CONTOUR_AVERAGE_RADIUS ).setClamp( true );
        
        params.addInt( PARAM_NAME_CONVEXHULL_RESAMPLING ).setClamp(true).setRange(0, 1000);
        params.addInt( PARAM_NAME_CONVEXHULL_SMOOTHING ).setRange(0, 40).setClamp( true );
        params.addFloat( PARAM_NAME_CONVEXHULL_SIMPLIFICATION ).setRange( 0.0f, 50.0f ).setClamp( true ).setIncrement( 0.01f );
        params.addFloat( PARAM_NAME_CONVEXHULL_AVERAGE_RADIUS ).setClamp( true );
        params.addFloat( PARAM_NAME_CONVEXHULL_TRACK_DIST ).set( true ).set( 10.0f );
        params.addFloat( PARAM_NAME_CONVEXHULL_TRACK_PERS );
        params.addFloat( PARAM_NAME_CONVEXHULL_SAMPLE_SMOOTHING ).setClamp( true );
        
        params.addBool( PARAM_NAME_SKELETON_PRECISE_PROCESS ).set( false );
        params.addInt( PARAM_NAME_SKELETON_RESAMPLING ).setClamp(true).setRange(0, 1000);
        params.addInt( PARAM_NAME_SKELETON_SMOOTHING ).setRange(0, 40).setClamp( true );
        params.addFloat( PARAM_NAME_SKELETON_SIMPLIFICATION ).setRange( 0.0f, 50.0f ).setClamp( true ).setIncrement( 0.01f );
        params.addFloat( PARAM_NAME_SKELETON_TRACK_DIST ).set( true ).set( 10.0f );
        params.addFloat( PARAM_NAME_SKELETON_TRACK_PERS );
        params.addFloat( PARAM_NAME_SKELETON_SAMPLE_SMOOTHING ).setClamp( true );
        
        pointSampleTags.push_back( CENTROID_TAG );
        
        pointVectorSampleTags.push_back( TIPS_TAG );
        pointVectorSampleTags.push_back( SKELETON_POINTS_TAG );
        
        polylineSampleTags.push_back( CONTOUR_TAG );
        polylineSampleTags.push_back( CONVEXHULL_TAG );
        polylineSampleTags.push_back( SKELETON_LINES_TAG );
        
        pointSamplesMapDeque.push_back( PointSampleVectorRefMapT() );
        pointVectorSamplesMapDeque.push_back( PointSampleVectorVectorRefMapT() );
        polylineSamplesMapDeque.push_back( PolylineSampleVectorRefMapT() );
    };
    
    ~VideoAnalysisInput(){};
    
public:
    
    void setImage( ofImage & image_ )
    {
        image  = &image_;
    };
    
    void setCurrentFrameNew( bool value )
    {
        isCurrentFrameNew   = value;
    };
    
public:
    
    virtual void update()
    {
        if( params[ PARAM_NAME_CACHE_SIZE ].hasChanged() )
        {
            params[ PARAM_NAME_CACHE_OFFSET ].setRange(0, (int)params[ PARAM_NAME_CACHE_SIZE ] ).setClamp(true);
        }
        
        if( (params.hasChanged() || isCurrentFrameNew) && image != NULL && image->bAllocated() )
        {
            pointSamplesMapDeque.push_back( PointSampleVectorRefMapT() );
            pointVectorSamplesMapDeque.push_back( PointSampleVectorVectorRefMapT() );
            polylineSamplesMapDeque.push_back( PolylineSampleVectorRefMapT() );
            
            if( pointSamplesMapDeque.size() > (int)params[ PARAM_NAME_CACHE_SIZE ] )
            {
                pointSamplesMapDeque.pop_front();
                pointVectorSamplesMapDeque.pop_front();
                polylineSamplesMapDeque.pop_front();
            }
            
            for (std::vector<string>::const_iterator it = getPointSampleTags().begin(); it != getPointSampleTags().end(); ++it )
            {
                pointSamplesMapDeque.back()[ *it ]          = PointSampleVectorRefT( new PointSampleVectorT() );
            }
            
            for (std::vector<string>::const_iterator it = getPointVectorSampleTags().begin(); it != getPointVectorSampleTags().end(); ++it )
            {
                pointVectorSamplesMapDeque.back()[ *it ]    = PointSampleVectorVectorRefT( new PointSampleVectorVectorT() );
            }
            
            for (std::vector<string>::const_iterator it = getPolylineSampleTags().begin(); it != getPolylineSampleTags().end(); ++it )
            {
                polylineSamplesMapDeque.back()[ *it ]       = PolylineSampleVectorRefT( new PolylineSampleVectorT() );
            }
            
            contourFinder.setThreshold(params[PARAM_NAME_THRESHOLD]);
            contourFinder.setBlur(params[PARAM_NAME_BLUR]);
            contourFinder.setDilate(params[PARAM_NAME_DILATE]);
            contourFinder.setErode(params[PARAM_NAME_ERODE]);
            contourFinder.setMinAreaRadius(params[PARAM_NAME_MIN_AREA]);
            contourFinder.setMaxAreaRadius(params[PARAM_NAME_MAX_AREA]);
            
            contourFinder.findContours( *image );
            skeletonFinder.findSkeletons( *image, contourFinder.getContours(), contourFinder.getBoundingRects(), (bool)params[ PARAM_NAME_SKELETON_PRECISE_PROCESS ] );
            
            bboxes.clear();
            
            for( int i = 0; i < contourFinder.size(); ++i )
            {
                bboxes.push_back( contourFinder.getBoundingRect(i) );
            }
            
            vector<unsigned int>    labels  = boundingBoxTracker.track( bboxes );
            
            if( (bool)params[ PARAM_NAME_PROC_CENTROID ] )
            {
                processCentroids(   params[PARAM_NAME_CENT_TRACK_DIST],
                                    params[PARAM_NAME_CENT_TRACK_PERS],
                                    params[PARAM_NAME_CENT_SAMPLE_SMOOTHING]
                                 );
            }
            
            if( (bool)params[ PARAM_NAME_PROC_TIPS ] )
            {
                processTips(    params[PARAM_NAME_TIPS_RESAMPLING],
                                params[PARAM_NAME_TIPS_SMOOTHING],
                                params[PARAM_NAME_TIPS_SIMPLIFICATION],
                                params[PARAM_NAME_TIPS_AVERAGE_RADIUS],
                                params[PARAM_NAME_TIPS_THRESHOLD],
                                params[PARAM_NAME_TIPS_TRACK_DIST],
                                params[PARAM_NAME_TIPS_TRACK_PERS],
                                params[PARAM_NAME_TIPS_SAMPLE_SMOOTHING]
                            );
            }
            
            if( (bool)params[ PARAM_NAME_PROC_CONTOUR ] )
            {
                processContour( labels,
                                params[PARAM_NAME_CONTOUR_RESAMPLING],
                                params[PARAM_NAME_CONTOUR_SMOOTHING],
                                params[PARAM_NAME_CONTOUR_SIMPLIFICATION],
                                params[PARAM_NAME_CONTOUR_AVERAGE_RADIUS]
                               );
            }
            
            if( (bool)params[ PARAM_NAME_PROC_CONVEXHULL ] )
            {
                processConvexHull(  labels,
                                    params[PARAM_NAME_CONVEXHULL_RESAMPLING],
                                    params[PARAM_NAME_CONVEXHULL_SMOOTHING],
                                    params[PARAM_NAME_CONVEXHULL_SIMPLIFICATION],
                                    params[PARAM_NAME_CONVEXHULL_AVERAGE_RADIUS],
                                    params[PARAM_NAME_CONVEXHULL_TRACK_DIST],
                                    params[PARAM_NAME_CONVEXHULL_TRACK_PERS],
                                    params[PARAM_NAME_CONVEXHULL_SAMPLE_SMOOTHING]
                                  );
            }
            
            if( (bool)params[ PARAM_NAME_PROC_SKLELETON ] )
            {
                processSkeletons(   labels,
                                    params[PARAM_NAME_SKELETON_RESAMPLING],
                                    params[PARAM_NAME_SKELETON_SMOOTHING],
                                    params[PARAM_NAME_SKELETON_SIMPLIFICATION],
                                    params[PARAM_NAME_SKELETON_TRACK_DIST],
                                    params[PARAM_NAME_SKELETON_TRACK_PERS],
                                    params[PARAM_NAME_SKELETON_SAMPLE_SMOOTHING]
                                 );
            }
        }
    };
    
    virtual const bool hasPointSamples( const std::string &tag ) const
    {
        int offset  = ofClamp( (int)pointSamplesMapDeque.size() - (int)params[ PARAM_NAME_CACHE_OFFSET ], 0, pointSamplesMapDeque.size() - 1 );
        
        if( pointSamplesMapDeque.size() > offset )
        {
            return pointSamplesMapDeque[ offset ].count( tag ) > 0;
        }
        
        return false;
    };
    
    virtual const bool hasPointVectorSamples( const std::string &tag ) const
    {
        int offset  = ofClamp( (int)pointVectorSamplesMapDeque.size() - (int)params[ PARAM_NAME_CACHE_OFFSET ], 0, pointVectorSamplesMapDeque.size() - 1 );
                
        if( pointVectorSamplesMapDeque.size() > offset )
        {
            return pointVectorSamplesMapDeque[ offset ].count( tag ) > 0;
        }
        
        return false;
    };
    
    virtual const bool hasPolylineSamples( const std::string &tag ) const
    {
        int offset  = ofClamp( (int)polylineSamplesMapDeque.size() - (int)params[ PARAM_NAME_CACHE_OFFSET ], 0, polylineSamplesMapDeque.size() - 1 );
        
        if( polylineSamplesMapDeque.size() > offset )
        {
            return polylineSamplesMapDeque[ offset ].count( tag ) > 0;
        }
        
        return false;
    };
    
    virtual const PointSampleVectorRefT getPointSamples( const std::string &tag ) const
    {
        int offset  = ofClamp( (int)pointSamplesMapDeque.size() - (int)params[ PARAM_NAME_CACHE_OFFSET ], 0, pointSamplesMapDeque.size() - 1 );
        
        if( pointSamplesMapDeque.size() > offset )
        {
            if( pointSamplesMapDeque[ offset ].count( tag ) )
            {
                return pointSamplesMapDeque[ offset ].at( tag );
            }
        }
        
        return PointSampleVectorRefT( new PointSampleVectorT() );
    };
    
    virtual const PointSampleVectorVectorRefT getPointVectorSamples( const std::string &tag ) const
    {
        int offset  = ofClamp( (int)pointVectorSamplesMapDeque.size() - (int)params[ PARAM_NAME_CACHE_OFFSET ], 0, pointVectorSamplesMapDeque.size() - 1 );
        
        if( pointVectorSamplesMapDeque.size() > offset )
        {
            if( pointVectorSamplesMapDeque[ offset ].count( tag ) )
            {
                return pointVectorSamplesMapDeque[ offset ].at( tag );
            }
        }
        
        return PointSampleVectorVectorRefT( new PointSampleVectorVectorT() );
    };
    
    virtual const PolylineSampleVectorRefT getPolylineSamples( const std::string &tag ) const
    {
        int offset  = ofClamp( (int)polylineSamplesMapDeque.size() - (int)params[ PARAM_NAME_CACHE_OFFSET ], 0, polylineSamplesMapDeque.size() - 1 );
        
        if( polylineSamplesMapDeque.size() > offset )
        {
            if( polylineSamplesMapDeque[ offset ].count( tag ) )
            {
                return polylineSamplesMapDeque[ offset ].at( tag );
            }
        }
        
        return PolylineSampleVectorRefT( new PolylineSampleVectorT() );
    };
    
    virtual void draw( float width, float height )
    {
        float scale = 0.0f;
        
        if( width / image->getWidth() > height / image->getHeight() )
        {
            scale   = height / image->getHeight();
        } else {
            scale   = width / image->getWidth();
        }
        
        ofPushMatrix();
        ofScale(scale, scale);
        
        ofPushStyle();
        ofSetColor( 150 );
        
        image->draw(0, 0);
        
        ofPopMatrix();
                
        if( (bool)params[ PARAM_NAME_VIS_CENTROID ] )
        {
            ofSetColor( ofColor::green );
            drawPointSamples( getPointSamples( CENTROID_TAG ), 4.0f, scale );
        }
        
        if( (bool)params[ PARAM_NAME_VIS_TIPS ] )
        {
            ofSetColor( ofColor::yellow );
            drawPointVectorSamples( getPointVectorSamples( TIPS_TAG ), 2.0f, scale );
        }
        
        if( (bool)params[ PARAM_NAME_VIS_CONTOUR ] )
        {
            ofSetColor( ofColor::red );
            drawPolylineSamples( getPolylineSamples( CONTOUR_TAG ), scale );
        }
        
        if( (bool)params[ PARAM_NAME_VIS_CONVEXHULL ] )
        {
            ofSetColor( ofColor::pink );
            drawPolylineSamples( getPolylineSamples( CONVEXHULL_TAG ), scale );
        }
        
        if( (bool)params[ PARAM_NAME_VIS_SKLELETON ] )
        {
            ofSetColor( ofColor::salmon );
            drawPolylineSamples( getPolylineSamples( SKELETON_LINES_TAG ), scale );
        }
        
        if( (bool)params[ PARAM_NAME_VIS_SKLELETON_POINTS ] )
        {
            ofSetColor( ofColor::salmon );
            drawPointVectorSamples( getPointVectorSamples( SKELETON_POINTS_TAG ), 2.0f, scale );
        }
        
        float yOffset   = 0.0f;
        
        if( (bool)params[ PARAM_NAME_VIS_CENTROID ] )
        {
            ofSetColor( ofColor::green );
            ofDrawBitmapString( "-> Centroids", 20.0f, 20.0f );
            
            yOffset += 20.0f;
        }
        
        if( (bool)params[ PARAM_NAME_VIS_TIPS ] )
        {
            ofSetColor( ofColor::yellow );
            ofDrawBitmapString( "-> Tips", 20.0f, yOffset + 20.0f );
            
            yOffset += 20.0f;
        }
        
        if( (bool)params[ PARAM_NAME_VIS_CONTOUR ] )
        {
            ofSetColor( ofColor::red );
            ofDrawBitmapString( "-> Contour", 20.0f, yOffset + 20.0f );
            
            yOffset += 20.0f;
        }
        
        if( (bool)params[ PARAM_NAME_VIS_CONVEXHULL ] )
        {
            ofSetColor( ofColor::pink );
            ofDrawBitmapString( "-> Convex", 20.0f, yOffset + 20.0f );
            
            yOffset += 20.0f;
        }
        
        if( (bool)params[ PARAM_NAME_VIS_SKLELETON ] )
        {
            ofSetColor( ofColor::salmon );
            ofDrawBitmapString( "-> Skeleton", 20.0f, yOffset + 20.0f );
            
            yOffset += 20.0f;
        }
        
        if( (bool)params[ PARAM_NAME_VIS_SKLELETON_POINTS ] )
        {
            ofSetColor( ofColor::salmon );
            ofDrawBitmapString( "-> Skeleton Points", 20.0f, yOffset + 20.0f );
        }
        
        if( (bool)params[ PARAM_NAME_VIS_CENTROID_IDS ] )
        {
            ofSetColor( ofColor::white );
            
            for( std::vector<PointSampleT>::const_iterator it = getPointSamples( CENTROID_TAG )->begin(); it != getPointSamples( CENTROID_TAG )->end(); ++it )
            {
                ofDrawBitmapString( ofToString( it->getSampleID() ), it->getSample() * scale + ofPoint( 8.0f, 0.0f ) );
            }
        }
        
        if( (bool)params[ PARAM_NAME_VIS_TIPS_IDS ] )
        {
            ofSetColor( ofColor::yellow );
            
            for( PointSampleVectorVectorT::const_iterator it = getPointVectorSamples( TIPS_TAG )->begin(); it != getPointVectorSamples( TIPS_TAG )->end(); ++it )
            {
                for( PointSampleVectorT::const_iterator pit = it->begin(); pit != it->end(); ++pit )
                {
                    ofDrawBitmapString( ofToString( pit->getSampleID() ), pit->getSample() * scale + ofPoint( 4.0f, 0.0f ) );
                }
            }
        }
        
        if( (bool)params[ PARAM_NAME_VIS_SKLELETON_POINTS_IDS ] )
        {
            ofSetColor( ofColor::salmon );
            
            for( PointSampleVectorVectorT::const_iterator it = getPointVectorSamples( SKELETON_POINTS_TAG )->begin(); it != getPointVectorSamples( SKELETON_POINTS_TAG )->end(); ++it )
            {
                for( PointSampleVectorT::const_iterator pit = it->begin(); pit != it->end(); ++pit )
                {
                    ofDrawBitmapString( ofToString( pit->getSampleID() ), pit->getSample() * scale + ofPoint( 4.0f, 0.0f ) );
                }
            }
        }
        
        ofPopStyle();
    };
    
    virtual const ofVec2f getPanelSize() const
    {
        if( image != NULL )
        {
            return ofVec2f( image->getWidth(), image->getHeight() );
        }
        
        return ofVec2f( 150, 100 );
    };
    
    virtual const ofVec2f getSize() const
    {
        if( image != NULL )
        {
            return ofVec2f( image->getWidth(), image->getHeight() );
        }
        
        return ofVec2f( 150, 100 );
    };
    
private:
    
    void processCentroids( float trackingDistance, float trackingPersistance, float smoothing )
    {
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
            unsigned int sampleID   = centroidTracker.getLabelFromIndex( i );
            
            if( centroidSmoothersMap.count( sampleID ) == 0 )
            {
                centroidSmoothersMap[ sampleID ]    = PointSampleSmoother();
            }
            
            pointSamplesMapDeque.back()[ CENTROID_TAG ]->push_back( PointSampleT() );
            
            pointSamplesMapDeque.back()[ CENTROID_TAG ]->back().setSample( centroidSmoothersMap[ sampleID ].getSmoothed( ofPoint(ofxCv::toOf( allFloatPoints[i] ) ), smoothing ) );
            
            if( centroidTracker.existsPrevious( i ) )
            {
                pointSamplesMapDeque.back()[ CENTROID_TAG ]->back().setVelocity( ofPoint(ofxCv::toOf( allFloatPoints[ i ] ) ) - ofPoint(ofxCv::toOf( centroidTracker.getPrevious( i ) ) ) );
            } else {
                pointSamplesMapDeque.back()[ CENTROID_TAG ]->back().setVelocity( ofPoint() );
            }
            
            pointSamplesMapDeque.back()[ CENTROID_TAG ]->back().setSampleID( sampleID );
            pointSamplesMapDeque.back()[ CENTROID_TAG ]->back().setGroupID( i );
        }
    }
    
    void processTips( int resampleCount, int smoothAmount, float simplify, float averageRadius, float tipThreshold, float trackingDistance, float trackingPersistance, float smoothing )
    {
        vector<cv::Point2f>     allFloatPoints;
        vector<unsigned int>    groupIDs;
        
        for( int i = 0; i < contourFinder.size(); ++i )
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
            
            if( simplify > 0 )
            {
                polyline.simplify( simplify );
            }
            
            if( averageRadius > MIN_AVERAGE_RADIUS )
            {
                ofPolyline  temp;
                
                cv::Rect boundingRect(contourFinder.getBoundingRect(i));
                
                int         count           = 1;
                float       distThresh      = averageRadius * ( boundingRect.width + boundingRect.height ) * 0.5f;
                float       distThresh2     = distThresh * distThresh;
                
                for( int j=0; j < polyline.size(); j++ )
                {
                    ofPoint p = polyline[j];
                    
                    if( temp.size() > 0 && p.distanceSquared(temp.getVertices().back()) < distThresh2 )
                    {
                        ofPoint &back   = temp.getVertices().back();
                        ofPoint curAvg  = back * count;
                        
                        count++;
                        
                        back = (curAvg + p) / count;
                        
                    } else {
                        
                        count = 0;
                        temp.addVertex(p);
                    }
                }
                
                temp.close();
                
                polyline    = temp;
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
            
            for( vector<ofPoint>::iterator it = polyline.getVertices().begin(); it != polyline.getVertices().end(); ++it )
            {
                allFloatPoints.push_back( cv::Point2f( it->x, it->y ) );
                groupIDs.push_back( i );
            }
        }
        
        tipTracker.setMaximumDistance(trackingDistance);
        tipTracker.setPersistence(trackingPersistance);
        tipTracker.track( allFloatPoints );
        
        int currentGroupId      = -1;
        
        for( int i = 0; i < allFloatPoints.size(); ++i )
        {
            if( currentGroupId != groupIDs[ i ] )
            {
                currentGroupId  = groupIDs[ i ];
                
                pointVectorSamplesMapDeque.back()[ TIPS_TAG ]->push_back( PointSampleVectorT() );
            }
            
            unsigned int sampleID   = tipTracker.getLabelFromIndex( i );
            
            if( tipsSmoothersMap.count( sampleID ) == 0 )
            {
                tipsSmoothersMap[ sampleID ]    = PointSampleSmoother();
            }
            
            pointVectorSamplesMapDeque.back()[ TIPS_TAG ]->back().push_back( PointSampleT() );
                        
            pointVectorSamplesMapDeque.back()[ TIPS_TAG ]->back().back().setSample( tipsSmoothersMap[ sampleID ].getSmoothed( ofPoint(ofxCv::toOf( allFloatPoints[i] ) ), smoothing ) );
            
            if( tipTracker.existsPrevious( i ) )
            {
                pointVectorSamplesMapDeque.back()[ TIPS_TAG ]->back().back().setVelocity( ofPoint(ofxCv::toOf( allFloatPoints[i] ) ) - ofPoint(ofxCv::toOf( tipTracker.getPrevious( i ) ) ) );
            } else {
                pointVectorSamplesMapDeque.back()[ TIPS_TAG ]->back().back().setVelocity( ofPoint() );
            }
            
            pointVectorSamplesMapDeque.back()[ TIPS_TAG ]->back().back().setSampleID( tipTracker.getLabelFromIndex( i ) );
            pointVectorSamplesMapDeque.back()[ TIPS_TAG ]->back().back().setGroupID( groupIDs[ i ] );
        }
    }
    
    void processContour( vector<unsigned int> & labels, int resampleCount, int smoothAmount, float simplify, float averageRadius )
    {
        
        for( int i = 0; i < contourFinder.size(); ++i )
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
            
            if( averageRadius > MIN_AVERAGE_RADIUS )
            {
                ofPolyline  temp;
                
                cv::Rect boundingRect(contourFinder.getBoundingRect(i));
                
                int         count           = 1;
                float       distThresh      = averageRadius * ( boundingRect.width + boundingRect.height ) * 0.5f;
                float       distThresh2     = distThresh * distThresh;
                
                for( int j=0; j < polyline.size(); j++ )
                {
                    ofPoint p = polyline[j];
                    
                    if( temp.size() > 0 && p.distanceSquared(temp.getVertices().back()) < distThresh2 )
                    {
                        ofPoint &back   = temp.getVertices().back();
                        ofPoint curAvg  = back * count;
                        
                        count++;
                        
                        back = (curAvg + p) / count;
                        
                    } else {
                        
                        count = 0;
                        temp.addVertex(p);
                    }
                }
                
                temp.close();
                
                polyline    = temp;
            }
            
            polylineSamplesMapDeque.back()[ CONTOUR_TAG ]->push_back( PolylineSampleT() );
            
            polylineSamplesMapDeque.back()[ CONTOUR_TAG ]->back().setSample( polyline );
            polylineSamplesMapDeque.back()[ CONTOUR_TAG ]->back().setVelocity( ofPoint( ofxCv::toOf( contourFinder.getVelocity( i ) ) ) );
            polylineSamplesMapDeque.back()[ CONTOUR_TAG ]->back().setSampleID( labels[i] );
            polylineSamplesMapDeque.back()[ CONTOUR_TAG ]->back().setGroupID( i );
        }

    }
    
    void processConvexHull( vector<unsigned int> & labels, int resampleCount, int smoothAmount, float simplify, float averageRadius, float trackingDistance, float trackingPersistance, float smoothing )
    {
        vector<cv::Point2f>     allFloatPoints;
        vector<unsigned int>    groupIDs;
        
        for( int i = 0; i < contourFinder.size(); ++i )
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
            
            if( averageRadius > MIN_AVERAGE_RADIUS )
            {
                ofPolyline  temp;
                
                cv::Rect boundingRect(contourFinder.getBoundingRect(i));
                
                int         count           = 1;
                float       distThresh      = averageRadius * ( boundingRect.width + boundingRect.height ) * 0.5f;
                float       distThresh2     = distThresh * distThresh;
                
                for( int j=0; j < polyline.size(); j++ )
                {
                    ofPoint p = polyline[j];
                    
                    if( temp.size() > 0 && p.distanceSquared(temp.getVertices().back()) < distThresh2 )
                    {
                        ofPoint &back   = temp.getVertices().back();
                        ofPoint curAvg  = back * count;
                        
                        count++;
                        
                        back = (curAvg + p) / count;
                        
                    } else {
                        
                        count = 0;
                        temp.addVertex(p);
                    }
                }
                
                temp.close();
                
                polyline    = temp;
            }
            
            for( vector<ofPoint>::iterator it = polyline.getVertices().begin(); it != polyline.getVertices().end(); ++it )
            {
                allFloatPoints.push_back( cv::Point2f( it->x, it->y ) );
                groupIDs.push_back( i );
            }
        }
        
        convexHullPointTracker.setMaximumDistance(trackingDistance);
        convexHullPointTracker.setPersistence(trackingPersistance);
        convexHullPointTracker.track( allFloatPoints );
        
        std::vector<std::vector<ofPoint> > polylinePointsVector;
        
        int currentGroupId      = -1;
        
        for( int i = 0; i < allFloatPoints.size(); ++i )
        {
            if( currentGroupId != groupIDs[ i ] )
            {
                currentGroupId  = groupIDs[ i ];
                
                polylinePointsVector.push_back( std::vector<ofPoint>() );
            }
            
            unsigned int sampleID   = convexHullPointTracker.getLabelFromIndex( i );
            
            if( convexHullSmoothersMap.count( sampleID ) == 0 )
            {
                convexHullSmoothersMap[ sampleID ]  = PointSampleSmoother();
            }
            
            polylinePointsVector.back().push_back( convexHullSmoothersMap[ sampleID ].getSmoothed( ofPoint(ofxCv::toOf( allFloatPoints[i] ) ), smoothing ) );
        }
        
        for( int i = 0; i < polylinePointsVector.size(); ++i )
        {
            ofPolyline  polyline;
            
            polyline.addVertices( polylinePointsVector[i] );
            polyline.close();
            
            polylineSamplesMapDeque.back()[ CONVEXHULL_TAG ]->push_back( PolylineSampleT() );
            
            polylineSamplesMapDeque.back()[ CONVEXHULL_TAG ]->back().setSample( polyline );
            polylineSamplesMapDeque.back()[ CONVEXHULL_TAG ]->back().setVelocity( ofPoint( ofxCv::toOf( contourFinder.getVelocity( i ) ) ) );
            polylineSamplesMapDeque.back()[ CONVEXHULL_TAG ]->back().setSampleID( labels[i] );
            polylineSamplesMapDeque.back()[ CONVEXHULL_TAG ]->back().setGroupID( i );
        }
    }
    
    void processSkeletons( vector<unsigned int> & labels, int resampleCount, int smoothAmount, float simplify, float trackingDistance, float trackingPersistance, float smoothing )
    {
        vector<cv::Point2f>     allFloatPoints;
        vector<unsigned int>    groupIDs;
        
        int i = 0;
        
        for( vector<ofPolyline>::const_iterator it = skeletonFinder.getSkeletons().begin(); it != skeletonFinder.getSkeletons().end(); ++it )
        {
            ofPolyline polyline = *it;
            
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
            
            for( vector<ofPoint>::iterator it = polyline.getVertices().begin(); it != polyline.getVertices().end(); ++it )
            {
                allFloatPoints.push_back( cv::Point2f( it->x, it->y ) );
                groupIDs.push_back( i );
            }
            
            polylineSamplesMapDeque.back()[ SKELETON_LINES_TAG ]->push_back( PolylineSampleT() );
            
            polylineSamplesMapDeque.back()[ SKELETON_LINES_TAG ]->back().setSample( polyline );
            polylineSamplesMapDeque.back()[ SKELETON_LINES_TAG ]->back().setVelocity( ofPoint( ofxCv::toOf( contourFinder.getVelocity( i ) ) ) );
            polylineSamplesMapDeque.back()[ SKELETON_LINES_TAG ]->back().setSampleID( labels[i] );
            polylineSamplesMapDeque.back()[ SKELETON_LINES_TAG ]->back().setGroupID( i );
            
            ++i;
        }
        
        skeletonPointsTracker.setMaximumDistance(trackingDistance);
        skeletonPointsTracker.setPersistence(trackingPersistance);
        skeletonPointsTracker.track( allFloatPoints );
        
        int currentGroupId      = -1;
        
        for( int i = 0; i < allFloatPoints.size(); ++i )
        {
            if( currentGroupId != groupIDs[ i ] )
            {
                currentGroupId  = groupIDs[ i ];
                
                pointVectorSamplesMapDeque.back()[ SKELETON_POINTS_TAG ]->push_back( PointSampleVectorT() );
            }
            
            unsigned int sampleID   = skeletonPointsTracker.getLabelFromIndex( i );
            
            if( skeletonSmoothersMap.count( sampleID ) == 0 )
            {
                skeletonSmoothersMap[ sampleID ]    = PointSampleSmoother();
            }
            
            pointVectorSamplesMapDeque.back()[ SKELETON_POINTS_TAG ]->back().push_back( PointSampleT() );
            
            pointVectorSamplesMapDeque.back()[ SKELETON_POINTS_TAG ]->back().back().setSample( skeletonSmoothersMap[ sampleID ].getSmoothed( ofPoint(ofxCv::toOf( allFloatPoints[i] ) ), smoothing ) );
            
            if( skeletonPointsTracker.existsPrevious( i ) )
            {
                pointVectorSamplesMapDeque.back()[ SKELETON_POINTS_TAG ]->back().back().setVelocity( ofPoint(ofxCv::toOf( allFloatPoints[i] ) ) - ofPoint(ofxCv::toOf( skeletonPointsTracker.getPrevious( i ) ) ) );
            } else {
                pointVectorSamplesMapDeque.back()[ SKELETON_POINTS_TAG ]->back().back().setVelocity( ofPoint() );
            }
            
            pointVectorSamplesMapDeque.back()[ SKELETON_POINTS_TAG ]->back().back().setSampleID( skeletonPointsTracker.getLabelFromIndex( i ) );
            pointVectorSamplesMapDeque.back()[ SKELETON_POINTS_TAG ]->back().back().setGroupID( groupIDs[ i ] );
        }
    };
    
    void drawPointSamples( const PointSampleVectorRefT & samples, float circleR, float scale )
    {
        for( PointSampleVectorT::const_iterator it = samples->begin(); it != samples->end(); ++it )
        {
            ofCircle( it->getSample() * scale, circleR );
            ofLine( it->getSample() * scale , it->getSample() * scale + it->getVelocity().normalized() * circleR * 20.0f );
        }
    }
    
    void drawPointVectorSamples( const PointSampleVectorVectorRefT & samples, float circleR, float scale  )
    {
        for( PointSampleVectorVectorT::const_iterator it = samples->begin(); it != samples->end(); ++it )
        {
            for( PointSampleVectorT::const_iterator pit = it->begin(); pit != it->end(); ++pit )
            {
                ofCircle( pit->getSample() * scale, circleR );
                ofLine( pit->getSample() * scale , pit->getSample() * scale + pit->getVelocity().normalized() * circleR * 20.0f );
            }
        }
    }
    
    void drawPolylineSamples( const PolylineSampleVectorRefT & samples, float scale )
    {
        PolylineSampleVectorRefT newRef = samples;
        
        ofPushMatrix();
        
        ofScale( scale, scale );
        
        for( PolylineSampleVectorT::iterator it = newRef->begin(); it != newRef->end(); ++it )
        {
            it->getSample().draw();
        }
        
        ofPopMatrix();
    }
    
private:
    
    bool                                isCurrentFrameNew;
    
    ofImage                             *image;
    
    ofxCv::RectTracker                  boundingBoxTracker;
    
    ofxCv::PointTracker                 centroidTracker;
    ofxCv::PointTracker                 tipTracker;
    ofxCv::PointTracker                 convexHullPointTracker;
    ofxCv::PointTracker                 skeletonPointsTracker;
    
    ofxCv::ContourFinder2               contourFinder;
    
    ofxCv::SkeletonFinder               skeletonFinder;
    
    std::vector<cv::Rect>               bboxes;
        
    PointSampleVectorRefMapDequeT       pointSamplesMapDeque;
    PointSampleVectorVectorRefMapDequeT pointVectorSamplesMapDeque;
    PolylineSampleVectorRefMapDequeT    polylineSamplesMapDeque;
    
    std::map<unsigned int, PointSampleSmoother>  centroidSmoothersMap;
    std::map<unsigned int, PointSampleSmoother>  tipsSmoothersMap;
    std::map<unsigned int, PointSampleSmoother>  convexHullSmoothersMap;
    std::map<unsigned int, PointSampleSmoother>  skeletonSmoothersMap;
    
};