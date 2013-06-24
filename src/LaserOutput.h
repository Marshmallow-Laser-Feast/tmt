//
//  LaserOutput.h
//  TheMeasuresTaken
//
//  Created by Ali Nakipoglu on 6/18/13.
//
//

#pragma once

#include "ofMain.h"

#include "IControlFreakMapper.h"
#include "IControlFreakMapperOSCExt.h"
#include "IControlFreakMapperMidiExt.h"

#include "ofxEtherdream.h"
#include "ofxIldaFrame.h"

#include "IPanelDraws.h"

#include "IVisualizer.h"
#include "IFilter.h"

#define PARAM_NAME_ENABLED                      "Enabled"

#define PARAM_NAME_ILDA_OUTPUT_CALIBRATION_ONLY "Calibration Only"

#define PARAM_NAME_ILDA_DRAW_LINES              "Draw Lines"
#define PARAM_NAME_ILDA_DRAW_POINTS             "Draw Points"

#define PARAM_NAME_ILDA_FLIPX                   "Flip X"
#define PARAM_NAME_ILDA_FLIPY                   "Flip Y"

#define PARAM_NAME_ILDA_DOCAPX                  "Cap X"
#define PARAM_NAME_ILDA_DOCAPY                  "Cap Y"

#define PARAM_NAME_ILDA_PPS                     "PPS"

#define PARAM_NAME_ILDA_BLANK_COUNT             "Blank Count"
#define PARAM_NAME_ILDA_END_COUNT               "End Count"
#define PARAM_NAME_ILDA_OFFSET_X                "Offset X"
#define PARAM_NAME_ILDA_OFFSET_Y                "Offset Y"
#define PARAM_NAME_ILDA_SCALE_X                 "Scale X"
#define PARAM_NAME_ILDA_SCALE_Y                 "Scale Y"

#define PARAM_NAME_ILDA_SMOOTH_AMOUNT           "Smooth Amount"
#define PARAM_NAME_ILDA_OPTIMIZE_TOLERANCE      "Optimize Tolerance"
#define PARAM_NAME_ILDA_COLLAPSE                "Collapse"
#define PARAM_NAME_ILDA_POINT_COUNT             "Point Count"
#define PARAM_NAME_ILDA_SPACING                 "Spacing"

#define PARAM_NAME_POINT_COUNT_ORIG             "Point Count Orig"
#define PARAM_NAME_POINT_COUNT_PROC             "Point Count Processed"

class LaserOutput: public IControlFreakMapper, public IControlFreakMapperMidiExt, public IControlFreakMapperOSCExt, public IPanelDraws
{
    
public:
    
    LaserOutput()
    
    :IControlFreakMapper( "Laser Output" )
    
    {
        IControlFreakMapperMidiExt::setParams( params );
        IControlFreakMapperOSCExt::setParams( params );
        
        setupMidi();
        setupOCS();
        
        params.addBool( PARAM_NAME_ENABLED );
        
        params.addBool( PARAM_NAME_ILDA_OUTPUT_CALIBRATION_ONLY );
        
        params.addBool( PARAM_NAME_ILDA_DRAW_LINES );
        params.addBool( PARAM_NAME_ILDA_DRAW_POINTS );
        
        params.addBool( PARAM_NAME_ILDA_FLIPX );
        params.addBool( PARAM_NAME_ILDA_FLIPY );
        
        params.addBool( PARAM_NAME_ILDA_DOCAPX );
        params.addBool( PARAM_NAME_ILDA_DOCAPY );
        
        params.addInt(PARAM_NAME_ILDA_PPS).setRange(10000, 90000).setClamp(true).set(30000);
        
        params.addInt( PARAM_NAME_ILDA_BLANK_COUNT ).setRange( 0, 1000 ).setClamp( true );
        params.addInt( PARAM_NAME_ILDA_END_COUNT ).setRange( 0, 1000 ).setClamp( true );
        
        params.addFloat( PARAM_NAME_ILDA_OFFSET_X ).setRange( -1.0f, 1.0f ).setClamp( true ).setIncrement( 0.01f );
        params.addFloat( PARAM_NAME_ILDA_OFFSET_Y ).setRange( -1.0f, 1.0f ).setClamp( true ).setIncrement( 0.01f );
        
        params.addFloat( PARAM_NAME_ILDA_SCALE_X ).setRange( 0, 4.0f ).setClamp( true ).setIncrement( 0.01f );
        params.addFloat( PARAM_NAME_ILDA_SCALE_Y ).setRange( 0, 4.0f ).setClamp( true ).setIncrement( 0.01f );
        
        params.addInt( PARAM_NAME_ILDA_SMOOTH_AMOUNT ).setRange( 0, 50 ).setClamp( true );
        params.addFloat( PARAM_NAME_ILDA_OPTIMIZE_TOLERANCE ).setIncrement( 0.01f ).setClamp(true);
        params.addInt( PARAM_NAME_ILDA_POINT_COUNT ).setRange( 0, 2000 ).setClamp( true );
        
        params.addFloat( PARAM_NAME_ILDA_SPACING ).setIncrement( 0.01f ).setClamp(true);
        
        params.addInt( PARAM_NAME_POINT_COUNT_ORIG );
        params.addInt( PARAM_NAME_POINT_COUNT_PROC );
        
        etherdream.setup();
    };
    
    ~LaserOutput()
    {};
    
    
public:
    
    void update( const vector<IVisualizer *> &visualizers, vector<IFilter *> &filters, float audioAmp )
    {
        if( params[ PARAM_NAME_ILDA_PPS ].hasChanged() )
        {
            etherdream.setPPS( params[PARAM_NAME_ILDA_PPS] );
        }
        
        ildaFrame.clear();
        
        ildaFrame.params.output.transform.doFlipX           = (bool)params[ PARAM_NAME_ILDA_FLIPX ];
        ildaFrame.params.output.transform.doFlipY           = (bool)params[ PARAM_NAME_ILDA_FLIPY ];
        
        ildaFrame.params.output.transform.offset.x          = (float)params[ PARAM_NAME_ILDA_OFFSET_X ];
        ildaFrame.params.output.transform.offset.y          = (float)params[ PARAM_NAME_ILDA_OFFSET_Y ];
        
        ildaFrame.params.output.transform.scale.x           = (float)params[ PARAM_NAME_ILDA_SCALE_X ];
        ildaFrame.params.output.transform.scale.y           = (float)params[ PARAM_NAME_ILDA_SCALE_Y ];
        
        ildaFrame.params.output.blankCount                  = (int)params[ PARAM_NAME_ILDA_BLANK_COUNT ];
        ildaFrame.params.output.endCount                    = (int)params[ PARAM_NAME_ILDA_END_COUNT ];
        
        ildaFrame.params.output.doCapX                      = (bool)params[ PARAM_NAME_ILDA_DOCAPX ];
        ildaFrame.params.output.doCapY                      = (bool)params[ PARAM_NAME_ILDA_DOCAPY ];
        
        ildaFrame.params.draw.lines                         = (bool)params[ PARAM_NAME_ILDA_DRAW_LINES ];
        ildaFrame.params.draw.points                        = (bool)params[ PARAM_NAME_ILDA_DRAW_POINTS ];
        
        ildaFrame.polyProcessor.params.smoothAmount         = (int)params[ PARAM_NAME_ILDA_SMOOTH_AMOUNT ];
        ildaFrame.polyProcessor.params.optimizeTolerance    = (float)params[ PARAM_NAME_ILDA_OPTIMIZE_TOLERANCE ];
        ildaFrame.polyProcessor.params.targetPointCount     = (int)params[ PARAM_NAME_ILDA_POINT_COUNT ];
        ildaFrame.polyProcessor.params.spacing              = (float)params[ PARAM_NAME_ILDA_SPACING ];
        
        params[ PARAM_NAME_POINT_COUNT_ORIG ].set( ildaFrame.stats.pointCountOrig );
        params[ PARAM_NAME_POINT_COUNT_PROC ].set( ildaFrame.stats.pointCountProcessed );
        
        if( (bool)params[ PARAM_NAME_ILDA_OUTPUT_CALIBRATION_ONLY ] )
        {
            ildaFrame.drawCalibration();
        } else {
            for(int i=0; i<visualizers.size(); i++)
            {
                PolylineVectorRefT visualizedLines = visualizers[i]->getOutput();
                
                for( vector<IFilter*>::iterator it = filters.begin(); it != filters.end(); ++it )
                {
                    (*it)->apply( *visualizedLines, audioAmp );
                }
                
                ildaFrame.addPolys( *visualizedLines, ofFloatColor( visualizers[i]->getBrightness() ) );
            }
        }
        
        ildaFrame.update();
        
        if( (bool)params[PARAM_NAME_ENABLED] )
        {
            etherdream.setPoints( ildaFrame );
        }
    }

    void draw( float x, float y, float width, float height )
    {
        ildaFrame.draw( x, y, width, height );
    };
    
    virtual const std::string getPanelName() const { return "Laser Output"; };
    
    virtual void draw( float width, float height )
    {
        ildaFrame.draw( 0, 0, width, height );
    };
    
    virtual const ofVec2f getPanelSize() const
    {
        return ofVec2f( 150, 100 );
    };
    
private:
    
    ofxIlda::Frame  ildaFrame;
    ofxEtherdream   etherdream;
};
