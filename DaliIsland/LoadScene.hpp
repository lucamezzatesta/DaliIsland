//
//  LoadScene.hpp
//  DaliIsland
//
//  Created by Chiara Lurgo on 15/06/16.
//  Copyright © 2016 Luca Mezzatesta. All rights reserved.
//

#ifndef LoadScene_hpp
#define LoadScene_hpp

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <map>

#include <GLUT/glut.h>
#include "IL/il.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/cimport.h"

const struct aiScene* loadScene(const char* pathName); //carica la scena
GLuint getList(); //restituisce la lista di rendering per la scena

//funzioni interne
void color4_to_float4(const aiColor4D *c, float f[4]);
void set_float4(float f[4], float a, float b, float c, float d);
void apply_material(const struct aiMaterial *mtl);
void Color4f(const aiColor4D *color);
int LoadGLTextures(const aiScene* scene);
void recursive_render(const struct aiScene *sc, const struct aiNode* nd, float scale);
void display();
void get_bounding_box_for_node (const struct aiNode* nd,
                                aiVector3D* min,
                                aiVector3D* max,
                                aiMatrix4x4* trafo
                                );
void get_bounding_box (aiVector3D* min, aiVector3D* max);
int importScene(const char* pathName);
void ReleaseImport();


#endif /* LoadScene_hpp */
