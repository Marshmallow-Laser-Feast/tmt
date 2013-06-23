#pragma once

#include "ofMain.h"

#include "ofxMidi.h"
#include "ofxOsc.h"

#include "ofxMSAControlFreak/src/ofxMSAControlFreak.h"
#include "ofxMSAControlFreakGui/src/ofxMSAControlFreakGui.h"

#include "ofxUI.h"

#include "ofxXmlSettings.h"

#include "IControlFreakMapper.h"
#include "IControlFreakMapperMidiExt.h"
#include "IControlFreakMapperOSCExt.h"

#include "Input.h"

#include "MultiTouchInput.h"
#include "VideoAnalysisInput.h"

#include "Video.h"

#include "VideoCamera.h"
#include "VideoFile.h"

#include "LaserOutput.h"

#include "AudioInput.h"

#include "Panel.h"
#include "PanelGroup.h"

#include "HelpTextWidget.h"
#include "StatsWidget.h"

#include "AppParams.h"
#include "VideoParams.h"

#include "IVisualizer.h"

#include "DotVisualizer.h"
#include "DotTrailsVisualizer.h"
#include "ParticleVisualizer.h"
#include "ContourVisualizer.h"
#include "ConvexHullVisualizer.h"

#define MIDI_PORT           0
#define OSC_PORT            12345
#define OSC_STATUS_TIMEOUT  60
#define PANELS_FILE         "panels.xml"

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
    
    // IO
    
    void newMidiMessage(ofxMidiMessage& eventArgs);
    void audioIn(float * input, int bufferSize, int nChannels); 
    
    // GUI
    
    void guiEvent(ofxUIEventArgs &e);
    
private:
    
    void initVisualizers();
    void updateVisualizers();
    
    void initParams();
    
    void loadPanels();
    void savePanels();
    
    void initPanelDraws();
    void addWidgets();
    void updateWidgets();
    void initContextGUI();
    
    void initAudioInput();
    void updateAudioInput();
    
    void initLaserOutput();
    void updateLaserOutput();
    
    void initVideo();
    void updateVideo();
    
    void initInputs();
    void updateInputs();
    
    void setupMidi();
    void updateMidiMappedObjects();
    
    void setupOCS();
    void updateOCSData();
    void updateOCSMappedObjects();
    
    void loadGUI();
    void saveGUI();
    
    void loadGuiMappedObjectsIntoGui();
    
private:
    
    // Visualizers
    
    DotVisualizer                           *dotVisualizer;
    DotTrailsVisualizer                     *dotTrailsVisualizer;
    ParticleVisualizer                      *particleVisualizer;
    ContourVisualizer                       *contourVisualizer;
    ConvexHullVisualizer                    *convexHullVisualizer;
    
    vector<IVisualizer *>                   visualizers;
    
    // Audio
    
    AudioInput                              *audioInput;
    
    // Laser
    
    LaserOutput                             *laserOutput;
    
    // Video
    
    VideoFile                               *videoFile;
    VideoCamera                             *videoCamera;
    
    vector<Video *>                         videos;
    
    // Inputs
    
    VideoAnalysisInput                      *videoAnalysisInput;
    
    MultiTouchInput                         *multiTouchInput;
    
    vector<Input *>                         inputs;
    InputsMapT                              inputsMap;
    
    // OCS Input
    
    ofxOscReceiver                          oscReceiver;
    
    std::map<string, vector<float> >        oscData;
        
    // Midi
    
    ofxMidiIn                               midiIn;
    
    std::map<std::pair<int,int>, int>       midiData;
        
    // GUI
    
    msa::controlfreak::gui::Gui             gui;
    PanelGroup                              panelGroup;
    ofxUICanvas                             *contextGui;
    
    HelpTextWidget                          helpTextWidget;
    StatsWidget                             statsWidget;
    
    map<string, IPanelDraws*>               panelDrawsMap;
    vector<string>                          panelDrawNames;
    vector<IPanelDraws*>                    panelDraws;
    
    // Mapped Objects
    
    vector<IControlFreakMapper *>           guiMappedObjects;
    vector<IControlFreakMapperMidiExt *>    midiMappedObjects;
    vector<IControlFreakMapperOSCExt *>     ocsMappedObjects;
    
    // Other Parameters
    
    AppParams                               *appParams;
    VideoParams                             *videoParams;
    
    
};
