//
//  FaceTexture.cpp
//  PiMP
//
//  Created by Michelle Alexander on 11/2/11.
//  Copyright 2011 ??? . All rights reserved.
//

#include "FaceTexture.h"
#include "Pimp.h"
#include <highgui.h>
#include <iostream>
#include <sys/stat.h>

using namespace std;

FaceTexture::FaceTexture(int x, int y, int w, int h)
{
    // Defualt values
    name = "unknown";
    fileName = "unknown";
    image = cvCreateImage(cvSize(10, 10), IPL_DEPTH_8U, 3);
    processed = cvCreateImage(cvSize(10, 10), IPL_DEPTH_8U, 1);
    
    //Dimensions
    xpos = x;
    ypos = y;
    width = w;
    height = h;
    
    invalid = false;
}

void FaceTexture::setFace(IplImage *img, IplImage *proc, CvRect r)
{
    image = cvCloneImage (img);
    processed = cvCloneImage(proc);
    displayRegion = r;
    wholeRegion = cvRect(0, 0, img->width, img->height);
    setTexture();
}

void FaceTexture::setInvalid(bool invalidate)
{
    invalid = invalidate;
    cout<<"Invalid set to : "<<invalid<<endl;
}

bool FaceTexture::isInvalid()
{
    return invalid;
}

bool FaceTexture::pointInBounds(int x, int y)
{
    if(x > xpos - width/2 &&
       x < xpos + width/2 &&
       y > ypos - height/2 &&
       y < ypos + height/2)
        return true;
    else
        return false;
}

// Returns true if the button was successfully clicked
bool FaceTexture::mouse(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && pointInBounds(x,y))
        if( state == GLUT_UP)
            return true;
    return false;
}

// OpenGL draw face as a texture
void FaceTexture::draw()
{
    glPushMatrix();
    setTexture();
    glColor3f(1,1,1);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    
     //bind the texture to it's array
    glBindTexture( GL_TEXTURE_2D, texture );
    
    // Get the x and y values to display only the face
    float x1 = displayRegion.x / (float)wholeRegion.width;
    float x2 = (displayRegion.x + displayRegion.width) / (float) wholeRegion.width;
    float y1 = displayRegion.y / (float)wholeRegion.height;
    float y2 = (displayRegion.y + displayRegion.height) / (float) wholeRegion.height;
    
    // Draw in the correct location
    glTranslatef(xpos, ypos, 0);
    
    // Set the texture to a 100X100 square
    glBegin( GL_QUADS );
    glTexCoord2f(x1,y1); glVertex2f(width/2,height/2);
    glTexCoord2f(x2,y1); glVertex2f(-(width/2),width/2);
    glTexCoord2f(x2,y2); glVertex2f(-(width/2),-(width/2));
    glTexCoord2f(x1,y2); glVertex2f(width/2,-(width/2));
    glEnd(); 
    
    // Draw a red slash to show image is invalid
    if(invalid)
    {
        glColor3f(1, 0, 0);
        glBegin( GL_LINES );
        glVertex2f(-width/2, -height/2);
        glVertex2f(width/2, height/2);
        glEnd();
    }
    
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void FaceTexture::saveProcessedFaceAsJPG(string file)
{
    // Make sure filename is always lowercase
    fileName = file;
    transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
    
    // Get the face directory
    string dir = Pimp::sharedPimp().getFaceDir() + name;

    // Make sure the directory exists
    int r = mkdir((char *)dir.c_str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if(r > 0)
        cout<<"Directory not made! "<<dir<<endl;
    
    // Save the image
    if(processed)
    {
        stringstream s;
        s << dir << "/"<< fileName <<".jpg";
        cvSaveImage((char*)string(s.str()).c_str() ,processed);
    }
}