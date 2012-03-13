//
//  RoomEntryPageGTK.cpp
//  Pimp
//
//  Created by Michelle Alexander on 11/11/11.
//  Copyright 2011 ??? . All rights reserved.
//

#include "RoomEntryPageGTK.h"
#include "MainPageGTK.h"
#include "KeyboardGTK.h"
#include <iostream>

using namespace std;

extern "C"{

	void RoomEntryPage_onBackExit(GtkWidget *btn)
	{
		// turn off camera
		Pimp::sharedPimp().videoOff();
		// Exit to Main Page
		Pimp::sharedPimp().setDisplayPage(new MainPageGTK());
	}
	
	void RoomEntryPage_onBackPage(GtkWidget *btn, GtkWidget *label)
	{
		// Go back to INIT_ROOM mode pass name from label
		Pimp::sharedPimp().setDisplayPage(new RoomEntryPageGTK(INIT_ROOM, gtk_label_get_text(GTK_LABEL(label))));
	}
	
	void RoomEntryPage_onNewImg(GtkWidget *btn, GtkWidget *label)
	{
		Pimp::sharedPimp().setDisplayPage(new RoomEntryPageGTK(IMG_ROOM, gtk_label_get_text(GTK_LABEL(label))));
	}

	void RoomEntryPage_onGetImg(GtkWidget *btn, GtkWidget *dialog)
	{	
		Pimp::sharedPimp().setDisplayPage(new RoomEntryPageGTK(IMG_ROOM, gtk_entry_get_text((GtkEntry*)dialog)));
	}
	
	void RoomEntryPage_onAdd(GtkWidget *btn, RoomTextureGTK *roomtext)
	{
		// Add the image to known rooms	
		Pimp::sharedPimp().addRoom(roomtext);
		// turn off camera
		Pimp::sharedPimp().videoOff();
		// Exit to Main Page
		Pimp::sharedPimp().setDisplayPage(new MainPageGTK());
	
	}
	
	void RoomEntryPage_keyType(GtkWidget *keyboard, GtkWidget *dialog)
	{
		gtk_entry_append_text((GtkEntry*)dialog, ((KeyboardGTK*)keyboard)->activeKey);
	}

	void RoomEntryPage_keyDelete(GtkWidget *keyboard, GtkWidget *dialog)
	{
		int len = gtk_entry_get_text_length((GtkEntry*)dialog);
		if(len >= 1)
		{
			string text = gtk_entry_get_text((GtkEntry*)dialog);
	        text.erase(len-1);
			gtk_entry_set_text((GtkEntry*)dialog,(char*)text.c_str());
		}
	}
}

RoomEntryPageGTK::RoomEntryPageGTK()
{
	initPage(INIT_ROOM, "");
}


RoomEntryPageGTK::RoomEntryPageGTK(enum RoomEntryMode mode, std::string rName)
{
	initPage(mode, rName);
}

void RoomEntryPageGTK::initPage(enum RoomEntryMode mode, std::string rName)
{
    // Window box to contain this page
    window = gtk_vbox_new (FALSE,1); 

	if(mode == INIT_ROOM)
	{
		// label - "Room Entry"
		title = gtk_label_new("Room Entry");
		gtk_box_pack_start(GTK_BOX (window), title, TRUE, TRUE, 0);
		
		// hBox
		GtkWidget *hbox = gtk_hbox_new (TRUE, 1);
		gtk_box_pack_start( GTK_BOX(window), hbox, TRUE, TRUE, 0);

		// Back - RoomEntryPage_onBackExit
		btn_back = gtk_button_new_with_label ("Back");
		gtk_signal_connect (GTK_OBJECT (btn_back), "clicked",
		                    GTK_SIGNAL_FUNC (RoomEntryPage_onBackExit), NULL);
		gtk_box_pack_start( GTK_BOX(hbox), btn_back, TRUE, TRUE, 0);

		dialog_name = gtk_entry_new();

		// TakeImg - RoomEntryPage_onGetImg (Dialog)
		btn_new = gtk_button_new_with_label("Get Image");
		gtk_signal_connect (GTK_OBJECT (btn_new), "clicked",
							GTK_SIGNAL_FUNC (RoomEntryPage_onGetImg), dialog_name);
		gtk_box_pack_start( GTK_BOX(hbox), btn_new, TRUE, TRUE, 0);
		
		// Dialog - rName
		gtk_entry_set_text((GtkEntry*)dialog_name, (char*)rName.c_str());
		gtk_box_pack_start( GTK_BOX(window), dialog_name, TRUE, TRUE, 0);
		
		// Keyboard - RoomEntryPage_keyType (dialog)
		GtkWidget *keyWindow = gtk_hbox_new (FALSE, 1);
		gtk_box_pack_start(GTK_BOX(window), keyWindow, TRUE,TRUE,0);
    	GtkWidget *separator = gtk_vseparator_new();
		keyboard = keyboardGTK_new();
		gtk_signal_connect (GTK_OBJECT (keyboard), "keyboardGTK",
							GTK_SIGNAL_FUNC (RoomEntryPage_keyType), dialog_name);
		gtk_signal_connect (GTK_OBJECT (keyboard), "keyboardGTK_delete",
							GTK_SIGNAL_FUNC (RoomEntryPage_keyDelete), dialog_name);
		gtk_box_pack_start(GTK_BOX(keyWindow), separator, FALSE,TRUE,15);
		gtk_box_pack_start(GTK_BOX(keyWindow), keyboard, TRUE,TRUE,0);

	}
	else if(mode == IMG_ROOM)
	{
		// Turn on the camera
    	Pimp::sharedPimp().videoOn();

		// label - rName
		title = gtk_label_new((char*)rName.c_str());
		gtk_box_pack_start(GTK_BOX (window), title, TRUE, TRUE, 0);

		// img
		imgBox = gtk_hbox_new (TRUE,1);
		gtk_box_pack_start( GTK_BOX(window), imgBox, TRUE, TRUE, 0);
		room = new RoomTextureGTK(imgBox);
		room->setName(rName);
		
		// hBox
		GtkWidget *hbox = gtk_hbox_new (TRUE, 1);
		gtk_box_pack_start( GTK_BOX(window), hbox, TRUE, TRUE, 0);

		// Back - RoomEntryPage_onBackPage (label)
		btn_back = gtk_button_new_with_label ("Back");
		gtk_signal_connect (GTK_OBJECT (btn_back), "clicked",
		                    GTK_SIGNAL_FUNC (RoomEntryPage_onBackPage), title);
		gtk_box_pack_start( GTK_BOX(hbox), btn_back, TRUE, TRUE, 0);

		// NewImg - RoomEntryPage_onNewImg (label)
		btn_new = gtk_button_new_with_label("New Image");
		gtk_signal_connect (GTK_OBJECT (btn_new), "clicked",
							GTK_SIGNAL_FUNC (RoomEntryPage_onNewImg), title);
		gtk_box_pack_start( GTK_BOX(hbox), btn_new, TRUE, TRUE, 0);

		// Add - RoomEntryPage_onAdd (img)
		btn_add = gtk_button_new_with_label("Add");
		gtk_signal_connect (GTK_OBJECT (btn_add), "clicked",
							GTK_SIGNAL_FUNC (RoomEntryPage_onAdd), room);
		gtk_box_pack_start( GTK_BOX(hbox), btn_add, TRUE, TRUE, 0);
		
		// Get an image
	    Pimp::sharedPimp().getNewTexture(room);
		Pimp::sharedPimp().videoOff();
	}
}

// Display
void RoomEntryPageGTK::display()
{   
    
}
