//
//  CameraInput.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/10/13.
//
//

#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"

#include "BlobResult.h"
#include "BlobContour.h"
#include "BlobOperators.h"
#include "BlobProperties.h"
#include "ComponentLabeling.h"
#include "blob.h"

#include "Input.h"

class CameraInput : public Input
{
    
public:
    
    CameraInput( float width_, float height_ )
    
    :threshold( 0 )
    ,minBlobArea( 0 )
    
    {
        videoInput.initGrabber( width_, height_ );
        
        cvColorImage.allocate( width_, height_ );
        cvGrayscaleImage.allocate( width_, height_ );
    };
    
    ~CameraInput()
    {}
    
public:
    
    void setThreshold( int value )
    {
        threshold   = value;
    }
    
    void setMinBlobArea( float value )
    {
        minBlobArea   = value;
    }
    
    virtual void update()
    {
        videoInput.update();
        
        if( videoInput.isFrameNew() )
        {
            samples.clear();
            
            cvColorImage.setFromPixels( videoInput.getPixelsRef() );
            
            cvGrayscaleImage    = cvColorImage;
            
            cvGrayscaleImage.mirror( false, true );
            cvGrayscaleImage.threshold( threshold );
            
            CBlobResult blobs   = CBlobResult( cvGrayscaleImage.getCvImage() , NULL, 255 );
         
            blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, minBlobArea );
            blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_GREATER, cvGrayscaleImage.getWidth() * cvGrayscaleImage.getHeight() / 2 );
            
            for ( int i = 0; i < blobs.GetNumBlobs(); i++ )
            {
                CBlob *currentBlob = blobs.GetBlob(i);
                
                PointInputSampleT   sample;
                
                sample.setSampleID( currentBlob->GetID() );
                
                sample.setSample( ofPoint( currentBlob->MinX(), currentBlob->MinY(), 0.0f ) );
                
                samples.push_back( sample );
            }

        }
    };
    
    void draw()
    {
        cvGrayscaleImage.draw(0, 0);
    }
    
    virtual const vector<PointInputSampleT> & getSamples() const
    {
        return samples;
    };
    
private:
    
    ofVideoGrabber                  videoInput;
    
    ofxCvColorImage                 cvColorImage;
    ofxCvGrayscaleImage             cvGrayscaleImage;
    
    int                             threshold;
    float                           minBlobArea;
    
    std::vector<PointInputSampleT>  samples;
};