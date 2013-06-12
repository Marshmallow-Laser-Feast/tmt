#pragma once

#include "ofMain.h"

#include "ofxMidi.h"
#include "ofxOsc.h"

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
#include "ContourVisualizer.h"
#include "RoofVisualizer.h"

#include "IFilter.h"

#include "AudioDistorterFilter.h"
#include "NoiseDistortionFilter.h"

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
    void audioIn(float * input, int bufferSize, int nChannels); 
    
private:
    
    void drawVisualization();
    void drawVisualizationArea();
    void drawInputVisualization();
    
private:
    
    // Inputs
    
    MultiTouchInput                     *multitouchInput;
    FlockingInput                       *flockingInput;
    CameraContourInput                  *cameraContourInput;
    
    vector<Input*>                      inputs;
    
//    IImageSeqInput                      *iimageSeqInputs[IMAGESEQINPUT_COUNT];
    vector<IImageSeqInput*>             iimageSeqInputs;
    
    InputAnalyser                       *multiTouchInputAnalyser;
    InputAnalyser                       *flockingInputAnalyser;
//    InputAnalyser                       *cameraCentroidsInputAnalyser;
//    InputAnalyser                       *cameraConvexHullInputAnalyser;
    InputAnalyser                       *cameraContourAnalyser;
//    InputAnalyser                       *cameraContourTipsAnalyser;
    
//    InputAnalyser                       *inputAnalysers[ INPUT_COUNT ];
    vector<InputAnalyser*>              inputAnalysers;
    
    InputAnalyser                       *currentInputAnalyser;
    
    // Gui & Params
    
    msa::controlfreak::ParameterGroup   inputParams;
    msa::controlfreak::ParameterGroup   cameraParams;
    msa::controlfreak::ParameterGroup   ildaParams;
    msa::controlfreak::ParameterGroup   audioParams;
    
    msa::controlfreak::gui::Gui         gui;
    
    // Midi
    
    ofxMidiIn                           midiIn;
	ofxMidiMessage                      midiMessage;
    
    std::pair<int, int>                 pathAnalyserSmoothingMidiKey;
    std::pair<int, int>                 cameraGainMidiKey;
    
    std::map<std::pair<int,int>, int>   midiData;
    
    // OSC
    
    ofxOscReceiver                      receiver;
    
    std::map<std::string, float>        oscData;
    
    // Visualisation
    
    ofVec3f                             offset;
    ofVec3f                             scale;
    
    vector<IVisualizer*>                visualizers;
    
    DotVisualizer                       *dotVisualizer;
    DotTrailsVisualizer                 *dotTrailsVisualizer;
    ConnectedDotsVisualizer             *connectedDotVisualizer;
    NearestDotsVisualizer               *nearestDotsVisualizer;
    LineVisualizer                      *lineVisualizer;
    FixedPointVisualizer                *fixedPointVisualizer;
    QualitiesVisualizer                 *qualitiesVisualizer;
    ConvexHullVisualizer                *convexHullVisualizer;
    ContourVisualizer                   *contourVisualizer;
    RoofVisualizer                      *roofVisualizer;
    
    // Filters
    
    AudioDistorterFilter                *audioDistorterFilter;
    NoiseDistortionFilter               *noiseDistortionFilter;
    
    vector<IFilter*>                    preFilters;
    
    bool                                visualizeInput;
    
    // Ilda
    
    ofxIlda::Frame                      ildaFrame;
    ofxEtherdream                       etherdream;
    
    // Camera
    
    ofxLibdc::Grabber                   grabber;
    ofVideoPlayer                       videoPlayer;
    ofBaseVideo                         *videoPtr;
    ofImage                             imageInput;
    
    // Audio
    
    ofSoundStream                       soundStream;
    
    
};
