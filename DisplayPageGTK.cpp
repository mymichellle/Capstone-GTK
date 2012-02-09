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
    title = "Display Page";
    personTitle = "Person: ";
    roomTitle = "Room: ";
    personName = "";
    roomName = "";
    
    // Buttons
    btn_back = gtk_button_new_with_label ("Back");
    gtk_signal_connect (GTK_OBJECT (btn_back), "clicked",
                        GTK_SIGNAL_FUNC (DisplayPage_onBackPress), NULL);
    gtk_box_pack_start( GTK_BOX(window), btn_back, TRUE, TRUE, 0);
    
    // Start the camera
    Pimp::sharedPimp().videoOn();
    Pimp::sharedPimp().initProcess();
}

void DisplayPageGTK::display()
{
    
    // Draw the title
        
    
    // Draw the person and room names
        
    
    
    // Draw the buttons and sliders
    gtk_widget_show (btn_back);
    
    
    // Run the main loop
    Pimp::sharedPimp().mainProcess();
    personName = Pimp::sharedPimp().getRecognizedPerson();
    roomName = Pimp::sharedPimp().getRecognizedRoom();
}