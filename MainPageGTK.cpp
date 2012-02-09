//
//  MainPageGTK.cpp
//  Collage
//
//  Created by Michelle Alexander on 10/27/11.
//  Copyright 2011 ??? . All rights reserved.
//
//  Template for a display page

#include "MainPageGTK.h"
#include "Pimp.h"
#include "SettingsPageGTK.h"
#include "DisplayPageGTK.h"
/*#include "FaceEntryPage.h"
#include "RoomEntryPage.h"
#include "FaceTexture.h"*/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

// Define button callbacks
extern "C"{
    // Goes to the add a face page
    void MainPage_onNewFacePress()
    {
        // Pimp::sharedPimp().setDisplayPage(new FaceEntryPage());
        g_print ("MainPage-face\n");
    }
    
    // Goes to the add a face page
    void MainPage_onNewRoomPress()
    {
        //Pimp::sharedPimp().setDisplayPage(new RoomEntryPage());
        g_print ("MainPage-room\n");
    }
    
    // Runs room and face recognition
    void MainPage_onRecPress()
    {
        Pimp::sharedPimp().setDisplayPage(new DisplayPageGTK());
        g_print ("MainPage-rec\n");
    }
    
    
    void MainPage_onSettingsPress()
    {
        Pimp::sharedPimp().setDisplayPage(new SettingsPageGTK());
        g_print ("MainPage-set\n");
    }
    
    void MainPage_onTestPress()
    {
        // Pimp::sharedPimp().testProc();
        g_print ("MainPage-test\n");
    }
}

// Initialize the pages dialog boxes, buttons, and title
MainPageGTK::MainPageGTK()
{
    // Window box to contain this page
    window = gtk_vbox_new (TRUE,1);
    
    label = gtk_label_new("Capstone Testing");
    gtk_box_pack_start(GTK_BOX (window), label, TRUE, TRUE, 0);
    
    btn_newFace = gtk_button_new_with_label ("New Face");
    gtk_signal_connect (GTK_OBJECT (btn_newFace), "clicked",
                        GTK_SIGNAL_FUNC (MainPage_onNewFacePress), NULL);
    gtk_box_pack_start(GTK_BOX (window), btn_newFace, TRUE, TRUE,0);

    btn_newRoom = gtk_button_new_with_label ("New Room");  
    gtk_signal_connect (GTK_OBJECT (btn_newRoom), "clicked",
                        GTK_SIGNAL_FUNC (MainPage_onNewRoomPress), NULL);
    gtk_box_pack_start(GTK_BOX (window), btn_newRoom, TRUE, TRUE,0);
    
    btn_recogntion = gtk_button_new_with_label ("Recognition"); 
    gtk_signal_connect (GTK_OBJECT (btn_recogntion), "clicked",
                        GTK_SIGNAL_FUNC (MainPage_onRecPress), NULL); 
    gtk_box_pack_start(GTK_BOX (window), btn_recogntion, TRUE, TRUE,0);
    
    btn_settings = gtk_button_new_with_label ("Settings");  
    gtk_signal_connect (GTK_OBJECT (btn_settings), "clicked",
                        GTK_SIGNAL_FUNC (MainPage_onSettingsPress), NULL);
    gtk_box_pack_start(GTK_BOX (window), btn_settings, TRUE, TRUE,0);
    
    btn_test = gtk_button_new_with_label ("Test"); 
    gtk_signal_connect (GTK_OBJECT (btn_test), "clicked",
                        GTK_SIGNAL_FUNC (MainPage_onTestPress), NULL);
    gtk_box_pack_start(GTK_BOX (window), btn_test, TRUE, TRUE,0);
        
}



// Display
void MainPageGTK::display()
{
    // Draw the title
    gtk_widget_show (label);
    
    // Draw the buttons
    gtk_widget_show (btn_newFace);
    gtk_widget_show (btn_newRoom);
    gtk_widget_show (btn_recogntion);
    gtk_widget_show (btn_settings);
    gtk_widget_show (btn_test); 
}