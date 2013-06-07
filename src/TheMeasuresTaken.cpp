#include "TheMeasuresTaken.h"

//--------------------------------------------------------------
void TheMeasuresTaken::setup()
{
    // Setup App Settings
    
    ofSetWindowTitle( "The Measures Taken" );
    ofSetFrameRate( 60 );
    ofSetBackgroundColor( 0 );
    
    // Initiate Inputs
    
    multitouchInput = new MultiTouchInput( INPUT_WIDTH, INPUT_HEIGHT );
    flockingInput   = new FlockingInput( FLOCKING_SAMPLE_COUT, INPUT_WIDTH, INPUT_HEIGHT, FLOCKING_MIN_VEL, FLOCKING_MAX_VEL, FLOCKING_COLUMNS, FLOCKING_ROWS, FLOCKING_MIN_FORCE, FLOCKING_MAX_FORCE, FLOCKING_ATTRAC_RAD_RATIO );
    
    inputs[0]       = multitouchInput;
    inputs[1]       = flockingInput;
    
    currentInput    = inputs[0];
    
    // Setup Gui & Params
    
    params.setName("Parameters");
    
    params.addNamedIndex( PARAM_NAME_CURRENT_INPUT ).setLabels( 2, "MultiTouch", "Flocking" );
    
    gui.addPage(params);
    
    gui.toggleDraw();
    
    // Timeline
    
    timeline.setup();
    timeline.setDurationInSeconds(60);
    timeline.setLoopType(OF_LOOP_NORMAL);
        
    timeline.toggleShow();
    
    // Visualisation
    
    visualizeInput  = false;
}

//--------------------------------------------------------------
void TheMeasuresTaken::update()
{
    msa::controlfreak::update();
    
    currentInput    = inputs[ params[ PARAM_NAME_CURRENT_INPUT ] ];
    
    currentInput->update();
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
    
    ofPushStyle();
    
    ofSetColor( 125 );
    
    ofDrawBitmapString( GUIDE_STRING, 10.0f, ofGetHeight() - 5.0f );
    
    ofPopStyle();
    
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
    
    ofSetColor( 255 );
    
    ofNoFill();
    ofSetLineWidth( 1.0f );
    
    ofRect( 0, 0, INPUT_WIDTH, INPUT_HEIGHT );
    
    ofFill();
    
    for( vector<PointInputSampleT>::const_iterator it = currentInput->getSamples().begin(); it != currentInput->getSamples().end(); ++it )
    {
        ofCircle( it->getSample(), 2.0f );
    }
    
    ofPopStyle();
    ofPopMatrix();
}

void TheMeasuresTaken::drawVisualizationArea()
{
    ofPushMatrix();
    ofPushStyle();
    
    ofNoFill();
    ofSetLineWidth( 1.0f );
    ofSetColor( ofColor::darkGray );
    
    ofTranslate( ( ofGetWindowWidth() - PROJECTION_AREA_WIDTH ) * 0.5f , ( ofGetWindowHeight() - PROJECTION_AREA_HEIGHT ) * 0.5f );
    
    ofRect( 0, 0, PROJECTION_AREA_WIDTH, PROJECTION_AREA_HEIGHT );
    
    ofPopStyle();
    ofPopMatrix();
}