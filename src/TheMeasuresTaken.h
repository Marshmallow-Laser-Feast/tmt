#pragma once

#include "ofMain.h"

#include "ofxMidi.h"
#include "ofxOsc.h"

#include "ofxEtherdream.h"
#include "ofxIldaFrame.h"

#include "ofxMSAControlFreak/src/ofxMSAControlFreak.h"
#include "ofxMSAControlFreakGui/src/ofxMSAControlFreakGui.h"

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
    
private:
    
        
    
};
