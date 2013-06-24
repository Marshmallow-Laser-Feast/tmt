//
//  PointSampleSmoothing.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/24/13.
//
//

#pragma once

#include "ofMain.h"

#include "ofxCv.h"

#include "InputSample.h"

class PointSampleSmoother
{
    
public:
    
    PointSampleSmoother()
    {
//        using namespace ofxCv;
//        using namespace cv;
//        
//        kalmanFilter.init(4, 2, 0);
//        
//        kalmanFilter.transitionMatrix = *(Mat_<float>(4, 4) <<
//                                1,0,1,0,
//                                0,1,0,1,
//                                0,0,1,0,
//                                0,0,0,1);
//		
//        measurement = Mat_<float>::zeros(2,1);
//        
//        setIdentity(kalmanFilter.measurementMatrix);
//        setIdentity(kalmanFilter.processNoiseCov, Scalar::all(1e-4));
//        setIdentity(kalmanFilter.measurementNoiseCov, Scalar::all(1e-1));
//        setIdentity(kalmanFilter.errorCovPost, Scalar::all(-.1));
        
        firstSampleSet  = false;
    };
    
    ~PointSampleSmoother(){};
    
public:
    
    ofPoint getSmoothed( const ofPoint & sample, float smoothing )
    {
//        using namespace ofxCv;
//        using namespace cv;
        
        if( !firstSampleSet )
        {
            firstSampleSet      = true;
            
            oldSample.set( sample );
            
            return sample;
            
//            kalmanFilter.statePre.at<float>(0) = sample.x;
//            kalmanFilter.statePre.at<float>(1) = sample.y;
//            kalmanFilter.statePre.at<float>(2) = 0;
//            kalmanFilter.statePre.at<float>(3) = 0;
//            
//            return sample;
        } else {
                        
            oldSample   = sample.getInterpolated( oldSample , smoothing );
            
            return oldSample;
            
//            ofPoint result;
//            Mat     prediction  = kalmanFilter.predict();
//            
//            cv::Point predictPt(prediction.at<float>(0),prediction.at<float>(1));
//            
//            result.set( toOf(predictPt) );
//            
//            measurement(0)      = sample.x;
//            measurement(1)      = sample.y;
//            
//            kalmanFilter.correct(measurement);
//            
//            return result;
        }
        
        return ofPoint();
    };
    
private:
    
    bool                firstSampleSet;
    ofPoint             oldSample;
    
//    cv::KalmanFilter    kalmanFilter;
//	cv::Mat_<float>     measurement;
};

