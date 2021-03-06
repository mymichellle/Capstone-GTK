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
#include "BaseTextureGTK.h"
#include "RoomTextureGTK.h"
#include "FaceTextureGTK.h"
#include "AudioUtility.h"

/************    Debug enable triggers    *******************/
//#define DEBUG // Toggle all Debug code on
#ifdef DEBUG
            
            #define SHOW_VIDEO // Show the live video stream
			//#define DEBUG_ROOMS // Show testing images of room recognition

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
	int lastFaceAudio;
	int lastRoomAudio;
    
    // Recognition Rate
    int faceRate;
    int roomRate;
	int actualFaceRate;
	int actualRoomRate;
	int audioRoomRate;
	int audioFaceRate;
    
    // Video Image Capture
    VideoStream *video;
    IplImage *current_frame;
    IplImage *display_frame;
	IplImage *gray_frame;
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
    
    // Audio I/O
    AudioUtility *audioUtility;
    void initAudioUtility();
    

    // File path
    std::string dir;
    std::string faceDir;
    std::string knownFacesDoc;
    std::string unknownFacesDoc;
    std::string roomDir;
    std::string soundDir;
	std::string	audioFile;
    
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
	void setFaceRec(bool faceRecOn);
	void setFaceRecRate(int faceRecRate);
	void setRoomRec(bool roomRecOn);
	void setRoomRecRate(int roomRecRate);
	void setRoomAudioRate(int roomAudio);
	void setFaceAudioRate(int faceAudio);    
    
    // Set active page
    void setDisplayPage(BasePageGTK *newPage);
    void draw();
    
    // Run the app
    void initProcess();
    void mainProcess();
    void addFace(FaceTextureGTK *face);
    void addRoom(RoomTextureGTK *room);
    void updateEigenFaces();
    
    // Video controls
    void videoOn();
    void videoOff();
    
	// Helper Function
    void getNewFaceTextures(std::vector<FaceTextureGTK*> faceText);
	void getNewFace(FaceTextureGTK* faceText);
	void getNewTexture(BaseTextureGTK *tex);

    void testProc();
    
	// Room Texture
	RoomTextureGTK *roomTexture;

    // Sound Functions
    void recordSound(char *);
    void playbackSound(char *);

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
