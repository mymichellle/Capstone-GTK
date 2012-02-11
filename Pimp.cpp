//
//  Pimp.cpp
//  PiMP
//
//  Created by Michelle Alexander on 10/27/11.
//  Copyright 2011 ??? . All rights reserved.
//


#include "Pimp.h"
#include "Utility.h"

#include <cstdio> // FILENAME_MAX defined here
#include <cerrno> // errno defined here
#include <iostream>
#include <cassert>
#include <unistd.h>
#include <ctime>
#include <sys/stat.h>

const char * WINDOW_NAME = "PiMP";
const char * FACE_WINDOW_NAME = "PiMP - Faces";
const char * SURF_WINDOW_NAME = "PiMP - SURF";

using namespace std;

extern "C"{
    void pimp_destroy(void)
    {
        gtk_main_quit ();
    }
}

// One instance of Pimp is created when the application is run
Pimp::Pimp()
{
    // Initialize the main window
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "CAPSTONE");
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH,WINDOW_HEIGHT);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    
    // Setup the button
    gtk_signal_connect (GTK_OBJECT (window), "destroy",
                        GTK_SIGNAL_FUNC (pimp_destroy), NULL);
    gtk_container_border_width (GTK_CONTAINER (window), 10);
    
    // Initial Page
    displayPage = new BasePageGTK();
    
    // Find the Faces directory
    char cCurrentPath[FILENAME_MAX];
    if (!getcwd(cCurrentPath, sizeof(cCurrentPath)))
    {
        //return errno;
    }
    
    // Initialize Directory information
    dir = string(cCurrentPath);
    faceDir = dir +"/Faces/Known/";
    roomDir = dir +"/Rooms/";
    initialized = false;
    cycleNum = 0;
    lastRoomCycle = 0;
    lastFaceCycle = 0;
}

void Pimp::startupProcess()
{
    initialized = true;
    // Video Capture
    video = new VideoStream();
    current_frame = NULL;
    
    // Face Recognition
   // fisherfaces = new Fisherfaces(faceDir+"NewFaceSet");
    eigenfaces = new Eigenfaces(faceDir+"NewFaceSet");
    
    // Room Recognition
    roomRec = new RoomRecognition(roomDir+"PimpRooms");
    
    // State Variables
    currentMode = RECOGNITION;
    roomRate = 1;
    faceRate = 1;
    
#ifdef SHOW_VIDEO
    // Windows to show live video stream
    cvNamedWindow( WINDOW_NAME, CV_WINDOW_AUTOSIZE );
    
    // Font to be used in live video stream
    cvInitFont(&font,CV_FONT_HERSHEY_PLAIN, 1.0, 1.0, 0,1,CV_AA);
#endif
    
}

// Change the Active Page
void Pimp::setDisplayPage(BasePageGTK *newPage){
    gtk_widget_destroy(displayPage->getWindow());
    displayPage = NULL;
    displayPage = newPage;
    gtk_container_add (GTK_CONTAINER (window), displayPage->getWindow());
    draw();
    // Make sure the starup process has been run
    if(!initialized)
        startupProcess();
}

void Pimp::draw()
{
    gtk_window_resize(GTK_WINDOW(window), WINDOW_WIDTH,WINDOW_HEIGHT);
    gtk_widget_show_all (window);
}

// Turn the video stream on
void Pimp::videoOn(){
    video->startCapture();
}

// Turn the video stream off
void Pimp::videoOff(){
    video->endCapture();
}

// Update the eigenfaces
void Pimp::updateEigenFaces(){
    eigenfaces->update();
}

// Set the Recognition Mode
void Pimp::setMode(bool faceRecOn, int faceRecRate, bool roomRecOn, int roomRecRate)
{
    if(faceRecOn && roomRecOn)
    {
        currentMode = RECOGNITION;
        faceRate = faceRecRate;
        roomRate = roomRecRate;
    }
    else if(faceRecOn)
    {
        currentMode = RECOGNITION_FACE;
        faceRate = faceRecRate;
    }
    else if(roomRecOn)
    {
        currentMode = RECOGNITION_ROOM;
        roomRate = roomRecRate;
    }
    else
    {
        currentMode = TEST_MODE;
    }
}

// Get Recognition modes
bool Pimp::isFaceRecognition()
{
    if(currentMode == RECOGNITION || currentMode == RECOGNITION_FACE)
        return true;
    else
        return false;
}

bool Pimp::isRoomRecognition()
{
    if(currentMode == RECOGNITION || currentMode == RECOGNITION_ROOM)
        return true;
    else
        return false;
}

// Setup Recognition Processes
void Pimp::initProcess()
{
    cycleNum = 0;
    lastFaceCycle = 0;
    lastRoomCycle = 0;
    if( currentMode == RECOGNITION || currentMode == RECOGNITION_FACE)
        initFaceRecognition();
    if( currentMode == RECOGNITION || currentMode == RECOGNITION_ROOM)
        initRoomRecognition();
    current_frame = video->getImage();
    string test = dir + "test.jpg";
    cvSaveImage((char *)test.c_str(), current_frame);
}

// Main Processing Loop
void Pimp::mainProcess()
{
    current_frame = video->getImage();
    display_frame = cvCreateImage(cvSize (current_frame->width, current_frame->height), IPL_DEPTH_8U, 3);
    assert (current_frame && display_frame);
    
    current_frame = video->getImage();
    // set display_frame to flipped current_frame
    cvFlip (current_frame, display_frame, 1);
    /*
    // Face Recognition
    if( (cycleNum - lastFaceCycle == faceRate) && (currentMode == RECOGNITION || currentMode == RECOGNITION_FACE) )
    {
        faceRecognition();
        lastFaceCycle = cycleNum;
    }*/
    
    // Room Recognition
    if( (cycleNum - lastRoomCycle == roomRate) && (currentMode == RECOGNITION || currentMode == RECOGNITION_ROOM))
    {
        roomRecognition();
        lastRoomCycle = cycleNum;
    }
    
    
#ifdef SHOW_VIDEO
    // Draw the FPS figure
    drawFPS(display_frame);
    
    // Show the debug live video stream
    cvShowImage(WINDOW_NAME, display_frame);
    cvWaitKey(1);
#endif
    
    cycleNum++;
}

void Pimp::testProc()
{
}

// Initialize Face Recognition
void Pimp::initFaceRecognition()
{
    const int scale = 1;
    
    faces = new FaceDetect(scale);
    facePreProcess = new FacePreProcess();
    
    searchArea = cvRect(0,0,0,0);
}

// Recognize the face in the current frame
void Pimp::faceRecognition()
{
    // Find a face
    CvRect r = faces->getFace(current_frame, searchArea);
    if(r.width != 10 && r.height != 10)
    {
        // Pre-process and recognize the face
        cvSetImageROI(display_frame,r);
        processed_face = facePreProcess->preProcess(display_frame);
        personInImage = eigenfaces->recognizeFace(processed_face);
        cvResetImageROI(display_frame);
        
        if(personInImage == "0")
            searchArea = cvRect(0,0,0,0);
        else
            searchArea = r;
    }
    else
    {
        personInImage = "";
    }
#ifdef SHOW_VIDEO
    // Add the recognized text to display frame
    cvPutText(display_frame, (char*)personInImage.c_str(), cvPoint(r.x, r.y), &font, CV_RGB(0,255,255));
    // Draw a rectangle around the face
    cvRectangle( display_frame,
                cvPoint( r.x, r.y ),
                cvPoint( r.x + r.width, r.y + r.height ),
                CV_RGB( 255, 0, 0 ), 1, 8, 0 );
#endif
    
}

// Initialize variables needed for Room Recognition
void Pimp::initRoomRecognition()
{
}

// Room Recognition run on the current_frame
void Pimp::roomRecognition()
{
    string roomNumber = roomRec->recognizeRoom(display_frame);
    roomInImage = roomNumber;
    cvPutText(display_frame, (char*)roomNumber.c_str(), cvPoint(50, 50), &font, CV_RGB(255,0,0));
}

