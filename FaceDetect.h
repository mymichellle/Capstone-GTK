//
//  FaceDetect.h
//  PiMP
//
//  Created by Michelle Alexander on 9/22/11.
//  Copyright 2011 ??? . All rights reserved.
//

#ifndef FaceDetect_h
#define FaceDetect_h

#include <cassert>
#include <cv.h>

class FaceDetect
{
private:
    CvHaarClassifierCascade* cascade;
    CvMemStorage* storage;
    int scale;
    
public:
    FaceDetect(int pScale = 1);
    CvSeq* getFaces(IplImage *frame, CvRect prevFace);
    CvRect getBoundsForFace(CvRect *face, IplImage *frame);
    CvRect getFace(IplImage *frame, CvRect prevFace);
};

#endif