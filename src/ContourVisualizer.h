//
//  ContourVisualizer.h
//  TheMeasuresTaken
//
//  Created by Memo Akten on 11/10/13.
//
//

#pragma once

#include "IVisualizer.h"

#include "ofMain.h"
#include "Params.h"


class ContourVisualizer : public IVisualizer
{
    
public:
    
    ContourVisualizer()
    {
        params.setName("ContourVisualizer");
    };
    
    ~ContourVisualizer()
    {};
    
public:
    
    virtual PolylineVectorRefT visualize( InputAnalyser *inputAnalyser, ofVec3f & offset, ofVec3f scale )
    {
        int timeOffset = params[PARAM_NAME_TIME_OFFSET];
        
        PolylineVectorRefT  result( new std::vector<ofPolyline>() );
        if((int)params[PARAM_NAME_BRIGHTNESS] == 0) {
            return result;
        }
        
        const vector<ofPolyline> &polys = inputAnalyser->getInput()->getPolys();
        for(int i=0; i<polys.size(); i++) {
            const ofPolyline &poly = polys[i];
            ofPolyline newPoly;
            for(int j=0; j<poly.size(); j++) {
                newPoly.addVertex(offset + poly[j] * scale);
            }
            newPoly.close();
            result->push_back(newPoly);
        }
//        *result = inputAnalyser->getInput()->getPolys();
        
//        int count = inputAnalyser->getPathAnalysers().size();
//        ofPolyline polyline;
//        for(int i=0; i<count; i++) {
//            int index = ofClamp(inputAnalyser->getPathAnalysers()[i]->getSamples().size()-1-timeOffset, 0, inputAnalyser->getPathAnalysers()[i]->getSamples().size()-1);
//            polyline.addVertex(offset + inputAnalyser->getPathAnalysers()[i]->getSamples()[index] * scale);
//        }
//        result->push_back(ofxCv::convexHull(polyline));
        return result;
    };
    
private:
};

