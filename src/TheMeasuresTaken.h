#pragma once

#include "ofMain.h"

#include "ofxMidi.h"

#include "ofxEtherdream.h"
#include "ofxIldaFrame.h"

#include "ofxMSAControlFreak/src/ofxMSAControlFreak.h"
#include "ofxMSAControlFreakGui/src/ofxMSAControlFreakGui.h"

#include "Grabber.h"

#include "Input.h"
#include "IImageSeqInput.h"

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
#include "QualitiesVisualizer.h"

////////////////////////////
//      APP SETTINGS      //
////////////////////////////

#define FRAMERATE                               60

////////////////////////////
//     INPUT SETTINGS     //
////////////////////////////

#define INPUT_WIDTH                             640
#define INPUT_HEIGHT                            480
#define INPUT_TIMEOUT_FRAMES                    20
#define INPUT_COUNT                             3
#define IMAGESEQINPUT_COUNT                     1

#define MAX_PATH_ANALYSER_SAMPLES               60
#define MAX_PATH_ANALYSER_HISTORY               60 * 5
#define MAX_PATH_ANALYSER_LENGTH                3700.0f

// Flocking input settings

#define FLOCKING_SAMPLE_COUT                    60
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

#define PARAM_NAME_DOT_VIS_RATIO                "Dots Ratio"

#define PARAM_NAME_DOT_TRAILS_VIS_RATIO         "Dots Trails Ratio"
#define PARAM_NAME_DOT_TRAILS_VIS_TRAILS_COUNT  "Dots Trails Count"

#define PARAM_NAME_CONNECTED_DOT_VIS_COUNT      "Connected Dots Count"
#define PARAM_NAME_CONNECTED_DOT_VIS_N_OFFSET   "Connected Neighbor Offset"

#define PARAM_NAME_NEAREST_DOT_VIS_COUNT        "Nearest Dots Count"

#define PARAM_NAME_LINE_VIS_COUNT               "Line Vis Count"

#define PARAM_NAME_FIXED_POINT_VIS_COUNT        "Fixed Point Vis Count"
#define PARAM_NAME_FIXED_POINT_FIX              "Fixed Point Vis FIX Bang"
#define PARAM_NAME_FIXED_POINT_CLEAR            "Fixed Point Vis Clear Bang"

#define PARAM_NAME_ILDA_DRAW_LINES              "Draw Lines"
#define PARAM_NAME_ILDA_DRAW_POINTS             "Draw Points"

#define PARAM_NAME_ILDA_FLIPX                   "Flip X"
#define PARAM_NAME_ILDA_FLIPY                   "Flip Y"

#define PARAM_NAME_ILDA_DOCAPX                  "Cap X"
#define PARAM_NAME_ILDA_DOCAPY                  "Cap Y"

#define PARAM_NAME_ILDA_SMOOTH_AMOUNT           "Smooth Amount"
#define PARAM_NAME_ILDA_OPTIMIZE_TOLERANCE      "Optimize Tolerance"
#define PARAM_NAME_ILDA_COLLAPSE                "Collapse"
#define PARAM_NAME_ILDA_POINT_COUNT             "Point Count"
#define PARAM_NAME_ILDA_SPACING                 "Spacing"

#define PARAM_NAME_ILDA_OUTPUT_CALIBRATION_ONLY "Calibration Only"

#define GUIDE_STRING                            "<f> Toggle Fullscreen  <s> Save Settings  <p> Toggle GUI  <i>  Toggle Input Visualization  <l>  List Midi Ports"

////////////////////////////
//     Visualization      //
////////////////////////////

#define SCREEN_VIS_AREA_WIDTH                   1080
#define SCREEN_VIS_AREA_HEIGHT                  600

#define VISUALIZER_COUNT                        7

class TheMeasuresTaken : public ofBaseApp, public ofxMidiListener
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
    void newMidiMessage(ofxMidiMessage& eventArgs);
    
private:
    
    void drawVisualization();
    void drawVisualizationArea();
    void drawInputVisualization();
    
private:
    
    // Inputs
    
    MultiTouchInput                     *multitouchInput;
    FlockingInput                       *flockingInput;
    CameraInput                         *cameraInput;
    
    IImageSeqInput                      *iimageSeqInputs[IMAGESEQINPUT_COUNT];
    
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
    
    ofxMidiIn                           midiIn;
	ofxMidiMessage                      midiMessage;
    
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
    QualitiesVisualizer                 *qualitiesVisualizer;
    
    std::vector<ofPolyline>             visualizationData;
    
    bool                                visualizeInput;
    
    // Ilda
    
    ofxIlda::Frame                      ildaFrame;
    ofxEtherdream                       etherdream;
    
    // Camera
    
    ofPixelsSharedPtrT                  pixelsSharedPtr;
    
    ofxLibdc::Grabber                   grabber;
};
