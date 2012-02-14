//
//  BaseTextureGTK.cpp
//  PiMP
//
//  Created by Michelle Alexander on 12/1/11.
//  Copyright 2011 ??? . All rights reserved.
//

#include "BaseTextureGTK.h"
#include "Pimp.h"
#include <highgui.h>
#include <iostream>
#include <sys/stat.h>

using namespace std;

BaseTextureGTK::BaseTextureGTK()
{
    // Defualt values
    name = "unknown";
    fileName = "unknown";
    image = cvCreateImage(cvSize(10, 10), IPL_DEPTH_8U, 3);
	box = gtk_hbox_new (TRUE,1);
}

BaseTextureGTK::BaseTextureGTK(GtkWidget *holder)
{
    // Defualt values
    name = "unknown";
    fileName = "unknown";
    image = cvCreateImage(cvSize(10, 10), IPL_DEPTH_8U, 3);
	box = gtk_hbox_new (TRUE,1);
	gtk_box_pack_start( GTK_BOX(holder), box, TRUE, TRUE, 0);
	
}

void BaseTextureGTK::setTexture()
{
}

GtkWidget *BaseTextureGTK::convertOpenCv2Gtk(IplImage *img)
{ 
	// make the gtk image 160x120
	IplImage *gtkMask = cvCreateImage(cvSize (160, 120), IPL_DEPTH_8U, 3);
	cvResize(img, gtkMask,CV_INTER_LINEAR);
	cvCvtColor( gtkMask, gtkMask, CV_BGR2RGB ); // Usually opencv image is BGR, so we need to change it to RGB 
	GdkPixbuf *pix = gdk_pixbuf_new_from_data ((guchar*)gtkMask->imageData, 
		GDK_COLORSPACE_RGB, 
		FALSE, 
		gtkMask->depth, 
		gtkMask->width, 
		gtkMask->height, 
		(gtkMask->widthStep), 
		NULL, 
		NULL); 
	GtkWidget *gtkWidg = gtk_image_new_from_pixbuf (pix);
	return gtkWidg;
} 


void BaseTextureGTK::setImage(IplImage *img)
{
	if(gtk_img != NULL)
		gtk_widget_destroy(gtk_img);
    image = cvCloneImage (img);
    gtk_img = convertOpenCv2Gtk(image);
	gtk_box_pack_start( GTK_BOX(box), gtk_img, TRUE, TRUE, 0);
}



// OpenGL draw as a texture
void BaseTextureGTK::draw()
{
   gtk_widget_show (gtk_img);
}

string BaseTextureGTK::getName()
{return name;}

void BaseTextureGTK::setName(string n)
{
    // Name should always lower case
    name = n;
    transform(name.begin(), name.end(), name.begin(), ::tolower);
}

void BaseTextureGTK::saveTextureAsJPG(string file)
{
    // Make sure filename is always lowercase
    fileName = file;
    transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
    
    // Get the face directory
    string dir = Pimp::sharedPimp().getDir()+ "/Textures/" + name;
    
    // Make sure the directory exists
    int r = mkdir((char *)dir.c_str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if(r > 0)
        cout<<"Directory not made! "<<dir<<endl;
    
    // Save the image
    if(image)
    {
        stringstream s;
        s << dir << "/"<< fileName <<".jpg";
        cvSaveImage((char*)string(s.str()).c_str() ,image);
    }
}
