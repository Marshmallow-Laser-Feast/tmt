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
#include "CameraCentroidsInput.h"

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
#define INPUT_TIMEOUT_FRAMES                    5
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

#define PARAM_NAME_CAMERA_ROI_X1                "Camera ROI X1"
#define PARAM_NAME_CAMERA_ROI_Y1                "Camera ROI Y1"
#define PARAM_NAME_CAMERA_ROI_X2                "Camera ROI X2"
#define PARAM_NAME_CAMERA_ROI_Y2                "Camera ROI Y2"

#define PARAM_NAME_CAMERA_DRAW_COLOR            "Draw Color Input"
#define PARAM_NAME_CAMERA_DRAW_THRESHOLD        "Draw Threshold Input"
#define PARAM_NAME_CAMERA_DRAW_ROI              "Draw ROI"

#define PARAM_NAME_CAMERA_SCREEN_SCALE          "Screen Scale"
#define PARAM_NAME_LIBDC_BRIGHTNESS             "Libdc brightness"
#define PARAM_NAME_LIBDC_GAMMA                  "Libdc gamma"
#define PARAM_NAME_LIBDC_SHUTTER                "Libdc shutter"
#define PARAM_NAME_LIBDC_GAIN                   "Libdc gain"
#define PARAM_NAME_CAMERA_USE_VIDEO             "Use Video"
#define PARAM_NAME_VIDEO_PLAY                   "Video Play"
#define PARAM_NAME_VIDEO_FRAME                  "Video Frame"

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

#define PARAM_NAME_ILDA_BLANK_COUNT             "Blank Count"
#define PARAM_NAME_ILDA_END_COUNT               "End Count"
#define PARAM_NAME_ILDA_OFFSET_X                "Offset X"
#define PARAM_NAME_ILDA_OFFSET_Y                "Offset Y"
#define PARAM_NAME_ILDA_SCALE_X                 "Scale X"
#define PARAM_NAME_ILDA_SCALE_Y                 "Scale Y"

#define PARAM_NAME_POINT_COUNT_ORIG             "Point Count Orig"
#define PARAM_NAME_POINT_COUNT_PROC             "Point Count Processed"

#define PARAM_NAME_ILDA_SMOOTH_AMOUNT           "Smooth Amount"
#define PARAM_NAME_ILDA_OPTIMIZE_TOLERANCE      "Optimize Tolerance"
#define PARAM_NAME_ILDA_COLLAPSE                "Collapse"
#define PARAM_NAME_ILDA_POINT_COUNT             "Point Count"
#define PARAM_NAME_ILDA_SPACING                 "Spacing"

#define PARAM_NAME_ILDA_OUTPUT_CALIBRATION_ONLY "Calibration Only"

#define GUIDE_STRING                            "<f> Toggle Fullscreen  <s> Save Settings  <p> Toggle GUI  <i>  Toggle Input Visualization  <o> Fixed Point Set  <O> Fixed Point Clear  <l>  List Midi Ports"

////////////////////////////
//     Visualization      //
////////////////////////////

#define SCREEN_VIS_AREA_WIDTH                   700
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
    CameraCentroidsInputs               *cameraCentroidsInput;
    
    IImageSeqInput                      *iimageSeqInputs[IMAGESEQINPUT_COUNT];
    
    InputAnalyser                       *multiTouchInputAnalyser;
    InputAnalyser                       *flockingInputAnalyser;
    InputAnalyser                       *cameraCentroidsInputAnalyser;
    
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
    
//    std::vector<ofPolyline>             visualizationData;
    
    bool                                visualizeInput;
    
    // Ilda
    
    ofxIlda::Frame                      ildaFrame;
    ofxEtherdream                       etherdream;
    
    // Camera
    ofxLibdc::Grabber                   grabber;
    ofVideoPlayer                       videoPlayer;
    ofBaseVideo                         *videoPtr;
};
