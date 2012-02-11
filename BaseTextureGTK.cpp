//
//  BaseTexture.cpp
//  PiMP
//
//  Created by Michelle Alexander on 12/1/11.
//  Copyright 2011 ??? . All rights reserved.
//

#include "BaseTexture.h"
#include "Pimp.h"
#include <highgui.h>
#include <iostream>
#include <sys/stat.h>

using namespace std;

BaseTexture::BaseTexture()
{
    // Defualt values
    name = "unknown";
    fileName = "unknown";
    image = cvCreateImage(cvSize(10, 10), IPL_DEPTH_8U, 3);
}

void BaseTexture::setTexture()
{
    if (image != NULL)
    {
        glGenTextures(1, &texture);
        
        glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture to it's array
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height,0, GL_BGR, GL_UNSIGNED_BYTE, image->imageData);    
    }    
}

void BaseTexture::setImage(IplImage *img)
{
    image = cvCloneImage (img);
    setTexture();
}



// OpenGL draw as a texture
void BaseTexture::draw()
{
    setTexture();
    glColor3f(1,1,1);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    
    glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture to it's array
    
    // Set the texture to a 100X100 square
    glBegin( GL_QUADS );
    glTexCoord2f(0,0); glVertex2f(image->width/8,image->height/8);
    glTexCoord2f(1,0); glVertex2f(-image->width/8,image->height/8);
    glTexCoord2f(1,1); glVertex2f(-image->width/8,-image->height/8);
    glTexCoord2f(0,1); glVertex2f(image->width/8,-image->height/8);
    glEnd(); 
    
    glDisable(GL_TEXTURE_2D);
}

string BaseTexture::getName()
{return name;}

void BaseTexture::setName(string n)
{
    // Name should always lower case
    name = n;
    transform(name.begin(), name.end(), name.begin(), ::tolower);
}

void BaseTexture::saveTextureAsJPG(string file)
{
    // Make sure filename is always lowercase
    fileName = file;
    transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
    
    // Get the face directory
    string dir = Pimp::sharedPimp().getDir()+ "/Textures/" + name;
    
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