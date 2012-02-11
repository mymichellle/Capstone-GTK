//
//  RoomTexture.h
//  PiMP
//
//  Created by Michelle Alexander on 11/2/11.
//  Copyright 2011 ??? . All rights reserved.
//

#ifndef RoomTexture_h
#define RoomTexture_h

#include "BaseTexture.h"

#ifdef __APPLE__
#include <GL/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <cv.h>
#include <string>

class RoomTexture: public BaseTexture
{
    
public:
    RoomTexture();
    
    void saveRoomAsJPG(std::string file);
};

#endif