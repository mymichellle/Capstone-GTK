//
//  MainPageGTK.h
//  Pimp
//
//  Created by Michelle Alexander on 10/27/11.
//  Copyright 2011 ??? . All rights reserved.
//

#ifndef MainPageGTK_h
#define MainPageGTK_h

#include "BasePageGTK.h"
#include <gtk/gtk.h>
#include <string>

class MainPageGTK : public BasePageGTK
{
private:
    // Title
    GtkWidget *label;
        
    // Buttons
    GtkWidget *btn_newFace;
    GtkWidget *btn_newRoom;
    GtkWidget *btn_recogntion;
    GtkWidget *btn_settings;
    GtkWidget *btn_test;
        
public:
    MainPageGTK();
        
    // Display
    void display();
};

#endif