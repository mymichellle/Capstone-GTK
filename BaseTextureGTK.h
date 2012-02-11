//
//  BaseTexture.h
//  PiMP
//
//  Created by Michelle Alexander on 12/1/11.
//  Copyright 2011 ??? . All rights reserved.
//

#ifndef BaseTexture_h
#define BaseTexture_h

#ifdef __APPLE__
#include <GL/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <cv.h>
#include <string>

class BaseTexture
{
protected:
    // Name
    std::string name;
    std::string fileName;
    
    // Texture
    GLuint texture;
    void setTexture();
    
    // Image and display area
    IplImage *image;
    CvRect displayRegion;
    
public:
    BaseTexture();
    
    // Display Function
    virtual void draw();
    
    void saveTextureAsJPG(std::string file);
    
    // Getters and Setters
    IplImage* getImage(){return image;};
    void setImage(IplImage *img);
    virtual void setName(std::string n);
    virtual std::string getName();
};

#endif
