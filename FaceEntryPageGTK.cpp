//
//  FaceEntryPage.h
//  Pimp
//
//  Created by Michelle Alexander on 11/1/11.
//  Copyright 2011 ??? . All rights reserved.
//

#include "FaceEntryPage.h"
#include "MainPage.h"
#ifdef __APPLE__
#include <GL/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <iostream>

using namespace std;

FaceEntryPage::FaceEntryPage()
{
    // Begin in Name Entry mode
    currentMode = NAME_ENTRY;
    
    // Initialize the buttons
    btn_back = new BaseButton("Back", 75, WINDOW_HEIGHT-50,100,50);
    btn_next = new BaseButton("Next", WINDOW_WIDTH-75, WINDOW_HEIGHT-50,100,50);
    btn_next->setDisabled(true);
    btn_new = new BaseButton("More", WINDOW_WIDTH/2, WINDOW_HEIGHT - 50,100,50);
    btn_add = new BaseButton("Done", WINDOW_WIDTH-75, WINDOW_HEIGHT-50,100,50);
    dialog_name = new BaseDialog("Name: ", "", WINDOW_WIDTH/2, WINDOW_HEIGHT-150, 100);
    alphaKeys = new BaseKeyboard(false);//Do not include a close button
    alphaKeys->setActive(true);
    
    // Initialize the vector of face textures
    face.clear();
    int faceImgDim = 80;
    int xpos = faceImgDim/2;
    int ypos = faceImgDim/2;
    for(int i = 0; i < NUM_FACES; i++)
    {
        if(i == 5)
        {
            ypos += faceImgDim;
            xpos = faceImgDim/2;
        }
        face.push_back(new FaceTexture(xpos, ypos,faceImgDim,faceImgDim));
        xpos +=faceImgDim;
    }
    
    // Turn on the camera
    Pimp::sharedPimp().videoOn();
}

// Goes back to main page
void FaceEntryPage::onBackPress()
{
    if(currentMode == NAME_ENTRY)
    {
        Pimp::sharedPimp().setDisplayPage(new MainPage());
        cleanUp();
        Pimp::sharedPimp().videoOff();
    }
    else if(currentMode == FACE_ACCEPTANCE)
    {
        currentMode = NAME_ENTRY;
        glutPostRedisplay();
    }
}

// Add Faces and return to main page
void FaceEntryPage::onAddPress()
{
    // Add Faces
    for(int i = 0; i < NUM_FACES; i++)
    {
        face[i]->setName(dialog_name->getValue());
        Pimp::sharedPimp().addFace(face[i]);
    }
    
    // Return to main page
    Pimp::sharedPimp().setDisplayPage(new MainPage());
    cleanUp();
    Pimp::sharedPimp().videoOff();
    
    // Update known faces
    Pimp::sharedPimp().updateEigenFaces();
}

// Progress to the next mode
void FaceEntryPage::onNextPress()
{
    currentMode = FACE_RETRIEVAL;
    glutPostRedisplay();
    // Capture faces
    Pimp::sharedPimp().getNewFaceTextures(face);
    currentMode = FACE_ACCEPTANCE;
    glutPostRedisplay();
}

// Get new image
void FaceEntryPage::onNewPress()
{
    // Create a new faceTexture vector of invalid faces
    vector<FaceTexture*> invalidFaces;
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
    glutPostRedisplay();
}

// Handles mouse events
void FaceEntryPage::mouse(int button, int state, int x, int y)
{
    // Check for button/dialog/keyboard presses
    if(currentMode == NAME_ENTRY)
    {
        if(btn_next->mouse(button, state, x, y))
            onNextPress();
        else if(dialog_name->mouse(button,state, x, y))
            alphaKeys->setActive(true);
        else if(alphaKeys->mouse(button, state,x, y))
            onVirtualKeyboard();
        
        if(btn_back->mouse(button, state, x, y))
            onBackPress();
    }
    else if(currentMode == FACE_ACCEPTANCE)
    {
        if(btn_add->mouse(button, state, x, y))
            onAddPress();
        else if(btn_new->mouse(button, state, x, y))
            onNewPress();
        else
        {
            for(int i = 0; i < NUM_FACES; i++)
            {
                if(face[i]->mouse(button, state, x, y))
                {
                    face[i]->setInvalid(!face[i]->isInvalid());
                    break;
                }
            }
        }
        
        if(btn_back->mouse(button, state, x, y))
            onBackPress();
    }
    
}

// Handles Keyboard events
void FaceEntryPage::keyboard(unsigned char key, int x, int y)
{
    if(currentMode == NAME_ENTRY)
        dialog_name->keyboard(key, x, y);
}

void FaceEntryPage::onVirtualKeyboard()
{
    if(currentMode == NAME_ENTRY)
        dialog_name->addChar(alphaKeys->getActiveKey());
}

// Display
void FaceEntryPage::display()
{
    glPushMatrix();
    
    
    
    if(currentMode == NAME_ENTRY)
    {
        title = "Please Enter Your Name";
        // Draw Btns and Dialogs
        btn_back->draw();
        if(dialog_name->getValue() != "")
            btn_next->setDisabled(false);
        btn_next->draw();
        dialog_name->draw();
    
        // Keyboard
        alphaKeys->draw();
    }
    else if(currentMode == FACE_RETRIEVAL)
    {
        title = "Recording...";
    }
    else if(currentMode == FACE_ACCEPTANCE)
    {
        title = "Please Select any Image that is NOT you";
        btn_back->draw();
        btn_add->draw();
        btn_new->draw();
        
        // Draw the face
        for(int i = 0; i < NUM_FACES; i++)
            face[i]->draw();
    }
    
    
    // Draw the title
    glColor3f(0, 0, .4);
    int labelW = displayStringWidth(title);
    int labelH = displayStringHeight(title);
    glRasterPos3f((WINDOW_WIDTH/2 - labelW/2), (WINDOW_HEIGHT -10 - labelH/2), 0.5);  
    displayString(title);
    
    glFlush();
    glPopMatrix();
    
}

void FaceEntryPage::cleanUp()
{
    //delete[] face;
    face.clear();
}