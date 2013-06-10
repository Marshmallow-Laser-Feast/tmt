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
    cameraInput             = new CameraInput( INPUT_WIDTH, INPUT_HEIGHT );
    
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
    visualizationParams.setName( "Visualisation Parameters" );
    outputParams.setName( "Output Parameters" );
    
    inputParams.addNamedIndex( PARAM_NAME_CURRENT_INPUT ).setLabels( 3, "MultiTouch", "Flocking", "Camera" );
    
    inputParams.addInt( PARAM_NAME_CAMERA_THRESHOLD ).setRange( 0, 255 ).setClamp( true );
    inputParams.addFloat( PARAM_NAME_CAMERA_MIN_BLOB_SIZE ).setRange( 0, 2000.0f ).setClamp( true );
    
    visualizationParams.addInt( PARAM_NAME_VIS_DOT_TRAILS_MIN );
    visualizationParams.addInt( PARAM_NAME_VIS_DOT_TRAILS_MAX );
    
    outputParams.addNamedIndex( PARAM_NAME_CURRENT_OUTPUT ).setLabels( 2, "Visualisation", "Calibration" );
    
    outputParams.addFloat( PARAM_NAME_CALIBRATION_X1 ).setRange( 0.0f, 1.0f ).setClamp( true );
    outputParams.addFloat( PARAM_NAME_CALIBRATION_Y1 ).setRange( 0.0f, 1.0f ).setClamp( true );
    outputParams.addFloat( PARAM_NAME_CALIBRATION_X2 ).setRange( 0.0f, 1.0f ).setClamp( true );
    outputParams.addFloat( PARAM_NAME_CALIBRATION_Y2 ).setRange( 0.0f, 1.0f ).setClamp( true );
    
    gui.addPage(inputParams);
    gui.addPage(visualizationParams);
    gui.addPage(outputParams);
    
    gui.toggleDraw();
    
    inputParams.loadXmlSchema();
    visualizationParams.loadXmlSchema();
    outputParams.loadXmlSchema();
    
    // Timeline
    
    timeline.setup();
    timeline.setFrameRate( FRAMERATE );
    timeline.setDurationInSeconds( TIMELINE_DURATION_IN_SECONDS );
    timeline.setLoopType( OF_LOOP_NONE );
    timeline.setAutosave( false );
    
    timeline.addCurves( CURVE_DOTVISUALIZER_DOT_RATIO, ofRange(0, 1.0f) );
    timeline.addCurves( CURVE_DOTTRAILSVISUALIZER_DOT_RATIO, ofRange(0, 1.0f) );
    timeline.addCurves( CURVE_DOTTRAILSVISUALIZER_TRAILS_COUNT, ofRange(0, 1.0f) );
    
    timeline.setOffset( ofVec2f( 0.0f, ofGetWindowHeight() - timeline.getHeight() ) );
        
    timeline.toggleShow();
    
    // Visualisation
    
    dotVisualizer       = new DotVisualizer();
    dotTrailsVisualizer = new DotTrailsVisualizer();
    
    visualizers[0]      = dotVisualizer;
    visualizers[1]      = dotTrailsVisualizer;
    
    visualizeInput      = false;
}

//--------------------------------------------------------------
void TheMeasuresTaken::update()
{
    // Update GUI
    
    msa::controlfreak::update();
    
    // Select current input analyser
    
    currentInputAnalyser    = inputAnalysers[ inputParams[ PARAM_NAME_CURRENT_INPUT ] ];
    
    // Update camera input parameters
    
    cameraInput->setThreshold( inputParams[ PARAM_NAME_CAMERA_THRESHOLD ] );
    cameraInput->setMinBlobArea( inputParams[ PARAM_NAME_CAMERA_MIN_BLOB_SIZE ] );
    
    // Calculate projection offset and scale
    
    float x1                = (float)outputParams[ PARAM_NAME_CALIBRATION_X1 ] * PROJECTION_AREA_WIDTH;
    float y1                = (float)outputParams[ PARAM_NAME_CALIBRATION_Y1 ] * PROJECTION_AREA_HEIGHT;
    
    float x2                = (float)outputParams[ PARAM_NAME_CALIBRATION_X2 ] * PROJECTION_AREA_WIDTH;
    float y2                = (float)outputParams[ PARAM_NAME_CALIBRATION_Y2 ] * PROJECTION_AREA_HEIGHT;
    
    float outputWidth       = x2 - x1;
    float outputHeight      = y2 - y1;
    
    float scaleX            = outputWidth / (float)INPUT_WIDTH;
    float scaleY            = outputHeight / (float)INPUT_HEIGHT;
    
    offset.set( x1, x2, 0.0f );
    scale.set( scaleX, scaleY, 1.0f );
    
    // Update current input analyser
    
    currentInputAnalyser->update();
    
    // Update visualiser settings
    
    dotVisualizer->setRenderRatio( timeline.getValue( CURVE_DOTVISUALIZER_DOT_RATIO ) );
    
    dotTrailsVisualizer->setRenderRatio( timeline.getValue( CURVE_DOTTRAILSVISUALIZER_DOT_RATIO ) );
    dotTrailsVisualizer->setTrailCount( ofLerp( (float)visualizationParams[ PARAM_NAME_VIS_DOT_TRAILS_MIN ], (float)visualizationParams[ PARAM_NAME_VIS_DOT_TRAILS_MAX ], timeline.getValue( CURVE_DOTTRAILSVISUALIZER_TRAILS_COUNT ) ) );
        
    // Collect data from all visualizers
    
    visualizationData.clear();
    
    for( int i = 0; i < VISUALIZER_COUNT; ++i )
    {
        PolylineVectorRefT visualizedLines = visualizers[i]->visualize( currentInputAnalyser , offset, scale );
        
        visualizationData.insert( visualizationData.end(), visualizedLines->begin(), visualizedLines->end() );
    }
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
    
    ofTranslate( ( ofGetWindowWidth() - PROJECTION_AREA_WIDTH ) * 0.5f , ( ofGetWindowHeight() - PROJECTION_AREA_HEIGHT ) * 0.5f + PROJECTION_AREA_HEIGHT );
    
    ofDrawBitmapString( GUIDE_STRING, 100.0f, 20.0f );
    
    ofPopStyle();
    ofPopMatrix();
    
    timeline.draw();
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
        visualizationParams.saveXmlSchema();
        outputParams.saveXmlSchema();
        
        timeline.save();
    }
    
    if( key == 'p' )
    {
        gui.toggleDraw();
    }
    
    if( key == 't' )
    {
        timeline.toggleShow();
    }
    
    if( key == 'i' )
    {
        visualizeInput  = !visualizeInput;
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
    timeline.setOffset( ofVec2f( 0.0f, ofGetWindowHeight() - timeline.getHeight() ) );
}

//--------------------------------------------------------------
void TheMeasuresTaken::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void TheMeasuresTaken::dragEvent(ofDragInfo dragInfo)
{

}

void TheMeasuresTaken::drawVisualization()
{
    ofPushMatrix();
    ofPushStyle();
    
    ofSetColor( 255 );
    
    ofTranslate( ( ofGetWindowWidth() - SCREEN_VIS_AREA_WIDTH ) * 0.5f , ( ofGetWindowHeight() - SCREEN_VIS_AREA_HEIGHT ) * 0.5f );
    ofTranslate( offset * ofVec3f( -1, -1, 1.0f ) );
    ofScale( (float)SCREEN_VIS_AREA_WIDTH / (float)PROJECTION_AREA_WIDTH , (float)SCREEN_VIS_AREA_HEIGHT / (float)PROJECTION_AREA_HEIGHT );
    
    for ( vector<ofPolyline>::iterator it = visualizationData.begin(); it != visualizationData.end(); ++it )
    {
        it->draw();
        
        for ( vector<ofPoint>::iterator pit = it->getVertices().begin() ; pit != it->getVertices().end(); ++pit )
        {
            ofCircle( *pit , 2.0f );
        }
    }
    
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