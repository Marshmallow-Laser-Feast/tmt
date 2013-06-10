#pragma once

#include "ofMain.h"

#include "ofMain.h"
#include "ofxTimeline.h"

#include "ofxMSAControlFreak/src/ofxMSAControlFreak.h"
#include "ofxMSAControlFreakGui/src/ofxMSAControlFreakGui.h"

#include "Input.h"
#include "MultiTouchInput.h"
#include "FlockingInput.h"
#include "CameraInput.h"

#include "InputAnalyser.h"

#include "IVisualizer.h"
#include "DotVisualizer.h"
#include "DotTrailsVisualizer.h"

////////////////////////////
//      APP SETTINGS      //
////////////////////////////

#define FRAMERATE                               60

////////////////////////////
//   PROJECTION SETTINGS  //
////////////////////////////

#define PROJECTION_AREA_WIDTH                   1080
#define PROJECTION_AREA_HEIGHT                  600

////////////////////////////
//     INPUT SETTINGS     //
////////////////////////////

#define INPUT_WIDTH                             640
#define INPUT_HEIGHT                            480
#define INPUT_TIMEOUT_FRAMES                    60
#define INPUT_COUNT                             3

#define MAX_PATH_ANALYSER_SAMPLES               60
#define MAX_PATH_ANALYSER_HISTORY               60 * 5
#define MAX_PATH_ANALYSER_LENGTH                3700.0f

// Flocking input settings

#define FLOCKING_SAMPLE_COUT                    10
#define FLOCKING_MAX_SPEED                      20.0
#define FLOCKING_COLUMNS                        20
#define FLOCKING_ROWS                           20
#define FLOCKING_MIN_FORCE                      -200
#define FLOCKING_MAX_FORCE                      100
#define FLOCKING_ATTRAC_RAD_RATIO               0.8f

////////////////////////////
//      Gui & Params      //
////////////////////////////

#define PARAM_NAME_CURRENT_INPUT                "Input"

#define PARAM_NAME_CAMERA_THRESHOLD             "Camera Threshold"
#define PARAM_NAME_CAMERA_MIN_BLOB_SIZE         "Camera Min Blob Size"

#define PARAM_NAME_CURRENT_OUTPUT               "Output"

#define PARAM_NAME_CALIBRATION_X1               "X1"
#define PARAM_NAME_CALIBRATION_Y1               "Y1"
#define PARAM_NAME_CALIBRATION_X2               "X2"
#define PARAM_NAME_CALIBRATION_Y2               "Y2"

#define PARAM_NAME_VIS_DOT_TRAILS_MIN           "Min Dot Trails"
#define PARAM_NAME_VIS_DOT_TRAILS_MAX           "Max Dot Trails"

#define GUIDE_STRING                            "<f> Toggle Fullscreen  <s> Save Settings  <p> Toggle GUI  <t>  Toggle Timeline  <i>  Toggle Input Visualization"

////////////////////////////
//     Visualization      //
////////////////////////////

#define SCREEN_VIS_AREA_WIDTH                   1080
#define SCREEN_VIS_AREA_HEIGHT                  600

#define VISUALIZER_COUNT                        2

////////////////////////////
//  Narrative & Timeline  //
////////////////////////////

#define TIMELINE_DURATION_IN_SECONDS            15 * 60

#define CURVE_DOTVISUALIZER_DOT_RATIO           "Dot Visualizer Ratio"
#define CURVE_DOTTRAILSVISUALIZER_DOT_RATIO     "Dot Trails Visualizer Ratio"
#define CURVE_DOTTRAILSVISUALIZER_TRAILS_COUNT  "Dot Trails Visualizer Count"

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
    
    MultiTouchInput                     *multitouchInput;
    FlockingInput                       *flockingInput;
    CameraInput                         *cameraInput;
    
    InputAnalyser                       *multiTouchInputAnalyser;
    InputAnalyser                       *flockingInputAnalyser;
    InputAnalyser                       *cameraInputAnalyser;
    
    InputAnalyser                       *inputAnalysers[ INPUT_COUNT ];
    
    InputAnalyser                       *currentInputAnalyser;
    
    // Gui & Params
    
    msa::controlfreak::ParameterGroup   inputParams;
    msa::controlfreak::ParameterGroup   visualizationParams;
    msa::controlfreak::ParameterGroup   outputParams;
    
    msa::controlfreak::gui::Gui         gui;
    
    // Timeline
    
    ofxTimeline                         timeline;
    
    // Visualisation
    
    ofVec3f                             offset;
    ofVec3f                             scale;
    
    IVisualizer                         *visualizers[ VISUALIZER_COUNT ];
    
    DotVisualizer                       *dotVisualizer;
    DotTrailsVisualizer                 *dotTrailsVisualizer;
    
    std::vector<ofPolyline>             visualizationData;
    
    bool                                visualizeInput;
};
