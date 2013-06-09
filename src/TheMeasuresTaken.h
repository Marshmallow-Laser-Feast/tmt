#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"

#include "ofMain.h"
#include "ofxTimeline.h"

#include "ofxMSAControlFreak/src/ofxMSAControlFreak.h"
#include "ofxMSAControlFreakGui/src/ofxMSAControlFreakGui.h"

#include "Input.h"
#include "MultiTouchInput.h"
#include "FlockingInput.h"

#include "InputAnalyser.h"

////////////////////////////
//   PROJECTION SETTINGS  //
////////////////////////////

#define PROJECTION_AREA_WIDTH           1080
#define PROJECTION_AREA_HEIGHT          600

////////////////////////////
//     INPUT SETTINGS     //
////////////////////////////

#define INPUT_WIDTH                     640
#define INPUT_HEIGHT                    480
#define INPUT_TIMEOUT_FRAMES            60
#define INPUT_COUNT                     2

#define MAX_PATH_ANALYSER_SAMPLES       60
#define MAX_PATH_ANALYSER_HISTORY       60 * 5
#define MAX_PATH_ANALYSER_LENGTH        3700.0f

// Flocking input settings

#define FLOCKING_SAMPLE_COUT            10
#define FLOCKING_MAX_SPEED              10.0
#define FLOCKING_COLUMNS                20
#define FLOCKING_ROWS                   20
#define FLOCKING_MIN_FORCE              -200
#define FLOCKING_MAX_FORCE              100
#define FLOCKING_ATTRAC_RAD_RATIO       0.8f

// Gui & Params

#define PARAM_NAME_CURRENT_INPUT        "Input"

#define GUIDE_STRING                    "<f> Toggle Fullscreen  <s> Save Settings  <p> Toggle GUI  <t>  Toggle Timeline  <i>  Toggle Input Visualization"

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
    void drawInputVisualization();
    
private:
    
    // Inputs
    
    MultiTouchInput     *multitouchInput;
    FlockingInput       *flockingInput;
    
    InputAnalyser       *multiTouchInputAnalyser;
    InputAnalyser       *flockingInputAnalyser;
    
    InputAnalyser       *inputAnalysers[ INPUT_COUNT ];
    
    InputAnalyser       *currentInputAnalyser;
    
    // Gui & Params
    
    msa::controlfreak::ParameterGroup   params;
    msa::controlfreak::gui::Gui         gui;
    
    // Timeline
    
    ofxTimeline                         timeline;
    
    // Visualisation
    
    bool                                visualizeInput;
};
