#include "TheMeasuresTaken.h"

//--------------------------------------------------------------
void TheMeasuresTaken::setup()
{
    // Setup App Settings
    
    ofSetWindowTitle( "The Measures Taken" );
    ofSetFrameRate( FRAMERATE );
    ofSetBackgroundColor( 0 );
    ofSetFullscreen( true );
    ofSetLogLevel( OF_LOG_ERROR );
    
    // Initiate Inputs
    
    multitouchInput         = new MultiTouchInput( INPUT_WIDTH, INPUT_HEIGHT );
    flockingInput           = new FlockingInput( FLOCKING_SAMPLE_COUT, INPUT_WIDTH, INPUT_HEIGHT, FLOCKING_COLUMNS, FLOCKING_ROWS, FLOCKING_MAX_SPEED, FLOCKING_MIN_FORCE, FLOCKING_MAX_FORCE, FLOCKING_ATTRAC_RAD_RATIO );
    cameraInput             = new CameraInput();
    
    iimageSeqInputs[0]      = cameraInput;
    
    multiTouchInputAnalyser = new InputAnalyser( multitouchInput, INPUT_TIMEOUT_FRAMES );
    flockingInputAnalyser   = new InputAnalyser( flockingInput, INPUT_TIMEOUT_FRAMES );
    cameraInputAnalyser     = new InputAnalyser( cameraInput, INPUT_TIMEOUT_FRAMES );
    
    inputAnalysers[0]       = multiTouchInputAnalyser;
    inputAnalysers[1]       = flockingInputAnalyser;
    inputAnalysers[2]       = cameraInputAnalyser;
    
    currentInputAnalyser    = inputAnalysers[0];
    
    for ( int i = 0 ; i < INPUT_COUNT ; ++i )
    {
        inputAnalysers[i]->setMaxPathAnalyserSamples( MAX_PATH_ANALYSER_SAMPLES );
        inputAnalysers[i]->setMaxPathAnalyserHistory( MAX_PATH_ANALYSER_HISTORY );
        inputAnalysers[i]->setMaxPathAnalyserLength( MAX_PATH_ANALYSER_LENGTH );
    }
    
    // Setup Gui & Params
    
    inputParams.setName("Input Parameters");
    cameraParams.setName( "Camera Parameters" );
    visualizationParams.setName( "Visualisation Parameters" );
    outputParams.setName( "Output Parameters" );
    ildaParams.setName( "ILDA Parameters" );
    
    inputParams.addNamedIndex( PARAM_NAME_CURRENT_INPUT ).setLabels( 3, "MultiTouch", "Flocking", "Camera" );
    
    cameraParams.addInt( PARAM_NAME_CAMERA_THRESHOLD ).setRange( 0, 255 ).setClamp( true );
    cameraParams.addFloat( PARAM_NAME_CAMERA_MIN_BLOB_SIZE ).setRange( 0, 2000.0f ).setClamp( true );
    cameraParams.addBool( PARAM_NAME_CAMERA_DRAW_COLOR );
    cameraParams.addBool( PARAM_NAME_CAMERA_DRAW_THRESHOLD );
    cameraParams.addFloat( PARAM_NAME_CAMERA_SCREEN_SCALE ).setRange( 0, 1.0f ).setClamp( true ).setIncrement( 0.01f );
    
    visualizationParams.addFloat( PARAM_NAME_DOT_VIS_RATIO ).setRange( 0, 1.0f ).setClamp( true ).setIncrement( 0.01f );
    
    visualizationParams.addFloat( PARAM_NAME_DOT_TRAILS_VIS_RATIO ).setRange( 0, 1.0f ).setClamp( true ).setIncrement( 0.01f );
    visualizationParams.addInt( PARAM_NAME_DOT_TRAILS_VIS_TRAILS_COUNT ).setRange( 0, 1000 ).setClamp( true );
    
    visualizationParams.addInt( PARAM_NAME_CONNECTED_DOT_VIS_COUNT ).setRange( 0, 1000 ).setClamp( true );
    visualizationParams.addInt( PARAM_NAME_CONNECTED_DOT_VIS_N_OFFSET ).setRange( 0, 1000 ).setClamp( true );
    
    visualizationParams.addInt( PARAM_NAME_NEAREST_DOT_VIS_COUNT ).setRange( 0, 1000 ).setClamp( true );
    
    visualizationParams.addInt( PARAM_NAME_LINE_VIS_COUNT ).setRange( 0, 1000 ).setClamp( true );
    
    visualizationParams.addInt( PARAM_NAME_FIXED_POINT_VIS_COUNT ).setRange( 0, 1000 ).setClamp( true );
    visualizationParams.addBang( PARAM_NAME_FIXED_POINT_FIX );
    visualizationParams.addBang( PARAM_NAME_FIXED_POINT_CLEAR );
    
    outputParams.addNamedIndex( PARAM_NAME_CURRENT_OUTPUT ).setLabels( 2, "Visualisation", "Calibration" );
        
    ildaParams.addBool( PARAM_NAME_ILDA_DRAW_LINES );
    ildaParams.addBool( PARAM_NAME_ILDA_DRAW_POINTS );
    
    ildaParams.addBool( PARAM_NAME_ILDA_OUTPUT_CALIBRATION_ONLY );
    
    ildaParams.addBool( PARAM_NAME_ILDA_FLIPX );
    ildaParams.addBool( PARAM_NAME_ILDA_FLIPY );
    
    ildaParams.addBool( PARAM_NAME_ILDA_DOCAPX );
    ildaParams.addBool( PARAM_NAME_ILDA_DOCAPY );
    
    ildaParams.addInt( PARAM_NAME_ILDA_BLANK_COUNT ).setRange( 0, 1000 ).setClamp( true );
    ildaParams.addInt( PARAM_NAME_ILDA_END_COUNT ).setRange( 0, 1000 ).setClamp( true );
    
    ildaParams.addFloat( PARAM_NAME_ILDA_OFFSET_X ).setRange( -1.0f, 1.0f ).setClamp( true ).setIncrement( 0.01f );
    ildaParams.addFloat( PARAM_NAME_ILDA_OFFSET_Y ).setRange( -1.0f, 1.0f ).setClamp( true ).setIncrement( 0.01f );
    
    ildaParams.addFloat( PARAM_NAME_ILDA_SCALE_X ).setRange( 0, 1.0f ).setClamp( true ).setIncrement( 0.01f );
    ildaParams.addFloat( PARAM_NAME_ILDA_SCALE_Y ).setRange( 0, 1.0f ).setClamp( true ).setIncrement( 0.01f );
    
    ildaParams.addInt( PARAM_NAME_POINT_COUNT_ORIG );
    ildaParams.addInt( PARAM_NAME_POINT_COUNT_PROC );
    
    ildaParams.addInt( PARAM_NAME_ILDA_SMOOTH_AMOUNT ).setRange( 0, 1000 ).setClamp( true );
    ildaParams.addFloat( PARAM_NAME_ILDA_OPTIMIZE_TOLERANCE ).setIncrement( 0.01f );
    ildaParams.addBool( PARAM_NAME_ILDA_COLLAPSE );
    ildaParams.addInt( PARAM_NAME_ILDA_POINT_COUNT ).setRange( 0, 1000 ).setClamp( true );
    ildaParams.addFloat( PARAM_NAME_ILDA_SPACING ).setIncrement( 0.01f );
    
    gui.addPage(inputParams);
    gui.addPage( cameraParams );
    gui.addPage(visualizationParams);
    gui.addPage(outputParams);
    gui.addPage(ildaParams);
    
    gui.toggleDraw();
    
    inputParams.loadXmlSchema();
    cameraParams.loadXmlSchema();
    visualizationParams.loadXmlSchema();
    outputParams.loadXmlSchema();
    ildaParams.loadXmlSchema();
    
    // Timeline
    
    midiIn.listPorts();
    midiIn.openPort(1);
    midiIn.addListener(this);
    midiIn.setVerbose(true);
    
    // Visualisation
    
    dotVisualizer           = new DotVisualizer();
    dotTrailsVisualizer     = new DotTrailsVisualizer();
    connectedDotVisualizer  = new ConnectedDotsVisualizer();
    nearestDotsVisualizer   = new NearestDotsVisualizer();
    lineVisualizer          = new LineVisualizer();
    fixedPointVisualizer    = new FixedPointVisualizer();
    qualitiesVisualizer     = new QualitiesVisualizer();
    
    visualizers[0]          = dotVisualizer;
    visualizers[1]          = dotTrailsVisualizer;
    visualizers[2]          = connectedDotVisualizer;
    visualizers[3]          = nearestDotsVisualizer;
    visualizers[4]          = lineVisualizer;
    visualizers[5]          = fixedPointVisualizer;
    visualizers[6]          = qualitiesVisualizer;
    
    offset.set( 0.0f, 0.0f, 0.0f );
    scale.set( 1.0f / (float)INPUT_WIDTH, 1.0f / (float)INPUT_HEIGHT, 1.0f );
    
    visualizeInput          = false;
    
    // Ilda
    
    etherdream.setup();
    etherdream.setPPS(30000);
    
    // Camera &  Grabber
    
    pixelsSharedPtr         = ofPixelsSharedPtrT( new ofPixels() );
    
    grabber.setSize( INPUT_WIDTH , INPUT_HEIGHT );
    grabber.setImageType( OF_IMAGE_GRAYSCALE );
    
    grabber.setup();
}

//--------------------------------------------------------------
void TheMeasuresTaken::update()
{
    // Camera & Grabber
    
    grabber.update();
    
    if( grabber.isFrameNew() )
    {
        for( int i = 0; i < IMAGESEQINPUT_COUNT; ++i )
        {
            pixelsSharedPtr->setFromPixels( grabber.getPixels() , grabber.getWidth(), grabber.getHeight(), OF_IMAGE_GRAYSCALE );
            
            iimageSeqInputs[i]->setPixels( pixelsSharedPtr );
        }
    }
    
    qualitiesVisualizer->setNoiseOffset( ofGetElapsedTimef() * 1.0f );
    
    // Update GUI
    
    msa::controlfreak::update();
    
    // Select current input analyser
    
    currentInputAnalyser    = inputAnalysers[ inputParams[ PARAM_NAME_CURRENT_INPUT ] ];
    
    // Update camera settings
    
    //cameraInput->setThreshold( cameraParams[ PARAM_NAME_CAMERA_THRESHOLD ] );
    //cameraInput->setMinBlobArea( cameraParams[ PARAM_NAME_CAMERA_MIN_BLOB_SIZE ] );
    
    if( currentInputAnalyser != cameraInputAnalyser && ( (bool)cameraParams[ PARAM_NAME_CAMERA_DRAW_COLOR ] || (bool)cameraParams[ PARAM_NAME_CAMERA_DRAW_THRESHOLD ] ) )
    {
        cameraInput->update();
    }
    
    // Update current input analyser
    
    currentInputAnalyser->update();
    
    // Update visualiser settings
    
    dotVisualizer->setRenderRatio( (float)visualizationParams[ PARAM_NAME_DOT_VIS_RATIO ] );
    
    dotTrailsVisualizer->setRenderRatio( (float)visualizationParams[ PARAM_NAME_DOT_TRAILS_VIS_RATIO ] );
    dotTrailsVisualizer->setTrailCount( (int)visualizationParams[ PARAM_NAME_DOT_TRAILS_VIS_TRAILS_COUNT ] );
    
    connectedDotVisualizer->setCount( (int)visualizationParams[ PARAM_NAME_CONNECTED_DOT_VIS_COUNT ] );
    connectedDotVisualizer->setNeighborsOffset( (int)visualizationParams[ PARAM_NAME_CONNECTED_DOT_VIS_N_OFFSET ] );
    
    nearestDotsVisualizer->setCount( (int)visualizationParams[ PARAM_NAME_NEAREST_DOT_VIS_COUNT ] );
    
    lineVisualizer->setCount( (int)visualizationParams[ PARAM_NAME_LINE_VIS_COUNT ] );
    
    fixedPointVisualizer->setCount( (int)visualizationParams[ PARAM_NAME_FIXED_POINT_VIS_COUNT ] );
    
    if( (bool)visualizationParams[PARAM_NAME_FIXED_POINT_FIX] )
    {
        fixedPointVisualizer->setFixedPoints( currentInputAnalyser );
    }
    
    if( (bool)visualizationParams[PARAM_NAME_FIXED_POINT_CLEAR] )
    {
        fixedPointVisualizer->clearFixedPoints();
    }
    
    // Collect data from all visualizers
    
    visualizationData.clear();
    
    for( int i = 0; i < VISUALIZER_COUNT; ++i )
    {
        PolylineVectorRefT visualizedLines = visualizers[i]->visualize( currentInputAnalyser , offset, scale );
        
        visualizationData.insert( visualizationData.end(), visualizedLines->begin(), visualizedLines->end() );
    }
    
    // Ilda
    
    ildaFrame.clear();
        
    ildaFrame.params.output.transform.doFlipX           = (bool)ildaParams[ PARAM_NAME_ILDA_FLIPX ];
    ildaFrame.params.output.transform.doFlipY           = (bool)ildaParams[ PARAM_NAME_ILDA_FLIPY ];
    
    ildaFrame.params.output.transform.offset.x          = (float)ildaParams[ PARAM_NAME_ILDA_OFFSET_X ];
    ildaFrame.params.output.transform.offset.y          = (float)ildaParams[ PARAM_NAME_ILDA_OFFSET_Y ];
    
    ildaFrame.params.output.transform.scale.x           = (float)ildaParams[ PARAM_NAME_ILDA_SCALE_X ];
    ildaFrame.params.output.transform.scale.y           = (float)ildaParams[ PARAM_NAME_ILDA_SCALE_Y ];
    
    ildaFrame.params.output.blankCount                  = (int)ildaParams[ PARAM_NAME_ILDA_BLANK_COUNT ];
    ildaFrame.params.output.endCount                    = (int)ildaParams[ PARAM_NAME_ILDA_END_COUNT ];
    
    ildaFrame.params.output.doCapX                      = (bool)ildaParams[ PARAM_NAME_ILDA_DOCAPX ];
    ildaFrame.params.output.doCapY                      = (bool)ildaParams[ PARAM_NAME_ILDA_DOCAPY ];
    
    ildaFrame.params.draw.lines                         = (bool)ildaParams[ PARAM_NAME_ILDA_DRAW_LINES ];
    ildaFrame.params.draw.points                        = (bool)ildaParams[ PARAM_NAME_ILDA_DRAW_POINTS ];
    
    ildaFrame.polyProcessor.params.smoothAmount         = (int)ildaParams[ PARAM_NAME_ILDA_SMOOTH_AMOUNT ];
    ildaFrame.polyProcessor.params.optimizeTolerance    = (float)ildaParams[ PARAM_NAME_ILDA_OPTIMIZE_TOLERANCE ];
    ildaFrame.polyProcessor.params.collapse             = (bool)ildaParams[ PARAM_NAME_ILDA_COLLAPSE ];
    ildaFrame.polyProcessor.params.targetPointCount     = (int)ildaParams[ PARAM_NAME_ILDA_POINT_COUNT ];
    ildaFrame.polyProcessor.params.spacing              = (float)ildaParams[ PARAM_NAME_ILDA_SPACING ];
    
    ildaParams[ PARAM_NAME_POINT_COUNT_ORIG ].set( ildaFrame.stats.pointCountOrig );
    ildaParams[ PARAM_NAME_POINT_COUNT_PROC ].set( ildaFrame.stats.pointCountProcessed );
    
    if( (bool)ildaParams[ PARAM_NAME_ILDA_OUTPUT_CALIBRATION_ONLY ] )
    {
        ildaFrame.drawCalibration();
    } else {
        for ( std::vector<ofPolyline>::iterator it = visualizationData.begin() ; it != visualizationData.end(); ++it )
        {
            ildaFrame.addPoly( *it );
        }
    }
    
    ildaFrame.update();
    
    etherdream.setPoints(ildaFrame);

}

//--------------------------------------------------------------
void TheMeasuresTaken::draw()
{
    ofClear( 0 );
    
    drawVisualizationArea();
    drawVisualization();
    
    if( visualizeInput )
    {
        drawInputVisualization();
    }
    
    ofPushMatrix();
    ofPushStyle();
    
    ofSetColor( 125 );
    
    ofTranslate( ( ofGetWindowWidth() - 1240.0f ) * 0.5f , ( ofGetWindowHeight() - SCREEN_VIS_AREA_HEIGHT ) * 0.5f + SCREEN_VIS_AREA_HEIGHT );
    
    ofDrawBitmapString( GUIDE_STRING, 0.0f, 20.0f );
    
    ofPopStyle();
    ofPopMatrix();
    
    float verticalOffset    = 0.0f;
    float scale             = (float)cameraParams[ PARAM_NAME_CAMERA_SCREEN_SCALE ];
    
    ofPushMatrix();
    
    if( (bool)cameraParams[ PARAM_NAME_CAMERA_DRAW_COLOR ] )
    {
        ofTranslate( ofGetWidth() - INPUT_WIDTH * scale , 0.0f );
        ofScale( scale, scale );
        
        grabber.draw( 0.0f, 0.0f );
        
        verticalOffset      = INPUT_HEIGHT * scale;
    }
    
    ofPopMatrix();
    
    ofPushMatrix();
    
    if( (bool)cameraParams[ PARAM_NAME_CAMERA_DRAW_THRESHOLD ] )
    {
        ofTranslate( ofGetWidth() - INPUT_WIDTH * scale, verticalOffset );
        ofScale( scale, scale );
        
        //cameraInput->drawThreshold();
    }
    
    ofPopMatrix();
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
        inputParams.saveXmlSchema();
        cameraParams.saveXmlSchema();
        visualizationParams.saveXmlSchema();
        outputParams.saveXmlSchema();
        ildaParams.saveXmlSchema();
    }
    
    if( key == 'p' )
    {
        gui.toggleDraw();
    }
    
    if( key == 'i' )
    {
        visualizeInput  = !visualizeInput;
    }
    
    if( key == 'l' )
    {
        midiIn.listPorts();
    }
    
    if( key == 'o' )
    {
        visualizationParams[PARAM_NAME_FIXED_POINT_FIX].set( true );
    }
    
    if( key == 'O' )
    {
        visualizationParams[PARAM_NAME_FIXED_POINT_CLEAR].set( true );
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
    midiMessage = eventArgs;
}

void TheMeasuresTaken::drawVisualization()
{
    ofPushMatrix();
    ofPushStyle();
    
    ofSetColor( 255 );
    
    ildaFrame.draw( ( ofGetWindowWidth() - SCREEN_VIS_AREA_WIDTH ) * 0.5f, ( ofGetWindowHeight() - SCREEN_VIS_AREA_HEIGHT ) * 0.5f, SCREEN_VIS_AREA_WIDTH, SCREEN_VIS_AREA_HEIGHT );
    
    ofPopStyle();
    ofPopMatrix();
}

void TheMeasuresTaken::drawInputVisualization()
{
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate( ( ofGetWindowWidth() - INPUT_WIDTH ) * 0.5f , ( ofGetWindowHeight() - INPUT_HEIGHT ) * 0.5f );
    
    ofSetColor( ofColor::black, 125 );
    
    ofRect( 0, 0, INPUT_WIDTH, INPUT_HEIGHT );
    
    ofSetColor( 125 );
    
    ofNoFill();
    ofSetLineWidth( 1.0f );
    
    ofRect( 0, 0, INPUT_WIDTH, INPUT_HEIGHT );
    
    glEnable( GL_SCISSOR_TEST );
    glScissor( ( ofGetWindowWidth() - INPUT_WIDTH ) * 0.5f, ( ofGetWindowHeight() - INPUT_HEIGHT ) * 0.5f, INPUT_WIDTH, INPUT_HEIGHT );
    
    ofSetColor( 100 );
    
    currentInputAnalyser->draw();
    
    ofSetColor( ofColor::cyan );
    
    currentInputAnalyser->drawCurvature( 10.0f );
    
    ofSetColor( ofColor::darkGreen );
    
    currentInputAnalyser->drawAcceleration( 10.0f );
    
    ofFill();
    
    ofSetColor( 100 );
    
    currentInputAnalyser->drawSamples( 2.0f );
    
    ofSetColor( 255 );
    
    for( vector<PointInputSampleT>::const_iterator it = currentInputAnalyser->getInput()->getSamples().begin(); it != currentInputAnalyser->getInput()->getSamples().end(); ++it )
    {
        ofCircle( it->getSample(), 4.0f );
    }
    
    ofPopStyle();
    ofPopMatrix();
    
    glDisable( GL_SCISSOR_TEST );
}

void TheMeasuresTaken::drawVisualizationArea()
{
    ofPushMatrix();
    ofPushStyle();
    
    ofNoFill();
    ofSetLineWidth( 4.0f );
    ofSetColor( ofColor::darkGray );
    
    ofTranslate( ( ofGetWindowWidth() - SCREEN_VIS_AREA_WIDTH ) * 0.5f , ( ofGetWindowHeight() - SCREEN_VIS_AREA_HEIGHT ) * 0.5f );
    
    ofRect( 0, 0, SCREEN_VIS_AREA_WIDTH, SCREEN_VIS_AREA_HEIGHT );
    
    ofPopStyle();
    ofPopMatrix();
}