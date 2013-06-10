//
//  Rope.cpp
//  ofxILDA demo
//
//  Created by Memo Akten on 23/05/2013.
//
//

#include "Rope.h"


int Rope::numParticles = 10;
float Rope::stiffness = 0.5;
float Rope::restLengthMult = 1;
float Rope::restLengthSpeed = 1;
float Rope::radiusBase = 0.1;
float Rope::radiusMult = 0.1;

msa::physics::World2D *Rope::physics = NULL;
