//
//  seaGenerator.hpp
//  test_sea
//
//  Created by Luca Mezzatesta on 15/05/16.
//  Copyright © 2016 Luca Mezzatesta. All rights reserved.
//

// PROBLEMI NOTI:
// - Controlli di sicurezza non effettuati

#ifndef Sea_hpp
#define Sea_hpp

#include <stdlib.h>
#include <exception>
#include <math.h>

#ifndef MATH
#define MATH
#include <math.h>
#endif

#ifndef GLUT
#define GLUT
#include <GLUT/GLUT.h>
#endif

// DEFAULT SETTINGS AND FEATURES
#define DEFAULT_SEA_DIMENSION 51
#define TIME_SPEED 0.03
#define WAVES_FREQUENCY 17.1
#define WAVES_AMPLITUDE 0.18

// MATERIAL (cambia questo per modificare il materiale del mare)
#define SEA_NO_MAT {.0,.0,.0,1.0}
#define SEA_MAT_DIFFUSE {.11372549,.15294118,.35294118,1.0}
#define SEA_MAT_SPECULAR {.52941176,0.8078414,0.98039216,1.0}
#define SEA_LOW_SHININESS {100}

class Sea {
    
public:
    Sea();
    Sea(unsigned int x_wide, unsigned int z_wide);
    ~Sea();
    
    void drawSea();
    void animation();
    
private:
    float **sea;
    float time = 0;
    float dimension_x;
    float dimension_z;
    unsigned int seaType = 3;
    
    void allocateSea();
    void generateWaves1();
    void generateWaves2();
    void generateWaves3();
    void generateSea();
    void sea1();
    void sea2();
    void sea3();
    
};

#endif /* seaGenerator_hpp */
