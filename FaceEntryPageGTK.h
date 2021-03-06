//
//  FaceEntryPage.h
//  Pimp
//
//  Created by Michelle Alexander on 11/1/11.
//  Copyright 2011 ??? . All rights reserved.
//

#ifndef FaceEntryPageGTK_h
#define FaceEntryPageGTK_h

#include "FaceTextureGTK.h"
#include "Pimp.h"
#include <string>
#include <gtk/gtk.h>

#define NUM_FACES 10

enum FaceEntryMode{
        NAME_ENTRY,
		NAME_RECORD,
        FACE_RETRIEVAL,
        FACE_ACCEPTANCE
    };

class FaceEntryPageGTK : public BasePageGTK
{
private:    
	// Initialize function
	void initPage(enum FaceEntryMode mode, std::string rName);

    // Title
    GtkWidget *title;
    
    // Buttons
    GtkWidget *btn_back;
    GtkWidget *btn_next;
	GtkWidget *btn_play;
	GtkWidget *btn_record;
    
    // Dialog
    GtkWidget *dialog_name;
    
    // Keyboard
    GtkWidget *keyboard;
    
    // Properties
    std::string name;
        
    // Face Textures
    std::vector<FaceTextureGTK*> face;
	guint delayFunc;
    
    void cleanUp();
    
public:
	FaceEntryPageGTK();
    FaceEntryPageGTK(enum FaceEntryMode mode, std::string rName);
    
    // Display
    void display();
	void updateFaceDisplay();

};

#endif
