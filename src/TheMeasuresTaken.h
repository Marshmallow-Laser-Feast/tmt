#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"

#include "PathAnalyser.h"

#define PROJECTION_AREA_WIDTH   1080
#define PROJECTION_AREA_HEIGHT  720


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
    
    void drawVisualization();
    void drawVisualizationArea();
    
};
