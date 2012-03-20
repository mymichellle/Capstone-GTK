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
	//cout<<"test "<<CV_CAP_PROP_POS_FRAMES<<endl;
	//double pos_frames = cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);
	//cout<<"getImage frame position: "<<pos_frames<<endl;
    /*
	property identifier. Can be one of the following:
	CV_CAP_PROP_POS_MSEC - film current position in milliseconds or video capture timestamp
	CV_CAP_PROP_POS_FRAMES - 0-based index of the frame to be decoded/captured next
	CV_CAP_PROP_POS_AVI_RATIO - relative position of video file (0 - start of the film, 1 - end of the film)
	CV_CAP_PROP_FRAME_WIDTH - width of frames in the video stream
	CV_CAP_PROP_FRAME_HEIGHT - height of frames in the video stream
	CV_CAP_PROP_FPS - frame rate
	CV_CAP_PROP_FOURCC - 4-character code of codec. CV_CAP_PROP_FRAME_COUNT - number of frames in video file.
	*/

    // Resize the frame
    cvResize(cvQueryFrame(capture), small_frame,CV_INTER_LINEAR);

    return small_frame;
}

void VideoStream::cleanUpVideo()
{
    cvReleaseCapture( &capture );
    capture = NULL;
}
