
//
//  Panel.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/19/13.
//
//

#pragma once

#include "ofMain.h"

#include "IPanelDraws.h"

#include "PanelBar.h"
#include "ResizeCorner.h"

#define TITLE_BAR_HEIGHT    20.0f
#define CLOSE_BUTTON_WIDTH  50.0f
#define RESIZE_CORNER_SIZE  20.0f

class Panel
{
  
public:
    
    Panel( float x_, float y_, float width_, float height_, IPanelDraws * panelDraws_ )
    
    :panelDraws( panelDraws_ )
    ,x( x_ )
    ,y( y_ )
    ,width( width_ )
    ,height( height_ )
    ,isActivePanel( false )
    ,isClose( false )
    
    {
        titleBar        = new PanelBar( panelDraws_->getPanelName() );
        closeBar        = new PanelBar( "Close" );
        resizeCorner    = new ResizeCorner();
        
        titleBar->set( x, y, width - CLOSE_BUTTON_WIDTH, TITLE_BAR_HEIGHT );
        closeBar->set( x + width - CLOSE_BUTTON_WIDTH, y, CLOSE_BUTTON_WIDTH, TITLE_BAR_HEIGHT );
        resizeCorner->set( x + width - RESIZE_CORNER_SIZE, y + TITLE_BAR_HEIGHT + height - RESIZE_CORNER_SIZE, RESIZE_CORNER_SIZE, RESIZE_CORNER_SIZE );
    };
    
    ~Panel(){};
    
public:
    
    float getX()
    {
        return x;
    }
    
    float getY()
    {
        return y;
    }
    
    float getWidth()
    {
        return width;
    }
    
    float getHeight()
    {
        return height;
    }
    
    string getName()
    {
        return panelDraws->getPanelName();
    }
    
    bool isActive()
    {
        return isActivePanel;
    }
    
    bool isClosed()
    {
        return isClose;
    }
    
    void update()
    {
        titleBar->update();
        resizeCorner->update();
        closeBar->update();
        
        isActivePanel   = titleBar->isMousePressed() || resizeCorner->isMousePressed() ;
        isClose         = closeBar->isMousePressed();
        
        if( titleBar->isDragging() )
        {
            x           += titleBar->getDragOffset().x;
            y           += titleBar->getDragOffset().y;
        }
        
        if( resizeCorner->isDragging()  )
        {
            width       += resizeCorner->getDragOffset().x;
            height      += resizeCorner->getDragOffset().y;
        }
        
        titleBar->set( x, y, width - CLOSE_BUTTON_WIDTH, TITLE_BAR_HEIGHT );
        closeBar->set( x + width - CLOSE_BUTTON_WIDTH, y, CLOSE_BUTTON_WIDTH, TITLE_BAR_HEIGHT );
        resizeCorner->set( x + width - RESIZE_CORNER_SIZE, y + TITLE_BAR_HEIGHT + height - RESIZE_CORNER_SIZE, RESIZE_CORNER_SIZE, RESIZE_CORNER_SIZE );
    }
    
    void draw()
    {
        ofEnableAlphaBlending();
        
        ofPushMatrix();
        ofPushStyle();
        
        ofTranslate( x, y + TITLE_BAR_HEIGHT );
        
        ofSetColor( 30 );
        ofRect( 0, 0, width, height );
        
        ofNoFill();
        
        ofSetColor( ofColor( 100, 200 ) );
        ofRect( 0, 0, width, height );
        
        ofFill();
        
        glEnable( GL_SCISSOR_TEST );
        glScissor( x + 2, ofGetHeight() - ( (y + TITLE_BAR_HEIGHT + height) - 2), width - 4, height - 4 );
        
        ofTranslate( 3, 3 );
        
        panelDraws->draw( width, height );
        
        glDisable( GL_SCISSOR_TEST );
        
        ofPopStyle();
        ofPopMatrix();
        
        ofDisableAlphaBlending();
        
        resizeCorner->setVisible( true );
        titleBar->setVisible( true );
        closeBar->setVisible( true );
        
        titleBar->draw();
        resizeCorner->draw();
        closeBar->draw();
        
        resizeCorner->setVisible( false );
        titleBar->setVisible( false );
        closeBar->setVisible( false );
    };
    
private:
    
    float           x;
    float           y;
    float           width;
    float           height;
    
    bool            isActivePanel;
    bool            isClose;
    
    IPanelDraws     *panelDraws;
    
    PanelBar        *titleBar;
    PanelBar        *closeBar;
    ResizeCorner    *resizeCorner;
};