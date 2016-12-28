//
//  Terrain.hpp
//  DaliIsland
//
//  Created by Luca Mezzatesta on 11/06/16.
//  Copyright © 2016 Luca Mezzatesta. All rights reserved.
//

// Cerca FIXME per correggere i metodi che non vanno bene!

#ifndef Terrain_hpp
#define Terrain_hpp

#include <stdlib.h>

#ifndef MATH
#define MATH
#include <math.h>
#endif

#include <iostream>
#include <fstream>

#ifndef GLUT
#define GLUT
#include <GLUT/GLUT.h>
#endif

// DEFAULT SETTINGS AND FEATURES
#define TERRAIN_FILENAME "./hight_map3.txt"

// MATERIAL (cambia questo per modificare il materiale del terreno)
#define TERRAIN_NO_MAT {.0,.0,.0,1.0}
#define TERRAIN_MAT_DIFFUSE {.6,.3,.0,1.0}
#define TERRAIN_MAT_SPECULAR {.0,.0,.0,1.0}
#define TERRAIN_LOW_SHININESS {1.0}

class Terrain {
    
public:
    Terrain();
    ~Terrain();
    
    void drawTerrain();
    bool correctMove(float x, float z);
    float hight(float x, float z);
    
private:
    float **terrain;
    bool **walkable;
    unsigned int terrain_dim;
    
};

#endif /* Terrain_hpp */
