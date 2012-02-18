//
//  FaceTexture.h
//  PiMP
//
//  Created by Michelle Alexander on 11/2/11.
//  Copyright 2011 ??? . All rights reserved.
//

#ifndef FaceTextureGTK_h
#define FaceTextureGTK_h

#include "BaseTextureGTK.h"

#include <cv.h>
#include <string>

class FaceTextureGTK: public BaseTextureGTK
{
private:
    // Processed Image and display area
    IplImage *processed;
    CvRect displayRegion;
    CvRect wholeRegion;
    
    // Dimensions
    int width;
    int height;
    
    bool invalid;
    
    
public:
    FaceTextureGTK(GtkWidget *holder, int d);
    
    // Display Function
    void draw();
    
    // Getters and Setters
    void setFace(IplImage *img, IplImage *proc, CvRect r);
    void saveProcessedFaceAsJPG(std::string file);
    void setInvalid(bool invalidate);
    bool isInvalid();
};

#endif
