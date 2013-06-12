//
//  RoofVisualizer.h
//  TheMeasuresTaken
//
//  Created by Memo Akten on 11/10/13.
//
//

#pragma once

#include "IVisualizer.h"

#include "ofMain.h"
#include "Params.h"


class RoofVisualizer : public IVisualizer
{
    
public:
    
    RoofVisualizer()
    {
        params.setName("RoofVisualizer");
        params.addFloat("amp").setRange(0, 5).setClamp(true);
        params.addInt("numPoints").setRange(0, 1000).setClamp(true);
        params.addFloat("centerPos").setClamp(true);
        params.addInt("smoothAmount").setClamp(true);
        params.addFloat("easeAmount").setClamp(true);
    };
    
    ~RoofVisualizer()
    {};
    
public:
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale )
    {
        int timeOffset = params[PARAM_NAME_TIME_OFFSET];
        int numPoints = params["numPoints"];
        float amp = params["amp"];
        float centerPos = params["centerPos"];
        int smoothAmount = params["smoothAmount"];
        float easeAmount = params["easeAmount"];
        
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        if((int)params[PARAM_NAME_BRIGHTNESS] == 0) {
            return result;
        }
        
        ofPolyline newPoly;
        const vector<ofPolyline> &polys = inputAnalyser->getInput()->getPolys();
        
        for(int j=0; j<numPoints; j++) {
            float x = ofMap(j, 0, numPoints-1, 0, INPUT_WIDTH);
            ofPoint p(x, INPUT_HEIGHT * centerPos);;
            for(int i=0; i<polys.size(); i++) {
                const ofPolyline &poly = polys[i];
                if(ofInRange(x, poly.getBoundingBox().getLeft(), poly.getBoundingBox().getRight())) {
                    p.interpolate(poly.getClosestPoint(ofPoint(x, 0)), amp);
                }
            }

            p = p * scale + offset;
            if(easeAmount > 0.01 && oldPoly.size() == numPoints) {
                p.interpolate(oldPoly[j], easeAmount);
            }
            
            newPoly.lineTo(p);

        }
        if(smoothAmount) newPoly = newPoly.getSmoothed(smoothAmount);
        
        
        result->push_back(newPoly);
        oldPoly = newPoly;

        return result;
    };
    
private:
    ofPolyline oldPoly;
};

