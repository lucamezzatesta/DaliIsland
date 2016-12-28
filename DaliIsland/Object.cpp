//
//  Object.cpp
//  DaliIsland
//
//  Created by Luca Mezzatesta on 26/05/16.
//  Copyright © 2016 Luca Mezzatesta. All rights reserved.
//

#include "Object.hpp"

// Da realizzare dopo aver capito come funziona assimp

Object::Object(float x_pos, float y_pos, float z_pos) {
    
    this->x_pos = x_pos;
    this->y_pos = y_pos;
    this->z_pos = z_pos;
    
    for (int i = 0; i < 3; i++) {
        amb_mat[i] = .0;
        diff_mat[i] = .0;
        spec_mat[i] = .0;
        emi_mat[i] = .0;
    }
    amb_mat[3] = 1.0;
    diff_mat[3] = 1.0;
    spec_mat[3] = 1.0;
    emi_mat[3] = 1.0;
    shininess[0] = 1.0;
    
}