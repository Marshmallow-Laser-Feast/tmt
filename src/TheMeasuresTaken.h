#pragma once

#include "ofMain.h"

#include "ofxMidi.h"

#include "ofxEtherdream.h"
#include "ofxIldaFrame.h"

#include "ofxMSAControlFreak/src/ofxMSAControlFreak.h"
#include "ofxMSAControlFreakGui/src/ofxMSAControlFreakGui.h"

#include "Grabber.h"

#include "Params.h"

#include "Input.h"
#include "IImageSeqInput.h"

#include "MultiTouchInput.h"
#include "FlockingInput.h"
#include "CameraCentroidsInput.h"
#include "CameraConvexHullInput.h"
#include "CameraContourInput.h"

#include "InputAnalyser.h"

#include "IVisualizer.h"
#include "DotVisualizer.h"
#include "DotTrailsVisualizer.h"
#include "ConnectedDotsVisualizer.h"
#include "NearestDotsVisualizer.h"
#include "LineVisualizer.h"
#include "FixedPointVisualizer.h"
#include "QualitiesVisualizer.h"

#include "ConvexHullVisualizer.h"

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
    CameraConvexHullInput               *cameraConvexHullInput;
    CameraContourInput                  *cameraContourInput;
    
    IImageSeqInput                      *iimageSeqInputs[IMAGESEQINPUT_COUNT];
    
    InputAnalyser                       *multiTouchInputAnalyser;
    InputAnalyser                       *flockingInputAnalyser;
    InputAnalyser                       *cameraCentroidsInputAnalyser;
    InputAnalyser                       *cameraConvexHullInputAnalyser;
    InputAnalyser                       *cameraContourAnalyser;
    
    InputAnalyser                       *inputAnalysers[ INPUT_COUNT ];
    
    InputAnalyser                       *currentInputAnalyser;
    
    // Gui & Params
    
    msa::controlfreak::ParameterGroup   inputParams;
    msa::controlfreak::ParameterGroup   cameraCentroidInputParams;
    msa::controlfreak::ParameterGroup   cameraConvexHullInputParams;
    msa::controlfreak::ParameterGroup   cameraContourInputParams;
    
    msa::controlfreak::ParameterGroup   cameraParams;
    msa::controlfreak::ParameterGroup   outputParams;
    msa::controlfreak::ParameterGroup   ildaParams;
    
    msa::controlfreak::gui::Gui         gui;
    
    // Timeline
    
    ofxMidiIn                           midiIn;
	ofxMidiMessage                      midiMessage;
    
    // Visualisation
    
    ofVec3f                             offset;
    ofVec3f                             scale;
    
//    IVisualizer                         *visualizers[ VISUALIZER_COUNT ];
    vector<IVisualizer*>                visualizers;
    
    DotVisualizer                       *dotVisualizer;
    DotTrailsVisualizer                 *dotTrailsVisualizer;
    ConnectedDotsVisualizer             *connectedDotVisualizer;
    NearestDotsVisualizer               *nearestDotsVisualizer;
    LineVisualizer                      *lineVisualizer;
    FixedPointVisualizer                *fixedPointVisualizer;
    QualitiesVisualizer                 *qualitiesVisualizer;
    ConvexHullVisualizer                *convexHullVisualizer;
    
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
