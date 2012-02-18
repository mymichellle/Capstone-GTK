//
//  FaceTexture.cpp
//  PiMP
//
//  Created by Michelle Alexander on 11/2/11.
//  Copyright 2011 ??? . All rights reserved.
//

#include "FaceTextureGTK.h"
#include "Pimp.h"
#include <highgui.h>
#include <iostream>
#include <sys/stat.h>

using namespace std;

FaceTextureGTK::FaceTextureGTK(GtkWidget *holder, int d)
{
    // Defualt values
    name = "unknown";
    fileName = "unknown";
    image = cvCreateImage(cvSize(10, 10), IPL_DEPTH_8U, 3);
    processed = cvCreateImage(cvSize(10, 10), IPL_DEPTH_8U, 1);
    
    //Dimensions
    width = d;
    height = d;
    
	box = gtk_hbox_new (TRUE,1);
	gtk_box_pack_start( GTK_BOX(holder), box, TRUE, TRUE, 0);

    invalid = false;
}

void FaceTextureGTK::setFace(IplImage *img, IplImage *proc, CvRect r)
{
    image = cvCloneImage (img);
    processed = cvCloneImage(proc);
    displayRegion = r;
	 cvSetImageROI(image,r);
    wholeRegion = cvRect(0, 0, img->width, img->height);
    gtk_img = convertOpenCv2Gtk(image, width, height);
	gtk_box_pack_start( GTK_BOX(box), gtk_img, TRUE, TRUE, 0);
	cvResetImageROI(image);
}

void FaceTextureGTK::setInvalid(bool invalidate)
{
    invalid = invalidate;
    cout<<"Invalid set to : "<<invalid<<endl;
}

bool FaceTextureGTK::isInvalid()
{
    return invalid;
}

// OpenGL draw face as a texture
void FaceTextureGTK::draw()
{
   
}

void FaceTextureGTK::saveProcessedFaceAsJPG(string file)
{
    // Make sure filename is always lowercase
    fileName = file;
    transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
    
    // Get the face directory
    string dir = Pimp::sharedPimp().getFaceDir() + name;

    // Make sure the directory exists
    int r = mkdir((char *)dir.c_str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if(r > 0)
        cout<<"Directory not made! "<<dir<<endl;
    
    // Save the image
    if(processed)
    {
        stringstream s;
        s << dir << "/"<< fileName <<".jpg";
        cvSaveImage((char*)string(s.str()).c_str() ,processed);
    }
}
