#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"

#include "PathAnalyser.h"

#define MAX_SEGMENT_ROTATION        1.0f
#define MAX_PATH_LENGTH             3700.0f

#define VIDEO_RES_WIDTH             640
#define VIDEO_RES_HEIGHT            480

#define VISUALISATION

#ifdef VISUALISATION

#define MAX_VISUALISATION_SAMPLES   200
#define VISUALISATION_WIDTH         200.0f
#define VISUALISATION_HEIGHT        160.0f

#endif


class TheMeasuresTaken : public ofBaseApp
{

public:
    
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
private:
    
#ifdef VISUALISATION
    
    void visualize( const deque<float> &values, const float width, const float height );
    
#endif
    
private:
    
    PathAnalyser        pathAnalyser;
    
#ifdef LIVE_INPUT
    
    ofVideoGrabber      videoGrabber;
    
    ofxCvColorImage     videoColorImage;
    ofxCvGrayscaleImage videoGrayImage;
    ofxCvContourFinder  videoContourFinder;
    
    int                 videoThresholdingValue;
    
    int                 minBlobArea;
    int                 maxBlobArea;
    
#endif
    
#ifdef VISUALISATION
    
    bool                visualizeAmplitude;
    bool                visualizeCurvature;
    bool                visualizeAcceleration;
    bool                visualizeVelocity;
    
#endif
    
};
