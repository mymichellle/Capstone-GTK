//
//  RoomRecognition.h
//  PiMP
//
//  Created by Michelle Alexander on 12/1/11.
//  Copyright 2011 ??? . All rights reserved.
//

#ifndef RoomRecognition_h
#define RoomRecognition_h

#include "RoomTextureGTK.h"
#include <cstdio>
#include <string.h>
#include "OpenSurf/surflib.h"

class RoomRecognition
{
private:
    // Files
    std::string xmlFileName;
    void writePointToStream(Ipoint p, std::ostream& s);
    Ipoint readPointFromStream(std::istream& s);
    void saveToXml();
    void loadFromXml();
    
    // Rooms
    std::vector<IpVec> roomRefIpts;
    std::vector<int> roomLookUp;
    std::vector<std::string> fileNames;
    std::vector<std::string> roomNames;
    std::vector<int> nImgsPerRoom;
    int nRooms; // Number of test images
    int nNames; // Number of unique room names
	int prevRoom; // Number of the previously identified room
	int prevCount; // Count of consecutive times the room has been the same
    
    // Standard Corners
    CvPoint src_corners[4];
    
    // Helper Functions
    void loadRoomIpts(std::string file, std::string name);
    
    
public:
    RoomRecognition(std::string roomsFile);
    void addRoom(RoomTextureGTK *room);

    std::string recognizeRoom(IplImage *image);
    int getConsecutiveRoomCount(){return prevCount;};
	void resetCount(){prevCount = 0;};
};

#endif
