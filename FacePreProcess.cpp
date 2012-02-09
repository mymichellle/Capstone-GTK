//
//  FacePreProcess.cpp
//  PiMP
//
//  Created by Michelle Alexander on 9/22/11.
//  Copyright 2011 ??? . All rights reserved.
//

#include "FacePreProcess.h"

FacePreProcess::FacePreProcess()
{
    
}

IplImage* FacePreProcess::preProcess(IplImage *input)
{
    // Get a grey scale version of the input image
    IplImage *imgGrey, *imgProcess;
    if(input->nChannels == 3)
    {
        imgGrey = cvCreateImage( cvGetSize(input), IPL_DEPTH_8U, 1);
        cvCvtColor( input, imgGrey, CV_BGR2GRAY);
    }
    else
    {
        imgGrey = input;
    }
    
    // Resize the image to a consistent size
    imgProcess = cvCreateImage(cvSize(100,100), IPL_DEPTH_8U, 1);
    
    // CV_INTER_CUBIC or CV_INTER_LINEAR is good for enlarging, and
    // CV_INTER_AREA is good for shrinking / decimation, but bad at enlarging.
    cvResize(imgGrey, imgProcess, CV_INTER_LINEAR);
    
    // Give the image a standard brightness and contrast
    cvEqualizeHist(imgProcess, imgProcess);
    
    if(imgGrey)
        cvReleaseImage(&imgGrey);
    
    return imgProcess;
}