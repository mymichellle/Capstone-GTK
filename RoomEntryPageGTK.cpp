//
//  RoomEntryPageGTK.cpp
//  Pimp
//
//  Created by Michelle Alexander on 11/11/11.
//  Copyright 2011 ??? . All rights reserved.
//

#include "RoomEntryPageGTK.h"
#include "MainPageGTK.h"
#include <iostream>

extern "C"{
	// Goes back to main page
	void RoomEntryPage_onBackPress()
	{
		Pimp::sharedPimp().setDisplayPage(new MainPageGTK());
		Pimp::sharedPimp().videoOff();
	}

	// Add Face
	void RoomEntryPage_onAddPress(RoomTextureGTK *roomTex)
	{    
		//room->setName(dialog_name->getValue());
		//Pimp::sharedPimp().addRoom(room);
	}

	// Get new image
	void RoomEntryPage_onNewPress(RoomTextureGTK *roomTex)
	{
		//Pimp::sharedPimp().getNewTexture(roomTex);
	}
}

RoomEntryPageGTK::RoomEntryPageGTK()
{
    // Window box to contain this page
    window = gtk_vbox_new (FALSE,1);  

	// Title
    title = gtk_label_new("Add A Room");
    gtk_box_pack_start(GTK_BOX (window), title, TRUE, TRUE, 0);

	// Image
	imgBox = gtk_hbox_new (TRUE,1);
	gtk_box_pack_start( GTK_BOX(window), imgBox, TRUE, TRUE, 0);
    room = new RoomTextureGTK(imgBox);

	// Buttons
	GtkWidget *hbox = gtk_hbox_new (TRUE, 1);
	gtk_box_pack_start( GTK_BOX(window), hbox, TRUE, TRUE, 0);
    btn_new = gtk_button_new_with_label("New Image");
	gtk_signal_connect (GTK_OBJECT (btn_new), "clicked",
						GTK_SIGNAL_FUNC (RoomEntryPage_onNewPress), room);
	gtk_box_pack_start( GTK_BOX(hbox), btn_new, TRUE, TRUE, 0);

    btn_add = gtk_button_new_with_label("Add");
	gtk_signal_connect (GTK_OBJECT (btn_add), "clicked",
						GTK_SIGNAL_FUNC (RoomEntryPage_onAddPress), room);
	gtk_box_pack_start( GTK_BOX(hbox), btn_add, TRUE, TRUE, 0);

    btn_back = gtk_button_new_with_label ("Back");
    gtk_signal_connect (GTK_OBJECT (btn_back), "clicked",
                        GTK_SIGNAL_FUNC (RoomEntryPage_onBackPress), NULL);
    gtk_box_pack_start( GTK_BOX(window), btn_back, TRUE, TRUE, 0);



    //dialog_name = new BaseDialog("Name: ", "Unknown", WINDOW_WIDTH/2, WINDOW_HEIGHT/4, 100);
    
    
	// Turn on the camera and take an image
    Pimp::sharedPimp().videoOn();
    Pimp::sharedPimp().getNewTexture(room);
}

// Display
void RoomEntryPageGTK::display()
{   
    // Draw the title
    gtk_widget_show (title);

    // Draw the buttons and dialogs
    gtk_widget_show (btn_back);
    gtk_widget_show (btn_add);
    gtk_widget_show (btn_new);
    //gtk_widget_show (dialog_name->draw();
    
    // Draw the room image
    //room->draw();
    
}
