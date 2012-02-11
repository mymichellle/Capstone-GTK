//
//  RoomTextureGTK.h
//  PiMP
//
//  Created by Michelle Alexander on 11/2/11.
//  Copyright 2011 ??? . All rights reserved.
//

#ifndef RoomTextureGTK_h
#define RoomTextureGTK_h

#include "BaseTextureGTK.h"

#include <cv.h>
#include <string>

class RoomTextureGTK: public BaseTextureGTK
{
    
public:
    RoomTextureGTK();
	RoomTextureGTK(GtkWidget *holder);
    
    void saveRoomAsJPG(std::string file);
};

#endif
