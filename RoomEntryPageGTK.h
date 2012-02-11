//
//  RoomEntryPage.h
//  Pimp
//
//  Created by Michelle Alexander on 11/11/11.
//  Copyright 2011 ??? . All rights reserved.
//

#ifndef RoomEntryPage_h
#define RoomEntryPage_h

#include "BasePage.h"
#include "BaseButton.h"
#include "BaseDialog.h"
#include "RoomTexture.h"
#include "Pimp.h"
#include <string>

class RoomEntryPage : public BasePage
{
private:
    // Title
    std::string title;
    
    // Buttons
    BaseButton *btn_back;
    BaseButton *btn_add;
    BaseButton *btn_new;
    
    // Dialog
    BaseDialog *dialog_name;
    
    // Properties
    std::string *name;
    
    // Callbacks
    void onAddPress();
    void onBackPress();
    void onNewPress();
    
    // Textures
    RoomTexture *room;
    
public:
    RoomEntryPage();
    
    // User input
    void mouse(int button, int state, int x, int y);
    void keyboard(unsigned char key, int x, int y);
    
    // Display
    void display();
};

#endif