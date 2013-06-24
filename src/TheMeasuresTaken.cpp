#include "TheMeasuresTaken.h"



//--------------------------------------------------------------
void TheMeasuresTaken::setup()
{
    // Setup App Settings
    
    ofSetWindowTitle( "The Measures Taken" );
    ofSetWindowShape( 1000 , 800 );
    ofSetFullscreen( true );
    ofSetFrameRate( 60 );
    ofSetBackgroundColor( 0 );
    ofSetLogLevel( OF_LOG_ERROR );
    
    initInputs();
    initVisualizers();
    initVideo();
    initAudioInput();
    initFilters();
    initLaserOutput();
    addWidgets();
    initPanelDraws();
    initContextGUI();
    initParams();
    
    setupMidi();
    setupOCS();
    
    loadGuiMappedObjectsIntoGui();
    loadGUI();
    loadPanels();
    
    gui.toggleDraw();
    gui.setDefaultKeys(true);
    
    usageInfoBarAlpha   = 1.0f;
}

//--------------------------------------------------------------
void TheMeasuresTaken::update()
{
    msa::controlfreak::update();
    
    statsWidget.setMidi( false );
    
    updateOCSData();
    
    if( (bool)appParams->params[ PARAM_OSC_ENABLED ] )
    {
        updateOCSMappedObjects();
    }
    
    if( (bool)appParams->params[ PARAM_MIDI_ENABLED ] )
    {
        updateMidiMappedObjects();
    }

    updateVideo();
    
    switch ( (int)videoParams->params[ INPUT_VIDEO ] )
    {
        case VideoParams::VIDEO_FILE    :
            
            videoAnalysisInput->setImage( videoFile->getRoiImage() );
            videoAnalysisInput->setCurrentFrameNew( videoFile->isFrameNew() );
            
        break;
        
        case VideoParams::VIDEO_CAMERA  :
            
            videoAnalysisInput->setImage( videoCamera->getRoiImage() );
            videoAnalysisInput->setCurrentFrameNew( videoCamera->isFrameNew() );
            
        break;
    }
    
    updateWidgets();
    updateInputs();
    updateAudioInput();
    updateVisualizers();
    updateLaserOutput();
    
    panelGroup.update();
}

//--------------------------------------------------------------
void TheMeasuresTaken::draw()
{
    ofClear( 0 );
    
    if( usageInfoBarAlpha > 0.0f )
    {
        usageInfoBarAlpha   = usageInfoBarAlpha - ( 1.0f / (ofGetFrameRate() * 8.0f) );
        
        float rectWidth     = 460.0f;
        float rectHeight    = 20.0f;
        
        ofPushStyle();
        ofEnableAlphaBlending();
        
        ofSetColor( 30, 255 * usageInfoBarAlpha );
        ofRectRounded( ( ofGetWidth() - rectWidth ) * 0.5f , ( ofGetHeight() - rectHeight ) * 0.5f, rectWidth, rectHeight, 10.0f );
        
        ofSetColor( 80, 255 * usageInfoBarAlpha );
        ofDrawBitmapString( "<space> parameter controls <tab> contextual panels menu", ( ofGetWidth() - rectWidth ) * 0.5f + 8.0f, ( ofGetHeight() - rectHeight ) * 0.5f + 13.0f );
        
        ofDisableAlphaBlending();
        ofPopStyle();
    }
    
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
    
    if( key == 'r' )
    {
        panelGroup.clear();
        
        loadPanels();
    }
    
    if( key == 9 )
    {
        contextGui->setPosition( ofGetMouseX(), ofGetMouseY() );
        contextGui->setVisible(true);
    } else {
        contextGui->setVisible(false); 
    }
    
    if(key == ',') {
        videoFile->params[PARAM_NAME_VIDEO_FRAME] = (int)videoFile->params[PARAM_NAME_VIDEO_FRAME] - 1;
    } else if(key == '.') {
        videoFile->params[PARAM_NAME_VIDEO_FRAME] = (int)videoFile->params[PARAM_NAME_VIDEO_FRAME] + 1;
    } else if(key == '<') {
        videoFile->params[PARAM_NAME_VIDEO_FRAME] = (int)videoFile->params[PARAM_NAME_VIDEO_FRAME] - 25;
    } else if(key == '>') {
        videoFile->params[PARAM_NAME_VIDEO_FRAME] = (int)videoFile->params[PARAM_NAME_VIDEO_FRAME] + 25;
    }

    
    if(key == 'v') {
        videoFile->params[PARAM_NAME_ENABLE_PLAYBACK] = !(bool)videoFile->params[PARAM_NAME_ENABLE_PLAYBACK];
    }
    
    if(key == 'x') {
        laserOutput->reconnect();
    }
    
    
}

//--------------------------------------------------------------
void TheMeasuresTaken::keyReleased(int key){}

//--------------------------------------------------------------
void TheMeasuresTaken::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void TheMeasuresTaken::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void TheMeasuresTaken::mousePressed(int x, int y, int button)
{
    if(!contextGui->isHit(x, y))
    {
        contextGui->setVisible(false);
    }
}

//--------------------------------------------------------------
void TheMeasuresTaken::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void TheMeasuresTaken::windowResized(int w, int h){}

//--------------------------------------------------------------
void TheMeasuresTaken::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void TheMeasuresTaken::dragEvent(ofDragInfo dragInfo){}

//--------------------------------------------------------------
void TheMeasuresTaken::newMidiMessage(ofxMidiMessage& eventArgs)
{
    statsWidget.setMidi( true );
    
    midiData[ std::pair<int, int>( eventArgs.channel, eventArgs.control ) ] = eventArgs.value;
}

//--------------------------------------------------------------
void TheMeasuresTaken::audioIn(float * input, int bufferSize, int nChannels){}

//--------------------------------------------------------------
void TheMeasuresTaken::initFilters()
{
    noiseDistortionFilter   = new NoiseDistortionFilter();
    
    filters.push_back( noiseDistortionFilter );
    
    for (vector<IFilter *>::iterator it = filters.begin(); it != filters.end(); ++it )
    {
        guiMappedObjects.push_back( *it );
        midiMappedObjects.push_back( *it );
        ocsMappedObjects.push_back( *it );
    }
};

//--------------------------------------------------------------
void TheMeasuresTaken::initVisualizers()
{
    dotVisualizer           = new DotVisualizer();
    dotTrailsVisualizer     = new DotTrailsVisualizer();
    particleVisualizer      = new ParticleVisualizer();
    contourVisualizer       = new ContourVisualizer();
    convexHullVisualizer    = new ConvexHullVisualizer();
    skeletonVisualizer      = new SkeletonVisualizer();
    fixedPointVisualizer    = new FixedPointVisualizer();
    lineVisualizer          = new LineVisualizer();
    nearesDotVisualizer     = new NearestDotsVisualizer();
    
    visualizers.push_back( dotVisualizer );
    visualizers.push_back( dotTrailsVisualizer );
    visualizers.push_back( particleVisualizer );
    visualizers.push_back( contourVisualizer );
    visualizers.push_back( convexHullVisualizer );
    visualizers.push_back( skeletonVisualizer );
    visualizers.push_back( fixedPointVisualizer );
    visualizers.push_back( lineVisualizer );
    visualizers.push_back( nearesDotVisualizer );
    
    for (vector<IVisualizer *>::iterator it = visualizers.begin(); it != visualizers.end(); ++it )
    {
        guiMappedObjects.push_back( *it );
        midiMappedObjects.push_back( *it );
        ocsMappedObjects.push_back( *it );
        
        panelDraws.push_back( *it );
        
        (*it)->setup( inputsMap );
    }
}

//--------------------------------------------------------------
void TheMeasuresTaken::updateVisualizers()
{
    for (vector<IVisualizer *>::iterator it = visualizers.begin(); it != visualizers.end(); ++it )
    {
        (*it)->update();
        (*it)->visualize(   inputsMap,
                            audioInput->getAmp(),
                            audioInput->getFFTSampleCount(),
                            audioInput->getFFTData(),
                            ofGetElapsedTimef()
                         );
    }
}

//--------------------------------------------------------------
void TheMeasuresTaken::initParams()
{
    appParams   = new AppParams();
    videoParams = new VideoParams();
    
    guiMappedObjects.push_back( appParams );
    guiMappedObjects.push_back( videoParams );
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

void TheMeasuresTaken::addWidgets()
{
    panelDraws.push_back( &helpTextWidget );
    panelDraws.push_back( &statsWidget );
}

void TheMeasuresTaken::updateWidgets()
{
    statsWidget.oscEnabled          = appParams->params[ PARAM_OSC_ENABLED ];
    statsWidget.midiEnbled          = appParams->params[ PARAM_MIDI_ENABLED ];
}

//--------------------------------------------------------------
void TheMeasuresTaken::initPanelDraws()
{
    for( vector<IPanelDraws*>::iterator it = panelDraws.begin(); it != panelDraws.end(); ++it )
    {
        panelDrawNames.push_back( (*it)->getPanelName() );
        
        panelDrawsMap[ (*it)->getPanelName() ]  = *it;
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
    contextGui->autoSizeToFitWidgets();
    
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
    laserOutput->update( visualizers, filters, audioInput->getAmp() );
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
    videoAnalysisInput  = new VideoAnalysisInput();
    
    guiMappedObjects.push_back( multiTouchInput );
    guiMappedObjects.push_back( videoAnalysisInput );
    
    midiMappedObjects.push_back( multiTouchInput );
    midiMappedObjects.push_back( videoAnalysisInput );
    
    ocsMappedObjects.push_back( multiTouchInput );
    ocsMappedObjects.push_back( videoAnalysisInput );
    
    panelDraws.push_back( multiTouchInput );
    panelDraws.push_back( videoAnalysisInput );
    
    inputs.push_back( multiTouchInput );
    inputs.push_back( videoAnalysisInput );
        
    for(  vector<Input *>::const_iterator it = inputs.begin(); it != inputs.end(); ++it )
    {
        inputsMap[ (*it)->getName() ]   = *it;
    }
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
    oscReceiver.setup( OSC_PORT );
}

//--------------------------------------------------------------
void TheMeasuresTaken::updateOCSData()
{
    statsWidget.setOSC( oscReceiver.hasWaitingMessages() );
    
    while ( oscReceiver.hasWaitingMessages() )
    {        
        ofxOscMessage   m;
        
        oscReceiver.getNextMessage( &m );
        
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
        
        panelGroup.addPanel( new Panel( ofGetMouseX(), ofGetMouseY(), panelDraws->getPanelSize().x, panelDraws->getPanelSize().y, panelDraws ));
        
        contextGui->setVisible(false); 
    }
}

//--------------------------------------------------------------
void TheMeasuresTaken::loadPanels()
{
    ofxXmlSettings  panelXML;

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
                if( panelDrawsMap.count( name ) > 0 )
                {
                    IPanelDraws *panelDraws = panelDrawsMap[ name ];
                    
                    panelGroup.addPanel( new Panel( x, y, width, height, panelDraws ));
                } else {
                    ofLogError( "TheMeasuresTaken::loadPanels", name + " paneldraws not found! ");
                }
            }        
        }
    }
}

//--------------------------------------------------------------
void TheMeasuresTaken::savePanels()
{
    ofxXmlSettings  panelXML;
    
    int i = 0;
    
    panelXML.addTag( "PanelsData" );
    
    panelXML.pushTag( "PanelsData" );
    
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