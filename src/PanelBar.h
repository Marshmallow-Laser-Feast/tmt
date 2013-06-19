//
//  PanelBar.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/19/13.
//
//

#pragma once

#include <string>

#include "ofMain.h"

#include "ofxMSAInteractiveObject.h"

class PanelBar: public ofxMSAInteractiveObject
{
    
public:
    
    PanelBar( std::string title_ )
    
    :title( title_ )
    ,dragging( false )
    
    {
        enableMouseEvents();
    };
    
    ~PanelBar()
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
        
        ofSetColor( ofColor( 60, 200 ) );
        ofRect( 0, 0, getWidth(), getHeight() );
        
        ofNoFill();
        
        ofSetColor( ofColor( 100, 200 ) );
        ofRect( 0, 0, getWidth(), getHeight() );
        
        glEnable( GL_SCISSOR_TEST );
        glScissor( getX(), ofGetHeight() - (getY() + getHeight()), getWidth() - 4.0f, getHeight() );
        
        ofSetColor( ofColor( 150, 200 ) );
        ofDrawBitmapString( title, 4.0f, 15.0f );
        
        glDisable( GL_SCISSOR_TEST );
        
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
    
    std::string title;
};
