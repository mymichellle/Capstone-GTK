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

	void SettingsPage_onRoomCheck(GtkWidget *chkBox)
	{
		Pimp::sharedPimp().setRoomRec(gtk_toggle_button_get_active((GtkToggleButton*)chkBox));
	}

	void SettingsPage_onFaceCheck(GtkWidget *chkBox)
	{
		Pimp::sharedPimp().setFaceRec(gtk_toggle_button_get_active((GtkToggleButton*)chkBox));
	}

	void SettingsPage_onRoomSlider(GtkWidget *range)
	{
		//Pimp::sharedPimp().setRoomRecRate(gtk_range_get_value((GtkRange*)range));
		Pimp::sharedPimp().setRoomAudioRate(gtk_range_get_value((GtkRange*)range));
	}

	void SettingsPage_onFaceSlider(GtkWidget *range)
	{
		//Pimp::sharedPimp().setFaceRecRate(gtk_range_get_value((GtkRange*)range));
		Pimp::sharedPimp().setFaceAudioRate(gtk_range_get_value((GtkRange*)range));
	}
}

SettingsPageGTK::SettingsPageGTK()
{
    // Window box to contain this page
    window = gtk_vbox_new (TRUE,1);
    
    // Title
    label = gtk_label_new("Settings");
    gtk_box_pack_start(GTK_BOX (window), label, TRUE, TRUE, 0);
    
	// hBox
	GtkWidget *hbox = gtk_hbox_new (TRUE, 1);
	gtk_box_pack_start( GTK_BOX(window), hbox, TRUE, TRUE, 0);

	toggle_face = gtk_check_button_new_with_label("Face Recognition");
	gtk_signal_connect (GTK_OBJECT (toggle_face), "toggled",
	                    GTK_SIGNAL_FUNC (SettingsPage_onFaceCheck), NULL);
	gtk_box_pack_start( GTK_BOX(hbox), toggle_face, TRUE, TRUE, 0);
	slider_face = gtk_hscale_new_with_range (0,10,1);
	gtk_signal_connect (GTK_OBJECT (slider_face), "value-changed",
	                    GTK_SIGNAL_FUNC (SettingsPage_onFaceSlider), NULL);
	gtk_box_pack_start( GTK_BOX(hbox), slider_face, TRUE, TRUE, 0);

	//hbox
	hbox = gtk_hbox_new (TRUE, 1);
	gtk_box_pack_start( GTK_BOX(window), hbox, TRUE, TRUE, 0);

	toggle_room = gtk_check_button_new_with_label("Room Recognition");
	gtk_signal_connect (GTK_OBJECT (toggle_room), "toggled",
	                    GTK_SIGNAL_FUNC (SettingsPage_onRoomCheck), NULL);
	gtk_box_pack_start( GTK_BOX(hbox), toggle_room, TRUE, TRUE, 0);
	slider_room = gtk_hscale_new_with_range (0,10,1);
	gtk_signal_connect (GTK_OBJECT (slider_room), "value-changed",
	                    GTK_SIGNAL_FUNC (SettingsPage_onRoomSlider), NULL);

	gtk_box_pack_start( GTK_BOX(hbox), slider_room, TRUE, TRUE, 0);

    // Buttons
    btn_back = gtk_button_new_with_label ("Back");
    gtk_signal_connect (GTK_OBJECT (btn_back), "clicked",
                        GTK_SIGNAL_FUNC (SettingsPageGTK_onBackPress), NULL);
    gtk_box_pack_start(GTK_BOX (window), btn_back, TRUE, TRUE,0);
    
    // Get the current app settings
    if(Pimp::sharedPimp().isFaceRecognition())
        gtk_toggle_button_set_active((GtkToggleButton*)toggle_face,true);
    else
       	gtk_toggle_button_set_active((GtkToggleButton*)toggle_face,false);
    
    if(Pimp::sharedPimp().isRoomRecognition())
        gtk_toggle_button_set_active((GtkToggleButton*)toggle_room,true);
    else
        gtk_toggle_button_set_active((GtkToggleButton*)toggle_room,false);

	gtk_range_set_value((GtkRange*)slider_face, Pimp::sharedPimp().getFaceRecognitionRate());
	gtk_range_set_value((GtkRange*)slider_room, Pimp::sharedPimp().getRoomRecognitionRate());
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
