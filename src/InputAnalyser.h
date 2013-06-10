//
//  InputAnalyser.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/9/13.
//
//

#pragma once

#include <map>
#include <vector>

#include "Input.h"
#include "PathAnalyser.h"

class InputAnalyser {
    
private:
    
    struct PathAnalyserInfo
    {
        PathAnalyser    *analyser;
        bool            isFound;
        int             timeOutFrames;
    };
    
public:
    
    InputAnalyser( Input *input_, int timeOutFrames_ )
    
    :input( input_ )
    ,timeOutFrames( timeOutFrames_ )
    ,maxLength( 0.0f )
    ,maxSamples( 0 )
    ,maxHistory( 0 )
    
    {};
    
    ~InputAnalyser()
    {};
    
public:
    
    void update()
    {
        input->update();
        
        for( std::map<int, PathAnalyserInfo>::iterator it = idPathAnalyserInfoMap.begin(); it != idPathAnalyserInfoMap.end(); ++it )
        {
            it->second.isFound  = false;
        }
        
        for( vector<PointInputSampleT>::const_iterator it = input->getSamples().begin(); it != input->getSamples().end(); ++it )
        {
            if( idPathAnalyserInfoMap.count( it->getSampleID() ) == 0 )
            {
                PathAnalyser    *analyser                                   = new PathAnalyser();
                
                idPathAnalyserInfoMap[ it->getSampleID() ]                  = PathAnalyserInfo();
                idPathAnalyserInfoMap[ it->getSampleID() ].analyser         = analyser;
                
                analyser->setMaxSamples( maxSamples );
                analyser->setMaxHistory( maxHistory );
                analyser->setMaxLength( maxLength );
            }
            
            idPathAnalyserInfoMap[ it->getSampleID() ].isFound              = true;
            idPathAnalyserInfoMap[ it->getSampleID() ].timeOutFrames        = 0;
            
            idPathAnalyserInfoMap[ it->getSampleID() ].analyser->addSample( it->getSample() );
            
            idPathAnalyserInfoMap[ it->getSampleID() ].analyser->update();
        }
        
        std::map<int, PathAnalyserInfo>::iterator it = idPathAnalyserInfoMap.begin();
        
        while( it != idPathAnalyserInfoMap.end() )
        {
            if( !it->second.isFound )
            {
                if( it->second.timeOutFrames < timeOutFrames )
                {
                    it->second.timeOutFrames++;
                    ++it;
                } else {
                    idPathAnalyserInfoMap.erase( it++ );
                }
            } else {
               ++it; 
            }
        }
        
        pathAnalysers.clear();
        
        for( std::map<int, PathAnalyserInfo>::iterator it = idPathAnalyserInfoMap.begin(); it != idPathAnalyserInfoMap.end(); ++it )
        {
            pathAnalysers.push_back( it->second.analyser );
        }
    };
    
    void setMaxPathAnalyserSamples( int value )
    {
        maxSamples  = value;
        
        for( std::vector<PathAnalyser*>::iterator it = pathAnalysers.begin(); it != pathAnalysers.end(); ++it )
        {
            (*it)->setMaxSamples( maxSamples );
        }
    }
    
    void setMaxPathAnalyserHistory( int value )
    {
        maxHistory  = value;
        
        for( std::vector<PathAnalyser*>::iterator it = pathAnalysers.begin(); it != pathAnalysers.end(); ++it )
        {
            (*it)->setMaxHistory( maxHistory );
        }
    }
    
    void setMaxPathAnalyserLength( float value )
    {
        maxLength   = value;
        
        for( std::vector<PathAnalyser*>::iterator it = pathAnalysers.begin(); it != pathAnalysers.end(); ++it )
        {
            (*it)->setMaxLength( maxLength );
        }
    }
    
    void draw()
    {
        for( std::vector<PathAnalyser*>::iterator it = pathAnalysers.begin(); it != pathAnalysers.end(); ++it )
        {
            (*it)->draw();
        }
    };
    
    void drawSamples( float circleSize )
    {
        for( std::vector<PathAnalyser*>::iterator it = pathAnalysers.begin(); it != pathAnalysers.end(); ++it )
        {
            (*it)->drawSamples( circleSize );
        }
    };
    
    void drawCurvature( float length )
    {
        for( std::vector<PathAnalyser*>::iterator it = pathAnalysers.begin(); it != pathAnalysers.end(); ++it )
        {
            (*it)->drawCurvature( length );
        }
    };
    
    void drawAcceleration( float length )
    {
        for( std::vector<PathAnalyser*>::iterator it = pathAnalysers.begin(); it != pathAnalysers.end(); ++it )
        {
            (*it)->drawAcceleration( length );
        }
    };
    
    float getAvarageLength( int historyOffset = 0 )
    {
        float   value   = 0.0f;
        
        for( std::vector<PathAnalyser*>::iterator it = pathAnalysers.begin(); it != pathAnalysers.end(); ++it )
        {
            if( (*it)->getLengths().size() > 0 )
            {
                value   += (*it)->getLengths()[ MAX( 0, (*it)->getLengths().size() - ( historyOffset + 1 ) ) ];
            }
        }
        
        return value / (float)pathAnalysers.size();
    };
    
    float getAvarageAmplitude( int historyOffset = 0 )
    {
        float   value   = 0.0f;
        
        for( std::vector<PathAnalyser*>::iterator it = pathAnalysers.begin(); it != pathAnalysers.end(); ++it )
        {
            if( (*it)->getAmplitudes().size() > 0 )
            {
                value   += (*it)->getAmplitudes()[ MAX( 0, (*it)->getAmplitudes().size() - ( historyOffset + 1 ) ) ];
            }
        }
        
        return value / (float)pathAnalysers.size();
    };
    
    float getAvarageCurvature( int historyOffset = 0 )
    {
        float   value   = 0.0f;
        
        for( std::vector<PathAnalyser*>::iterator it = pathAnalysers.begin(); it != pathAnalysers.end(); ++it )
        {
            if( (*it)->getCurvatures().size() > 0 )
            {
                value   += (*it)->getCurvatures()[ MAX( 0, (*it)->getCurvatures().size() - ( historyOffset + 1 ) ) ];
            }
        }
        
        return value / (float)pathAnalysers.size();
    };
    
    float getAvarageAcceleration( int historyOffset = 0 )
    {
        float   value   = 0.0f;
        
        for( std::vector<PathAnalyser*>::iterator it = pathAnalysers.begin(); it != pathAnalysers.end(); ++it )
        {
            if( (*it)->getAccelerations().size() > 0 )
            {
                value   += (*it)->getAccelerations()[ MAX( 0, (*it)->getAccelerations().size() - ( historyOffset + 1 ) ) ];
            }
        }
        
        return value / (float)pathAnalysers.size();
    };
    
    float getAvarageVelocity( int historyOffset = 0 )
    {
        float   value   = 0.0f;
        
        for( std::vector<PathAnalyser*>::iterator it = pathAnalysers.begin(); it != pathAnalysers.end(); ++it )
        {
            if( (*it)->getVelocities().size() > 0 )
            {
                value   += (*it)->getVelocities()[ MAX( 0, (*it)->getVelocities().size() - ( historyOffset + 1 ) ) ];
            }
        }
        
        return value / (float)pathAnalysers.size();
    };
    
    Input * getInput()
    {
        return input;
    };
    
    std::vector<PathAnalyser*> & getPathAnalysers()
    {
        return pathAnalysers;
    };
    
private:
    
    Input                               *input;
    
    int                                 timeOutFrames;
    
    float                               maxLength;
    
    int                                 maxSamples;
    int                                 maxHistory;
    
    std::map<int, PathAnalyserInfo>     idPathAnalyserInfoMap;
    std::vector<PathAnalyser*>          pathAnalysers;
};
