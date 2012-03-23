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
		//Pimp::sharedPimp().videoOff();
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
	//videoOn();
    
    // Face Recognition
   // fisherfaces = new Fisherfaces(faceDir+"NewFaceSet");
    eigenfaces = new Eigenfaces(faceDir+"NewFaceSet");
    initFaceRecognition();
    
    // Room Recognition
    roomRec = new RoomRecognition(roomDir+"NewRooms");
    initRoomRecognition();
    
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
cout<<"VIDEO ON!"<<endl;
    video->startCapture();
}

// Turn the video stream off
void Pimp::videoOff(){
cout<<"VIDEO OFF!"<<endl;
    video->endCapture();
}

// Update the eigenfaces
void Pimp::updateEigenFaces(){
    eigenfaces->update();
}

// add new face
void Pimp::addFace(FaceTextureGTK *face)
{
    eigenfaces->addFace(face);
}

// add new room
void Pimp::addRoom(RoomTextureGTK *room){
    roomRec->addRoom(room);
}

void Pimp::getNewTexture(BaseTextureGTK *tex){
    // Create a new face texture
    current_frame = video->getImage();
	if( current_frame != NULL )
	{
		display_frame = cvCreateImage(cvSize (current_frame->width, current_frame->height), IPL_DEPTH_8U, 3);	
		cvFlip (current_frame, display_frame, 1);
		tex->setImage(display_frame);
	}
	else
		cout<<"ERROR: getNewTexture - no current frame"<<endl;
}

void Pimp::getNewFace(FaceTextureGTK* faceText)
{
	// Create a new face texture
    current_frame = video->getImage();
	if(display_frame == NULL)
    	display_frame = cvCreateImage(cvSize (current_frame->width, current_frame->height), IPL_DEPTH_8U, 3);
    cvFlip (current_frame, display_frame, 1);
    
    // Get the largest face in the frame
    CvRect r = cvRect(0,0,0,0);
    r = faces->getFace(current_frame, r);
    
    cvSetImageROI(display_frame,r);
    processed_face = facePreProcess->preProcess(display_frame);
    cvResetImageROI(display_frame);
    
    // Save the processed face to the FaceTexture
    faceText->setFace(display_frame, processed_face, r);
}

void Pimp::getNewFaceTextures(vector<FaceTextureGTK*> faceText)
{    
    int maxFaces = faceText.size();
    cout<<"Number of faces "<<maxFaces<<endl;
    for(int i = 0; i < maxFaces; i++)
    {
        // Create a new face texture
		getNewFace(faceText[i]);
    }
    
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


void Pimp::setFaceRec(bool faceRecOn)
{
	if(currentMode == RECOGNITION || currentMode == RECOGNITION_ROOM)
		setMode(faceRecOn, faceRate, true, roomRate);
	else
		setMode(faceRecOn, faceRate, false, roomRate);
}

void Pimp::setFaceRecRate(int faceRecRate)
{
	if(currentMode == RECOGNITION)
	{
		setMode(true, faceRecRate, true, roomRate);
	}
	else if(currentMode == RECOGNITION_ROOM)
	{
		setMode(false, faceRecRate, true, roomRate);
	}
	else if(currentMode == RECOGNITION_FACE)
	{
		setMode(true, faceRecRate, false, roomRate);
	}
}

void Pimp::setRoomRec(bool roomRecOn)
{
	if(currentMode == RECOGNITION || currentMode == RECOGNITION_FACE)
		setMode(true, faceRate, roomRecOn, roomRate);
	else
		setMode(false, faceRate, roomRecOn, roomRate);
}
void Pimp::setRoomRecRate(int roomRecRate)
{
	if(currentMode == RECOGNITION)
	{
		setMode(true, faceRate, true, roomRecRate);
	}
	else if(currentMode == RECOGNITION_ROOM)
	{
		setMode(false, faceRate, true, roomRecRate);
	}
	else if(currentMode == RECOGNITION_FACE)
	{
		setMode(true, faceRate, false, roomRecRate);
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
    current_frame = video->getImage();
    string test = dir + "test.jpg";
    cvSaveImage((char *)test.c_str(), current_frame);
}

// Main Processing Loop
void Pimp::mainProcess()
{
	cout<<"HERE"<<endl;
    current_frame = video->getImage();
    display_frame = cvCreateImage(cvSize (current_frame->width, current_frame->height), IPL_DEPTH_8U, 3);
    assert (current_frame && display_frame);
    
    current_frame = video->getImage();
    // set display_frame to flipped current_frame
    cvFlip (current_frame, display_frame, 1);
    
    // Face Recognition
    if( (cycleNum - lastFaceCycle == faceRate) && (currentMode == RECOGNITION || currentMode == RECOGNITION_FACE) )
    {
        faceRecognition();
        lastFaceCycle = cycleNum;
    }
    
    // Room Recognition
    if( (cycleNum - lastRoomCycle == roomRate) && (currentMode == RECOGNITION || currentMode == RECOGNITION_ROOM))
    {	
        gray_frame = cvCreateImage( cvGetSize(display_frame), IPL_DEPTH_8U, 1);
        cvCvtColor( display_frame, gray_frame, CV_BGR2GRAY);
        roomRecognition();
        lastRoomCycle = cycleNum;
    }
    
    
#ifdef SHOW_VIDEO
    // Draw the FPS figure
    drawFPS(display_frame);
    
    // Show the debug live video stream
    cvShowImage(WINDOW_NAME, display_frame);
   
#endif
    // cvWaitKey(1);
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
    string roomNumber = roomRec->recognizeRoom(gray_frame);
    roomInImage = roomNumber;
    cvPutText(display_frame, (char*)roomNumber.c_str(), cvPoint(50, 50), &font, CV_RGB(255,0,0));
}

