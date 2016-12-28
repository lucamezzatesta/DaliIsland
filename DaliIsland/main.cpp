//
//  main.cpp
//  DaliIsland
//
//  Created by Luca Mezzatesta on 26/05/16.
//  Copyright © 2016 Luca Mezzatesta. All rights reserved.
//

#include "Engine.hpp"

int main(int argc, char * argv[]) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WIN_WIDTH, WIN_HIGHT);
    glutCreateWindow(WIN_TITLE);
    
    glutSetCursor(GLUT_CURSOR_NONE);    // Nasconde la freccia del mouse quando si trova nella finestra
    glutWarpPointer(WIN_WIDTH/2, WIN_HIGHT/2);  // Posiziona il mouse al centro della finestra
    // glutFullScreen();
    glShadeModel(GL_FLAT);  // Imposta lo shader di openGL come piatto
    
    load();
    
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(animations);
    glutVisibilityFunc(visible);
    glutSpecialFunc(pressSpecialKey);
    glutKeyboardFunc(pressNormalKey);
    glutSpecialUpFunc(releaseKey);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMoved);
    glutPassiveMotionFunc(mouseMoved);
    // glutEntryFunc() // TODO: Callback del mouse che entra/esce dalla finestra
    
    glShadeModel(GL_FLAT);  // Imposta lo shader piatto di OpenGL
    //glEnable(GL_CULL_FACE); // Non disegna le facce che non venogno visualizzate
    glEnable(GL_DEPTH_TEST);
    lighting();
    
    glPointSize(3.0);
#if GL_SGIS_point_parameters
    glPointParameterfvSGIS(GL_DISTANCE_ATTENUATION_SGIS, quad);
#endif
    makePointList();
    
    glutTimerFunc(1, Timer, 0);
    
    glutMainLoop();
    
    //cleanup because the library keeps internal resources until the scene is freed again. Not doing so can cause severe resource leaking.
    ReleaseImport();
    
    // Se entra qui allora c'è un errore
    
    return -1;
}
