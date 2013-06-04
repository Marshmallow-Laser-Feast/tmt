#include "TheMeasuresTaken.h"

//--------------------------------------------------------------
void TheMeasuresTaken::setup()
{
    ofSetWindowTitle( "The Measures Taken" );
    ofSetFrameRate( 60 );
    ofSetBackgroundColor( 0 );
    
    pathAnalyser.setMaxSegmentRotation( MAX_SEGMENT_ROTATION );
    pathAnalyser.setMaxLength( MAX_PATH_LENGTH );
    pathAnalyser.setMaxSamples( 60 );
    pathAnalyser.setMaxHistory( MAX_VISUALISATION_SAMPLES );
    
#ifdef LIVE_INPUT
    
    videoGrabber.setVerbose(true);
	videoGrabber.initGrabber( VIDEO_RES_WIDTH, VIDEO_RES_HEIGHT );
    
    videoColorImage.allocate( VIDEO_RES_WIDTH, VIDEO_RES_HEIGHT );
    videoGrayImage.allocate( VIDEO_RES_WIDTH, VIDEO_RES_HEIGHT );
    
    videoThresholdingValue  = 80;
    
    minBlobArea             = 20;
    maxBlobArea             = 200;
    
#endif
    
#ifdef VISUALISATION
    
    visualizeAmplitude      = true;
    visualizeCurvature      = true;
    visualizeAcceleration   = true;
    visualizeVelocity       = true;
    
#endif
    
}

//--------------------------------------------------------------
void TheMeasuresTaken::update()
{
    pathAnalyser.addSample( ofGetMouseX(), ofGetMouseY(), 0 );
    pathAnalyser.update();
    
#ifdef LIVE_INPUT    

    videoGrabber.update();
    
    if ( videoGrabber.isFrameNew() )
    {
        videoColorImage.setFromPixels( videoGrabber.getPixels(), VIDEO_RES_WIDTH, VIDEO_RES_HEIGHT );
        
        videoGrayImage  = videoColorImage;
        
        videoGrayImage.threshold( videoThresholdingValue );
        
        videoContourFinder.findContours( videoGrayImage, minBlobArea, maxBlobArea, 0, false );
    }
    
#endif
    
}

//--------------------------------------------------------------
void TheMeasuresTaken::draw()
{
    ofClear( 0 );
    
    ofSetColor( 255 );
    
#ifdef LIVE_INPUT    
    
    
    
    
    
#endif
    
    ofNoFill();
    
    ofSetColor( ofColor::red );
    
    pathAnalyser.draw();

#ifdef VISUALISATION
    
    ofVec3f offset( 40.0f, 40.0f );
    ofVec3f visualizationOffset;
    
    if( visualizeAmplitude )
    {
        ofPushMatrix();
        ofTranslate( offset + visualizationOffset );
        
        ofSetColor( ofColor::white );
        ofDrawBitmapString( "Amplitude", 0, 0 );
        
        ofTranslate( 0, 5.0f );
        
        ofFill();
        
        ofSetColor( 30 );
        ofRect( 0, 0, VISUALISATION_WIDTH, VISUALISATION_HEIGHT );
        
        ofNoFill();
        ofSetLineWidth( 1.0f );
        
        ofSetColor( ofColor::gold );
        
        visualize( pathAnalyser.getAmplitudes(), VISUALISATION_WIDTH, VISUALISATION_HEIGHT );
        
        ofPopMatrix();
        
        visualizationOffset.y   += VISUALISATION_HEIGHT + 20.0f;
    }
    
    if( visualizeCurvature )
    {
        ofPushMatrix();
        ofTranslate( offset + visualizationOffset );
        
        ofSetColor( ofColor::white );
        ofDrawBitmapString( "Curvature", 0, 0 );
        
        ofTranslate( 0, 5.0f );
        
        ofFill();
        
        ofSetColor( 30 );
        ofRect( 0, 0, VISUALISATION_WIDTH, VISUALISATION_HEIGHT );
        
        ofNoFill();
        ofSetLineWidth( 1.0f );
        
        ofSetColor( ofColor::pink );
        
        visualize( pathAnalyser.getCurvatures(), VISUALISATION_WIDTH, VISUALISATION_HEIGHT );
        
        ofPopMatrix();
        
        visualizationOffset.y   += VISUALISATION_HEIGHT + 20.0f;
    }
    
    if( visualizeAcceleration )
    {
        ofPushMatrix();
        ofTranslate( offset + visualizationOffset );
        
        ofSetColor( ofColor::white );
        ofDrawBitmapString( "Acceleraiton", 0, 0 );
        
        ofTranslate( 0, 5.0f );
        
        ofFill();
        
        ofSetColor( 30 );
        ofRect( 0, 0, VISUALISATION_WIDTH, VISUALISATION_HEIGHT );
        
        ofNoFill();
        ofSetLineWidth( 1.0f );
        
        ofSetColor( ofColor::aqua );
        
        visualize( pathAnalyser.getAccelerations(), VISUALISATION_WIDTH, VISUALISATION_HEIGHT );
        
        ofPopMatrix();
        
        visualizationOffset.y   += VISUALISATION_HEIGHT + 20.0f;
    }
    
    if( visualizeVelocity )
    {
        ofPushMatrix();
        ofTranslate( offset + visualizationOffset );
        
        ofSetColor( ofColor::white );
        ofDrawBitmapString( "Velocity", 0, 0 );
        
        ofTranslate( 0, 5.0f );
        
        ofFill();
        
        ofSetColor( 30 );
        ofRect( 0, 0, VISUALISATION_WIDTH, VISUALISATION_HEIGHT );
        
        ofNoFill();
        ofSetLineWidth( 1.0f );
        
        ofSetColor( ofColor::yellowGreen );
        
        visualize( pathAnalyser.getVelocities(), VISUALISATION_WIDTH, VISUALISATION_HEIGHT );
        
        ofPopMatrix();
        
        visualizationOffset.y   += VISUALISATION_HEIGHT + 20.0f;
    }
    
#endif
    
    ofSetColor( ofColor::red );
    
    ofDrawBitmapString( ofToString( ofGetFrameRate() ) , 40.0, 10.0f );
    
}

//--------------------------------------------------------------
void TheMeasuresTaken::keyPressed(int key)
{
    if( key == 'f' )
    {
        ofToggleFullscreen();
    }
    
    if( key == 'c' )
    {
        pathAnalyser.clear();
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

#ifdef VISUALISATION

void TheMeasuresTaken::visualize( const deque<float> &values, const float width, const float height )
{
    float   horizontalOffsetPerSample   = width / (float)values.size();
    int     sampleIndex                 = 0;
    
    ofBeginShape();
    
    for( deque<float>::const_iterator it = values.begin(); it != values.end(); ++it )
    {
        ofVertex( (float)sampleIndex * horizontalOffsetPerSample, ofClamp(*it, 0.0f, 1.0f) * height, 0.0f );
        
        sampleIndex++;
    }
    
    ofEndShape();
}

#endif