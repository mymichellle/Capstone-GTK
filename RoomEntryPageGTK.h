//
//  RoomEntryPageGTK.h
//  Pimp
//
//  Created by Michelle Alexander on 11/11/11.
//  Copyright 2011 ??? . All rights reserved.
//

#ifndef RoomEntryPageGTK_h
#define RoomEntryPageGTK_h

#include "BasePageGTK.h"
#include "RoomTextureGTK.h"
#include "Pimp.h"

class RoomEntryPageGTK : public BasePageGTK
{
private:
    // Title
    GtkWidget *title;
    
    // Buttons
    GtkWidget *btn_back;
    GtkWidget *btn_add;
    GtkWidget *btn_new;
    
    // Dialog
    GtkWidget *dialog_name;
    
    // Properties
    std::string *name;
    
    // Textures
	GtkWidget *imgBox;    
	RoomTextureGTK *room;
    
public:
    RoomEntryPageGTK();
    
    // Display
    void display();
};

#endif
