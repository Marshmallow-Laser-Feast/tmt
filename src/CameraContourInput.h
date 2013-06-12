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
        params.addNamedIndex("Type").setLabels(4, "Contours", "Convex Hull", "Centroid", "Tips");
        params.addInt( PARAM_NAME_CONTOUR_THRESHOLD ).setRange(0, 255).setClamp(true);
        params.addInt( PARAM_NAME_CONTOUR_BLUR ).setRange(0, 50).setClamp(true);
        params.addInt( PARAM_NAME_CONTOUR_DILATE ).setRange(0, 50).setClamp(true);;
        params.addInt( PARAM_NAME_CONTOUR_ERODE ).setRange(0, 50).setClamp(true);;
        params.addInt( PARAM_NAME_CONTOUR_MIN_CONTOUR ).setRange(0, 100).setClamp(true);;
        params.addInt( PARAM_NAME_CONTOUR_MAX_CONTOUR ).setRange(0, 10000).setClamp(true);;
        params.addFloat( PARAM_NAME_CONTOUR_SIMPLIFY ).setRange( 0.0f, 50.0f ).setClamp( true ).setIncrement( 0.01f );
        params.addInt(PARAM_NAME_RESAMPLE_COUNT).setClamp(true).setRange(0, 1000);
        params.addFloat(PARAM_NAME_CONTOUR_STRETCH).setClamp(true);
        params.addFloat(PARAM_NAME_TIP_THRESHOLD).setRange(-1, 1).setClamp(true);
        params.addFloat(PARAM_NAME_TRACKING_DISTANCE).setClamp(true).setRange(0, 640);
        params.addInt(PARAM_NAME_TRACKING_PERSISTENCE).setClamp(true).setRange(0, 100);
        //        params.addFloat(PARAM_NAME_SMOOTHING).setClamp(true);
        params.addFloat("Debug point size").setRange(0, 10).setClamp(true);
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
            int analysisType = params["Type"];
            contourFinder.setThreshold(params[PARAM_NAME_CONTOUR_THRESHOLD]);
            contourFinder.setBlur(params[PARAM_NAME_CONTOUR_BLUR]);
            contourFinder.setDilate(params[PARAM_NAME_CONTOUR_DILATE]);
            contourFinder.setErode(params[PARAM_NAME_CONTOUR_ERODE]);
            contourFinder.setMinAreaRadius(params[PARAM_NAME_CONTOUR_MIN_CONTOUR]);
            contourFinder.setMaxAreaRadius(params[PARAM_NAME_CONTOUR_MAX_CONTOUR]);
            float simplify = params[PARAM_NAME_CONTOUR_SIMPLIFY];
            int resampleCount = params[PARAM_NAME_RESAMPLE_COUNT];
            float stretch = params[PARAM_NAME_CONTOUR_STRETCH];
            float tipThreshold = params[PARAM_NAME_TIP_THRESHOLD];
            float trackingDistance = params[PARAM_NAME_TRACKING_DISTANCE];
            float trackingPersistance = params[PARAM_NAME_TRACKING_PERSISTENCE];
            //            float smoothing = params[PARAM_NAME_SMOOTHING];
            
            

            // find contours
            ofImage image;
            float w = roiX2 - roiX1;
            float h = roiY2 - roiY1;
            image.setFromPixels( *pixels );
            image.crop( roiX1 * width , roiY1 * height, w * width, h * height );
            contourFinder.findContours( image );
            
            
            lines.clear();
            vector<cv::Point2f>     allFloatPoints;
            
            // iterate all contours
            for( int i = 0; i < contourFinder.size(); ++i )
            {
                // create new line
                lines.push_back(ofPolyline());
                ofPolyline& line = lines.back();
                cv::Rect r(contourFinder.getBoundingRect(i));
                
//                ofPolyline contour;
                bool doFindTip = false;
                switch(analysisType) {
                    case 0: line = ofxCv::toOf(contourFinder.getContour(i)); break;
                    case 1: line = ofxCv::toOf(contourFinder.getConvexHull(i)); break;
                    case 2: line.addVertex(ofxCv::toOf(contourFinder.getCentroid(i))); break;
                    case 3: doFindTip = true; line = ofxCv::toOf(contourFinder.getContour(i));  break;
                }
                line.close();
                
                // iterate all points on contour
                for(int j=0; j<line.size(); ++j ) {
                    if(doFindTip) {
//                        ofVec2f
                    }
                    
                    ofPoint p = line[j];
//                    p += ofPoint( roiX1 * width, roiY1 * height );
                    if(stretch > 0) {
                        ofVec2f pNorm;
                        pNorm.x = ofMap(p.x, r.x, r.x + r.width, 0, image.width);
                        pNorm.y = ofMap(p.y, r.y, r.y + r.height, 0, image.height);
                        p.interpolate(pNorm, stretch);
                    }
                    line[j] = p;
                }
                
//                line.close();
                
                if(resampleCount) line = line.getResampledByCount(resampleCount);   // TEST
                if(simplify > 0) line.simplify( simplify );
                
                for ( int k = 0; k < line.getVertices().size(); ++k )
                {
                    allFloatPoints.push_back( cv::Point2f( line.getVertices()[k].x, line.getVertices()[k].y ) );
                }
            }


            // track points
            if(trackingDistance > 0.1) {
                pointTracker.setMaximumDistance(trackingDistance);
                pointTracker.setPersistence(trackingPersistance);
                pointTracker.track( allFloatPoints );
            }
            
            // add points to samples and mesh (for debug draw)
            samplesMesh.clear();
            samples.clear();
            for( int i  = 0; i < allFloatPoints.size(); ++i )
            {
                PointInputSampleT   sample;
                int label = (trackingDistance > 0.1) ? pointTracker.getLabelFromIndex(i) : i;
                sample.setSampleID(label);
                
                ofPoint p(ofxCv::toOf( allFloatPoints[i] ) );
//                p-= ofPoint( roiX1 * width, roiY1 * height );
                sample.setSample(p);
                samples.push_back( sample );
                samplesMesh.addVertex(p);
            }
        }
    };
    
    virtual const vector<PointInputSampleT> & getSamples() const
    {
        return samples;
    };
    
    virtual void drawDebug()
    {
        ofPushStyle();
        for(int i=0; i<lines.size(); i++) {
            lines[i].draw();
        }
        float pointSize = params["Debug point size"];
        if(pointSize > 0.1) {
            glPointSize(pointSize);
            ofSetColor(0, 200, 0);
            samplesMesh.drawVertices();
        }
        ofPopStyle();

    };
    
private:
    
    ofxCv::ContourFinder2           contourFinder;
    ofxCv::PointTracker             pointTracker;
    vector<ofPolyline>              lines;

    std::vector<PointInputSampleT>  samples;
    ofMesh samplesMesh;
};