//
//  Pimp.h
//  PiMP
//
//  Created by Michelle Alexander on 10/27/11.
//  Copyright 2011 ??? . All rights reserved.
//

#ifndef Pimp_h
#define Pimp_h

#include <gtk/gtk.h>
#include <string>

#include "BasePageGTK.h"
#include "VideoStream.h"
#include "FaceDetect.h"
#include "FacePreProcess.h"
#include "Eigenfaces.h"
#include "RoomRecognition.h"

/************    Debug enable triggers    *******************/
#define DEBUG // Toggle all Debug code on
#ifdef DEBUG
            
            #define SHOW_VIDEO // Show the live video stream

#endif
/************************************************************/

#define FACE_RECOGNITION_CONFIDENCE 0.5

class Pimp
{
private:
    // Constructors
    Pimp();
    // these are declared but not implemented
    Pimp(Pimp const&);
    void operator=(Pimp const&);
        
    // Base Window
    GtkWidget *window;
    
    // Current page
    BasePageGTK *displayPage;
    
    // Modes
    enum Mode{
        RECOGNITION,
        RECOGNITION_FACE,
        RECOGNITION_ROOM,
        NEW_FACE,
        NEW_ROOM,
        TEST_MODE
    };
    enum Mode currentMode;
    
    // State vars
    int cycleNum;
    int lastRoomCycle;
    int lastFaceCycle;
    
    // Recognition Rate
    int faceRate;
    int roomRate;
    
    // Video Image Capture
    VideoStream *video;
    IplImage *current_frame;
    IplImage *display_frame;
    CvFont font;

    // Face Recognition
    void initFaceRecognition();
    void faceRecognition();
    CvRect searchArea;
    FaceDetect *faces;
    FacePreProcess *facePreProcess;
    Eigenfaces *eigenfaces;
    //Fisherfaces *fisherfaces;
    IplImage *processed_face;
    
    // Room Recognition
    RoomRecognition *roomRec;
    void initRoomRecognition();
    void roomRecognition();
    
    // File path
    string dir;
    string faceDir;
    string knownFacesDoc;
    string unknownFacesDoc;
    string roomDir;
    
    // Output
    std::string personInImage;
    std::string roomInImage;
    
    // Helper functions
    void startupProcess();
    bool initialized;

    
public:
    // Singleton Pimp
    static Pimp& sharedPimp()
    {
        static Pimp instance;
        return instance;
    }
    // Set the recognition mode
    void setMode(bool faceRecOn, int faceRecRate, bool roomRecOn, int roomRecRate);    
    
    // Set active page
    void setDisplayPage(BasePageGTK *newPage);
    void draw();
    
    // Run the app
    void initProcess();
    void mainProcess();
    void updateEigenFaces();
    
    // Video controls
    void videoOn();
    void videoOff();
    
    void testProc();
    
    // Getters
    std::string getFaceDir(){return faceDir;};
    std::string getDir(){return dir;};
    std::string getRecognizedPerson(){return personInImage;};
    std::string getRecognizedRoom(){return roomInImage;};
    int getFaceRecognitionRate(){return faceRate;};
    int getRoomRecognitionRate(){return roomRate;};
    bool isFaceRecognition();
    bool isRoomRecognition();
    
   };

#endif