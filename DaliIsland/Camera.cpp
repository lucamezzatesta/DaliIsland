//
//  Camera.cpp
//  DaliIsland
//
//  Created by Luca Mezzatesta on 26/06/16.
//  Copyright © 2016 Luca Mezzatesta. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include <GLUT/glut.h>
#include <iostream>
#include "Camera.hpp"

#define INCR_ROT 0.001
#define HEIGHT_OFFSET 5

void Camera::init() {
    m_yaw = .0;
    m_pitch = .0;
    
    m_x = .0;
    m_y = .0;
    m_z = .0;
}

void Camera::refresh() {
    m_lx = cos(m_yaw) * cos(m_pitch);
    m_ly = sin(m_pitch);
    m_lz = sin(m_yaw) * cos(m_pitch);
    
    m_strafe_lx = cos(m_yaw - M_PI_2);
    m_strafe_lz = sin(m_yaw - M_PI_2);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_x, m_y, m_z,
              m_x + m_lx, m_y + m_ly, m_z + m_lz,
              0.0, 1.0, 0.0);
    
    //std::cout << "x: " << m_x << std::endl << "z: " << m_z << std::endl << "y: " << m_y << std::endl;
}

void Camera::setPosition(float x, float z) {
    
    if (collider.canMove(x, z)) {
        m_x = x;
        m_y = collider.getHeight(x, z) + HEIGHT_OFFSET;
        m_z = z;
        
        refresh();
    } else {
        std::cerr << "CAMERA: illegal position!" << std::endl;
    }
}

void Camera::getPosition(float &x, float &y, float &z) {
    x = m_x;
    y = m_y;
    z = m_z;
}

void Camera::move(float incr) {
    float lx = cos(m_yaw)*cos(m_pitch);
    float ly = sin(m_pitch);                // TODO: Delete this?
    float lz = sin(m_yaw)*cos(m_pitch);
    
    float tmp_m_x = m_x + incr*lx;
    float tmp_m_z = m_z + incr*lz;
    
    if (collider.canMove(tmp_m_x, tmp_m_z)) {
        m_x = tmp_m_x;
        m_y = collider.getHeight(tmp_m_x, tmp_m_z) + HEIGHT_OFFSET;
        m_z = tmp_m_z;
        
        refresh();
    } else {
        std::cerr << "CAMERA: cannot move this way!" << std::endl;
    }
}

void Camera::strafe(float incr) {
    
    float tmp_m_x = m_x + incr * m_strafe_lx;
    float tmp_m_z = m_z + incr * m_strafe_lz;
    
    
    if (collider.canMove(tmp_m_x, tmp_m_z)) {
        m_x = tmp_m_x;
        m_y = collider.getHeight(m_x, m_z) + HEIGHT_OFFSET;
        m_z = tmp_m_z;
        
        refresh();
    } else {
        std::cerr << "CAMERA: cannot move this way!" << std::endl;
    }
}

void Camera::rotateYaw(float angle) {
    m_yaw += angle;
    
    refresh();
}

void Camera::rotatePitch(float angle) {
    
    const float limit = 89 * M_PI / 180;
    
    m_pitch += angle;
    
    if (m_pitch < -limit) m_pitch = -limit;
    if (m_pitch > limit) m_pitch = limit;
    
    refresh();
}

void Camera::setYaw(float angle) {
    m_yaw = angle;
    refresh();
}

void Camera::setPitch(float angle) {
    m_pitch = angle;
    refresh();
}