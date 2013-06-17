#include "TheMeasuresTaken.h"



//--------------------------------------------------------------
void TheMeasuresTaken::setup()
{
    // Setup App Settings
    
    ofSetWindowTitle( "The Measures Taken" );
    ofSetFrameRate( 60 );
    ofSetBackgroundColor( 0 );
    ofSetLogLevel( OF_LOG_ERROR );
    
    gui.toggleDraw();
    gui.setDefaultKeys(true);
    
    setupMidi();
    setupOCS();
    
    loadGuiMappedObjectsIntoGui();
    loadGUI();
}

//--------------------------------------------------------------
void TheMeasuresTaken::update()
{
    updateOCSData();
    updateMidiMappedObjects();
    updateOCSMappedObjects();
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
        saveGUI();
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
    midiData[ std::pair<int, int>( eventArgs.channel, eventArgs.control ) ] = eventArgs.value;
}

//--------------------------------------------------------------
void TheMeasuresTaken::audioIn(float * input, int bufferSize, int nChannels)
{
    
}

//--------------------------------------------------------------
void TheMeasuresTaken::loadGUI()
{
    for( int i = 0; i < gui.getNumPages(); ++i )
    {
        gui.getPage( i + 1 ).loadXMLValues();
    }
}

//--------------------------------------------------------------
void TheMeasuresTaken::saveGUI()
{
    for( int i = 0; i < gui.getNumPages(); ++i )
    {
        gui.getPage( i + 1 ).saveXMLValues();
    }
}

//--------------------------------------------------------------
void TheMeasuresTaken::loadGuiMappedObjectsIntoGui()
{
    for( vector<IControlFreakMapper *>::iterator it = guiMappedObjects.begin(); it != guiMappedObjects.end(); ++it )
    {
        gui.addPage( (*it)->params );
    }
}

//--------------------------------------------------------------
void TheMeasuresTaken::setupMidi()
{
    midiIn.listPorts();
    midiIn.openPort( MIDI_PORT );
    midiIn.addListener(this);
    midiIn.setVerbose(false);
}

//--------------------------------------------------------------
void TheMeasuresTaken::updateMidiMappedObjects()
{
    for ( vector<IControlFreakMapperMidiExt *>::iterator it = midiMappedObjects.begin() ; it != midiMappedObjects.end(); ++it )
    {
        (*it)->applyMidi( midiData );
    }
}

//--------------------------------------------------------------
void TheMeasuresTaken::setupOCS()
{
    ocsReceiver.setup( OSC_PORT );
}

//--------------------------------------------------------------
void TheMeasuresTaken::updateOCSData()
{
    while ( ocsReceiver.hasWaitingMessages() )
    {        
        ofxOscMessage   m;
        
        ocsReceiver.getNextMessage( &m );
        
        if( m.getNumArgs() > 0 )
        {
            if( oscData.count( m.getAddress() ) == 0 )
            {
                oscData[ m.getAddress() ]   = vector<float>();
            }
            
            oscData[ m.getAddress() ].clear();
            
            for( int i = 0 ; i < m.getNumArgs(); ++i )
            {
                switch ( m.getArgType( i ) )
                {
                    case OFXOSC_TYPE_INT32  :
                    case OFXOSC_TYPE_INT64  :
                    case OFXOSC_TYPE_FLOAT  :
                        
                        oscData[ m.getAddress() ].push_back( m.getArgAsFloat(i) );
                        
                        break;
                        
                    case OFXOSC_TYPE_BUNDLE :
                        break;
                        
                    case OFXOSC_TYPE_BLOB   :
                        break;
                        
                    case OFXOSC_TYPE_INDEXOUTOFBOUNDS :
                        break;
                        
                    case OFXOSC_TYPE_STRING :
                        break;
                        
                    case OFXOSC_TYPE_NONE   :
                        break;
                }
            }
        }
    }
}

//--------------------------------------------------------------
void TheMeasuresTaken::updateOCSMappedObjects()
{
    for ( vector<IControlFreakMapperOSCExt *>::iterator it = ocsMappedObjects.begin() ; it != ocsMappedObjects.end(); ++it )
    {
        (*it)->applyOCS( oscData );
    }
}