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

    
    // Setup Gui & Params
    
    inputParams.setName("Input Parameters");
    cameraParams.setName( "Camera Parameters" );
    ildaParams.setName( "ILDA Parameters" );
    
    inputParams.addNamedIndex( PARAM_NAME_CURRENT_INPUT ).setLabels( 5, "MultiTouch", "Flocking", "Camera Centroids", "Camera Convex Hull", "Camera Contour" );
    
    cameraParams.addFloat( PARAM_NAME_CAMERA_ROI_X1 ).setRange( 0, 1.0f ).setClamp( true );
    cameraParams.addFloat( PARAM_NAME_CAMERA_ROI_Y1 ).setRange( 0, 1.0f ).setClamp( true );
    cameraParams.addFloat( PARAM_NAME_CAMERA_ROI_X2 ).setRange( 0, 1.0f ).setClamp( true );
    cameraParams.addFloat( PARAM_NAME_CAMERA_ROI_Y2 ).setRange( 0, 1.0f ).setClamp( true );
    
    cameraParams.addBool(PARAM_NAME_CAMERA_FLIP_X);
    cameraParams.addBool(PARAM_NAME_CAMERA_FLIP_Y);
    
    cameraParams.addBool( PARAM_NAME_CAMERA_DRAW_COLOR );
    cameraParams.addBool( PARAM_NAME_CAMERA_DRAW_ROI );
    cameraParams.addBool( PARAM_NAME_CAMERA_DRAW_CONTOURS );
    cameraParams.addNamedIndex( PARAM_NAME_CAMERA_CONTOUR_SOURCE ).setLabels( 3, "Centroid", "ConvexHull", "Contour" );

    cameraParams.addFloat(PARAM_NAME_LIBDC_BRIGHTNESS).setClamp(true).setSnap(true);
    cameraParams.addFloat(PARAM_NAME_LIBDC_GAMMA).setClamp(true).setSnap(true);
    cameraParams.addFloat(PARAM_NAME_LIBDC_SHUTTER).setClamp(true).setSnap(true);
    cameraParams.addFloat(PARAM_NAME_LIBDC_GAIN).setClamp(true).setSnap(true);

    cameraParams.addFloat( PARAM_NAME_CAMERA_SCREEN_SCALE ).setRange( 0, 1.0f ).setClamp( true );
    cameraParams.addBool(PARAM_NAME_CAMERA_USE_VIDEO);
    cameraParams.addBool(PARAM_NAME_VIDEO_PLAY);
    cameraParams.addInt(PARAM_NAME_VIDEO_FRAME).setRange(0, 10 * 60 * 60).setClamp(true);
    
//    outputParams.addNamedIndex( PARAM_NAME_CURRENT_OUTPUT ).setLabels( 2, "Visualisation", "Calibration" );
    
    ildaParams.addBool(PARAM_NAME_ENABLED);
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
    
    ildaParams.addFloat( PARAM_NAME_ILDA_SCALE_X ).setRange( 0, 4.0f ).setClamp( true ).setIncrement( 0.01f );
    ildaParams.addFloat( PARAM_NAME_ILDA_SCALE_Y ).setRange( 0, 4.0f ).setClamp( true ).setIncrement( 0.01f );
    
    ildaParams.addInt( PARAM_NAME_POINT_COUNT_ORIG );
    ildaParams.addInt( PARAM_NAME_POINT_COUNT_PROC );
    
    ildaParams.addInt( PARAM_NAME_ILDA_SMOOTH_AMOUNT ).setRange( 0, 50 ).setClamp( true );
    ildaParams.addFloat( PARAM_NAME_ILDA_OPTIMIZE_TOLERANCE ).setIncrement( 0.01f ).setClamp(true);
//    ildaParams.addBool( PARAM_NAME_ILDA_COLLAPSE );
    ildaParams.addInt( PARAM_NAME_ILDA_POINT_COUNT ).setRange( 0, 2000 ).setClamp( true );
    ildaParams.addFloat( PARAM_NAME_ILDA_SPACING ).setIncrement( 0.01f ).setClamp(true);
    
    gui.addPage(inputParams);
    gui.addPage( cameraParams );
    gui.addPage(ildaParams);
    
    gui.toggleDraw();
    gui.setDefaultKeys(true);
    
    inputParams.loadXmlValues();
    cameraParams.loadXmlValues();
    ildaParams.loadXmlValues();

    
    
    // Initiate Inputs
    
    multitouchInput                 = new MultiTouchInput( INPUT_WIDTH, INPUT_HEIGHT );
    flockingInput                   = new FlockingInput( FLOCKING_SAMPLE_COUT, INPUT_WIDTH, INPUT_HEIGHT, FLOCKING_COLUMNS, FLOCKING_ROWS, FLOCKING_MAX_SPEED, FLOCKING_MIN_FORCE, FLOCKING_MAX_FORCE, FLOCKING_ATTRAC_RAD_RATIO );
    iimageSeqInputs.push_back(cameraCentroidsInput            = new CameraCentroidsInputs());
    iimageSeqInputs.push_back(cameraConvexHullInput           = new CameraConvexHullInput());
    iimageSeqInputs.push_back(cameraContourInput              = new CameraContourInput());
    
    for( int i = 0; i < iimageSeqInputs.size(); ++i )
    {
        iimageSeqInputs[i]->setDimensions( INPUT_WIDTH , INPUT_HEIGHT );
        iimageSeqInputs[i]->init();
        
        gui.addPage(iimageSeqInputs[i]->params);
        iimageSeqInputs[i]->params.loadXmlValues();
    }
    
    inputAnalysers.push_back(multiTouchInputAnalyser         = new InputAnalyser( multitouchInput, INPUT_TIMEOUT_FRAMES ));
    inputAnalysers.push_back(flockingInputAnalyser           = new InputAnalyser( flockingInput, INPUT_TIMEOUT_FRAMES ));
    inputAnalysers.push_back(cameraCentroidsInputAnalyser    = new InputAnalyser( cameraCentroidsInput, INPUT_TIMEOUT_FRAMES ));
    inputAnalysers.push_back(cameraConvexHullInputAnalyser   = new InputAnalyser( cameraConvexHullInput, INPUT_TIMEOUT_FRAMES ));
    inputAnalysers.push_back(cameraContourAnalyser           = new InputAnalyser( cameraContourInput, INPUT_TIMEOUT_FRAMES ));
    
    currentInputAnalyser    = inputAnalysers[0];
    
    for ( int i = 0 ; i < inputAnalysers.size() ; ++i )
    {
        inputAnalysers[i]->setMaxPathAnalyserSamples( MAX_PATH_ANALYSER_SAMPLES );
        inputAnalysers[i]->setMaxPathAnalyserHistory( MAX_PATH_ANALYSER_HISTORY );
        inputAnalysers[i]->setMaxPathAnalyserLength( MAX_PATH_ANALYSER_LENGTH );
    }
    
    
    
    // Visualisation
    
    visualizers.push_back( dotVisualizer           = new DotVisualizer());
    visualizers.push_back( dotTrailsVisualizer     = new DotTrailsVisualizer());
    visualizers.push_back( connectedDotVisualizer  = new ConnectedDotsVisualizer());
    visualizers.push_back( nearestDotsVisualizer   = new NearestDotsVisualizer());
    visualizers.push_back( lineVisualizer          = new LineVisualizer());
    visualizers.push_back( fixedPointVisualizer    = new FixedPointVisualizer());
    visualizers.push_back( qualitiesVisualizer     = new QualitiesVisualizer());
    visualizers.push_back( convexHullVisualizer    = new ConvexHullVisualizer());
    
    for(int i=0; i<visualizers.size(); i++) {
        gui.addPage(visualizers[i]->params);
        visualizers[i]->params.loadXmlValues();
    }

    
    
    // Timeline
    
    midiIn.listPorts();
    midiIn.openPort(1);
    midiIn.addListener(this);
    midiIn.setVerbose(true);
    
    
    
    
  
    offset.set( 0.0f, 0.0f, 0.0f );
    scale.set( 1.0f / (float)INPUT_WIDTH, 1.0f / (float)INPUT_HEIGHT, 1.0f );
    
    visualizeInput          = false;
    
    // Ilda
    
    etherdream.setup();
    etherdream.setPPS(50000);
    
    // Camera &  Grabber
    grabber.setSize( INPUT_WIDTH , INPUT_HEIGHT );
    grabber.setImageType( OF_IMAGE_GRAYSCALE );
    grabber.setup();
    
    {
        ofDirectory dir;
        dir.listDir("inputVideo");
        if(dir.size() > 0) {
            videoPlayer.loadMovie(dir.getPath(dir.size() - 1));
            videoPlayer.setLoopState(OF_LOOP_NORMAL);
            cameraParams[PARAM_NAME_VIDEO_FRAME].setRange(0, videoPlayer.getTotalNumFrames());
        }
    }
    
    videoPtr = &grabber;
}

//--------------------------------------------------------------
void TheMeasuresTaken::update()
{
    // Camera & Grabber
    if(cameraParams[PARAM_NAME_CAMERA_USE_VIDEO]) {
        videoPtr = &videoPlayer;
        if(cameraParams[PARAM_NAME_VIDEO_PLAY].hasChanged()) {
            if(cameraParams[PARAM_NAME_VIDEO_PLAY]) videoPlayer.play();
            else videoPlayer.stop();
        }
        if(videoPlayer.isPlaying()) cameraParams[PARAM_NAME_VIDEO_FRAME] = videoPlayer.getCurrentFrame();
        else if(cameraParams[PARAM_NAME_VIDEO_FRAME].hasChanged()) videoPlayer.setFrame(cameraParams[PARAM_NAME_VIDEO_FRAME]);
        
    } else {
        videoPtr = &grabber;
        if(cameraParams[PARAM_NAME_LIBDC_BRIGHTNESS].hasChanged()) grabber.setBrightness(cameraParams[PARAM_NAME_LIBDC_BRIGHTNESS]);
        if(cameraParams[PARAM_NAME_LIBDC_GAMMA].hasChanged()) grabber.setGamma(cameraParams[PARAM_NAME_LIBDC_GAMMA]);
        if(cameraParams[PARAM_NAME_LIBDC_SHUTTER].hasChanged()) grabber.setShutter(cameraParams[PARAM_NAME_LIBDC_SHUTTER]);
        if(cameraParams[PARAM_NAME_LIBDC_GAIN].hasChanged()) grabber.setGain(cameraParams[PARAM_NAME_LIBDC_GAIN]);
    }
    
    videoPtr->update();

    imageInput.setFromPixels(videoPtr->getPixelsRef());
    imageInput.update();
    
    bool doFlipX = cameraParams[PARAM_NAME_CAMERA_FLIP_X];
    bool doFlipY = cameraParams[PARAM_NAME_CAMERA_FLIP_Y];
    if(doFlipX || doFlipY) imageInput.mirror(doFlipY, doFlipX);

    for( int i = 0; i < iimageSeqInputs.size(); ++i )
    {
        iimageSeqInputs[i]->setCurrentFrameNew( true );//videoPtr->isFrameNew() );
    }
    
//    if( videoPtr->isFrameNew() )
//    {
        for( int i = 0; i < iimageSeqInputs.size(); ++i )
        {
            iimageSeqInputs[i]->setROI( (float)cameraParams[PARAM_NAME_CAMERA_ROI_X1], (float)cameraParams[PARAM_NAME_CAMERA_ROI_Y1], (float)cameraParams[PARAM_NAME_CAMERA_ROI_X2], (float)cameraParams[PARAM_NAME_CAMERA_ROI_Y2] );
            
            iimageSeqInputs[i]->setPixels( imageInput.getPixelsRef() );
        }
//    }
    
    // Update GUI
    
    msa::controlfreak::update();
    
    // Select current input analyser
    
    currentInputAnalyser    = inputAnalysers[ inputParams[ PARAM_NAME_CURRENT_INPUT ] ];
    
    // Update current input analyser
    currentInputAnalyser->update();
    
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
    ildaFrame.polyProcessor.params.targetPointCount     = (int)ildaParams[ PARAM_NAME_ILDA_POINT_COUNT ];
    ildaFrame.polyProcessor.params.spacing              = (float)ildaParams[ PARAM_NAME_ILDA_SPACING ];
    
    ildaParams[ PARAM_NAME_POINT_COUNT_ORIG ].set( ildaFrame.stats.pointCountOrig );
    ildaParams[ PARAM_NAME_POINT_COUNT_PROC ].set( ildaFrame.stats.pointCountProcessed );
    
    scale.set( 1.0f / ((float)INPUT_WIDTH * ( (float)cameraParams[PARAM_NAME_CAMERA_ROI_X2] - (float)cameraParams[PARAM_NAME_CAMERA_ROI_X1] )), 1.0f / ((float)INPUT_HEIGHT * ( (float)cameraParams[PARAM_NAME_CAMERA_ROI_Y2] - (float)cameraParams[PARAM_NAME_CAMERA_ROI_Y1] )), 1.0f );
    
    if( (bool)ildaParams[ PARAM_NAME_ILDA_OUTPUT_CALIBRATION_ONLY ] )
    {
        ildaFrame.drawCalibration();
    } else {
        for(int i=0; i<visualizers.size(); i++) {
            PolylineVectorRefT visualizedLines = visualizers[i]->visualize( currentInputAnalyser , offset, scale );
            ildaFrame.addPolys( *visualizedLines, ofFloatColor(visualizers[i]->getBrightness()) );
        }
    }
    
    ildaFrame.update();
    
    if(ildaParams[PARAM_NAME_ENABLED]) etherdream.setPoints(ildaFrame);

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
    
    ofPushStyle();
    ofSetColor( 125 );
    ofDrawBitmapString( GUIDE_STRING, 0.0f, ofGetHeight() - 20.0f );
    ofPopStyle();
    
    float scale             = (float)cameraParams[ PARAM_NAME_CAMERA_SCREEN_SCALE ];
    
    ofPushMatrix();
    
    if( (bool)cameraParams[ PARAM_NAME_CAMERA_DRAW_COLOR ] )
    {
        ofTranslate( ofGetWidth() - INPUT_WIDTH * scale , 0.0f );
        ofScale( scale, scale );
        
        imageInput.draw(0, 0);
        
        float x1 = INPUT_WIDTH * (float)cameraParams[PARAM_NAME_CAMERA_ROI_X1];
        float y1 = INPUT_HEIGHT * (float)cameraParams[PARAM_NAME_CAMERA_ROI_Y1];
        
        float x2 = INPUT_WIDTH * (float)cameraParams[PARAM_NAME_CAMERA_ROI_X2];
        float y2 = INPUT_HEIGHT * (float)cameraParams[PARAM_NAME_CAMERA_ROI_Y2];
        
        if( (bool)cameraParams[PARAM_NAME_CAMERA_DRAW_ROI] )
        {
            ofPushStyle();
            
            ofNoFill();
            ofSetLineWidth( 2.0f );
            ofSetColor( ofColor::red );
            
            ofRect( x1, y1, x2 - x1, y2 - y1 );
            
            ofPopStyle();
        }
        
        if( (bool)cameraParams[PARAM_NAME_CAMERA_DRAW_CONTOURS] )
        {
            ofPushStyle();
            ofPushMatrix();
            ofSetColor( ofColor::red );

            iimageSeqInputs[ cameraParams[ PARAM_NAME_CAMERA_CONTOUR_SOURCE ] ]->drawDebug();
            
            ofPopMatrix();
            ofPopStyle();
        }
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
        inputParams.saveXmlValues();
        cameraParams.saveXmlValues();
        ildaParams.saveXmlValues();
        for(int i=0; i<visualizers.size(); i++) visualizers[i]->params.saveXmlValues();
        for(int i=0; i<iimageSeqInputs.size(); i++) iimageSeqInputs[i]->params.saveXmlValues();
    }
    
//    if( key == 'p' )
//    {
//        gui.toggleDraw();
//    }
    
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
        fixedPointVisualizer->params[PARAM_NAME_FIXED_POINT_FIX].set( true );
    }
    
    if( key == 'O' )
    {
        fixedPointVisualizer->params[PARAM_NAME_FIXED_POINT_CLEAR].set( true );
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
    ofPushStyle();
    ofSetColor( 255 );
    
//    ildaFrame.draw( ( ofGetWindowWidth() - SCREEN_VIS_AREA_WIDTH ) * 0.5f, ( ofGetWindowHeight() - SCREEN_VIS_AREA_HEIGHT ) * 0.5f, SCREEN_VIS_AREA_WIDTH, SCREEN_VIS_AREA_HEIGHT );
    
    ildaFrame.draw(0, ofGetWindowHeight() - SCREEN_VIS_AREA_HEIGHT, SCREEN_VIS_AREA_WIDTH, SCREEN_VIS_AREA_HEIGHT );
    ofPopStyle();
}

void TheMeasuresTaken::drawInputVisualization()
{
    ofPushMatrix();
    ofPushStyle();
    
//    ofTranslate( ( ofGetWindowWidth() - INPUT_WIDTH ) * 0.5f , ( ofGetWindowHeight() - INPUT_HEIGHT ) * 0.5f );
    ofTranslate(0, ( ofGetWindowHeight() - INPUT_HEIGHT ));
    
    ofSetColor( ofColor::black, 125 );
    
    ofRect( 0, 0, INPUT_WIDTH, INPUT_HEIGHT );
    
    ofSetColor( 125 );
    
    ofNoFill();
    ofSetLineWidth( 1.0f );
    
    ofRect( 0, 0, INPUT_WIDTH, INPUT_HEIGHT );
    
//    glEnable( GL_SCISSOR_TEST );
//    glScissor( ( ofGetWindowWidth() - INPUT_WIDTH ) * 0.5f, ( ofGetWindowHeight() - INPUT_HEIGHT ) * 0.5f, INPUT_WIDTH, INPUT_HEIGHT );
//    glScissor(0, ( ofGetWindowHeight() - INPUT_HEIGHT ), INPUT_WIDTH, INPUT_HEIGHT );
    
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
    
//    ofTranslate( ( ofGetWindowWidth() - SCREEN_VIS_AREA_WIDTH ) * 0.5f , ( ofGetWindowHeight() - SCREEN_VIS_AREA_HEIGHT ) * 0.5f );
    ofTranslate(0, ofGetWindowHeight() - SCREEN_VIS_AREA_HEIGHT);
    
    
    ofRect( 0, 0, SCREEN_VIS_AREA_WIDTH, SCREEN_VIS_AREA_HEIGHT );
    
    ofPopStyle();
    ofPopMatrix();
}