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

#define PARAM_NAME_CACHE_SIZE                       "Cache Size"
#define PARAM_NAME_CACHE_OFFSET                     "Cache Offset"

#define PARAM_NAME_THRESHOLD                        "Threshold"
#define PARAM_NAME_BLUR                             "Blur"
#define PARAM_NAME_DILATE                           "Dilate"
#define PARAM_NAME_ERODE                            "Erode"
#define PARAM_NAME_MIN_AREA                         "Min Area"
#define PARAM_NAME_MAX_AREA                         "Max Area"

#define PARAM_NAME_CENT_TRACK_DIST                  "Centroid Dist"
#define PARAM_NAME_CENT_TRACK_PERS                  "Centroid Pers"

#define PARAM_NAME_TIPS_RESAMPLING                  "Tips Resampling"
#define PARAM_NAME_TIPS_SMOOTHING                   "Tips Smoothing"
#define PARAM_NAME_TIPS_SIMPLIFICATION              "Tips Simplification"
#define PARAM_NAME_TIP_THRESHOLD                    "Tip Threshold"
#define PARAM_NAME_TIP_TRACK_DIST                   "Tip Dist"
#define PARAM_NAME_TIP_TRACK_PERS                   "Tip Pers"

#define PARAM_NAME_CONTOURT_RESAMPLING              "Contour Resampling"
#define PARAM_NAME_CONTOURT_SMOOTHING               "Contour Smoothing"
#define PARAM_NAME_CONTOURT_SIMPLIFICATION          "Contour Simplification"

#define PARAM_NAME_CONVEXHULL_RESAMPLING            "Convex Hull Resampling"
#define PARAM_NAME_CONVEXHULL_SMOOTHING             "Convex Hull Smoothing"
#define PARAM_NAME_CONVEXHULL_SIMPLIFICATION        "Convex Hull Simplification"

#define PARAM_NAME_SKELETON_PRECISE_PROCESS         "Skeleton Precise Processing"
#define PARAM_NAME_SKELETON_RESAMPLING              "Skeleton Resampling"
#define PARAM_NAME_SKELETON_SMOOTHING               "Skeleton Smoothing"
#define PARAM_NAME_SKELETON_SIMPLIFICATION          "Skeleton Simplification"

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
    
    static const std::string CONTOUR_TAG;
    static const std::string CONVEXHULL_TAG;
    static const std::string SKELETON_TAG;
    static const std::string TIPS_TAG;
    static const std::string CENTROID_TAG;
    
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
        
        params.addInt( PARAM_NAME_TIPS_RESAMPLING ).setClamp(true).setRange(0, 1000);
        params.addInt( PARAM_NAME_TIPS_SMOOTHING ).setRange(0, 40).setClamp( true );
        params.addFloat( PARAM_NAME_TIPS_SIMPLIFICATION ).setRange( 0.0f, 50.0f ).setClamp( true ).setIncrement( 0.01f );
        params.addFloat( PARAM_NAME_TIP_THRESHOLD ).setRange( -180, 180 ).setClamp(true);
        params.addFloat( PARAM_NAME_TIP_TRACK_DIST ).set( true ).set( 10.0f );
        params.addFloat( PARAM_NAME_TIP_TRACK_PERS );
        
        params.addInt( PARAM_NAME_CONTOURT_RESAMPLING ).setClamp(true).setRange(0, 1000);
        params.addInt( PARAM_NAME_CONTOURT_SMOOTHING ).setRange(0, 40).setClamp( true );
        params.addFloat( PARAM_NAME_CONTOURT_SIMPLIFICATION ).setRange( 0.0f, 50.0f ).setClamp( true ).setIncrement( 0.01f );
        
        params.addInt( PARAM_NAME_CONVEXHULL_RESAMPLING ).setClamp(true).setRange(0, 1000);
        params.addInt( PARAM_NAME_CONVEXHULL_SMOOTHING ).setRange(0, 40).setClamp( true );
        params.addFloat( PARAM_NAME_CONVEXHULL_SIMPLIFICATION ).setRange( 0.0f, 50.0f ).setClamp( true ).setIncrement( 0.01f );
        
        params.addBool( PARAM_NAME_SKELETON_PRECISE_PROCESS ).set( false );
        params.addInt( PARAM_NAME_SKELETON_RESAMPLING ).setClamp(true).setRange(0, 1000);
        params.addInt( PARAM_NAME_SKELETON_SMOOTHING ).setRange(0, 40).setClamp( true );
        params.addFloat( PARAM_NAME_SKELETON_SIMPLIFICATION ).setRange( 0.0f, 50.0f ).setClamp( true ).setIncrement( 0.01f );
        
        pointSampleTags.push_back( CENTROID_TAG );
        
        pointVectorSampleTags.push_back( TIPS_TAG );
        
        polylineSampleTags.push_back( CONTOUR_TAG );
        polylineSampleTags.push_back( CONVEXHULL_TAG );
        polylineSampleTags.push_back( SKELETON_TAG );
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
        
        if( (params.hasChanged() || isCurrentFrameNew) && image != NULL )
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
                                    params[PARAM_NAME_CENT_TRACK_PERS]
                                 );
            }
            
            if( (bool)params[ PARAM_NAME_PROC_TIPS ] )
            {
                processTips(    params[PARAM_NAME_TIPS_RESAMPLING],
                                params[PARAM_NAME_TIPS_SMOOTHING],
                                params[PARAM_NAME_TIPS_SIMPLIFICATION],
                                params[PARAM_NAME_TIP_THRESHOLD],
                                params[PARAM_NAME_CENT_TRACK_DIST],
                                params[PARAM_NAME_CENT_TRACK_PERS]
                            );
            }
            
            if( (bool)params[ PARAM_NAME_PROC_CONTOUR ] )
            {
                processContour( labels,
                                params[PARAM_NAME_CONTOURT_RESAMPLING],
                                params[PARAM_NAME_CONTOURT_SMOOTHING],
                                params[PARAM_NAME_CONTOURT_SIMPLIFICATION]
                               );
            }
            
            if( (bool)params[ PARAM_NAME_PROC_CONVEXHULL ] )
            {
                processConvexHull(  labels,
                                    params[PARAM_NAME_CONVEXHULL_RESAMPLING],
                                    params[PARAM_NAME_CONVEXHULL_SMOOTHING],
                                    params[PARAM_NAME_CONVEXHULL_SIMPLIFICATION]
                                  );
            }
            
            if( (bool)params[ PARAM_NAME_PROC_SKLELETON ] )
            {
                processSkeletons(   labels,
                                    params[PARAM_NAME_SKELETON_RESAMPLING],
                                    params[PARAM_NAME_SKELETON_SMOOTHING],
                                    params[PARAM_NAME_SKELETON_SIMPLIFICATION]
                                 );
            }
        }
    };
    
    virtual bool hasPointSamples( const std::string &tag )
    {
        int offset  = ofClamp( (int)pointSamplesMapDeque.size() - (int)params[ PARAM_NAME_CACHE_OFFSET ], 0, pointSamplesMapDeque.size() - 1 );
        
        return pointSamplesMapDeque[ offset ].count( tag ) > 0;
    };
    
    virtual bool hasPointVectorSamples( const std::string &tag )
    {
        int offset  = ofClamp( (int)pointVectorSamplesMapDeque.size() - (int)params[ PARAM_NAME_CACHE_OFFSET ], 0, pointVectorSamplesMapDeque.size() - 1 );
        
        return pointVectorSamplesMapDeque[ offset ].count( tag ) > 0;
    };
    
    virtual bool hasPolylineSamples( const std::string &tag )
    {
        int offset  = ofClamp( (int)polylineSamplesMapDeque.size() - (int)params[ PARAM_NAME_CACHE_OFFSET ], 0, polylineSamplesMapDeque.size() - 1 );
        
        return polylineSamplesMapDeque[ offset ].count( tag ) > 0;
    };
    
    virtual const PointSampleVectorRefT getPointSamples( const std::string &tag ) const
    {
        int offset  = ofClamp( (int)pointSamplesMapDeque.size() - (int)params[ PARAM_NAME_CACHE_OFFSET ], 0, pointSamplesMapDeque.size() - 1 );
        
        return pointSamplesMapDeque[ offset ].at( tag );
    };
    
    virtual const PointSampleVectorVectorRefT getPointVectorSamples( const std::string &tag ) const
    {
        int offset  = ofClamp( (int)pointVectorSamplesMapDeque.size() - (int)params[ PARAM_NAME_CACHE_OFFSET ], 0, pointVectorSamplesMapDeque.size() - 1 );
        
        return pointVectorSamplesMapDeque[ offset ].at( tag );
    };
    
    virtual const PolylineSampleVectorRefT getPolylineSamples( const std::string &tag ) const
    {
        int offset  = ofClamp( (int)polylineSamplesMapDeque.size() - (int)params[ PARAM_NAME_CACHE_OFFSET ], 0, polylineSamplesMapDeque.size() - 1 );
        
        return polylineSamplesMapDeque[ offset ].at( tag );
    };
    
    virtual void draw( float width, float height )
    {
        ofPushStyle();
        
        ofSetColor( 150 );
        
        image->draw(0, 0);
        
        float yOffset   = 0.0f;
        
        if( (bool)params[ PARAM_NAME_VIS_CENTROID ] )
        {
            ofSetColor( ofColor::green );
            ofDrawBitmapString( "-> Centroids", 20.0f, 20.0f );
            drawPointSamples( getPointSamples( CENTROID_TAG ), 4.0f );
            
            yOffset += 20.0f;
        }
        
        if( (bool)params[ PARAM_NAME_VIS_TIPS ] )
        {
            ofSetColor( ofColor::yellow );
            ofDrawBitmapString( "-> Tips", 20.0f, yOffset + 20.0f );
            drawPointVectorSamples( getPointVectorSamples( TIPS_TAG ), 2.0f );
        
            yOffset += 20.0f;
        }
        
        if( (bool)params[ PARAM_NAME_VIS_CONTOUR ] )
        {
            ofSetColor( ofColor::red );
            ofDrawBitmapString( "-> Contour", 20.0f, yOffset + 20.0f );
            drawPolylineSamples( getPolylineSamples( CONTOUR_TAG ) );
            
            yOffset += 20.0f;
        }
        
        if( (bool)params[ PARAM_NAME_VIS_CONVEXHULL ] )
        {
            ofSetColor( ofColor::pink );
            ofDrawBitmapString( "-> Convex", 20.0f, yOffset + 20.0f );
            drawPolylineSamples( getPolylineSamples( CONVEXHULL_TAG ) );
            
            yOffset += 20.0f;
        }
        
        if( (bool)params[ PARAM_NAME_VIS_SKLELETON ] )
        {
            ofSetColor( ofColor::salmon );
            ofDrawBitmapString( "-> Skeleton", 20.0f, yOffset + 20.0f );
            drawPolylineSamples( getPolylineSamples( SKELETON_TAG ) );
        }
        
        if( (bool)params[ PARAM_NAME_VIS_CENTROID_IDS ] )
        {
            ofSetColor( ofColor::white );
            
            for( std::vector<PointSampleT>::const_iterator it = getPointSamples( CENTROID_TAG )->begin(); it != getPointSamples( CENTROID_TAG )->end(); ++it )
            {
                ofDrawBitmapString( ofToString( it->getSampleID() ), it->getSample() + ofPoint( 8.0f, 0.0f ) );
            }
        }
        
        if( (bool)params[ PARAM_NAME_VIS_TIPS_IDS ] )
        {
            ofSetColor( ofColor::yellow );
            
            for( PointSampleVectorVectorT::const_iterator it = getPointVectorSamples( TIPS_TAG )->begin(); it != getPointVectorSamples( TIPS_TAG )->end(); ++it )
            {
                for( PointSampleVectorT::const_iterator pit = it->begin(); pit != it->end(); ++pit )
                {
                    ofDrawBitmapString( ofToString( pit->getSampleID() ), pit->getSample() + ofPoint( 4.0f, 0.0f ) );
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
    
    void processCentroids( float trackingDistance, float trackingPersistance )
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
            pointSamplesMapDeque.back()[ CENTROID_TAG ]->push_back( PointSampleT() );
            
            pointSamplesMapDeque.back()[ CENTROID_TAG ]->back().setSample( ofPoint(ofxCv::toOf( allFloatPoints[i] ) ) );
            
            if( centroidTracker.existsPrevious( i ) )
            {
                pointSamplesMapDeque.back()[ CENTROID_TAG ]->back().setVelocity( ofPoint(ofxCv::toOf( allFloatPoints[ i ] ) ) - ofPoint(ofxCv::toOf( centroidTracker.getPrevious( i ) ) ) );
            } else {
                pointSamplesMapDeque.back()[ CENTROID_TAG ]->back().setVelocity( ofPoint() );
            }
            
            pointSamplesMapDeque.back()[ CENTROID_TAG ]->back().setSampleID( centroidTracker.getLabelFromIndex( i ) );
            pointSamplesMapDeque.back()[ CENTROID_TAG ]->back().setGroupID( i );
        }
    }
    
    void processTips( int resampleCount, int smoothAmount, float simplify, float tipThreshold, float trackingDistance, float trackingPersistance )
    {
        vector<cv::Point2f> allFloatPoints;
        vector<int>         groupIDs;
        
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
            
            pointVectorSamplesMapDeque.back()[ TIPS_TAG ]->back().push_back( PointSampleT() );
            
            pointVectorSamplesMapDeque.back()[ TIPS_TAG ]->back().back().setSample( ofPoint(ofxCv::toOf( allFloatPoints[i] ) ) );
            
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
    
    void processContour( vector<unsigned int> & labels, int resampleCount, int smoothAmount, float simplify )
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
            
            polylineSamplesMapDeque.back()[ CONTOUR_TAG ]->push_back( PolylineSampleT() );
            
            polylineSamplesMapDeque.back()[ CONTOUR_TAG ]->back().setSample( polyline );
            polylineSamplesMapDeque.back()[ CONTOUR_TAG ]->back().setVelocity( ofPoint( ofxCv::toOf( contourFinder.getVelocity( i ) ) ) );
            polylineSamplesMapDeque.back()[ CONTOUR_TAG ]->back().setSampleID( labels[i] );
            polylineSamplesMapDeque.back()[ CONTOUR_TAG ]->back().setGroupID( i );
        }
    }
    
    void processConvexHull( vector<unsigned int> & labels, int resampleCount, int smoothAmount, float simplify )
    {        
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
            
            polylineSamplesMapDeque.back()[ CONVEXHULL_TAG ]->push_back( PolylineSampleT() );
            
            polylineSamplesMapDeque.back()[ CONVEXHULL_TAG ]->back().setSample( polyline );
            polylineSamplesMapDeque.back()[ CONVEXHULL_TAG ]->back().setVelocity( ofPoint( ofxCv::toOf( contourFinder.getVelocity( i ) ) ) );
            polylineSamplesMapDeque.back()[ CONVEXHULL_TAG ]->back().setSampleID( labels[i] );
            polylineSamplesMapDeque.back()[ CONVEXHULL_TAG ]->back().setGroupID( i );
        }
    }
    
    void processSkeletons( vector<unsigned int> & labels, int resampleCount, int smoothAmount, float simplify )
    {
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
            
            polylineSamplesMapDeque.back()[ SKELETON_TAG ]->push_back( PolylineSampleT() );
            
            polylineSamplesMapDeque.back()[ SKELETON_TAG ]->back().setSample( polyline );
            polylineSamplesMapDeque.back()[ SKELETON_TAG ]->back().setVelocity( ofPoint( ofxCv::toOf( contourFinder.getVelocity( i ) ) ) );
            polylineSamplesMapDeque.back()[ SKELETON_TAG ]->back().setSampleID( labels[i] );
            polylineSamplesMapDeque.back()[ SKELETON_TAG ]->back().setGroupID( i );
            
            ++i;
        }
    };
    
    void drawPointSamples( const PointSampleVectorRefT & samples, float circleR )
    {
        for( PointSampleVectorT::const_iterator it = samples->begin(); it != samples->end(); ++it )
        {
            ofCircle( it->getSample(), circleR );
            ofLine( it->getSample() , it->getSample() + it->getVelocity().normalized() * circleR * 2.0f );
        }
    }
    
    void drawPointVectorSamples( const PointSampleVectorVectorRefT & samples, float circleR )
    {
        for( PointSampleVectorVectorT::const_iterator it = samples->begin(); it != samples->end(); ++it )
        {
            for( PointSampleVectorT::const_iterator pit = it->begin(); pit != it->end(); ++pit )
            {
                ofCircle( pit->getSample(), circleR );
                ofLine( pit->getSample() , pit->getSample() + pit->getVelocity().normalized() * circleR * 2.0f );
            }
        }
    }
    
    void drawPolylineSamples( const PolylineSampleVectorRefT & samples )
    {
        PolylineSampleVectorRefT newRef = samples;
        
        for( PolylineSampleVectorT::iterator it = newRef->begin(); it != newRef->end(); ++it )
        {
            it->getSample().draw();
        }
    }
    
private:
    
    bool                                isCurrentFrameNew;
    
    ofImage                             *image;
    
    ofxCv::RectTracker                  boundingBoxTracker;
    
    ofxCv::PointTracker                 tipTracker;
    ofxCv::PointTracker                 centroidTracker;
    
    ofxCv::ContourFinder2               contourFinder;
    
    ofxCv::SkeletonFinder               skeletonFinder;
    
    std::vector<cv::Rect>               bboxes;
        
    PointSampleVectorRefMapDequeT       pointSamplesMapDeque;
    PointSampleVectorVectorRefMapDequeT pointVectorSamplesMapDeque;
    PolylineSampleVectorRefMapDequeT    polylineSamplesMapDeque;
    
};