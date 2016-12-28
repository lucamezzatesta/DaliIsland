//
//  Collision.hpp
//  DaliIsland
//
//  Created by Luca Mezzatesta on 14/07/16.
//  Copyright © 2016 Luca Mezzatesta. All rights reserved.
//

#ifndef Collision_hpp
#define Collision_hpp

#include <iostream>
#include <fstream>

class Collision {
    
    float** height_map = nullptr;
    int** collision_map = nullptr;
    int collision_map_dim = 0;
    int height_map_dim = 0;
    
public:
    Collision(std::string height_map, std::string collision_map);
    bool canMove(float x, float z);
    float getHeight(float x, float z);
};

#endif /* Collision_hpp */
