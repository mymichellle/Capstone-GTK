//
//  FaceEntryPage.h
//  Pimp
//
//  Created by Michelle Alexander on 11/1/11.
//  Copyright 2011 ??? . All rights reserved.
//

#ifndef FaceEntryPage_h
#define FaceEntryPage_h

#include "BasePage.h"
#include "BaseButton.h"
#include "BaseDialog.h"
#include "BaseKeyboard.h"
#include "FaceTexture.h"
#include "Pimp.h"
#include <string>
#ifdef __APPLE__
#include <GL/glut.h>
#else
#include <GL/freeglut.h>
#endif

#define NUM_FACES 10

class FaceEntryPage : public BasePage
{
private:
    // States
    enum Mode{
        NAME_ENTRY,
        FACE_RETRIEVAL,
        FACE_ACCEPTANCE
    };
    enum Mode currentMode;
    
    // Title
    std::string title;
    
    // Buttons
    BaseButton *btn_back;
    BaseButton *btn_add;
    BaseButton *btn_new;
    BaseButton *btn_next;
    
    // Dialog
    BaseDialog *dialog_name;
    
    // Keyboard
    BaseKeyboard *alphaKeys;
    
    // Properties
    std::string *name;
    
    // Callbacks
    void onAddPress();
    void onNextPress();
    void onBackPress();
    void onNewPress();
    void onVirtualKeyboard();
    
    // Face Textures
    //FaceTexture* face[NUM_FACES];
    std::vector<FaceTexture*> face;
    
    void cleanUp();
    
public:
    FaceEntryPage();
    
    // User input
    void mouse(int button, int state, int x, int y);
    void keyboard(unsigned char key, int x, int y);
    
    // Display
    void display();
};

#endif