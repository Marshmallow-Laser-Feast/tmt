#include "TheMeasuresTaken.h"

//--------------------------------------------------------------
void TheMeasuresTaken::setup()
{
    // Setup App Settings
    
    ofSetWindowTitle( "The Measures Taken" );
    ofSetFrameRate( 60 );
    ofSetBackgroundColor( 0 );
    ofSetFullscreen( true );
    
    // Initiate Inputs
    
    multitouchInput         = new MultiTouchInput( INPUT_WIDTH, INPUT_HEIGHT );
    flockingInput           = new FlockingInput( FLOCKING_SAMPLE_COUT, INPUT_WIDTH, INPUT_HEIGHT, FLOCKING_COLUMNS, FLOCKING_ROWS, FLOCKING_MAX_SPEED, FLOCKING_MIN_FORCE, FLOCKING_MAX_FORCE, FLOCKING_ATTRAC_RAD_RATIO );
    
    multiTouchInputAnalyser = new InputAnalyser( multitouchInput, INPUT_TIMEOUT_FRAMES );
    flockingInputAnalyser   = new InputAnalyser( flockingInput, INPUT_TIMEOUT_FRAMES );
    
    inputAnalysers[0]       = multiTouchInputAnalyser;
    inputAnalysers[1]       = flockingInputAnalyser;
    
    currentInputAnalyser    = inputAnalysers[0];
    
    for ( int i = 0 ; i < INPUT_COUNT ; ++i )
    {
        inputAnalysers[i]->setMaxPathAnalyserSamples( MAX_PATH_ANALYSER_SAMPLES );
        inputAnalysers[i]->setMaxPathAnalyserHistory( MAX_PATH_ANALYSER_HISTORY );
        inputAnalysers[i]->setMaxPathAnalyserLength( MAX_PATH_ANALYSER_LENGTH );
    }
    
    // Setup Gui & Params
    
    params.setName("Parameters");
    
    params.addNamedIndex( PARAM_NAME_CURRENT_INPUT ).setLabels( 2, "MultiTouch", "Flocking" );
    
    gui.addPage(params);
    
    gui.toggleDraw();
    
    // Timeline
    
    timeline.setup();
    timeline.setDurationInSeconds(60);
    timeline.setLoopType(OF_LOOP_NORMAL);
    timeline.setOffset( ofVec2f( 0.0f, ofGetWindowHeight() - timeline.getHeight() ) );
        
    timeline.toggleShow();
    
    // Visualisation
    
    visualizeInput  = false;
}

//--------------------------------------------------------------
void TheMeasuresTaken::update()
{
    msa::controlfreak::update();
    
    currentInputAnalyser    = inputAnalysers[ params[ PARAM_NAME_CURRENT_INPUT ] ];
    
    currentInputAnalyser->update();
}

//--------------------------------------------------------------
void TheMeasuresTaken::draw()
{
    ofClear( 0 );
    
    drawVisualizationArea();

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
        params.saveXmlValues();
        params.saveXmlSchema();
        
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
    
    ofTranslate( ( ofGetWindowWidth() - PROJECTION_AREA_WIDTH ) * 0.5f , ( ofGetWindowHeight() - PROJECTION_AREA_HEIGHT ) * 0.5f );
    
    ofRect( 0, 0, PROJECTION_AREA_WIDTH, PROJECTION_AREA_HEIGHT );
    
    ofPopStyle();
    ofPopMatrix();
}