//
//  FaceTexture.h
//  PiMP
//
//  Created by Michelle Alexander on 11/2/11.
//  Copyright 2011 ??? . All rights reserved.
//

#ifndef FaceTexture_h
#define FaceTexture_h

#include "BaseTexture.h"

#ifdef __APPLE__
#include <GL/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <cv.h>
#include <string>

class FaceTexture: public BaseTexture
{
private:
    // Processed Image and display area
    IplImage *processed;
    CvRect displayRegion;
    CvRect wholeRegion;
    
    // Dimensions / Locations
    int xpos;
    int ypos;
    int width;
    int height;
    
    bool invalid;
    
    // Helper Function
    bool pointInBounds(int x,int y);
    
public:
    FaceTexture(int x, int y, int w, int h);
    
    // Display Function
    void draw();
    bool mouse(int button, int state, int x, int y);
    
    // Getters and Setters
    void setFace(IplImage *img, IplImage *proc, CvRect r);
    void saveProcessedFaceAsJPG(std::string file);
    void setInvalid(bool invalidate);
    bool isInvalid();
};

#endif