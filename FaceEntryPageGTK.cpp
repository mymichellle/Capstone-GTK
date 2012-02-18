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

	void FaceEntryPage_onBackPage(GtkWidget *btn, GtkWidget *label)
	{
		// Go back to INIT_ROOM mode pass name from label
		Pimp::sharedPimp().setDisplayPage(new FaceEntryPageGTK(NAME_ENTRY, gtk_label_get_text(GTK_LABEL(label))));
	}

	void FaceEntryPage_onGetImg(GtkWidget *btn, GtkWidget *dialog)
	{	
		Pimp::sharedPimp().setDisplayPage(new FaceEntryPageGTK(FACE_ACCEPTANCE, gtk_entry_get_text((GtkEntry*)dialog)));
	}

	void FaceEntryPage_onNewImg(GtkWidget *btn, GtkWidget *label)
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

	// Get new image
	void FaceEntryPage_onNewPress()
	{
		// Create a new faceTexture vector of invalid faces
	/*	vector<FaceTexture*> invalidFaces;
		int numInvalid = 0;
		for(int i = 0; i < NUM_FACES; i++)
		{
		    if(face[i]->isInvalid())
		    {
		        face[i]->setInvalid(false);
		        invalidFaces.push_back(face[i]);
		    }
		}
		
		// Get more face textures to replace invalid ones
		Pimp::sharedPimp().getNewFaceTextures(invalidFaces);
		
		int sizeNewFaces = invalidFaces.size();
		int newFacePos = 0;
		
		// Replace the invalid faces with the new ones
		for(int i = 0; i < numInvalid; i++)
		{
		    if(face[i]->isInvalid() && newFacePos<sizeNewFaces)
		    {
		        cout<<"Replacing and setting valid"<<endl;
		        face[i] = invalidFaces[newFacePos];
		        face[i]->setInvalid(false);
		        newFacePos++;
		    }
		}
		
		invalidFaces.clear();
		// Redraw
		glutPostRedisplay();*/
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

    if(mode == NAME_ENTRY)
	{
		// label - "Room Entry"
		title = gtk_label_new("Face Entry");
		gtk_box_pack_start(GTK_BOX (window), title, TRUE, TRUE, 0);

		// hBox
		GtkWidget *hbox = gtk_hbox_new (TRUE, 1);
		gtk_box_pack_start( GTK_BOX(window), hbox, TRUE, TRUE, 0);

		// Back - RoomEntryPage_onBackExit
		btn_back = gtk_button_new_with_label ("Back");
		gtk_signal_connect (GTK_OBJECT (btn_back), "clicked",
		                    GTK_SIGNAL_FUNC (FaceEntryPage_onBackExit), NULL);
		gtk_box_pack_start( GTK_BOX(hbox), btn_back, TRUE, TRUE, 0);

		dialog_name = gtk_entry_new();

		// TakeImg - RoomEntryPage_onGetImg (Dialog)
		btn_next = gtk_button_new_with_label("Get Image");
		gtk_signal_connect (GTK_OBJECT (btn_next), "clicked",
							GTK_SIGNAL_FUNC (FaceEntryPage_onGetImg), dialog_name);
		gtk_box_pack_start( GTK_BOX(hbox), btn_next, TRUE, TRUE, 0);
		
		// Dialog - rName
		gtk_entry_set_text((GtkEntry*)dialog_name, (char*)rName.c_str());
		gtk_box_pack_start( GTK_BOX(window), dialog_name, TRUE, TRUE, 0);
		
		// Keyboard - RoomEntryPage_keyType (dialog)
		keyboard = keyboardGTK_new();
		gtk_signal_connect (GTK_OBJECT (keyboard), "keyboardGTK",
							GTK_SIGNAL_FUNC (FaceEntryPage_keyType), dialog_name);
		gtk_box_pack_start(GTK_BOX(window), keyboard, TRUE,TRUE,0);
	}
	else if(mode == FACE_ACCEPTANCE)
	{
		// Turn on the camera
    	Pimp::sharedPimp().videoOn();

		// label - rName
		title = gtk_label_new((char*)rName.c_str());
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

		// NewImg - RoomEntryPage_onNewImg (label)
		btn_new = gtk_button_new_with_label("New Image");
		gtk_signal_connect (GTK_OBJECT (btn_new), "clicked",
							GTK_SIGNAL_FUNC (FaceEntryPage_onNewImg), title);
		gtk_box_pack_start( GTK_BOX(hbox), btn_new, TRUE, TRUE, 0);

		// Add - RoomEntryPage_onAdd (img)
		btn_add = gtk_button_new_with_label("Add");
		gtk_signal_connect (GTK_OBJECT (btn_add), "clicked",
							GTK_SIGNAL_FUNC (FaceEntryPage_onAdd), &face);
		gtk_box_pack_start( GTK_BOX(hbox), btn_add, TRUE, TRUE, 0);
		
		// Get the face textures
		Pimp::sharedPimp().getNewFaceTextures(face);
	}   
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
