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
        params.addInt(PARAM_NAME_CONTOUR_RESAMPLE).setClamp(true).setRange(0, 1000);
        params.addFloat( PARAM_NAME_CONTOUR_SIMPLIFY ).setRange( 0.0f, 50.0f ).setClamp( true ).setIncrement( 0.01f );
        params.addInt( PARAM_NAME_CONTOUR_SMOOTH ).setRange(0, 40).setClamp( true );
        params.addFloat(PARAM_NAME_TIP_THRESHOLD).setRange(-180, 180).setClamp(true);
        params.addFloat("Average radius").setClamp(true);   // proprotion to size of rectangle
        params.addFloat("Stretch points").setClamp(true); // stretch points relative to contour
        params.addFloat("Scale points X").setRange(1, 5).setClamp(true); // scale points from center of rect
        params.addFloat("Scale points Y").setRange(1, 5).setClamp(true); // scale points from center of rect
        params.addFloat(PARAM_NAME_TRACKING_DISTANCE).setClamp(true).setRange(0, 640);
        params.addInt(PARAM_NAME_TRACKING_PERSISTENCE).setClamp(true).setRange(0, 100);
//        params.addFloat(PARAM_NAME_SMOOTHING).setClamp(true);
        params.addFloat("Debug point size").setRange(0, 10).setClamp(true);
        
        midiMappings[ &params.get(PARAM_NAME_CONTOUR_RESAMPLE) ]    = std::pair<int, int>( 10, 14 );
        midiMappings[ &params.get(PARAM_NAME_CONTOUR_SIMPLIFY) ]    = std::pair<int, int>( 10, 15 );
        midiMappings[ &params.get(PARAM_NAME_CONTOUR_SMOOTH) ]      = std::pair<int, int>( 10, 16 );
        midiMappings[ &params.get("Average radius") ]               = std::pair<int, int>( 10, 17 );
        midiMappings[ &params.get("Stretch points") ]               = std::pair<int, int>( 10, 18 );
        midiMappings[ &params.get("Scale points X") ]               = std::pair<int, int>( 10, 19 );
        midiMappings[ &params.get("Scale points Y") ]               = std::pair<int, int>( 10, 20 );
        midiMappings[ &params.get(PARAM_NAME_TRACKING_DISTANCE) ]   = std::pair<int, int>( 10, 21 );
        
        oscMappings[ &params.get("Type") ]                          = "/CameraContourInput Type";
        oscMappings[ &params.get(PARAM_NAME_CONTOUR_BLUR) ]         = "/CameraContourInput Blur";
        oscMappings[ &params.get(PARAM_NAME_CONTOUR_RESAMPLE) ]     = "/CameraContourInput Resample";
        oscMappings[ &params.get(PARAM_NAME_CONTOUR_SMOOTH) ]       = "/CameraContourInput Smooth";
        oscMappings[ &params.get("Average radius") ]                = "/CameraContourInput Avarage Radius";
        oscMappings[ &params.get("Stretch points") ]                = "/CameraContourInput Stretch Points";
        oscMappings[ &params.get(PARAM_NAME_TRACKING_DISTANCE) ]    = "/CameraContourInput Tracking Distance";
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
            int resampleCount = params[PARAM_NAME_CONTOUR_RESAMPLE];
            float simplify = params[PARAM_NAME_CONTOUR_SIMPLIFY];
            int smoothAmount = params[PARAM_NAME_CONTOUR_SMOOTH];
            float tipThreshold = params[PARAM_NAME_TIP_THRESHOLD];
            float averageRadius = params["Average radius"];
            float stretchPoints = params["Stretch points"];
            ofVec2f scalePoints = ofVec2f(params["Scale points X"], params["Scale points Y"]);
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
            
            
            polys.clear();
            vector<cv::Point2f>     allFloatPoints;
            
            // iterate all contours
            for( int i = 0; i < contourFinder.size(); ++i )
            {
                // create new line
                polys.push_back(ofPolyline());
                ofPolyline& line = polys.back();
                cv::Rect r(contourFinder.getBoundingRect(i));
                
                switch(analysisType) {
                    case 0: { // contour
                        line = ofxCv::toOf(contourFinder.getContour(i));
                        if(resampleCount) line = line.getResampledByCount(resampleCount);
                        if(smoothAmount) line = line.getSmoothed(smoothAmount);
                        if(simplify > 0) line.simplify( simplify );
                    } break;
                        
                    case 1: { // convex hull
                        line = ofxCv::toOf(contourFinder.getConvexHull(i)); ;
                        if(resampleCount) line = line.getResampledByCount(resampleCount);
                        if(simplify > 0) line.simplify( simplify );
                        if(smoothAmount) line = line.getSmoothed(smoothAmount);
                    } break;
                        
                    case 2: line.addVertex(ofxCv::toOf(contourFinder.getCentroid(i))); break; // centroid

                    case 3: {// tips
                        ofPolyline poly = ofxCv::toOf(contourFinder.getContour(i));
                        poly.close();
                        if(resampleCount) poly = poly.getResampledByCount(resampleCount);
                        if(smoothAmount) poly = poly.getSmoothed(smoothAmount);
                        
                        for(int j=0; j<poly.size(); ++j ) {
                            if(poly.getAngleAtIndex(j) > tipThreshold) {
                                line.addVertex(poly[j]);
                            }
                        }
                        line.close();
                        if(simplify > 0) line.simplify( simplify );
                    } break;
                }
                // if we should average points
                if(averageRadius > 0.01) {
                    ofPolyline temp;
                    int count = 1;
                    float distThresh = averageRadius * (r.width + r.height)/2;
                    float distThresh2 = distThresh * distThresh;
                    for(int j=0; j<line.size(); j++) {
                        ofPoint p = line[j];
                        if(temp.size() > 0 && p.distanceSquared(temp.getVertices().back()) < distThresh2) {
                            ofPoint &back = temp.getVertices().back();
                            ofPoint curAvg = back * count;
                            count++;
                            back = (curAvg + p) / count;
                        } else {
                            count = 0;
                            temp.addVertex(p);
                        }
                    }
                    line = temp;
                    line.close();
                }

                // iterate all points on contour and stretch to bounding box
                for(int j=0; j<line.size(); ++j ) {
                    ofVec2f p = line[j];
                    //                    p += ofPoint( roiX1 * width, roiY1 * height );
                    if(stretchPoints > 0) {
                        ofVec2f pNorm;
                        pNorm.x = ofMap(p.x, r.x, r.x + r.width, 0, image.width);
                        pNorm.y = ofMap(p.y, r.y, r.y + r.height, 0, image.height);
                        p.interpolate(pNorm, stretchPoints);
                    }
                    
                    if(scalePoints.lengthSquared() > 1) {
                        ofVec2f rcenter(r.x + r.width/2, r.y + r.height/2);
                        ofVec2f diff(p-rcenter);
                        p = rcenter + diff * scalePoints;
                    }
                    
                    line[j] = p;
                    allFloatPoints.push_back(ofxCv::toCv(p));
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
    
    virtual const vector<ofPolyline>& getPolys() const {
        return polys;
    }

    
    virtual void drawDebug()
    {
        ofPushStyle();
        for(int i=0; i<polys.size(); i++) {
            polys[i].draw();
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
    vector<ofPolyline>              polys;
    
    std::vector<PointInputSampleT>  samples;
    ofMesh samplesMesh;
};