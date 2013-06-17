#pragma once

#include "ofMain.h"

#include "ofxMidi.h"
#include "ofxOsc.h"

#include "ofxEtherdream.h"
#include "ofxIldaFrame.h"

#include "ofxMSAControlFreak/src/ofxMSAControlFreak.h"
#include "ofxMSAControlFreakGui/src/ofxMSAControlFreakGui.h"

#include "IControlFreakMapper.h"
#include "IControlFreakMapperMidiExt.h"
#include "IControlFreakMapperOSCExt.h"

#define MIDI_PORT       0
#define OSC_PORT        12345

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
    
private:
    
    void setupMidi();
    void updateMidiMappedObjects();
    
    void setupOCS();
    void updateOCSData();
    void updateOCSMappedObjects();
    
    void loadGUI();
    void saveGUI();
    
    void loadGuiMappedObjectsIntoGui();
    
private:
    
    // OCS Input
    
    ofxOscReceiver                          ocsReceiver;
    
    std::map<string, vector<float> >        oscData;
    
    // Midi
    
    ofxMidiIn                               midiIn;
    
    std::map<std::pair<int,int>, int>       midiData;
    
    // GUI
    
    msa::controlfreak::gui::Gui             gui;
    
    // Mapped Objects
    
    vector<IControlFreakMapper *>           guiMappedObjects;
    vector<IControlFreakMapperMidiExt *>    midiMappedObjects;
    vector<IControlFreakMapperOSCExt *>     ocsMappedObjects;
    
};
