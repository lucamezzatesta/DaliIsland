//
//  Sea.cpp
//  test_sea
//
//  Created by Luca Mezzatesta on 15/05/16.
//  Copyright © 2016 Luca Mezzatesta. All rights reserved.
//

#include "Sea.hpp"

// TODO: Write security checks (run out of memory etc...)
Sea::Sea() {
    
    dimension_x = DEFAULT_SEA_DIMENSION;
    dimension_z = DEFAULT_SEA_DIMENSION;
    
    allocateSea();
}

// TODO: Write checks on x_wide and z_wide (not zero!!!)
Sea::Sea(unsigned int x_wide, unsigned int z_wide) {
    
    dimension_x = x_wide;
    dimension_z = z_wide;
    
    allocateSea();
}

//Sea::Sea(unsigned int x_wide, unsigned int z_wide, unsigned int seaType) {
//    this->seaType = seaType;
//    Sea(x_wide,z_wide);
//}

//Sea::Sea(unsigned int seaType) {
//    this->seaType = seaType;
//    Sea();
//}

Sea::~Sea() {
    for (int i = 0; i < dimension_x; i++) free(sea[i]);
    free(sea);
}

void Sea::drawSea() {
    generateSea();
}

void Sea::allocateSea() {
    
    sea = (float**) malloc( sizeof(float*) * dimension_x );
    
    for (int i; i < dimension_x; i++) {
        sea[i] = (float*) malloc(sizeof(float) * dimension_z );
    }
}

void Sea::generateWaves1() {
    
    for(int i = 0; i < dimension_x; i++)
        for (int j = 0; j < dimension_z; j++) {
            sea[i][j] = 0.2*sinf(3*sqrtf(powf((i-(dimension_x/2))/2, 2)+powf((j-(dimension_z-2))/2, 2))+time);
        }
    
    time += TIME_SPEED;
}

// Function that fills the sea matrix for seaGen3
// The formula used to generate the waves is:
//      y = A * sin( f*sqrt((x-center_x)^2+(z-center_z)^2) + time )

// It produces concentric waves towars the (center_x,0,center_z) point
void Sea::generateWaves2(void) {
    
    float x;
    
    for (int i = 0; i < dimension_x; i++) {
        x = i;
        if ( i % 2 ) x += 0.5; // ODD LINE (only 50 cells needed)
        
        for (int j = 0; j < dimension_z; j++) sea[i][j] = WAVES_AMPLITUDE*sinf(WAVES_FREQUENCY*sqrtf(powf(x-(dimension_x/2), 2)+powf(j-(dimension_z/2), 2))+time);
    }
    
    time += TIME_SPEED;
}

void Sea::generateWaves3(void) {
    
    float x;
    
    for (int i = 0; i < dimension_x; i++) {
        x = i;
        if (i % 2) x+= 0.5;
        
        for (int j = 0; j < dimension_z; j++) sea[i][j] = sinf(5*x+time)*cosf(5*j+time)/5;
    }
    
    time += TIME_SPEED*0.5;
    
}

void Sea::generateSea() {
    
    switch (seaType) {
        case 1:
            sea1();
            break;
            
            case 2:
            sea2();
            break;
            
        case 3:
            sea3();
            break;
            
        default:
            sea3();
    }
}

void Sea::sea1() { // BLOCKY WAVES
    
    /*
     Division of the blocks:
     
     |-----| z+1
     | \   |
     |   \ |
     |-----| z
     x    x+1
     */
    
    generateWaves1();
    
    for (int x = 0; x < dimension_x-1; x++) {
        glBegin(GL_TRIANGLES);
        for (int z = 0; z < dimension_z-1; z++) {
            glColor3f(0.6, .6, .6);
            
            glVertex3f(x, sea[x][z+1], z+1);        // 1: Alto sx
            glVertex3f(x, sea[x][z], z);            // 2: Basso sx
            glVertex3f(x+1, sea[x+1][z], z);        // 3: Basso dx
            
            glColor3f(.9, .9, .9);
            glVertex3f(x+1, sea[x+1][z], z);        // 4: Basso dx
            glVertex3f(x,sea [x][z+1], z+1);        // 5: Alto sx
            glVertex3f(x+1, sea[x+1][z+1], z+1);    // 6: Alto dx
        }
        glEnd();
    }
}

void Sea::sea2(void) {   // Onde "quadrettate"
    
    /*
     Division of the blocks:
     
     |-----|-----| z+2
     | \   |   / |
     |   \ | /   |
     |-----|-----| z+1
     |   / | \   |
     | /   |   \ |
     |-----|-----| z
     x    x+1   x+2
     */
    
    generateWaves1();
    
    for (int x = 0; x < dimension_x-1; x += 2) {
        glBegin(GL_TRIANGLES);
        for (int z = 0; z < dimension_z-1; z += 2) {
            // Microblocco basso sx
            glColor3f(.9, .9, .9);
            glVertex3f(x, sea[x][z], z);
            glVertex3f(x+1, sea[x+1][z], z);
            glVertex3f(x+1, sea[x+1][z+1], z+1);
            
            glColor3f(.6, .6, .6);
            glVertex3f(x+1, sea[x+1][z+1], z+1);
            glVertex3f(x, sea[x][z], z);
            glVertex3f(x, sea[x][z+1], z+1);
            
            // Microblocco alto sx
            glColor3f(.9, .9, .9);
            glVertex3f(x+1, sea[x+1][z+1], z+1);
            glVertex3f(x, sea[x][z+1], z+1);
            glVertex3f(x, sea[x][z+2], z+2);
            
            glColor3f(.6, .6, .6);
            glVertex3f(x+1, sea[x+1][z+1], z+1);
            glVertex3f(x, sea[x][z+2], z+2);
            glVertex3f(x+1, sea[x+1][z+2], z+2);
            
            // Microblocco alto dx
            glColor3f(.9, .9, .9);
            glVertex3f(x+1, sea[x+1][z+1], z+1);
            glVertex3f(x+1, sea[x+1][z+2], z+2);
            glVertex3f(x+2, sea[x+2][z+2], z+2);
            
            glColor3f(.6, .6, .6);
            glVertex3f(x+1, sea[x+1][z+1], z+1);
            glVertex3f(x+2, sea[x+2][z+2], z+2);
            glVertex3f(x+2, sea[x+2][z+1], z+1);
            
            // Microblocco basso dx
            glColor3f(.9, .9, .9);
            glVertex3f(x+1, sea[x+1][z+1], z+1);
            glVertex3f(x+2, sea[x+2][z+1], z+1);
            glVertex3f(x+2, sea[x+2][z], z);
            
            glColor3f(.6, .6, .6);
            glVertex3f(x+1, sea[x+1][z+1], z+1);
            glVertex3f(x+2, sea[x+2][z], z);
            glVertex3f(x+1, sea[x+1][z], z);
        }
        glEnd();
    }
    
}

void Sea::sea3(void) {   // Onde con le punte
    
    GLfloat no_mat[] = SEA_NO_MAT;
    GLfloat mat_diffuse[] = SEA_MAT_DIFFUSE;
    GLfloat mat_specular[] = SEA_MAT_SPECULAR;
    GLfloat low_shininess[] = SEA_LOW_SHININESS;
    
    /*
     Division of the blocks:
     
     |-----------| z+2
     | \       / |
     |   \   /   |
     |     x     | z+1
     |   /   \   |
     | /       \ |
     |-----------| z
     x    x+1   x+2
     */
    
    for (int x = 0; x < dimension_x-2; x += 2) {
        
        glBegin(GL_TRIANGLES);
        
        // Setting the sea material
        glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
        
        for (int z = 0; z < dimension_z-2; z += 2) {
            
            GLfloat Nx, Ny, Nz, Ux, Uy, Uz, Vx, Vy, Vz;
            
            // Triangolo sx
            
            Ux = 0;                         // P2x - P1x
            Uy = sea[x][z+2] - sea[x][z];   // P2y - P1y
            Uz = 2;                         // P2z - P1z
            
            Vx = 1;                         // P3x - P1x
            Vy = sea[x+1][z+1] - sea[x][z]; // P3y - P1y
            Vz = 1;                         // P3z - P1z
            
            Nx = (Uy*Vz) - (Uz*Vy);
            Ny = (Uz*Vx) - (Ux*Vz);
            Nz = (Ux*Vy) - (Uy*Vx);
            
            glNormal3f(Nx, Ny, Nz);
            
            glVertex3f(x, sea[x][z], -z);
            glVertex3f(x, sea[x][z+2], -(z+2));
            glVertex3f(x+1, sea[x+1][z+1], -(z+1));
            
            // Triangolo sopra
            
            Ux = -1;
            Uy = sea[x][z+2] - sea[x+1][z+1];
            Uz = 1;
            
            Vx = 1;
            Vy = sea[x+2][z+2] - sea[x+1][z+1];
            Vz = 1;
            
            Nx = (Uy*Vz) - (Uz*Vy);
            Ny = (Uz*Vx) - (Ux*Vz);
            Nz = (Ux*Vy) - (Uy*Vx);
            
            glNormal3f(Nx, Ny, Nz);
            
            glVertex3f(x+1, sea[x+1][z+1], -(z+1));
            glVertex3f(x, sea[x][z+2], -(z+2));
            glVertex3f(x+2, sea[x+2][z+2], -(z+2));
            
            // Triangolo dx
            
            Ux = 1;
            Uy = sea[x+2][z+2] - sea[x+1][z+1];
            Uz = 1;
            
            Vx = 1;
            Vy = sea[x+2][z] - sea[x+1][z+1];
            Vz = -1;
            
            Nx = (Uy*Vz) - (Uz*Vy);
            Ny = (Uz*Vx) - (Ux*Vz);
            Nz = (Ux*Vy) - (Uy*Vx);
            
            glNormal3f(Nx, Ny, Nz);
            
            glVertex3f(x+1, sea[x+1][z+1], -(z+1));
            glVertex3f(x+2, sea[x+2][z+2], -(z+2));
            glVertex3f(x+2, sea[x+2][z], -z);
            
            // Triangolo sotto
            
            Ux = 1;
            Uy = sea[x+2][z] - sea[x+1][z+1];
            Uz = -1;
            
            Vx = -1;
            Vy = sea[x][z] - sea[x+1][z+1];
            Vz = -1;
            
            Nx = (Uy*Vz) - (Uz*Vy);
            Ny = (Uz*Vx) - (Ux*Vz);
            Nz = (Ux*Vy) - (Uy*Vx);
            
            glNormal3f(Nx, Ny, Nz);
            
            glVertex3f(x+1, sea[x+1][z+1], -(z+1));
            glVertex3f(x+2, sea[x+2][z], -z);
            glVertex3f(x, sea[x][z], -z);
        }
        
        glEnd();
    }
}

void Sea::animation() {
    generateWaves3();
}
