//
//  Engine.cpp
//  DaliIsland
//
//  Created by Luca Mezzatesta on 26/05/16.
//  Copyright © 2016 Luca Mezzatesta. All rights reserved.
//

#include "Engine.hpp"

// WINDOW
int win_hight = WIN_HIGHT;
int win_width = WIN_WIDTH;
int w;
int h;

// FRAME-RATE


// ILLUMINAZIONE GLOBALE (usata per GL_LIGTH0)
//GLfloat diffuse[] = {1.0,1.0,1.0,1.0};
//GLfloat position[] = {25,20.0,25,.0};       // Cambia questo per modificare la posizione della luce
//GLfloat lmodel_amb[] = {.4,.4,.4,1.0};
//GLfloat local_view[] = {.0};
//GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};

////GLfloat ambient[] = {.5,.5,.5,1.0};
GLfloat ambient[] = {.0,.0,.0,1.0};
GLfloat diffuse[] = {1.0,1.0,1.0,1.0};
//GLfloat specular[] = {1.0,1.0,1.0,0.1};
GLfloat specular[] = {.0,.0,.0,0.1};
GLfloat position[] = {100.0,1000,-1700,1.0};       // Cambia questo per modificare la posizione della luce

GLfloat ambient1[] = {.0,.0,.0,0.0};
//GLfloat ambient1[] = {.0,.0,.0,1.0};
GLfloat diffuse1[] = {1.0,1.0,1.0,1.0};
GLfloat specular1[] = {0.1,0.1,0.1,1.0};
GLfloat position1[] = {100,18.0,-100,1.0};

GLfloat ambient2[] = {.0,.0,.0,0.0};
//GLfloat ambient2[] = {.0,.0,.0,1.0};
GLfloat diffuse2[] = {1.0,1.0,1.0,1.0};
GLfloat specular2[] = {0.1,0.1,0.1,1.0};
GLfloat position2[] = {100,10.0,-100,.0};

GLfloat lmodel_amb[] = {.4,.4,.4,1.0};
GLfloat local_view[] = {.0};

// MOUSE
GLfloat vertMouseSensitivity = DEFAULT_VERT_MOUSE_SENSITIVITY;
GLfloat horizMouseSensitivity = DEFAULT_HORIZ_MOUSE_SENSITIVITY;

// KEYBOARD
std::map<int,bool> keys;

// MOVEMENT
float movementSpeed = DEFAULT_MOVEMENT_SPEED;
float rotationSpeed = DEFAULT_ROTATION_SPEED;

float xTranslationOpera1=0.0;
float zTranslationOpera1=0.0;
float yTranslationOpera1=-3.5;

float xTranslationOpera2=0.0;
float zTranslationOpera2=0.0;
float yTranslationOpera2=-25.0;

float xTranslationOpera3=0.0;
float zTranslationOpera3=0.0;
float yTranslationOpera3=-3.0;

float xTranslationOpera4=0.0;
float zTranslationOpera4=0.0;
float yTranslationOpera4=-8.0;

int suOpera1=0;
int giuOpera1=0;

int suOpera2=0;
int giuOpera2=0;

int suOpera3=0;
int giuOpera3=0;

int suOpera4=0;
int giuOpera4=0;

//VISIBILITA' SCENE
int visibilityOpera1 = 0;
int visibilityOpera2 = 0;
int visibilityOpera3 = 0;
int visibilityOpera4 = 0;

// OGGETTI NELLA SCENA
Camera camera;
std::vector<Object> objects;
Sea *mare;
Terrain *terreno;

//SCENE NEL MONDO
//std::vector<Scene*> scenes;
char path3DModelsToLoad[60] = "./completa.obj";
extern const struct aiScene* scene;
extern GLuint scene_list_opera1;
extern GLuint scene_list_opera2;
extern GLuint scene_list_opera3;
extern GLuint scene_list_opera4;
extern GLuint scene_list_terreno;
extern GLuint scene_list_nuvole;

//PARTICELLE
#ifndef M_PI
#define M_PI 3.14159265
#endif

#if 0  /* For debugging. */
#undef GL_SGIS_point_parameters
#endif

static GLfloat angle = -150;   /* in degrees */
static int spin = 0;
static int moving, begin;
static int newModel = 1;
static float t;
static int repeat = 1;
int useMipmaps = 1;
int linearFiltering = 1;

static GLfloat constant[3] = { 1/5.0, 0.0, 0.0 };
static GLfloat linear[3] = { 0.0, 1/5.0, 0.0 };
static GLfloat quad[3] = { 0.25, 0.0, 1/60.0 };

#define MAX_POINTS 2000

static int numPoints = 500;

static GLfloat pointList1[1][MAX_POINTS][3];
static GLfloat pointList2[3][MAX_POINTS][3];
static GLfloat pointList3[1][MAX_POINTS][3];
static GLfloat pointList4[3][MAX_POINTS][3];
static GLfloat pointTime1[MAX_POINTS];
static GLfloat pointTime2[MAX_POINTS];
static GLfloat pointTime3[MAX_POINTS];
static GLfloat pointTime4[MAX_POINTS];
static GLfloat pointVelocity1[1][MAX_POINTS][2];
static GLfloat pointVelocity2[3][MAX_POINTS][2];
static GLfloat pointVelocity3[1][MAX_POINTS][2];
static GLfloat pointVelocity4[3][MAX_POINTS][2];
static GLfloat pointDirection1[MAX_POINTS][2];
static GLfloat pointDirection2[MAX_POINTS][2];
static GLfloat pointDirection3[MAX_POINTS][2];
static GLfloat pointDirection4[MAX_POINTS][2];
static int colorList[MAX_POINTS];
static int animate = 1, motion = 0;

static GLfloat colorSet[][4] = {
    /* Shades of brown. */
    { 0.54509804, 0.27058824, 0.07450980, 1 },
    { 0.62745098, 0.32156863, 0.17647059, 1 },
    { 0.80392157, 0.52156863, 0.24705882, 1 },
    { 0.82352941, 0.70588235, 0.54901961, 1 },
    { 0.87058824, 0.72156863, 0.52941176, 1 },
    { 0.82352941, 0.41176471, 0.12, 1 },
};

#define NUM_COLORS (sizeof(colorSet)/sizeof(colorSet[0]))

#define DEAD (NUM_COLORS+1)
#if 0  /* drand48 might be better on Unix machines */
#define RANDOM_RANGE(lo, hi) ((lo) + (hi - lo) * drand48())
#else
static float float_rand(void) { return rand() / (float) RAND_MAX; }
#define RANDOM_RANGE(lo, hi) ((lo) + (hi - lo) * float_rand())
#endif

#define MEAN_VELOCITY 3.0
#define GRAVITY 2.0
#define TIME_DELTA 0.025  /* The speed of time. */

/* Modeling units of ground extent in each X and Z direction. */
#define EDGE 5


/*----------------------------------------------------------------------------------------------------------------------------------------------*/


void load(void) {   // FUNZIONE MODIFICABILE
    
    camera.setPosition(DEFAULT_CAMERA_X, DEFAULT_CAMERA_Z);
    
    // Carica qui le mesh
    mare = new Sea(400,400);
    terreno = new Terrain();
    
    //Carica qui le scene (setta il path!!!)
    loadScene(path3DModelsToLoad);
}

void renderSpacedBitmapString(float x, float y, int spacing, void *font, char *string) {
    char *c;
    int x1=x;
    
    glEnable ( GL_COLOR_MATERIAL );
    
    for (c=string; *c != '\0'; c++){
        glColor3f(.0f, .0f, .0f);
        glRasterPos2f(x1,y);
        glutBitmapCharacter(font, *c);
        x1 = x1 + glutBitmapWidth(font,*c) + spacing;
    }
    
    glPushMatrix();
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.0f, 49.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(200.0f, 49.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(200.0f, 1.0f);
    glEnd();
    glPopMatrix();
    
    glDisable(GL_COLOR_MATERIAL);
    
}

void restorePerspectiveProjection() {
    glMatrixMode(GL_PROJECTION);
    // restore previous projection matrix
    glPopMatrix();
    // get back to modelview mode
    glMatrixMode(GL_MODELVIEW);
}

void setOrthographicProjection() {
    // switch to projection mode
    glMatrixMode(GL_PROJECTION);
    // save previous matrix which contains the
    //settings for the perspective projection
    glPushMatrix();
    // reset matrix
    glLoadIdentity();
    // set a 2D orthographic projection
    gluOrtho2D(-10, w, 0, h);
    // invert the y axis, down is positive
    glScalef(1, -1, 1);
    // mover the origin from the bottom left corner
    // to the upper left corner
    glTranslatef(0, -h, 0);
    // switch back to modelview mode
    glMatrixMode(GL_MODELVIEW);
}

void makePointList(void)
{
    float angle, velocity, direction;
    int i;
    
    motion = 1;
    for (i=0; i<numPoints; i++) {
        pointList1[0][i][0] = 0.0;
        pointList1[0][i][1] = 0.0;
        pointList1[0][i][2] = 0.0;
        
        pointList2[0][i][0] = 0.0;
        pointList2[0][i][1] = 0.0;
        pointList2[0][i][2] = 0.0;
        pointList2[1][i][0] = 0.0;
        pointList2[1][i][1] = 0.0;
        pointList2[1][i][2] = 0.0;
        pointList2[2][i][0] = 0.0;
        pointList2[2][i][1] = 0.0;
        pointList2[2][i][2] = 0.0;
        pointList2[3][i][0] = 0.0;
        pointList2[3][i][1] = 0.0;
        pointList2[3][i][2] = 0.0;
        
        pointList3[0][i][0] = 0.0;
        pointList3[0][i][1] = 0.0;
        pointList3[0][i][2] = 0.0;
        
        pointList4[0][i][0] = 0.0;
        pointList4[0][i][1] = 0.0;
        pointList4[0][i][2] = 0.0;
        pointList4[1][i][0] = 0.0;
        pointList4[1][i][1] = 0.0;
        pointList4[1][i][2] = 0.0;
        pointList4[2][i][0] = 0.0;
        pointList4[2][i][1] = 0.0;
        pointList4[2][i][2] = 0.0;
        
        pointTime1[i] = 0.0;
        pointTime2[i] = 0.0;
        pointTime3[i] = 0.0;
        pointTime4[i] = 0.0;
        
        angle = (RANDOM_RANGE(60.0, 70.0)) * M_PI/180.0;
        direction = RANDOM_RANGE(0.0, 360.0) * M_PI/180.0;
        
        pointDirection1[i][0] = cos(direction);
        pointDirection1[i][1] = sin(direction);
        pointDirection2[i][0] = cos(direction);
        pointDirection2[i][1] = sin(direction);
        pointDirection3[i][0] = cos(direction);
        pointDirection3[i][1] = sin(direction);
        pointDirection4[i][0] = cos(direction);
        pointDirection4[i][1] = sin(direction);
        
        velocity = MEAN_VELOCITY + RANDOM_RANGE(-0.8, 1.0);
        
        pointVelocity1[0][i][0] = velocity * cos(angle);
        pointVelocity1[0][i][1] = velocity * sin(angle);
        pointVelocity2[0][i][0] = velocity * cos(angle);
        pointVelocity2[0][i][1] = velocity * sin(angle);
        pointVelocity2[1][i][0] = velocity * cos(angle);
        pointVelocity2[1][i][1] = velocity * sin(angle);
        pointVelocity2[2][i][0] = velocity * cos(angle);
        pointVelocity2[2][i][1] = velocity * sin(angle);
        pointVelocity2[3][i][0] = velocity * cos(angle);
        pointVelocity2[3][i][1] = velocity * sin(angle);
        pointVelocity3[0][i][0] = velocity * cos(angle);
        pointVelocity3[0][i][1] = velocity * sin(angle);
        pointVelocity4[0][i][0] = velocity * cos(angle);
        pointVelocity4[0][i][1] = velocity * sin(angle);
        pointVelocity4[1][i][0] = velocity * cos(angle);
        pointVelocity4[1][i][1] = velocity * sin(angle);
        pointVelocity4[2][i][0] = velocity * cos(angle);
        pointVelocity4[2][i][1] = velocity * sin(angle);
        
        colorList[i] = rand() % NUM_COLORS;
    }
    t = 0.0;
}

void updatePointList(void)
{
    float distance1;
    float distance2;
    float distance3;
    float distance4;
    int i;
    
    motion = 0;
    for (i=0; i<numPoints; i++) {
        distance1 = pointVelocity1[0][i][0] * t;
        distance2 = pointVelocity2[0][i][0] * t;
        distance3 = pointVelocity3[0][i][0] * t;
        distance4 = pointVelocity4[0][i][0] * t;
        
        /* X and Z */
        pointList1[0][i][0] = pointDirection1[i][0] * distance1;
        pointList1[0][i][2] = pointDirection1[i][1] * distance1;
        
        pointList2[0][i][0] = pointDirection2[i][0] * distance2;
        pointList2[0][i][2] = pointDirection2[i][1] * distance2;
        pointList2[1][i][0] = pointDirection2[i][0] * distance2;
        pointList2[1][i][2] = pointDirection2[i][1] * distance2;
        pointList2[2][i][0] = pointDirection2[i][0] * distance2;
        pointList2[2][i][2] = pointDirection2[i][1] * distance2;
        pointList2[3][i][0] = pointDirection2[i][0] * distance2;
        pointList2[3][i][2] = pointDirection2[i][1] * distance2;
        
        pointList3[0][i][0] = pointDirection3[i][0] * distance3;
        pointList3[0][i][2] = pointDirection3[i][1] * distance3;
        
        pointList4[0][i][0] = pointDirection4[i][0] * distance4;
        pointList4[0][i][2] = pointDirection4[i][1] * distance4;
        pointList4[1][i][0] = pointDirection4[i][0] * distance4;
        pointList4[1][i][2] = pointDirection4[i][1] * distance4;
        pointList4[2][i][0] = pointDirection4[i][0] * distance4;
        pointList4[2][i][2] = pointDirection4[i][1] * distance4;
        
        /* Z */
        pointList1[0][i][1] = (pointVelocity1[0][i][1] - 0.5 * GRAVITY * pointTime1[i])*pointTime1[i];
        
        pointList2[0][i][1] = (pointVelocity2[0][i][1] - 0.5 * GRAVITY * pointTime2[i])*pointTime2[i];
        pointList2[1][i][1] = (pointVelocity2[1][i][1] - 0.5 * GRAVITY * pointTime2[i])*pointTime2[i];
        pointList2[2][i][1] = (pointVelocity2[2][i][1] - 0.5 * GRAVITY * pointTime2[i])*pointTime2[i];
        pointList2[3][i][1] = (pointVelocity2[2][i][1] - 0.5 * GRAVITY * pointTime2[i])*pointTime2[i];
        
        pointList3[0][i][1] = (pointVelocity3[0][i][1] - 0.5 * GRAVITY * pointTime3[i])*pointTime3[i];
        
        pointList4[0][i][1] = (pointVelocity4[0][i][1] - 0.5 * GRAVITY * pointTime4[i])*pointTime4[i];
        pointList4[1][i][1] = (pointVelocity4[1][i][1] - 0.5 * GRAVITY * pointTime4[i])*pointTime4[i];
        pointList4[2][i][1] = (pointVelocity4[2][i][1] - 0.5 * GRAVITY * pointTime4[i])*pointTime4[i];
        
        /* If we hit the ground, bounce the point upward again. */
        if (pointList1[0][i][1] <= 0) {
            if (distance1 > EDGE) {
                /* Particle has hit ground past the distance duration of
                 the particles.  Mark particle as dead. */
                colorList[i] = NUM_COLORS;  /* Not moving. */
                continue;
            }
            
            pointVelocity1[0][i][1] *= 0.8;  /* 80% of previous up velocity. */
            pointTime1[i] = 0.0;  /* Reset the particles sense of up time. */
        }
        
        if (pointList2[0][i][1] <= -1.0) {
            if (distance2 > EDGE) {
                /* Particle has hit ground past the distance duration of
                 the particles.  Mark particle as dead. */
                colorList[i] = NUM_COLORS;  /* Not moving. */
                continue;
            }
            
            pointVelocity2[0][i][1] *= 0.8;  /* 80% of previous up velocity. */
            pointTime2[i] = 0.0;  /* Reset the particles sense of up time. */
        }
        if (pointList2[1][i][1] <= -1.0) {
            if (distance2 > EDGE-4) {
                /* Particle has hit ground past the distance duration of
                 the particles.  Mark particle as dead. */
                colorList[i] = NUM_COLORS;  /* Not moving. */
                continue;
            }
            
            pointVelocity2[1][i][1] *= 0.8;  /* 80% of previous up velocity. */
            pointTime2[i] = 0.0;  /* Reset the particles sense of up time. */
        }
        if (pointList2[2][i][1] <= -1.0) {
            if (distance2 > EDGE-4) {
                /* Particle has hit ground past the distance duration of
                 the particles.  Mark particle as dead. */
                colorList[i] = NUM_COLORS;  /* Not moving. */
                continue;
            }
            
            pointVelocity2[2][i][1] *= 0.8;  /* 80% of previous up velocity. */
            pointTime2[i] = 0.0;  /* Reset the particles sense of up time. */
        }
        if (pointList2[3][i][1] <= -1.0) {
            if (distance2 > EDGE) {
                /* Particle has hit ground past the distance duration of
                 the particles.  Mark particle as dead. */
                colorList[i] = NUM_COLORS;  /* Not moving. */
                continue;
            }
            
            pointVelocity2[2][i][1] *= 0.8;  /* 80% of previous up velocity. */
            pointTime2[i] = 0.0;  /* Reset the particles sense of up time. */
        }
        
        if (pointList3[0][i][1] <= 0.0) {
            if (distance3 > EDGE/2) {
                /* Particle has hit ground past the distance duration of
                 the particles.  Mark particle as dead. */
                colorList[i] = NUM_COLORS;  /* Not moving. */
                continue;
            }
            
            pointVelocity3[0][i][1] *= 0.8;  /* 80% of previous up velocity. */
            pointTime3[i] = 0.0;  /* Reset the particles sense of up time. */
        }
        
        if (pointList4[0][i][1] <= 0.0) {
            if (distance4 > EDGE) {
                /* Particle has hit ground past the distance duration of
                 the particles.  Mark particle as dead. */
                colorList[i] = NUM_COLORS;  /* Not moving. */
                continue;
            }
            
            pointVelocity4[0][i][1] *= 0.8;  /* 80% of previous up velocity. */
            pointTime4[i] = 0.0;  /* Reset the particles sense of up time. */
        }
        if (pointList4[1][i][1] <= 0.0) {
            if (distance4 > EDGE) {
                /* Particle has hit ground past the distance duration of
                 the particles.  Mark particle as dead. */
                colorList[i] = NUM_COLORS;  /* Not moving. */
                continue;
            }
            
            pointVelocity4[1][i][1] *= 0.8;  /* 80% of previous up velocity. */
            pointTime4[i] = 0.0;  /* Reset the particles sense of up time. */
        }
        if (pointList4[2][i][1] <= 0.0) {
            if (distance4 > EDGE) {
                /* Particle has hit ground past the distance duration of
                 the particles.  Mark particle as dead. */
                colorList[i] = NUM_COLORS;  /* Not moving. */
                continue;
            }
            
            pointVelocity4[2][i][1] *= 0.8;  /* 80% of previous up velocity. */
            pointTime4[i] = 0.0;  /* Reset the particles sense of up time. */
        }
        
        motion = 1;
        pointTime1[i] += TIME_DELTA;
        pointTime2[i] += TIME_DELTA;
        pointTime3[i] += TIME_DELTA;
        pointTime4[i] += TIME_DELTA;
    }
    t += TIME_DELTA;
    if (!motion && !spin) {
        if (repeat) {
            makePointList();
        } else {
            glutIdleFunc(NULL);
        }
    }
}


void animations(void) { // FUNZIONE MODIFICABILE
    // Inserisci qui tutti i calcoli che le animazioni devono fare
    mare->animation();
    updatePointList();
    if (spin) {
        angle += 0.3;
        newModel = 1;
    }
    glutPostRedisplay();
}

void visible(int vis)
{
    if (vis == GLUT_VISIBLE) {
        if (animate && (motion || spin)) {
            glutIdleFunc(animations);
        }
    } else {
        glutIdleFunc(NULL);
    }
}

void recalcModelView(void)
{
    glPopMatrix();
    glPushMatrix();
    glRotatef(angle, 0.0, 1.0, 0.0);
    newModel = 0;
}

void renderScene(void) {    // FUNZIONE MODIFICABILE
    
    int i;
    
    glClearColor(0.528941176, 0.80784314, 0.98039216, 1.0);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    
    camera.refresh();
    
    // Disegna qui quello che hai caricato
    glPushMatrix();
    glTranslated(-200.0, 0.0, 200.0);
    mare->drawSea();
    glPopMatrix();
    //    glPushMatrix();
    //    glTranslatef(20.0, 0, 20.0);
    //    glScalef(1.0, 20, 1.0);
    //    terreno->drawTerrain();
    //    glPopMatrix();
    
    // ANIMAZIONE DELL'OPERA
    float x;
    float y;
    float z;
    camera.getPosition(x, y, z);
    
    if (x >= 122 && x <= 147 && z <= -110 && z >= -134) { // Vicino ad opera 1
        visibilityOpera1 = 1;
        suOpera1 = 1;
    } else if (x >= 81 && x <= 128 && z <= -67 && z >= -87) { // Vicino ad opera 2
        visibilityOpera2 = 1;
        suOpera2 = 1;
    } else if (x >= 87 && x <= 103 && z <= -123 && z >= -140) { // Vicino ad opera 3
        visibilityOpera3 = 1;
        suOpera3 = 1;
    } else if (x >= 49 && x <= 75 && z <= -73 && z >= -125) { // Vicino ad opera 4
        visibilityOpera4 = 1;
        suOpera4 = 1;
    }
    
    if (newModel)
        recalcModelView();
    
    //Disegna qui le scene ma solo quelle per cui la visibilità è impostata a TRUE!
    
    //definisco la display list per il terreno
    if(scene_list_terreno == 0) {
        scene_list_terreno = glGenLists(1);
        glNewList(scene_list_terreno, GL_COMPILE);
        //recursive_render(scene,scene->mRootNode, 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[51], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[52], 1.0);
        glEndList();
    }
    
    if(scene_list_opera1==0) {
        scene_list_opera1 = glGenLists(1);
        glNewList(scene_list_opera1, GL_COMPILE);
        recursive_render(scene,scene->mRootNode->mChildren[0], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[1], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[2], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[3], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[4], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[5], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[6], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[7], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[8], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[9], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[10], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[11], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[12], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[13], 1.0);
        glEndList();
    }
    
    if(scene_list_opera2 == 0) {
        scene_list_opera2 = glGenLists(1);
        glNewList(scene_list_opera2, GL_COMPILE);
        recursive_render(scene,scene->mRootNode->mChildren[14], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[15], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[16], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[17], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[18], 1.0);
        glEndList();
    }
    
    if(scene_list_opera3 == 0) {
        scene_list_opera3 = glGenLists(1);
        glNewList(scene_list_opera3, GL_COMPILE);
        recursive_render(scene,scene->mRootNode->mChildren[19], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[20], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[21], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[22], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[23], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[24], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[25], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[26], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[27], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[28], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[29], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[30], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[31], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[32], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[33], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[34], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[35], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[36], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[37], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[38], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[39], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[40], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[41], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[42], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[43], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[44], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[45], 1.0);
        glEndList();
    }
    
    if(scene_list_opera4 == 0) {
        scene_list_opera4 = glGenLists(1);
        glNewList(scene_list_opera4, GL_COMPILE);
        recursive_render(scene,scene->mRootNode->mChildren[46], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[47], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[48], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[49], 1.0);
        recursive_render(scene,scene->mRootNode->mChildren[50], 1.0);
        glEndList();
    }
    
    if(scene_list_nuvole == 0) {
        scene_list_nuvole = glGenLists(1);
        glNewList(scene_list_nuvole, GL_COMPILE);
        recursive_render(scene,scene->mRootNode->mChildren[53], 1.0);
        glEndList();
    }

    
    //disegno terreno
    glCallList(scene_list_terreno);
    glCallList(scene_list_nuvole);
    
    //disegno prima opera
    if (visibilityOpera1) {
        //glPushMatrix();
        
        if (giuOpera1 && yTranslationOpera1 > -3.5) {
            yTranslationOpera1 -= 0.01;
            glPushMatrix();
            glTranslatef(0.0f, yTranslationOpera1, 0.0f);
            glCallList(scene_list_opera1);
            glPopMatrix();
            
            glDepthMask(GL_TRUE);
            glDisable(GL_TEXTURE_2D);
            
            glEnable ( GL_COLOR_MATERIAL );
            glPushMatrix();
            glTranslatef(130, 3.3, -120);
            glBegin(GL_POINTS);
            for (i=0; i<numPoints; i++) {
                /* Draw alive particles. */
                if (colorList[i] != DEAD) {
                    glColor4fv(colorSet[colorList[i]]);
                    glVertex3fv(pointList1[0][i]);
                }
            }
            glEnd();
            glPopMatrix();
            glDisable(GL_COLOR_MATERIAL);
        }
        else{
            if (suOpera1 && yTranslationOpera1 < 0) {
                yTranslationOpera1 += 0.01;
                glPushMatrix();
                glTranslatef(0.0f, yTranslationOpera1, 0.0f);
                glCallList(scene_list_opera1);
                glPopMatrix();
                
                glDepthMask(GL_TRUE);
                glDisable(GL_TEXTURE_2D);
                
                glEnable ( GL_COLOR_MATERIAL );
                glPushMatrix();
                glTranslatef(130, 3.3, -120);
                glBegin(GL_POINTS);
                for (i=0; i<numPoints; i++) {
                    /* Draw alive particles. */
                    if (colorList[i] != DEAD) {
                        glColor4fv(colorSet[colorList[i]]);
                        glVertex3fv(pointList1[0][i]);
                    }
                }
                glEnd();
                glPopMatrix();
                glDisable( GL_COLOR_MATERIAL );
            }
            else{
                glPushMatrix();
                glTranslatef(xTranslationOpera1, yTranslationOpera1, zTranslationOpera1); //influenzerà solo la prima opera
                glCallList(scene_list_opera1);
                glPopMatrix();
            }
        }
        //glCallList(scene_list_opera1);
        //glPopMatrix();
    }
    
    //disegno seconda opera
    if (visibilityOpera2) {
        //glPushMatrix();
        if (giuOpera2 && yTranslationOpera2 > -25) {
            yTranslationOpera2 -= 0.03;
            glPushMatrix();
            glTranslatef(0.0f, yTranslationOpera2, 0.0f);
            glCallList(scene_list_opera2);
            glPopMatrix();
            
            glDepthMask(GL_TRUE);
            glDisable(GL_TEXTURE_2D);
            
            glEnable ( GL_COLOR_MATERIAL );
            glPushMatrix();
            glTranslatef(115, 5.5, -78);
            glBegin(GL_POINTS);
            for (i=0; i<numPoints; i++) {
                /* Draw alive particles. */
                if (colorList[i] != DEAD) {
                    glColor4fv(colorSet[colorList[i]]);
                    glVertex3fv(pointList2[0][i]);
                }
            }
            glEnd();
            glPopMatrix();
            glDisable( GL_COLOR_MATERIAL );
            
            glEnable ( GL_COLOR_MATERIAL );
            glPushMatrix();
            glTranslatef(96, 7, -80);
            glBegin(GL_POINTS);
            for (i=0; i<numPoints; i++) {
                /* Draw alive particles. */
                if (colorList[i] != DEAD) {
                    glColor4fv(colorSet[colorList[i]]);
                    glVertex3fv(pointList2[3][i]);
                }
            }
            glEnd();
            glPopMatrix();
            glDisable( GL_COLOR_MATERIAL );
            
            if (yTranslationOpera2 > -2) {
                glEnable ( GL_COLOR_MATERIAL );
                glPushMatrix();
                glTranslatef(107, 6, -74);
                glBegin(GL_POINTS);
                for (i=0; i<numPoints; i++) {
                    /* Draw alive particles. */
                    if (colorList[i] != DEAD) {
                        glColor4fv(colorSet[colorList[i]]);
                        glVertex3fv(pointList2[1][i]);
                    }
                }
                glEnd();
                glPopMatrix();
                glDisable( GL_COLOR_MATERIAL );
                
                glEnable ( GL_COLOR_MATERIAL );
                glPushMatrix();
                glTranslatef(103, 6, -74);
                glBegin(GL_POINTS);
                for (i=0; i<numPoints; i++) {
                    /* Draw alive particles. */
                    if (colorList[i] != DEAD) {
                        glColor4fv(colorSet[colorList[i]]);
                        glVertex3fv(pointList2[2][i]);
                    }
                }
                glEnd();
                glPopMatrix();
                glDisable( GL_COLOR_MATERIAL );
            }
        }
        else{
            if (suOpera2 && yTranslationOpera2 < 0) {
                yTranslationOpera2 += 0.03;
                glPushMatrix();
                glTranslatef(0.0f, yTranslationOpera2, 0.0f);
                glCallList(scene_list_opera2);
                glPopMatrix();
                
                glDepthMask(GL_TRUE);
                glDisable(GL_TEXTURE_2D);
                
                glEnable ( GL_COLOR_MATERIAL );
                glPushMatrix();
                glTranslatef(115, 5.5, -78);
                glBegin(GL_POINTS);
                for (i=0; i<numPoints; i++) {
                    /* Draw alive particles. */
                    if (colorList[i] != DEAD) {
                        glColor4fv(colorSet[colorList[i]]);
                        glVertex3fv(pointList2[0][i]);
                    }
                }
                glEnd();
                glPopMatrix();
                glDisable( GL_COLOR_MATERIAL );
                
                glEnable ( GL_COLOR_MATERIAL );
                glPushMatrix();
                glTranslatef(96, 7, -80);
                glBegin(GL_POINTS);
                for (i=0; i<numPoints; i++) {
                    /* Draw alive particles. */
                    if (colorList[i] != DEAD) {
                        glColor4fv(colorSet[colorList[i]]);
                        glVertex3fv(pointList2[3][i]);
                    }
                }
                glEnd();
                glPopMatrix();
                glDisable( GL_COLOR_MATERIAL );
                
                if (yTranslationOpera2 > -2) {
                    glEnable ( GL_COLOR_MATERIAL );
                    glPushMatrix();
                    glTranslatef(110, 6, -74);
                    glBegin(GL_POINTS);
                    for (i=0; i<numPoints; i++) {
                        /* Draw alive particles. */
                        if (colorList[i] != DEAD) {
                            glColor4fv(colorSet[colorList[i]]);
                            glVertex3fv(pointList2[1][i]);
                        }
                    }
                    glEnd();
                    glPopMatrix();
                    glDisable( GL_COLOR_MATERIAL );
                    
                    glEnable ( GL_COLOR_MATERIAL );
                    glPushMatrix();
                    glTranslatef(103, 6, -74);
                    glBegin(GL_POINTS);
                    for (i=0; i<numPoints; i++) {
                        /* Draw alive particles. */
                        if (colorList[i] != DEAD) {
                            glColor4fv(colorSet[colorList[i]]);
                            glVertex3fv(pointList2[2][i]);
                        }
                    }
                    glEnd();
                    glPopMatrix();
                    glDisable( GL_COLOR_MATERIAL );
                }
            }
            else{
                glPushMatrix();
                glTranslatef(xTranslationOpera2, yTranslationOpera2, zTranslationOpera2); //influenzerà solo la seconda opera
                glCallList(scene_list_opera2);
                glPopMatrix();
            }
        }
        //glCallList(scene_list_opera2);
        //glPopMatrix();
    }
    
    //disegno terza opera
    if (visibilityOpera3) {
        //glPushMatrix();
        
        if (giuOpera3 && yTranslationOpera3 > -3) {
            yTranslationOpera3 -= 0.01;
            glPushMatrix();
            glTranslatef(0.0f, yTranslationOpera3, 0.0f);
            glCallList(scene_list_opera3);
            glPopMatrix();
            
            glDepthMask(GL_TRUE);
            glDisable(GL_TEXTURE_2D);
            
            glEnable ( GL_COLOR_MATERIAL );
            glPushMatrix();
            glTranslatef(96, 5, -130);
            glBegin(GL_POINTS);
            for (i=0; i<numPoints; i++) {
                /* Draw alive particles. */
                if (colorList[i] != DEAD) {
                    glColor4fv(colorSet[colorList[i]]);
                    glVertex3fv(pointList3[0][i]);
                }
            }
            glEnd();
            glPopMatrix();
            glDisable( GL_COLOR_MATERIAL );
        }
        else{
            if (suOpera3 && yTranslationOpera3 < 0) {
                yTranslationOpera3 += 0.01;
                glPushMatrix();
                glTranslatef(0.0f, yTranslationOpera3, 0.0f);
                glCallList(scene_list_opera3);
                glPopMatrix();
                
                glDepthMask(GL_TRUE);
                glDisable(GL_TEXTURE_2D);
                
                glEnable ( GL_COLOR_MATERIAL );
                glPushMatrix();
                glTranslatef(96, 5, -130);
                glBegin(GL_POINTS);
                for (i=0; i<numPoints; i++) {
                    /* Draw alive particles. */
                    if (colorList[i] != DEAD) {
                        glColor4fv(colorSet[colorList[i]]);
                        glVertex3fv(pointList3[0][i]);
                    }
                }
                glEnd();
                glPopMatrix();
                glDisable( GL_COLOR_MATERIAL );
            }
            else{
                glPushMatrix();
                glTranslatef(xTranslationOpera3, yTranslationOpera3, zTranslationOpera3); //influenzerà solo la seconda opera
                glCallList(scene_list_opera3);
                glPopMatrix();
            }
        }
        //glCallList(scene_list_opera3);
        //glPopMatrix();
    }
    
    //disegno quarta opera
    if (visibilityOpera4) {
        //glPushMatrix();
        
        if (giuOpera4 && yTranslationOpera4 > -8) {
            yTranslationOpera4 -= 0.02;
            glPushMatrix();
            glTranslatef(0.0f, yTranslationOpera4, 0.0f);
            glCallList(scene_list_opera4);
            glPopMatrix();
            
            glDepthMask(GL_TRUE);
            glDisable(GL_TEXTURE_2D);
            
            glEnable ( GL_COLOR_MATERIAL );
            glPushMatrix();
            glTranslatef(65, 7.5, -84);
            glBegin(GL_POINTS);
            for (i=0; i<numPoints; i++) {
                /* Draw alive particles. */
                if (colorList[i] != DEAD) {
                    glColor4fv(colorSet[colorList[i]]);
                    glVertex3fv(pointList4[0][i]);
                }
            }
            glEnd();
            glPopMatrix();
            glDisable( GL_COLOR_MATERIAL );
            
            glEnable ( GL_COLOR_MATERIAL );
            glPushMatrix();
            glTranslatef(60, 7, -96);
            glBegin(GL_POINTS);
            for (i=0; i<numPoints; i++) {
                /* Draw alive particles. */
                if (colorList[i] != DEAD) {
                    glColor4fv(colorSet[colorList[i]]);
                    glVertex3fv(pointList4[1][i]);
                }
            }
            glEnd();
            glPopMatrix();
            glDisable( GL_COLOR_MATERIAL );
            
            glEnable ( GL_COLOR_MATERIAL );
            glPushMatrix();
            glTranslatef(63, 7, -114);
            glBegin(GL_POINTS);
            for (i=0; i<numPoints; i++) {
                /* Draw alive particles. */
                if (colorList[i] != DEAD) {
                    glColor4fv(colorSet[colorList[i]]);
                    glVertex3fv(pointList4[2][i]);
                }
            }
            glEnd();
            glPopMatrix();
            glDisable( GL_COLOR_MATERIAL );
        }
        else{
            if (suOpera4 && yTranslationOpera4 < 0) {
                yTranslationOpera4 += 0.02;
                glPushMatrix();
                glTranslatef(0.0f, yTranslationOpera4, 0.0f);
                glCallList(scene_list_opera4);
                glPopMatrix();
                
                glDepthMask(GL_TRUE);
                glDisable(GL_TEXTURE_2D);
                
                glEnable ( GL_COLOR_MATERIAL );
                glPushMatrix();
                glTranslatef(65, 7.5, -84);
                glBegin(GL_POINTS);
                for (i=0; i<numPoints; i++) {
                    /* Draw alive particles. */
                    if (colorList[i] != DEAD) {
                        glColor4fv(colorSet[colorList[i]]);
                        glVertex3fv(pointList4[0][i]);
                    }
                }
                glEnd();
                glPopMatrix();
                glDisable( GL_COLOR_MATERIAL );
                
                glEnable ( GL_COLOR_MATERIAL );
                glPushMatrix();
                glTranslatef(60, 7, -96);
                glBegin(GL_POINTS);
                for (i=0; i<numPoints; i++) {
                    /* Draw alive particles. */
                    if (colorList[i] != DEAD) {
                        glColor4fv(colorSet[colorList[i]]);
                        glVertex3fv(pointList4[1][i]);
                    }
                }
                glEnd();
                glPopMatrix();
                glDisable( GL_COLOR_MATERIAL );
                
                glEnable ( GL_COLOR_MATERIAL );
                glPushMatrix();
                glTranslatef(63, 7, -114);
                glBegin(GL_POINTS);
                for (i=0; i<numPoints; i++) {
                    /* Draw alive particles. */
                    if (colorList[i] != DEAD) {
                        glColor4fv(colorSet[colorList[i]]);
                        glVertex3fv(pointList4[2][i]);
                    }
                }
                glEnd();
                glPopMatrix();
                glDisable( GL_COLOR_MATERIAL );
            }
            else{
                glPushMatrix();
                glTranslatef(xTranslationOpera4, yTranslationOpera4, zTranslationOpera4); //influenzerà solo la seconda opera
                glCallList(scene_list_opera4);
                glPopMatrix();
            }
        }
        //glCallList(scene_list_opera4);
        //glPopMatrix();
    }
    
    //    if (newModel)
    //        recalcModelView();
    //
    //glDepthMask(GL_FALSE);
    
    //    /* Draw the floor. */
    //    glPushMatrix();
    //    glTranslatef(130, 0, -50);
    //    glEnable(GL_TEXTURE_2D);
    //    glColor3f(0.5, 1.0, 0.5);
    //    glBegin(GL_QUADS);
    //    glTexCoord2f(0.0, 0.0);
    //    glVertex3f(-EDGE, -0.05, -EDGE);
    //    glTexCoord2f(20.0, 0.0);
    //    glVertex3f(EDGE, -0.05, -EDGE);
    //    glTexCoord2f(20.0, 20.0);
    //    glVertex3f(EDGE, -0.05, EDGE);
    //    glTexCoord2f(0.0, 20.0);
    //    glVertex3f(-EDGE, -0.05, EDGE);
    //    glEnd();
    //    glPopMatrix();
    
    
    //scrivo il contatore
    setOrthographicProjection();
    glPushMatrix();
    glLoadIdentity();
    char integer_string[32];
    int integer = visibilityOpera1+visibilityOpera2+visibilityOpera3+visibilityOpera4;
    sprintf(integer_string, "%d", integer);
    char first_string[64] = "Hai trovato ";
    strcat(first_string, integer_string);
    char second_string[64] = "/4 opere";
    strcat(first_string, second_string);
    renderSpacedBitmapString(5,30,1,GLUT_BITMAP_HELVETICA_18,first_string);
    glPopMatrix();
    restorePerspectiveProjection();
    
    glutSwapBuffers();
}


void lighting(void) {   // AGGIUNGI QUI NUOVE LUCI!
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
    glLightfv(GL_LIGHT1, GL_POSITION, position1);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, specular2);
    glLightfv(GL_LIGHT2, GL_POSITION, position2);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT2);
    
    //glClearColor(0.0,0.0,1.0, 1.0);    // TODO: Colore dello sfondo (imposta questo come colore del cielo)
}

void changeSize(int width, int height) {
    
    // Prevent a divide by zero, when window is too short
    if (height == 0) height = 1;
    if (width == 0) width = 1;
    
    w = width;
    h = height;
    
    float ratio = width * 1.0 / height;
    
    // Use the projection Matrix
    glMatrixMode(GL_PROJECTION);
    
    // Reset matrix
    glLoadIdentity();
    
    // Set the viewport to be the entire window
    glViewport(0, 0, width, height);
    gluPerspective(45.0f, ratio, .1f, 100.0f);
    
    // Get back to the ModelView state
    glMatrixMode(GL_MODELVIEW);
}

void pressNormalKey(unsigned char key, int xx, int yy) {    // AGGIUNGI QUI NUOVI BOTTONI
    
    switch (key) {
        case 033:
            exit(0);
        default:
            keys.clear();
            keys[key] = true;
            break;
    }
}

void releaseKey(int key, int xx, int yy) {
    keys[key] = false;
}

void pressSpecialKey(int key, int xx, int yy) { // AGGIUNGI QUI NUOVI BOTTONI
    
    switch (key) {
            
        case GLUT_KEY_LEFT:
            // Freccia sinistra premuta
            break;
            
        case GLUT_KEY_RIGHT:
            // Freccia destra premuta
            break;
            
        case GLUT_KEY_UP:
            // Freccia in alto premuta
            break;
            
        case GLUT_KEY_DOWN:
            // Freccia in basso premuta
            break;
            
        default:
            break;
    }
}

void mouseButton(int button, int state, int x, int y) {
    // TODO: if necessary
}

void mouseMoved(int x, int y) {
    
    static bool just_warped = false;
    
    if (just_warped) {
        just_warped = false;
        return;
    }
    
    int dx = x - win_width/2;
    int dy = y - win_hight/2;
    
    if (dx) camera.rotateYaw(rotationSpeed*dx);
    if (dy) camera.rotatePitch(-rotationSpeed*dy);
    
    glutWarpPointer(win_width/2, win_hight/2);
    
    just_warped = true;
}

void Timer(int value) {
        // KEYBOARD INPUT
        if (keys['w'] || keys['W']) camera.move(movementSpeed);
        else if (keys['s'] || keys['S']) camera.move(-movementSpeed);
        else if (keys['a'] || keys['A']) camera.strafe(movementSpeed);
        else if (keys['d'] || keys['D']) camera.strafe(-movementSpeed);
        glutTimerFunc(1, Timer, 1);
}
