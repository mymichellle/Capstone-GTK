//
//  FaceEntryPage.h
//  Pimp
//
//  Created by Michelle Alexander on 11/1/11.
//  Copyright 2011 ??? . All rights reserved.
//

#include "FaceEntryPageGTK.h"
#include "MainPageGTK.h"
#include "KeyboardGTK.h"
#include <iostream>

using namespace std;

extern "C"{
	// Goes back to main page
	void FaceEntryPage_onBackExit()
	{
		    Pimp::sharedPimp().setDisplayPage(new MainPageGTK());
		    //cleanUp();
		    Pimp::sharedPimp().videoOff();
	}

    void FaceEntryPage_onRecordEntry(GtkWidget *btn, GtkWidget *label)
    {
		string fileName = gtk_label_get_text(GTK_LABEL(label));
    	transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
		fileName = fileName+".dat";
        // Record Audio Clip for Current Face
        Pimp::sharedPimp().recordSound((char*)fileName.c_str());
    }

    void FaceEntryPage_onPlayEntry(GtkWidget *btn, GtkWidget *label)
    {
		string fileName = gtk_label_get_text(GTK_LABEL(label));
    	transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
		fileName = fileName+".dat";
        // Playback Current Audio Recording for Current Face
        Pimp::sharedPimp().playbackSound((char*)fileName.c_str());
    }

	void FaceEntryPage_onBackEntry(GtkWidget *btn, GtkWidget *label)
	{
		// Go back to INIT_ROOM mode pass name from label
		Pimp::sharedPimp().setDisplayPage(new FaceEntryPageGTK(NAME_ENTRY, gtk_label_get_text(GTK_LABEL(label))));
		    Pimp::sharedPimp().videoOff();
	}

	void FaceEntryPage_onBackPage(GtkWidget *btn, GtkWidget *label)
	{
		// Go back to INIT_ROOM mode pass name from label
		Pimp::sharedPimp().setDisplayPage(new FaceEntryPageGTK(NAME_RECORD, gtk_label_get_text(GTK_LABEL(label))));
		    Pimp::sharedPimp().videoOff();
	}

	void FaceEntryPage_onNextRecord(GtkWidget *btn, GtkWidget *dialog)
	{	
		Pimp::sharedPimp().setDisplayPage(new FaceEntryPageGTK(NAME_RECORD, gtk_entry_get_text((GtkEntry*)dialog)));
	}

	void FaceEntryPage_onGetImg(GtkWidget *btn, GtkWidget *label)
	{	
		Pimp::sharedPimp().setDisplayPage(new FaceEntryPageGTK(FACE_ACCEPTANCE, gtk_label_get_text(GTK_LABEL(label))));
	}

	void FaceEntryPage_onAdd(GtkWidget *btn, vector<FaceTextureGTK*> facetext)
	{
		// Add the images to known faces	
		for(int i = 0; i < NUM_FACES; i++)
		{
		    Pimp::sharedPimp().addFace(facetext[i]);
		}

		// turn off camera
		Pimp::sharedPimp().videoOff();
		// Exit to Main Page
		Pimp::sharedPimp().setDisplayPage(new MainPageGTK());		
	
		// Update known faces
		Pimp::sharedPimp().updateEigenFaces();
	}

	// Handles Keyboard events
	void FaceEntryPage_keyType(GtkWidget *keyboard, GtkWidget *dialog)
	{
		gtk_entry_append_text((GtkEntry*)dialog, ((KeyboardGTK*)keyboard)->activeKey);
	}

	void FaceEntryPage_keyDelete(GtkWidget *keyboard, GtkWidget *dialog)
	{
		int len = gtk_entry_get_text_length((GtkEntry*)dialog);
		if(len >= 1)
		{
			string text = gtk_entry_get_text((GtkEntry*)dialog);
	        text.erase(len-1);
			gtk_entry_set_text((GtkEntry*)dialog,(char*)text.c_str());
		}
	}

	void FaceEntry_updateDisplay(FaceEntryPageGTK* entryPage)
	{
		entryPage->updateFaceDisplay();
	}
}

FaceEntryPageGTK::FaceEntryPageGTK()
{
	initPage(NAME_ENTRY, "");
}

FaceEntryPageGTK::FaceEntryPageGTK(enum FaceEntryMode mode, std::string rName)
{
	initPage(mode, rName);
}

void FaceEntryPageGTK::initPage(enum FaceEntryMode mode, std::string rName)
{
    // Window box to contain this page
    window = gtk_vbox_new (FALSE,1); 
	name = rName;

    if(mode == NAME_ENTRY)
	{
		// label - "Room Entry"
		title = gtk_label_new("Face Entry");
		gtk_box_pack_start(GTK_BOX (window), title, TRUE, TRUE, 0);

		dialog_name = gtk_entry_new();
		
		// Dialog - rName
		gtk_entry_set_text((GtkEntry*)dialog_name, (char*)rName.c_str());
		gtk_box_pack_start( GTK_BOX(window), dialog_name, TRUE, TRUE, 0);
		
		// Keyboard - RoomEntryPage_keyType (dialog)
		GtkWidget *keyWindow = gtk_hbox_new (FALSE, 1);
		gtk_box_pack_start(GTK_BOX(window), keyWindow, TRUE,TRUE,0);
    	GtkWidget *separator = gtk_vseparator_new();
		keyboard = keyboardGTK_new();
		gtk_signal_connect (GTK_OBJECT (keyboard), "keyboardGTK",
							GTK_SIGNAL_FUNC (FaceEntryPage_keyType), dialog_name);
		gtk_signal_connect (GTK_OBJECT (keyboard), "keyboardGTK_delete",
							GTK_SIGNAL_FUNC (FaceEntryPage_keyDelete), dialog_name);
		gtk_box_pack_start(GTK_BOX(keyWindow), separator, FALSE,TRUE,15);
		gtk_box_pack_start(GTK_BOX(keyWindow), keyboard, TRUE,TRUE,0);


		// hBox
		GtkWidget *hbox = gtk_hbox_new (TRUE, 1);
		gtk_box_pack_start( GTK_BOX(window), hbox, TRUE, TRUE, 0);

		// Back - RoomEntryPage_onBackExit
		btn_back = gtk_button_new_with_label ("Back");
		gtk_signal_connect (GTK_OBJECT (btn_back), "clicked",
		                    GTK_SIGNAL_FUNC (FaceEntryPage_onBackExit), NULL);
		gtk_box_pack_start( GTK_BOX(hbox), btn_back, TRUE, TRUE, 0);


		// TakeImg - RoomEntryPage_onGetImg (Dialog)
		btn_next = gtk_button_new_with_label("Next");
		gtk_signal_connect (GTK_OBJECT (btn_next), "clicked",
							GTK_SIGNAL_FUNC (FaceEntryPage_onNextRecord), dialog_name);
		gtk_box_pack_start( GTK_BOX(hbox), btn_next, TRUE, TRUE, 0);
	}
	else if(mode == NAME_RECORD)
	{
		// label - rName
		title = gtk_label_new((char*)rName.c_str());
		gtk_box_pack_start(GTK_BOX (window), title, TRUE, TRUE, 0);

		btn_record = gtk_button_new_with_label ("RECORD");
        gtk_signal_connect (GTK_OBJECT (btn_record), "clicked",
		                    GTK_SIGNAL_FUNC (FaceEntryPage_onRecordEntry), title);
		gtk_box_pack_start( GTK_BOX(window), btn_record, TRUE, TRUE, 0);

		btn_play = gtk_button_new_with_label ("PLAY");
        gtk_signal_connect (GTK_OBJECT (btn_play), "clicked",
		                    GTK_SIGNAL_FUNC (FaceEntryPage_onPlayEntry), title);
		gtk_box_pack_start( GTK_BOX(window), btn_play, TRUE, TRUE, 0);

		// hBox
		GtkWidget *hbox = gtk_hbox_new (TRUE, 1);
		gtk_box_pack_start( GTK_BOX(window), hbox, TRUE, TRUE, 0);

		// Back - RoomEntryPage_onBackExit
		btn_back = gtk_button_new_with_label ("Back");
		gtk_signal_connect (GTK_OBJECT (btn_back), "clicked",
		                    GTK_SIGNAL_FUNC (FaceEntryPage_onBackEntry), title);
		gtk_box_pack_start( GTK_BOX(hbox), btn_back, TRUE, TRUE, 0);


		// TakeImg - RoomEntryPage_onGetImg (Dialog)
		btn_next = gtk_button_new_with_label("Next");
		gtk_signal_connect (GTK_OBJECT (btn_next), "clicked",
							GTK_SIGNAL_FUNC (FaceEntryPage_onGetImg), title);
		gtk_box_pack_start( GTK_BOX(hbox), btn_next, TRUE, TRUE, 0);
	}
	else if(mode == FACE_ACCEPTANCE)
	{
		// Turn on the camera
    	//Pimp::sharedPimp().videoOn();
		string procLabel = rName + " Processing ...";
		// label - rName
		title = gtk_label_new((char*)procLabel.c_str());
		gtk_box_pack_start(GTK_BOX (window), title, TRUE, TRUE, 0);

		// hBox
		GtkWidget *hbox = gtk_hbox_new (TRUE, 1);
		gtk_box_pack_start( GTK_BOX(window), hbox, TRUE, TRUE, 0);

		// Initialize the vector of face textures
		face.clear();
		int faceImgDim = 60;
		int xpos = faceImgDim/2;
		int ypos = faceImgDim/2;
		for(int i = 0; i < NUM_FACES; i++){
			if(i == NUM_FACES/2)
			{
				hbox = gtk_hbox_new (TRUE, 1);
				gtk_box_pack_start( GTK_BOX(window), hbox, TRUE, TRUE, 0);
			}
			//faceBtn.push_back(gtk_button_new());
		    face.push_back(new FaceTextureGTK(hbox, faceImgDim));
			face[i]->setName(rName);
		}

		hbox = gtk_hbox_new (TRUE, 1);
		gtk_box_pack_start( GTK_BOX(window), hbox, TRUE, TRUE, 0);

		// Back - RoomEntryPage_onBackPage (label)
		btn_back = gtk_button_new_with_label ("Back");
		gtk_signal_connect (GTK_OBJECT (btn_back), "clicked",
		                    GTK_SIGNAL_FUNC (FaceEntryPage_onBackPage), title);
		gtk_box_pack_start( GTK_BOX(hbox), btn_back, TRUE, TRUE, 0);

		// Add - RoomEntryPage_onAdd (img)
		btn_next = gtk_button_new_with_label("Add");
		gtk_signal_connect (GTK_OBJECT (btn_next), "clicked",
							GTK_SIGNAL_FUNC (FaceEntryPage_onAdd), &face);
		gtk_box_pack_start( GTK_BOX(hbox), btn_next, TRUE, TRUE, 0);
		
		// Get the face textures
		//Pimp::sharedPimp().getNewFaceTextures(face);

		// Create a timer
		delayFunc = gtk_idle_add_priority( G_PRIORITY_LOW, (GSourceFunc) FaceEntry_updateDisplay, (gpointer) this);
 
		//Pimp::sharedPimp().videoOff();
	}   
}

void FaceEntryPageGTK::updateFaceDisplay()
{
	Pimp::sharedPimp().videoOn();
	Pimp::sharedPimp().getNewFaceTextures(face);
	Pimp::sharedPimp().videoOff();
	gtk_label_set_text(GTK_LABEL(title), (gchar*)name.c_str());
	gtk_idle_remove(delayFunc);
}

// Display
void FaceEntryPageGTK::display()
{
       
}

void FaceEntryPageGTK::cleanUp()
{
    //delete[] face;
    face.clear();
}
