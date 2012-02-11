//
//  VideoStream.cpp
//  PiMP
//
//  Created by Michelle Alexander on 9/21/11.
//  Copyright 2011 ??? . All rights reserved.
//

#include <iostream>
#include "VideoStream.h"

#define STANDARD_WIDTH 640
#define STANDARD_HEIGHT 480

using namespace std;

VideoStream::VideoStream()
{
    capture = NULL;
   small_frame = cvCreateImage(cvSize (STANDARD_WIDTH/2, STANDARD_HEIGHT/2), IPL_DEPTH_8U, 3);
    
}

void VideoStream::startCapture()
{
#ifdef __APPLE__
    capture = cvCaptureFromCAM( 1 );
#else
    capture = cvCaptureFromCAM( CV_CAP_ANY );
#endif
}

void VideoStream::endCapture()
{
    cleanUpVideo();
}

IplImage* VideoStream::getImage()
{ 
    if ( !capture ) 
    {
        cout<<"ERROR: capture is NULL" << endl;
        return NULL;
    }
    
    // Resize the frame
    cvResize(cvQueryFrame(capture), small_frame,CV_INTER_LINEAR);
    
    return small_frame;
}

void VideoStream::cleanUpVideo()
{
    cvReleaseCapture( &capture );
    capture = NULL;
}
