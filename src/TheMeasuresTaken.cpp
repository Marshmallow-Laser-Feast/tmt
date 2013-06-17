#include "TheMeasuresTaken.h"


//--------------------------------------------------------------
void TheMeasuresTaken::setup()
{
    // Setup App Settings
    
    ofSetWindowTitle( "The Measures Taken" );
    ofSetFrameRate( 60 );
    ofSetBackgroundColor( 0 );
    ofSetLogLevel( OF_LOG_ERROR );
}

//--------------------------------------------------------------
void TheMeasuresTaken::update()
{
    
}

//--------------------------------------------------------------
void TheMeasuresTaken::draw()
{
    ofClear( 0 );
    
}

//--------------------------------------------------------------
void TheMeasuresTaken::keyPressed(int key)
{
    if( key == 'f' )
    {
        ofToggleFullscreen();
    }
    
    if( key == 's' )
    {
        
    }
}

//--------------------------------------------------------------
void TheMeasuresTaken::keyReleased(int key)
{
    
}

//--------------------------------------------------------------
void TheMeasuresTaken::mouseMoved(int x, int y )
{
    
}

//--------------------------------------------------------------
void TheMeasuresTaken::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void TheMeasuresTaken::mousePressed(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void TheMeasuresTaken::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void TheMeasuresTaken::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void TheMeasuresTaken::gotMessage(ofMessage msg)
{
    
}

//--------------------------------------------------------------
void TheMeasuresTaken::dragEvent(ofDragInfo dragInfo)
{
    
}

//--------------------------------------------------------------
void TheMeasuresTaken::newMidiMessage(ofxMidiMessage& eventArgs)
{
    
}

void TheMeasuresTaken::audioIn(float * input, int bufferSize, int nChannels)
{
    
}