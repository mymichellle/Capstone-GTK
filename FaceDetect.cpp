//
//  FaceDetect.cpp
//  PiMP
//
//  Created by Michelle Alexander on 9/22/11.
//  Copyright 2011 ??? . All rights reserved.
//

#include <iostream>
#include "FaceDetect.h"

using namespace std;

FaceDetect::FaceDetect(int pScale)
{
    scale = pScale;
    
    storage = cvCreateMemStorage(0);
    assert(storage);
    
    cascade = (CvHaarClassifierCascade*)cvLoad("haarcascade_frontalface_alt2.xml", 0, 0, 0);
    if( !cascade ) {
        cout<<"Couldnt load Face detector haarcascade_frontalface_alt2.xml";
        exit(1);
    }
}

CvSeq* FaceDetect::getFaces(IplImage *frame, CvRect prevFace)
{
    IplImage *  gray_image;
    IplImage * small_image;   
    CvRect searchRect = cvRect(prevFace.x-15, prevFace.y-15, prevFace.x+prevFace.width+15, prevFace.y+prevFace.height+15);
    // Use the search area as long as it is not 0
    if(false) // prevFace.width > 0 && prevFace.height > 0)
    {
        cout << "LOOKING IN RECT FACE DETECT"<<endl;
        
        cvSetImageROI(frame,searchRect);
        gray_image = cvCreateImage(cvSize (searchRect.width, searchRect.height), IPL_DEPTH_8U, 1);
        small_image = cvCreateImage(cvSize (searchRect.width, searchRect.height), IPL_DEPTH_8U, 1);
        
    }
    else
    { 
        //cout << "NORMAL FACE DETECT"<<endl;
        gray_image = cvCreateImage(cvSize (frame->width, frame->height), IPL_DEPTH_8U, 1);
        small_image = cvCreateImage(cvSize (frame->width / scale, frame->height / scale), IPL_DEPTH_8U, 1);
    }
    //cout << "1"<<endl;
        assert (gray_image);
    
    //cout << "2 frame w,h "<<frame->width<<", "<<frame->height<<endl;
    //cout << "  gray  w,h "<<gray_image->width<<", "<<gray_image->height<<endl;
    // convert to gray and downsize
    cvCvtColor (frame, gray_image, CV_BGR2GRAY);
    //cout << "3"<<endl;
    cvResize (gray_image, small_image, CV_INTER_LINEAR);
    
    //cout << "4"<<endl;
    
    if(prevFace.width > 0 && prevFace.height > 0)
        cvResetImageROI(frame);
    return cvHaarDetectObjects (small_image, cascade, storage,
                                1.1, 2, CV_HAAR_DO_CANNY_PRUNING,
                                        cvSize (30, 30));
}

CvRect FaceDetect::getBoundsForFace(CvRect *face, IplImage *frame)
{
    if(!face)
        return cvRect(0,0,10,10);
    else
        return cvRect(frame->width - (face->width)*scale - (face->x)*scale ,face->y*scale,face->width*scale,face->height*scale);
}

// Return the most dominant face in the frame (largest)
CvRect FaceDetect::getFace(IplImage *frame, CvRect prevFace)
{
    // Get all the faces and return the first one
    CvSeq* faces = getFaces(frame, prevFace); 
    CvRect* r;
    CvRect* rLarge = (CvRect*) cvGetSeqElem (faces, 0);
    for (int i = 0; i < (faces ? faces->total : 0); i++)
    {
        r = (CvRect*) cvGetSeqElem (faces, i);
        if(r->width > rLarge->width || r->height > rLarge->height)
            rLarge = r;
    } 
    //cout<<"FacesTotal "<<faces->total<<endl;
    if(faces->total == 0)
        return cvRect(0,0,10,10);
    else
        return getBoundsForFace(rLarge, frame);
}
