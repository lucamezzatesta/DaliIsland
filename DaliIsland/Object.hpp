//
//  Object.hpp
//  DaliIsland
//
//  Created by Luca Mezzatesta on 26/05/16.
//  Copyright © 2016 Luca Mezzatesta. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#ifndef GLUT
#define GLUT
#include <GLUT/glut.h>
#endif

#define DEFAULT_MAT {.0,.0,.0,1.0}

class Object {
    
    float x_pos;
    float y_pos;
    float z_pos;
    
    GLfloat amb_mat[4];
    GLfloat diff_mat[4];
    GLfloat spec_mat[4];
    GLfloat shininess[1];
    GLfloat emi_mat[4];
    
    // Aggiungi qui informazioni sulla mesh (assimp)
    
public:
    
    Object(float x_pos, float y_pos, float z_pos);  // TODO: Add here the file of the object
    
    float getX();
    float getY();
    float getZ();
    
    void draw();
    
    void translate(float x, float y, float z);
    void scale(float scale);
    void rotate(float x, float y, float z);
    
    void setAmbientMaterial(float r, float g, float b, float a);
    void setDiffuseMaterial(float r, float g, float b, float a);
    void setSpecularMaterial(float r, float g, float b, float a);
    void setShininess(float s);     // Un valore basso rappresenta il massimo della brillantezza
    void setEmissionMaterial(float r, float g, float b, float a);
    
    // TODO: Implementa un metodo per le collisioni
    
};

#endif /* Object_hpp */
