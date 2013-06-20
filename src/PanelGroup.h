//
//  PanelGroup.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/19/13.
//
//

#pragma once

#include <vector>

#include "ofMain.h"

#include "Panel.h"

class PanelGroup
{
    
public:
    
    PanelGroup()
    
    :visible( true )
    
    {};
    
    ~PanelGroup()
    {};
    
public:
    
    void clear()
    {
        for( std::vector<Panel *>::iterator it = panels.begin(); it != panels.end(); ++it )
        {
            delete *it;
        }
        
        panels.clear();
    }
    
    const std::vector<Panel *> & getPanels() const
    {
        return panels;
    }
    
    void hide()
    {
        visible = false;
    }
    
    void show()
    {
        visible = true;
    }
    
    void toggleVisibility()
    {
        visible = !visible;
    }
    
    void addPanel( Panel *panel )
    {
        panels.push_back( panel );
    }
    
    void update()
    {
        if( !visible )
        {
            return;
        }
        
        std::vector<Panel *>::iterator activePanelIt;
        
        bool activePanel    = false;
        
        for( std::vector<Panel *>::iterator it = panels.begin(); it != panels.end();  )
        {
            (*it)->update();
         
            if( (*it)->isClosed() )
            {
                delete *it;
                
                it = panels.erase( it );
            } else {
                if( (*it)->isActive() )
                {
                    activePanelIt   = it;
                    activePanel     = true;
                }
                
                ++it;
            }
        }
        
        if( activePanel )
        {
            Panel *panel        = *activePanelIt;
            
            panels.erase(activePanelIt);
            panels.push_back( panel );
        }
    };
    
    void draw()
    {
        if( !visible )
        {
            return;
        }
        
        for( std::vector<Panel *>::iterator it = panels.begin(); it != panels.end(); ++it )
        {
            (*it)->draw();
        }
    };
    
private:
    
    bool                    visible;
    std::vector<Panel *>    panels;
};
