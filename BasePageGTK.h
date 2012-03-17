//
//  BasePageGTK.h
//  Collage
//
//  Created by Michelle Alexander on 9/23/11.
//  Copyright 2011 ??? . All rights reserved.
//
//  Template for a display page 

#ifndef BasePageGTK_h
#define BasePageGTK_h

#include <string>
#include <gtk/gtk.h>

class BasePageGTK
{
protected:
    int width;
    int height;
    std::string title;
    GtkWidget *window;
    
public:
    BasePageGTK();
    virtual void display();
    GtkWidget* getWindow();
	void redraw();
    
};

#endif
