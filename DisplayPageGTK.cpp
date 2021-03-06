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
    void DisplayPage_onBackPress(gint* timeoutHandler)
    {
		// Stop the timeout loop
		//gtk_timeout_remove(*timeoutHandler);
        // Stop the camera and go back to the main page
        Pimp::sharedPimp().videoOff();
        Pimp::sharedPimp().setDisplayPage(new MainPageGTK());
    }

	static gboolean time_handler(GtkWidget *widget)
	{
		// Run the main loop until the display page does not exist
 		if (widget->window == NULL) return FALSE;
    	Pimp::sharedPimp().mainProcess();  
		gtk_widget_queue_draw(widget);
 	 	return TRUE;
	}

	// Update the recognized person display
	static gboolean update_person_name(GtkWidget *widget)
	{
 		if (widget->window == NULL) return FALSE;
		gtk_label_set_text(GTK_LABEL(widget),(gchar*)Pimp::sharedPimp().getRecognizedPerson().c_str());
	}

	// Update the recognized room display
	static gboolean update_room_name(GtkWidget *widget)
	{
 		if (widget->window == NULL) return FALSE;
		gtk_label_set_text(GTK_LABEL( widget), (gchar*)Pimp::sharedPimp().getRecognizedRoom().c_str());
	}

	static gboolean updateEverything(DisplayPageGTK *displayPage)
	{
		if(displayPage == NULL || displayPage->getWindow()->window == NULL) return FALSE;
  
		displayPage->setRoomName(Pimp::sharedPimp().getRecognizedRoom());
		displayPage->setPersonName(Pimp::sharedPimp().getRecognizedPerson());
		displayPage->redraw();

    	Pimp::sharedPimp().mainProcess();
	}

}

DisplayPageGTK::DisplayPageGTK()
{
    // Window box to contain this page
    window = gtk_vbox_new (TRUE,1);  
	
    
	// Title
	if(!Pimp::sharedPimp().isFaceRecognition() && !Pimp::sharedPimp().isRoomRecognition())
		title = gtk_label_new("No Recognition Mode is selected!");
	else
    	title = gtk_label_new("Display Page");
    gtk_box_pack_start(GTK_BOX (window), title, TRUE, TRUE, 0);

	GtkWidget *box = gtk_hbox_new (TRUE, 1);
	
	if(Pimp::sharedPimp().isFaceRecognition())
	{
    	personTitle = gtk_label_new("Person: ");
    	gtk_box_pack_start(GTK_BOX (box), personTitle, TRUE, TRUE, 0);
	}	
    personName =gtk_label_new( "");
    gtk_box_pack_start(GTK_BOX (box), personName, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(window), box, TRUE, TRUE, 0);

	box = gtk_hbox_new(TRUE,1);

	if(Pimp::sharedPimp().isRoomRecognition())
	{
    	roomTitle = gtk_label_new("Room: ");
    	gtk_box_pack_start(GTK_BOX (box), roomTitle, TRUE, TRUE, 0);
	}

    roomName =gtk_label_new( "");
    gtk_box_pack_start(GTK_BOX (box), roomName, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(window), box, TRUE, TRUE, 0);
    
    // Buttons
    btn_back = gtk_button_new_with_label ("Back");
    gtk_signal_connect (GTK_OBJECT (btn_back), "clicked",
                        GTK_SIGNAL_FUNC (DisplayPage_onBackPress), (gpointer) &timeout_handler_id);
    gtk_box_pack_start( GTK_BOX(window), btn_back, TRUE, TRUE, 0);
    
    // Start the camera
    Pimp::sharedPimp().videoOn();
    Pimp::sharedPimp().initProcess();

	// Create a timer
	gtk_idle_add_priority( G_PRIORITY_LOW, (GSourceFunc) updateEverything, (gpointer) this); 
}

void DisplayPageGTK::runLoop()
{
    // Run the main loop
	gtk_label_set_text(GTK_LABEL(personName),(gchar*)Pimp::sharedPimp().getRecognizedPerson().c_str());
    gtk_label_set_text(GTK_LABEL( roomName), (gchar*)Pimp::sharedPimp().getRecognizedRoom().c_str());
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

void DisplayPageGTK::setPersonName(std::string name)
{
	if(personName == NULL)
		return;

	if(Pimp::sharedPimp().isFaceRecognition())
		gtk_label_set_text(GTK_LABEL( personName), (gchar*)name.c_str());
	else
		gtk_label_set_text(GTK_LABEL( personName), "");		
}

void DisplayPageGTK::setRoomName(std::string name)
{
	if(roomName == NULL)
		return;
	if(Pimp::sharedPimp().isRoomRecognition())
		gtk_label_set_text(GTK_LABEL( roomName), (gchar*)name.c_str());
	else
		gtk_label_set_text(GTK_LABEL( roomName), "");
		
}

