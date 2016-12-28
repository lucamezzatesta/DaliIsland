//
//  Engine.hpp
//  DaliIsland
//
//  Created by Luca Mezzatesta on 26/05/16.
//  Copyright © 2016 Luca Mezzatesta. All rights reserved.
//

#ifndef Engine_hpp
#define Engine_hpp

#ifndef GLUT
#define GLUT
#include <GLUT/glut.h>
#endif

#include <vector>
#include <map>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>       /* for cos(), sin(), and sqrt() */
#include <GLUT/GLUT.h>
#include "SETTINGS.h"

#include "Object.hpp"
#include "Sea.hpp"
#include "Terrain.hpp"
//#include "Scene.hpp"
#include "LoadScene.hpp"
#include "Camera.hpp"

#include <ctime>

void load(void);
void lighting(void);
void renderScene(void);
void changeSize(int width, int height);
void pressNormalKey(unsigned char key, int xx, int yy);
void pressSpecialKey(int key, int xx, int yy);
void releaseKey(int key, int xx, int yy);
void mouseMoved(int x, int y);
void mouseButton(int button, int state, int x, int y);
void move();
void Timer(int value);
void animations();
void makePointList(void);
void updatePointList(void);
void visible(int vis);
void recalcModelView(void);

#endif /* Engine_hpp */
