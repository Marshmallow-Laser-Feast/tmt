//
//  ResizeCorner.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/19/13.
//
//

#pragma once

#include <string>

#include "ofMain.h"

#include "ofxMSAInteractiveObject.h"

class ResizeCorner: public ofxMSAInteractiveObject
{
    
public:
    
    ResizeCorner()
    {
        enableMouseEvents();
    };
    
    ~ResizeCorner()
    {};
    
public:
    
    void setVisible( bool value )
    {
        visible = value;
    }
    
    const bool isDragging() const
    {
        return dragging;
    }
    
    const ofPoint & getDragOffset() const
    {
        return offset;
    }
    
    virtual void update()
    {
        if( dragging )
        {
            ofPoint current( ofGetMouseX(), ofGetMouseY() );
            
            offset  = current - prevPos;
            
            prevPos = current;
        }
    }
    
    virtual void draw()
    {
        if( !visible )
        {
            return;
        }

        ofPushMatrix();
        ofPushStyle();
        
        ofTranslate( getX(), getY() );
        
        ofSetColor( ofColor( 100, 200 ) );
        
        ofTriangle( 0, getHeight(), getWidth(), 0, getWidth(), getHeight() );
                
        ofPopStyle();
        ofPopMatrix();
    };
    
    virtual void mousePressed(int x, int y, int button)
    {
        if( hitTest( x, y ) )
        {
            dragging    = true;
            
            prevPos.set( ofGetMouseX(), ofGetMouseY() );
        }
    }
    
    virtual void mouseReleased(int x, int y, int button)
    {
        dragging        = false;
        
        offset.set( 0, 0 );
    }

    
private:
    
    bool        dragging;
    bool        visible;
    
    ofPoint     prevPos;
    ofPoint     offset;
    
};