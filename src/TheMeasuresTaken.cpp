#include "TheMeasuresTaken.h"



//--------------------------------------------------------------
void TheMeasuresTaken::setup()
{
    // Setup App Settings
    
    ofSetWindowTitle( "The Measures Taken" );
    ofSetWindowShape( 1000 , 800 );
    ofSetFrameRate( 60 );
    ofSetBackgroundColor( 0 );
    ofSetLogLevel( OF_LOG_ERROR );
    
    gui.toggleDraw();
    gui.setDefaultKeys(true);
        
    initInputs();
    initVideo();
    initAudioInput();
    initLaserOutput();
    initPanelDraws();
    initContextGUI();
    
    setupMidi();
    setupOCS();
    
    loadGuiMappedObjectsIntoGui();
    loadGUI();
    loadPanels();
}

//--------------------------------------------------------------
void TheMeasuresTaken::update()
{
    msa::controlfreak::update();

    updateOCSData();
    updateMidiMappedObjects();
    updateOCSMappedObjects();
    updateVideo();
    updateInputs();
    updateAudioInput();
    updateLaserOutput();
    
    panelGroup.update();
}

//--------------------------------------------------------------
void TheMeasuresTaken::draw()
{
    ofClear( 0 );
    
    panelGroup.draw();
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
        savePanels();
    }
    
    if( key == 'p' )
    {
        panelGroup.toggleVisibility();
    }
    
    if( key == 9 )
    {
        contextGui->setPosition( ofGetMouseX(), ofGetMouseY() );
        contextGui->setVisible(true);
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
    if(!contextGui->isHit(x, y))
    {
        contextGui->setVisible(false);
    }
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
void TheMeasuresTaken::initPanelDraws()
{
    for( vector<IPanelDraws*>::iterator it = panelDraws.begin(); it != panelDraws.end(); ++it )
    {
        panelDrawNames.push_back( (*it)->getName() );
        
        panelDrawsMap[ (*it)->getName() ]   = *it;
    }
}

//--------------------------------------------------------------
void TheMeasuresTaken::initContextGUI()
{
    contextGui  = new ofxUICanvas();
    
    for (vector<string>::iterator it = panelDrawNames.begin(); it != panelDrawNames.end(); ++it )
    {
        contextGui->addButton( *it, false );
    }
    
    contextGui->setVisible(false); 
    
    ofAddListener(contextGui->newGUIEvent, this, &TheMeasuresTaken::guiEvent);    
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
void TheMeasuresTaken::initAudioInput()
{
    audioInput  = new AudioInput();
    
    guiMappedObjects.push_back( audioInput );
    midiMappedObjects.push_back( audioInput );
    ocsMappedObjects.push_back( audioInput );
    panelDraws.push_back( audioInput );
}

//--------------------------------------------------------------
void TheMeasuresTaken::updateAudioInput()
{
    audioInput->update();
}

//--------------------------------------------------------------
void TheMeasuresTaken::initLaserOutput()
{
    laserOutput = new LaserOutput();
    
    guiMappedObjects.push_back( laserOutput );
    midiMappedObjects.push_back( laserOutput );
    ocsMappedObjects.push_back( laserOutput );
    panelDraws.push_back( laserOutput );
}

//--------------------------------------------------------------
void TheMeasuresTaken::updateLaserOutput()
{
    laserOutput->update();
}

//--------------------------------------------------------------
void TheMeasuresTaken::initVideo()
{
    videoFile   = new VideoFile();
    videoCamera = new VideoCamera();
    
    guiMappedObjects.push_back( videoFile );
    guiMappedObjects.push_back( videoCamera );
    
    midiMappedObjects.push_back( videoFile );
    midiMappedObjects.push_back( videoCamera );
    
    ocsMappedObjects.push_back( videoFile );
    ocsMappedObjects.push_back( videoCamera );
    
    panelDraws.push_back( videoFile );
    panelDraws.push_back( videoCamera );
    
    videos.push_back( videoFile );
    videos.push_back( videoCamera );
}

//--------------------------------------------------------------
void TheMeasuresTaken::updateVideo()
{
    for ( std::vector<Video *>::iterator it = videos.begin(); it != videos.end(); ++it )
    {
        (*it)->update();
    }
}

//--------------------------------------------------------------
void TheMeasuresTaken::initInputs()
{
    multiTouchInput     = new MultiTouchInput();
    videoContourInput   = new VideoContourInput();
    
    guiMappedObjects.push_back( multiTouchInput );
    guiMappedObjects.push_back( videoContourInput );
    
    midiMappedObjects.push_back( multiTouchInput );
    midiMappedObjects.push_back( videoContourInput );
    
    ocsMappedObjects.push_back( multiTouchInput );
    ocsMappedObjects.push_back( videoContourInput );
    
    panelDraws.push_back( multiTouchInput );
    panelDraws.push_back( videoContourInput );
    
    inputs.push_back( multiTouchInput );
    inputs.push_back( videoContourInput );
}

//--------------------------------------------------------------
void TheMeasuresTaken::updateInputs()
{
    for ( std::vector<Input *>::iterator it = inputs.begin(); it != inputs.end(); ++it )
    {
        (*it)->update();
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

//--------------------------------------------------------------
void TheMeasuresTaken::guiEvent(ofxUIEventArgs &e)
{
    if( e.widget->getState() == 1 )
    {
        IPanelDraws *panelDraws = panelDrawsMap[ e.widget->getName() ];
        
        panelGroup.addPanel( new Panel( ofGetMouseX(), ofGetMouseY(), panelDraws->getSize().x, panelDraws->getSize().y, panelDraws ));
        
        contextGui->setVisible(false); 
    }
}

//--------------------------------------------------------------
void TheMeasuresTaken::loadPanels()
{
    if( panelXML.load( PANELS_FILE ) )
    {
        panelXML.pushTag( "PanelsData" );
        
        for ( int i = 0; i < panelXML.getNumTags( "Panel" ); ++i )
        {
            string  name    = panelXML.getAttribute( "Panel", "name", "", i );
            float   x       = panelXML.getAttribute( "Panel", "x", 0.0f, i );
            float   y       = panelXML.getAttribute( "Panel", "y", 0.0f, i );
            float   width   = panelXML.getAttribute( "Panel", "width", 0.0f, i );
            float   height  = panelXML.getAttribute( "Panel", "height", 0.0f, i );
            
            if( name.length() > 0 )
            {
                IPanelDraws *panelDraws = panelDrawsMap[ name ];
                
                panelGroup.addPanel( new Panel( x, y, width, height, panelDraws ));
            }        
        }
    }
}

//--------------------------------------------------------------
void TheMeasuresTaken::savePanels()
{
    panelXML.clear();
    
    int i = 0;
    
    panelXML.addTag( "PanelsData" );
        
    for( vector<Panel *>::const_iterator it = panelGroup.getPanels().begin(); it != panelGroup.getPanels().end(); ++it )
    {
        panelXML.addTag( "Panel" );
                
        panelXML.addAttribute( "Panel", "name", (*it)->getName(), i );
        panelXML.addAttribute( "Panel", "x", (*it)->getX(), i );
        panelXML.addAttribute( "Panel", "y", (*it)->getY(), i );
        panelXML.addAttribute( "Panel", "width", (*it)->getWidth(), i );
        panelXML.addAttribute( "Panel", "height", (*it)->getHeight(), i );
                
        ++i;        
    }
    
    panelXML.popTag();
    
    panelXML.save( PANELS_FILE );
}