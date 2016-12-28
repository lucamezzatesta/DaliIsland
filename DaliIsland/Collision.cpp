//
//  Collision.cpp
//  DaliIsland
//
//  Created by Luca Mezzatesta on 14/07/16.
//  Copyright © 2016 Luca Mezzatesta. All rights reserved.
//

#include "Collision.hpp"

using namespace std;

Collision::Collision(string height_map, string collision_map) {
    
    fstream height_map_f;
    fstream collision_map_f;
    
    height_map_f.open(height_map);
    collision_map_f.open(collision_map);
    
    if (height_map_f.fail() || collision_map_f.fail()) {
        cerr << "ERROR: height map or collision map not found. Please, copy them in the executable's folder" << endl;
        exit(-1);
    }
    
    height_map_f >> height_map_dim;
    collision_map_f >> collision_map_dim;
    
    if (height_map_dim != collision_map_dim) {
        cerr << "ERROR: maps with different dimension" << endl;
        exit(-1);
    }
    
    // Allocation of the height map and collision map
    this->height_map = (float**) malloc(sizeof(float*) * height_map_dim);
    this->collision_map = (int**) malloc(sizeof(int*) * collision_map_dim);
    
    if (this->height_map == nullptr || this->collision_map == nullptr) {
        cerr << "ERROR: height map or collision map cannot be allocated!" << endl;
        exit(-1);
    }
    
    for (int i = 0; i < height_map_dim; i++) { // the two dimensions must be equal
        this->collision_map[i] = (int*) malloc(sizeof(int) * collision_map_dim);
        this->height_map[i] = (float*) malloc(sizeof(float) * height_map_dim);
    }
    
    for (int i = 0; i < height_map_dim; i++ ) { // the two dimensions must be equal
        for (int j = 0; j < height_map_dim; j++ ) {
            height_map_f >> this->height_map[i][j];
            collision_map_f >> this->collision_map[i][j];
        }
    }
}

bool Collision::canMove(float x, float z) {
    
    // La z è negativa quindi bisogna sempre cambiare il suo segno per poterla utilizzare come riferimento nelle matrici
    
    int x_index, z_index;
    
    x_index = (int) x;
    z_index = (int) -z;
    
    if (x_index < 0 || z_index < 0 || x_index >= height_map_dim || z_index >= height_map_dim) {
        cerr << "COLLIDER: Out of the boundaries of the island!!!" << endl;
        exit(-1);
    }
    
    if (collision_map[x_index][z_index] == 1) return true;
    else return false;
}

float Collision::getHeight(float x, float z) {
    
    // La z è negativa quindi bisogna sempre cambiare il suo segno per poterla utilizzare come riferimento nelle matrici
    
    int x_index, z_index;
    
    x_index = (int) x;
    z_index = (int) -z;
    
    if (x_index < 0 || z_index < 0 || x_index >= height_map_dim || z_index >= height_map_dim) {
        cerr << "COLLIDER: Cannot move out of the boundaries of the island!!!" << endl;
        exit(-1);
    }
    
    return height_map[x_index][z_index];
}