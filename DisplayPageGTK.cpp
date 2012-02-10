//
//  DisplayPageGTK.cpp
//  Pimp
//
//  Created by Michelle Alexander on 12/30/11.
//  Copyright 2011 ??? . All rights reserved.
//

#include "DisplayPageGTK.h"
#include "MainPageGTK.h"

using namespace std;

extern "C"{
    void DisplayPage_onBackPress()
    {
        // Stop the camera and go back to the main page
        Pimp::sharedPimp().videoOff();
        Pimp::sharedPimp().setDisplayPage(new MainPageGTK());
    }
}

DisplayPageGTK::DisplayPageGTK()
{
    // Window box to contain this page
    window = gtk_vbox_new (TRUE,1);
    
    // Title
    title = gtk_label_new("Display Page");
    gtk_box_pack_start(GTK_BOX (window), title, TRUE, TRUE, 0);

	GtkWidget *box = gtk_hbox_new (TRUE, 1);
	
    personTitle = gtk_label_new("Person: ");
    gtk_box_pack_start(GTK_BOX (box), personTitle, TRUE, TRUE, 0);
    personName =gtk_label_new( "");
    gtk_box_pack_start(GTK_BOX (box), personName, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(window), box, TRUE, TRUE, 0);

	box = gtk_hbox_new(TRUE,1);

    roomTitle = gtk_label_new("Room: ");
    gtk_box_pack_start(GTK_BOX (box), roomTitle, TRUE, TRUE, 0);
    roomName =gtk_label_new( "");
    gtk_box_pack_start(GTK_BOX (box), roomName, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(window), box, TRUE, TRUE, 0);
    
    // Buttons
    btn_back = gtk_button_new_with_label ("Back");
    gtk_signal_connect (GTK_OBJECT (btn_back), "clicked",
                        GTK_SIGNAL_FUNC (DisplayPage_onBackPress), NULL);
    gtk_box_pack_start( GTK_BOX(window), btn_back, TRUE, TRUE, 0);
    
    // Start the camera
    Pimp::sharedPimp().videoOn();
    Pimp::sharedPimp().initProcess();
}

void DisplayPageGTK::runLoop()
{
    // Run the main loop
    Pimp::sharedPimp().mainProcess();
}

void DisplayPageGTK::display()
{
    // Draw the title
    gtk_widget_show (title);
    
    // Draw the person and room names
    gtk_widget_show (personTitle);
    gtk_widget_show (personName);
    gtk_widget_show (roomTitle);
    gtk_widget_show (roomName);
    
    
    // Draw the buttons and sliders
    gtk_widget_show (btn_back);
    
	gtk_label_set_text(GTK_LABEL(personName),(gchar*)Pimp::sharedPimp().getRecognizedPerson().c_str());
    gtk_label_set_text(GTK_LABEL( roomName), (gchar*)Pimp::sharedPimp().getRecognizedRoom().c_str());
}
