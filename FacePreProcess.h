//
//  FacePreProcess.h
//  PiMP
//
//  Created by Michelle Alexander on 9/22/11.
//  Copyright 2011 ??? . All rights reserved.
//

#ifndef FacePreProcess_h
#define FacePreProcess_h

#include <cv.h>

class FacePreProcess
{
public:
    FacePreProcess();
    IplImage* preProcess(IplImage *input);
    
};

#endif