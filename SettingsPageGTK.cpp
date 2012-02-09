//
//  SettingsPageGTK.cpp
//  Pimp
//
//  Created by Michelle Alexander on 12/30/11.
//  Copyright 2011 ??? . All rights reserved.
//

#include "SettingsPageGTK.h"
#include "MainPageGTK.h"

using namespace std;

extern "C"{
    
    void SettingsPageGTK_onBackPress()
    {
        //saveSettings();
        Pimp::sharedPimp().setDisplayPage(new MainPageGTK());
    }
}

SettingsPageGTK::SettingsPageGTK()
{
    // Window box to contain this page
    window = gtk_vbox_new (TRUE,1);
    
    // Title
    label = gtk_label_new("Settings");
    gtk_box_pack_start(GTK_BOX (window), label, TRUE, TRUE, 0);
    
    // Buttons
    btn_back = gtk_button_new_with_label ("Back");
    gtk_signal_connect (GTK_OBJECT (btn_back), "clicked",
                        GTK_SIGNAL_FUNC (SettingsPageGTK_onBackPress), NULL);
    gtk_box_pack_start(GTK_BOX (window), btn_back, TRUE, TRUE,0);
    
    //toggle_face = new ToggleButton("Face Recognition", WINDOW_WIDTH/2, WINDOW_HEIGHT-50);
    //toggle_room = new ToggleButton("Room Recognition", WINDOW_WIDTH/2, WINDOW_HEIGHT - 150);
    
    // Sliders
    //slider_face = new BaseSlider("Face Rate", WINDOW_WIDTH/2, WINDOW_HEIGHT-100, 1, 30);
    //slider_room = new BaseSlider("Room Rate", WINDOW_WIDTH/2, WINDOW_HEIGHT-200, 1, 30);
    
    // Get the current app settings
   /* if(Pimp::sharedPimp().isFaceRecognition())
        toggle_face->setSelected(true);
    else
        toggle_face->setSelected(false);
    
    if(Pimp::sharedPimp().isRoomRecognition())
        toggle_room->setSelected(true);
    else
        toggle_room->setSelected(false);

    slider_face->setValue(Pimp::sharedPimp().getFaceRecognitionRate());
    slider_room->setValue(Pimp::sharedPimp().getRoomRecognitionRate());*/
}


void SettingsPageGTK::saveSettings()
{
   /* bool faceRecOn = toggle_face->getSelected();
    bool roomRecOn = toggle_room->getSelected();
    int faceRecRate = slider_face->getValue();
    int roomRecRate = slider_room->getValue();
    Pimp::sharedPimp().setMode(faceRecOn, faceRecRate, roomRecOn, roomRecRate);*/
}


void SettingsPageGTK::display()
{
    // Draw the titles
    gtk_widget_show (label);
    
    // Draw the buttons and sliders
    gtk_widget_show (btn_back);
    
}