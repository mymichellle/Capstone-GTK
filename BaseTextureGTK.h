//
//  BaseTextureGTK.h
//  PiMP
//
//  Created by Michelle Alexander on 12/1/11.
//  Copyright 2011 ??? . All rights reserved.
//

#ifndef BaseTextureGTK_h
#define BaseTextureGTK_h

#include <gtk/gtk.h>
#include <cv.h>
#include <string>

class BaseTextureGTK
{
protected:
    // Name
    std::string name;
    std::string fileName;
    
    // Texture
	GtkWidget* gtk_img;
	GtkWidget* convertOpenCv2Gtk(IplImage *image);
    void setTexture();
    
    // Image and display area
    IplImage *image;
    CvRect displayRegion;

	// GTK box
	GtkWidget *box;
    
public:
    BaseTextureGTK();
	BaseTextureGTK(GtkWidget* holder);
    
    // Display Function
    virtual void draw();
    
    void saveTextureAsJPG(std::string file);
    
    // Getters and Setters
    IplImage* getImage(){return image;};
	GtkWidget* getGtkImage(){return gtk_img;};
    void setImage(IplImage *img);
    virtual void setName(std::string n);
    virtual std::string getName();
};

#endif
