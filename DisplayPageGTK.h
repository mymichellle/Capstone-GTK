//
//  DisplayPageGTK.h
//  Pimp
//
//  Created by Michelle Alexander on 12/30/11.
//  Copyright 2011 ??? . All rights reserved.
//

#ifndef DisplayPageGTK_h
#define DisplayPageGTK_h

#include "BasePageGTK.h"
#include "Pimp.h"

class DisplayPageGTK : public BasePageGTK
{
private:
    // Title
    GtkWidget *title;
    GtkWidget *personTitle;
    GtkWidget *roomTitle;
    
    // Display Text
    GtkWidget *personName;
    GtkWidget *roomName;

    // Buttons
    GtkWidget *btn_back;

	// timeout handler id
	guint timeout_handler_id;
 
	void runLoop();       
public:
    DisplayPageGTK();
    
    // Display
    void display();

	// Getters && Setters
	void setPersonName(std::string name);
	void setRoomName(std::string name);
	void redrawPage();
	bool isActive();
};

#endif
