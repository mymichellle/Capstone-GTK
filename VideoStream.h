//
//  VideoStream.h
//  PiMP
//
//  Created by Michelle Alexander on 9/21/11.
//  Copyright 2011 ??? . All rights reserved.
//

#ifndef VideoStream_h
#define VideoStream_h

#include <cv.h>
#include <highgui.h>

class VideoStream
{
private:
    // Properties
    CvCapture *capture;
    
    // Small Frame
    IplImage* small_frame;
    
    // Helper Functions
    void cleanUpVideo();
public:
    VideoStream();
    
    // Control Functions
    void startCapture();
    void endCapture();
    
    // Getters
    IplImage* getImage();
};

#endif