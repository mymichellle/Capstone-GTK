//
//  RoomTexture.cpp
//  PiMP
//
//  Created by Michelle Alexander on 12/23/11.
//  Copyright 2011 ??? . All rights reserved.
//

#include "RoomTexture.h"
#include "Pimp.h"

RoomTexture::RoomTexture()
{
    // Defualt values
    name = "unknown";
    fileName = "unknown";
    image = cvCreateImage(cvSize(10, 10), IPL_DEPTH_8U, 3);
}

void RoomTexture::saveRoomAsJPG(string file)
{
    // Make sure filename is always lowercase
    fileName = file;
    transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
    
    // Get the face directory
    string dir = Pimp::sharedPimp().getDir()+ "/Rooms/" + name;
    
    // Make sure the directory exists
    int r = mkdir((char *)dir.c_str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if(r > 0)
        cout<<"Directory not made! "<<dir<<endl;
    
    // Save the image
    if(image)
    {
        stringstream s;
        s << dir << "/"<< fileName <<".jpg";
        cvSaveImage((char*)string(s.str()).c_str() ,image);
    }
}