#pragma once

#include "ofMain.h"

#include "ofMain.h"
#include "ofxTimeline.h"

#include "ofxEtherdream.h"
#include "ofxIldaFrame.h"

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
#include "ConnectedDotsVisualizer.h"
#include "NearestDotsVisualizer.h"
#include "LineVisualizer.h"
#include "FixedPointVisualizer.h"

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

#define FLOCKING_SAMPLE_COUT                    20
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
#define PARAM_NAME_CAMERA_DRAW_COLOR            "Draw Color Input"
#define PARAM_NAME_CAMERA_DRAW_THRESHOLD        "Draw Threshold Input"
#define PARAM_NAME_CAMERA_SCREEN_SCALE          "Screen Scale"

#define PARAM_NAME_CURRENT_OUTPUT               "Output"

#define PARAM_NAME_CALIBRATION_X1               "X1"
#define PARAM_NAME_CALIBRATION_Y1               "Y1"
#define PARAM_NAME_CALIBRATION_X2               "X2"
#define PARAM_NAME_CALIBRATION_Y2               "Y2"

#define PARAM_NAME_VIS_DOT_TRAILS_MIN           "Min Dot Trails"
#define PARAM_NAME_VIS_DOT_TRAILS_MAX           "Max Dot Trails"

#define PARAM_NAME_VIS_CONNECTED_DOT_COUNT_MIN  "Min Connected Dots"
#define PARAM_NAME_VIS_CONNECTED_DOT_COUNT_MAX  "Max Connected Dots"

#define PARAM_NAME_VIS_CONNECTED_DOT_OFFSET_MIN "Min Connected Dots Offset"
#define PARAM_NAME_VIS_CONNECTED_DOT_OFFSET_MAX "Max Connected Dots Offset"

#define PARAM_NAME_VIS_NEAREST_DOT_COUNT_MIN    "Min Nearest Dots"
#define PARAM_NAME_VIS_NEAREST_DOT_COUNT_MAX    "Max Nearest Dots"

#define PARAM_NAME_VIS_LINE_COUNT_MIN           "Min Line Visualizer Count"
#define PARAM_NAME_VIS_LINE_COUNT_MAX           "Max Line Visualizer Count"

#define PARAM_NAME_VIS_FIXED_POINT_COUNT_MIN    "Min Fixed Point Visualizer Count"
#define PARAM_NAME_VIS_FIXED_POINT_COUNT_MAX    "Max Fixed Point Visualizer Count"

#define PARAM_NAME_VIS_FIXED_POINT_OFFSET_MIN   "Min Fixed Point Visualizer Offset"
#define PARAM_NAME_VIS_FIXED_POINT_OFFSET_MAX   "Max Fixed Point Visualizer Offset"

#define PARAM_NAME_ILDA_FLIPX                   "Flip X"
#define PARAM_NAME_ILDA_FLIPY                   "Flip Y"

#define PARAM_NAME_ILDA_DOCAPX                  "Cap X"
#define PARAM_NAME_ILDA_DOCAPY                  "Cap Y"

#define PARAM_NAME_ILDA_MIN_POINT_COUNT         "Min Point Count"
#define PARAM_NAME_ILDA_MAX_POINT_COUNT         "Max Point Count"

#define PARAM_NAME_ILDA_OUTPUT_CALIBRATION_ONLY "Calibration Only"

#define GUIDE_STRING                            "<f> Toggle Fullscreen  <s> Save Settings  <p> Toggle GUI  <t>  Toggle Timeline  <i>  Toggle Input Visualization"

////////////////////////////
//     Visualization      //
////////////////////////////

#define SCREEN_VIS_AREA_WIDTH                   1080
#define SCREEN_VIS_AREA_HEIGHT                  600

#define VISUALIZER_COUNT                        6

////////////////////////////
//  Narrative & Timeline  //
////////////////////////////

#define TIMELINE_DURATION_IN_SECONDS            15 * 60

#define CURVE_ILDA_COLOR                        "Laser Color"

#define CURVE_ILDA_POINT_COUNT                  "ILDA Point Count"
#define CURVE_ILDA_DRAW_LINES                   "ILDA Draw Lines"
#define CURVE_ILDA_DRAW_POINTS                  "ILDA Draw Points"

#define CURVE_DOTVISUALIZER_DOT_RATIO           "Dot Visualizer Ratio"

#define CURVE_DOTTRAILSVISUALIZER_DOT_RATIO     "Dot Trails Visualizer Ratio"
#define CURVE_DOTTRAILSVISUALIZER_TRAILS_COUNT  "Dot Trails Visualizer Count"

#define CURVE_NEARESTDOTVISUALIZER_COUNT        "Nearst Dots Visualizer Count"

#define CURVE_CONNECTEDVISUALIZER_COUNT         "Connected Dots Count"
#define CURVE_CONNECTEDVISUALIZER_OFFSET        "Connected Dots Offset"

#define CURVE_LINEVISUALIZER_COUNT              "Line Visualizer Count"

#define CURVE_FIXEDPOINTVISALIZER_COUNT         "Fixed Point Visualizer Count"
#define CURVE_FIXEDPOINTVISALIZER_OFFSET        "Fixed Point Visualizer Offset"

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
    msa::controlfreak::ParameterGroup   cameraParams;
    msa::controlfreak::ParameterGroup   visualizationParams;
    msa::controlfreak::ParameterGroup   outputParams;
    msa::controlfreak::ParameterGroup   ildaParams;
    
    msa::controlfreak::gui::Gui         gui;
    
    // Timeline
    
    ofxTimeline                         timeline;
    
    // Visualisation
    
    ofVec3f                             offset;
    ofVec3f                             scale;
    
    IVisualizer                         *visualizers[ VISUALIZER_COUNT ];
    
    DotVisualizer                       *dotVisualizer;
    DotTrailsVisualizer                 *dotTrailsVisualizer;
    ConnectedDotsVisualizer             *connectedDotVisualizer;
    NearestDotsVisualizer               *nearestDotsVisualizer;
    LineVisualizer                      *lineVisualizer;
    FixedPointVisualizer                *fixedPointVisualizer;
    
    std::vector<ofPolyline>             visualizationData;
    
    bool                                visualizeInput;
    
    // Ilda
    
    ofxIlda::Frame                      ildaFrame;
    ofxEtherdream                       etherdream;
};
