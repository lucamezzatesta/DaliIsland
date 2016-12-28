//
//  Terrain.cpp
//  DaliIsland
//
//  Created by Luca Mezzatesta on 11/06/16.
//  Copyright © 2016 Luca Mezzatesta. All rights reserved.
//

#include "Terrain.hpp"

Terrain::Terrain() {
    
    std::fstream terrain_file;
    terrain_file.open(TERRAIN_FILENAME);
    
    if (terrain_file.fail()) {
        printf("Hight map not found!\n");
        exit(-1);
    }
    
    // First number in the file = terrain_dim
    terrain_file >> terrain_dim;
    
    // Allocation of the terrain
    terrain = (float**) malloc(sizeof(float*) * terrain_dim);
    walkable = (bool**) malloc(sizeof(bool*) * terrain_dim);
    for (int i = 0; i < terrain_dim; i++) {
        terrain[i] = (float*) malloc(sizeof(float) * terrain_dim);
        walkable[i] = (bool*) malloc(sizeof(bool) * terrain_dim);
    }
    
    // Reading the file and filling the terrain matrix with hights
    for (int i = 0; i < terrain_dim; i++) {
        for (int j = 0; j < terrain_dim; j++) {
            terrain_file >> terrain[i][j];
            
            // If the terrain is under the level of the sea...
            if (terrain[i][j] == -1) walkable[i][j] = false;
            else walkable[i][j] = true;
        }
    }
}

void Terrain::drawTerrain() {
    
    GLfloat no_mat[] = TERRAIN_NO_MAT;
    GLfloat mat_diffuse[] = TERRAIN_MAT_DIFFUSE;
    GLfloat mat_specular[] = TERRAIN_MAT_SPECULAR;
    GLfloat low_shininess[] = TERRAIN_LOW_SHININESS;
    
    /*
     Division of the blocks:
     
     |-----| z+1
     | \   |
     |   \ |
     |-----| z
     x    x+1
     */

    
    for (int x = 0; x < terrain_dim - 1; x++) {
        glBegin(GL_TRIANGLES);
        
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
        
        for (int x = 0; x < terrain_dim - 1; x++) {
            
            for (int z = 0; z < terrain_dim - 1; z++) {
                
                GLfloat Nx, Ny, Nz, Ux, Uy, Uz, Vx, Vy, Vz;
                
                Ux = 1;                                 // P2x - P1x
                Uy = terrain[x+1][z] - terrain[x][z];   // P2y - P1y
                Uz = 0;                                 // P2z - P1z
                
                Vx = 0;                                 // P3x - P1x
                Vy = terrain[x+1][z] - terrain[x][z];   // P3y - P1y
                Vz = 1;                                 // P3z - P1z
                
                Nx = (Uy*Vz) - (Uz*Vy);
                Ny = (Uz*Vx) - (Ux*Vz);
                Nz = (Ux*Vy) - (Uy*Vx);
                
                glBegin(GL_QUADS);
                
                glNormal3f(Nx, Ny, Nz);
                glVertex3f(x, terrain[x][z], z);
                glVertex3f(x+1, terrain[x+1][z], z);
                glVertex3f(x+1, terrain[x+1][z+1], z+1);
                glVertex3f(x, terrain[x][z+1], z+1);
                
                glEnd();
            }
        }
    }
}


// FIXME: correct this method (non sono sicuro che funzioni!!!). Implementare i controlli!
bool Terrain::correctMove(float x, float z) {
    return walkable[(int)x][(int)z];
}

// FIXME: anche questo non sono sicuro che funzioni. Implementare i controlli!
float Terrain::hight(float x, float z) {
    return terrain[(int) x][(int) z];
}

Terrain::~Terrain() {
    
    for (int i = 0; i < terrain_dim; i++) {
        free(terrain[i]);
        free(walkable[i]);
    }
    
    free(terrain);
    free(walkable);
    
    terrain = NULL;
    walkable = NULL;
}

