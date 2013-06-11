#pragma once

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
//#define INPUT_COUNT                             5
//#define IMAGESEQINPUT_COUNT                     3

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

#define PARAM_NAME_ENABLED                      "Enabled"
#define PARAM_NAME_BRIGHTNESS                   "Brightness"
#define PARAM_NAME_TIME_OFFSET                  "Time Offset"
#define PARAM_NAME_FASTEST_COUNT                "Fastest Count"

#define PARAM_NAME_CAMERA_ROI_X1                "Camera ROI X1"
#define PARAM_NAME_CAMERA_ROI_Y1                "Camera ROI Y1"
#define PARAM_NAME_CAMERA_ROI_X2                "Camera ROI X2"
#define PARAM_NAME_CAMERA_ROI_Y2                "Camera ROI Y2"
#define PARAM_NAME_CAMERA_FLIP_X                "Camera Flip X"
#define PARAM_NAME_CAMERA_FLIP_Y                "Camera Flip Y"

#define PARAM_NAME_CAMERA_DRAW_COLOR            "Draw Color Input"
#define PARAM_NAME_CAMERA_DRAW_ROI              "Draw ROI"
#define PARAM_NAME_CAMERA_DRAW_CONTOURS         "Draw Contours"
#define PARAM_NAME_CAMERA_CONTOUR_SOURCE        "Contour Source"

#define PARAM_NAME_CONTOUR_THRESHOLD            "Threshold"
#define PARAM_NAME_CONTOUR_BLUR                 "Blur"
#define PARAM_NAME_CONTOUR_DILATE               "Dilate"
#define PARAM_NAME_CONTOUR_MIN_CONTOUR          "Min Contour"
#define PARAM_NAME_CONTOUR_MAX_CONTOUR          "Max Contour"
#define PARAM_NAME_CONTOUR_SIMPLIFY             "Simplify"
#define PARAM_NAME_CONTOUR_STRETCH              "Contour Stretch"


//#define CAMERA_CONVEX_HULL_THRESHOLD            "Threshold"
//#define CAMERA_CONVEX_HULL_BLUR                 "Blur"
//#define CAMERA_CONVEX_HULL_DILATE               "Dilate"
//#define CAMERA_CONVEX_HULL_MIN_CONTOUR          "Min Contour"
//#define CAMERA_CONVEX_HULL_MAX_CONTOUR          "Max Contour"
//#define CAMERA_CONVEX_HULL_SIMPLIFICATION       "Simplification"
//
//#define CAMERA_CONTOUR_THRESHOLD                "Threshold"
//#define CAMERA_CONTOUR_BLUR                     "Blur"
//#define CAMERA_CONTOUR_DILATE                   "Dilate"
//#define CAMERA_CONTOUR_MIN_CONTOUR              "Min Contour"
//#define CAMERA_CONTOUR_MAX_CONTOUR              "Max Contour"
//#define CAMERA_CONTOUR_SIMPLIFICATION           "Simplification"
//    
#define PARAM_NAME_CAMERA_SCREEN_SCALE          "Screen Scale"
#define PARAM_NAME_LIBDC_BRIGHTNESS             "Libdc brightness"
#define PARAM_NAME_LIBDC_GAMMA                  "Libdc gamma"
#define PARAM_NAME_LIBDC_SHUTTER                "Libdc shutter"
#define PARAM_NAME_LIBDC_GAIN                   "Libdc gain"
#define PARAM_NAME_CAMERA_USE_VIDEO             "Use Video"
#define PARAM_NAME_VIDEO_PLAY                   "Video Play"
#define PARAM_NAME_VIDEO_FRAME                  "Video Frame"

#define PARAM_NAME_CURRENT_OUTPUT               "Output"

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

#define GUIDE_STRING                            ("<f> Toggle Fullscreen  <s> Save Settings  <p> Toggle GUI  <i>  Toggle Input Visualization  <o> Fixed Point Set  <O> Fixed Point Clear  <l>  List Midi Ports   " + ofToString(ofGetFrameRate(), 2))

////////////////////////////
//     Visualization      //
////////////////////////////

#define SCREEN_VIS_AREA_WIDTH                   INPUT_WIDTH
#define SCREEN_VIS_AREA_HEIGHT                  INPUT_HEIGHT

//#define VISUALIZER_COUNT                        8
