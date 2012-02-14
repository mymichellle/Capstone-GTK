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

enum RoomEntryMode
{
	INIT_ROOM,
	IMG_ROOM
};

class RoomEntryPageGTK : public BasePageGTK
{
public:
    RoomEntryPageGTK();
	RoomEntryPageGTK(RoomEntryMode mode, std::string rName);

    // Display
    void display();
private:
	// Initialize function
	void initPage(enum RoomEntryMode mode, std::string rName);

    // Title
    GtkWidget *title;
    
    // Buttons
    GtkWidget *btn_back;
    GtkWidget *btn_add;
    GtkWidget *btn_new;
    
	// img
	RoomTextureGTK *room;
	// Dialog
	GtkWidget *dialog_name;

	// Keyboad
	GtkWidget *keyboard;

    // Properties
    std::string *name;
    
    // Textures
	GtkWidget *imgBox;
    
};

#endif
