//
//  RoomEntryPage.cpp
//  Pimp
//
//  Created by Michelle Alexander on 11/11/11.
//  Copyright 2011 ??? . All rights reserved.
//

#include "RoomEntryPage.h"
#include "MainPage.h"
#ifdef __APPLE__
#include <GL/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <iostream>

RoomEntryPage::RoomEntryPage()
{
    btn_back = new BaseButton("Back", 100, 30);
    btn_add = new BaseButton("Add", WINDOW_WIDTH/2, WINDOW_HEIGHT-250);
    btn_new = new BaseButton("New Image", WINDOW_WIDTH/2, WINDOW_HEIGHT - 50);
    dialog_name = new BaseDialog("Name: ", "Unknown", WINDOW_WIDTH/2, WINDOW_HEIGHT/4, 100);
    
    room = new RoomTexture();
    
    Pimp::sharedPimp().videoOn();
    Pimp::sharedPimp().getNewTexture(room);
    title = "Add A Room";
}

// Goes back to main page
void RoomEntryPage::onBackPress()
{
    Pimp::sharedPimp().setDisplayPage(new MainPage());
    Pimp::sharedPimp().videoOff();
}

// Add Face
void RoomEntryPage::onAddPress()
{    
    room->setName(dialog_name->getValue());
    Pimp::sharedPimp().addRoom(room);
}

// Get new image
void RoomEntryPage::onNewPress()
{
    Pimp::sharedPimp().getNewTexture(room);
}

// Handles mouse events
void RoomEntryPage::mouse(int button, int state, int x, int y)
{
    // Check if the new or load buttons were pressed
    if(btn_back->mouse(button, state, x, y))
        onBackPress();
    if(btn_add->mouse(button, state, x, y))
        onAddPress();
    if(btn_new->mouse(button, state, x, y))
        onNewPress();
    
    dialog_name->mouse(button,state, x, y);
}

// Handles Keyboard events
void RoomEntryPage::keyboard(unsigned char key, int x, int y)
{
    dialog_name->keyboard(key, x, y);
}

// Display
void RoomEntryPage::display()
{
    glPushMatrix();
    
    // Draw the title
    glColor3f(0, 0, .4);
    int labelW = displayStringWidth(title);
    int labelH = displayStringHeight(title);
    glRasterPos3f((WINDOW_WIDTH/2 - labelW/2), (WINDOW_HEIGHT -10 - labelH/2), 0.5);  
    displayString(title);
    
    // Draw the buttons and dialogs
    btn_back->draw();
    btn_add->draw();
    btn_new->draw();
    dialog_name->draw();
    
    // Draw the room image
    glPushMatrix();
    glTranslatef(WINDOW_WIDTH/2, WINDOW_HEIGHT/2+50, 0);
    room->draw();
    glPopMatrix();
    
    
    glFlush();
    glPopMatrix();
    
}