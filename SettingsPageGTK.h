//
//  SettingsPageGTK.h
//  Pimp
//
//  Created by Michelle Alexander on 12/30/11.
//  Copyright 2011 ??? . All rights reserved.
//

#ifndef SettingsPageGTK_h
#define SettingsPageGTK_h

#include "BasePageGTK.h"
#include <string>
#include "Pimp.h"

class SettingsPageGTK : public BasePageGTK
{
private:
    // Title
    GtkWidget *label;
    
    // Buttons
    GtkWidget *btn_back;
    GtkWidget *toggle_face;
    GtkWidget *toggle_room;
    
    // Sliders
    GtkWidget *slider_face;
    GtkWidget *slider_room;
    
    // Actions
    void saveSettings();
    
public:
    SettingsPageGTK();
        
    // Display
    void display();
};

#endif