//
//  Camera.hpp
//  DaliIsland
//
//  Created by Luca Mezzatesta on 26/06/16.
//  Copyright © 2016 Luca Mezzatesta. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include "Collision.hpp"

class Camera {
public:
    Camera(): collider("./height_map.txt", "./collision_map.txt") {init();}
    ~Camera() {}
    
    void init();
    void refresh();
    void setPosition(float x, float z);
    void getPosition(float &x, float &y, float &z);
    void setYaw(float angle);
    void setPitch(float angle);
    
    void move(float incr);
    void strafe(float incr);
    void rotateYaw(float angle);
    void rotatePitch(float angle);
    
private:
    float m_x, m_y, m_z;        // Posizione
    float m_lx, m_ly, m_lz;     // Look at vector
    float m_yaw, m_pitch;       // Angoli di rotazione
    float m_des_yaw, m_des_pitch;   // Angoli di rotazione desiderati
    float m_strafe_lx, m_strafe_lz;
    Collision collider;
};

#endif /* Camera_hpp */
