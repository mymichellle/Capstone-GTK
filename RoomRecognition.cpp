//
//  RoomRecognition.cpp
//  PiMP
//
//  Created by Michelle Alexander on 12/1/11.
//  Copyright 2011 ??? . All rights reserved.
//

#include "RoomRecognition.h"
#include "Pimp.h"
#include <iostream>
#include <sys/stat.h>

using namespace std;

RoomRecognition::RoomRecognition(string roomsFile)
{
    xmlFileName = roomsFile+".xml";
    nRooms = 0;
    loadFromXml();
    prevRoom = 0;
}

void RoomRecognition::saveToXml()
{
    CvFileStorage * fileStorage;
    char varname[200];
	
    // create a file-storage interface
	fileStorage = cvOpenFileStorage( (char*)xmlFileName.c_str(), 0, CV_STORAGE_WRITE );
    
    // save number room names
    cvWriteInt( fileStorage, "nNames", nNames );
    
    for(int i = 0; i < nNames; i++)
    {
        sprintf( varname, "roomName_%d", i);
        cvWriteString(fileStorage, varname, (char*)roomNames[i].c_str() );
        
        sprintf( varname, "nRoomImgs_%d", i);
        cvWriteInt(fileStorage, varname, nImgsPerRoom[i]);
    }
    
    // save number rooms
    cvWriteInt( fileStorage, "nRooms", nRooms );
	
    // Save the room refpoints
    ostringstream s;
    for(int i = 0; i < nRooms; i++)
    {
        // save the room id lookup
        sprintf( varname, "roomID_%d", i);
        cvWriteInt(fileStorage, varname, roomLookUp[i]);
        
        // save the file name
        sprintf( varname, "fileName_%d", i);
        cvWriteString( fileStorage, varname, (char*)fileNames[i].c_str(), 0);
        
        // save number of ref points
        sprintf( varname, "nRef_%d", i );
        cvWriteInt( fileStorage, varname, roomRefIpts[i].size() );
        for(int j = 0; j < roomRefIpts[i].size(); j++)
        {
            writePointToStream(roomRefIpts[i][j], s);
            sprintf( varname, "room_%d_%d", i, j );
            cvWriteString(fileStorage, varname, (char*)s.str().c_str(), 0);
            s.str("");
        }
    }
    
	// release the file-storage interface
	cvReleaseFileStorage( &fileStorage );
}

void RoomRecognition::loadFromXml()
{
    CvFileStorage * fileStorage;
	int num;
    char varname[200];
    string sPoint;
    string sName;
    istringstream s;
    IpVec v;
    Ipoint p;
    
	// create a file-storage interface
	fileStorage = cvOpenFileStorage( (char*)xmlFileName.c_str(), 0, CV_STORAGE_READ );
	if( !fileStorage ) {
        cout<<"Can't open training database file for rooms "<<xmlFileName<<endl;
	}
    
    // Empty Vectors
	roomRefIpts.clear();	// Make sure it starts as empty.
    roomLookUp.clear();     // Make sure it starts as empty.
    roomNames.clear();      // Make sure it starts as empty.
    fileNames.clear();      // Make sure it starts as empty.
    nImgsPerRoom.clear();
	
    // Room Names
    nNames = cvReadIntByName( fileStorage, 0, "nNames", 0);
    for(int i = 0; i < nNames; i++)
    {
        sprintf( varname, "roomName_%d", i);
        sName = cvReadStringByName(fileStorage, 0, varname );
        roomNames.push_back(sName);
        
        sprintf( varname, "nRoomImgs_%d", i);
        num = cvReadIntByName(fileStorage, 0, varname );
        nImgsPerRoom.push_back(num);
    }
    
    // Room Reference Points
    nRooms = cvReadIntByName( fileStorage, 0, "nRooms", 0 );
    
    for(int i = 0; i<nRooms; i++)
    {
        // Add the room id to the lookup table
		sprintf( varname, "roomID_%d", i );
        num = cvReadIntByName(fileStorage, 0, varname );
        roomLookUp.push_back(num);
        
        sprintf( varname, "fileName_%d", i);
        sName = cvReadStringByName(fileStorage, 0, varname);
        fileNames.push_back(sName);
        
        // Load the image and calculate the points
        loadRoomIpts(fileNames[i], roomNames[num]);
        // Below was an attempt to save and read in ipoints.. it did not work
        // it is best to save a picture of the room and recalculate the ipoints
        // when the project loads
		/*sprintf( varname, "nRef_%d", i );
		num = cvReadIntByName(fileStorage, 0, varname );
        cout<<"Room "<<i<<" has this many points "<<num<<endl;
        for(int j = 0; j < num; j++)
        {
            sprintf( varname, "room_%d_%d", i, j );
            sPoint = cvReadStringByName(fileStorage, 0, varname );
            s.str(sPoint);
            p = readPointFromStream(s);
            if(j == 0)
                writePointToStream(p, cout);
            v.push_back(p);
        }
        roomRefIpts.push_back(v);*/
    }
}

// Add the RefIpts for the JPG file
void RoomRecognition::loadRoomIpts(string file, string name)
{
    IpVec ref_ipts;
    IplImage *img;
    
    // Get the face directory
    string dir = Pimp::sharedPimp().getDir()+ "/Rooms/" + name;
    
    // Make sure the directory exists
    int r = mkdir((char *)dir.c_str() , S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
    if(r > 0)
        cout<<"Directory not made! "<<dir<<endl;
   
    stringstream s;
    s << dir << "/"<< file <<".jpg";
    img = cvLoadImage((char*)string(s.str()).c_str(), CV_LOAD_IMAGE_GRAYSCALE);
    if( !img )
    {
        cout<<"ERROR: cannot load the image from "<<string(s.str())<<endl;
    }
    surfDetDes(img, ref_ipts, false, 4, 4, 2, 0.0006f);

#ifdef DEBUG_ROOMS
 	// Draw box around object
     for(int i = 0; i < 4; i++ )
    {
        CvPoint r1 = src_corners[i%4];
        CvPoint r2 = src_corners[(i+1)%4];
        cvLine( img, cvPoint(r1.x, r1.y),
               cvPoint(r2.x, r2.y), cvScalar(255,255,255), 3 );
    }
    
    for (unsigned int i = 0; i < ref_ipts.size(); ++i)
        drawIpoint(img, ref_ipts[i]);

    cvShowImage((char*)file.c_str(), img);
#endif
    
    roomRefIpts.push_back(ref_ipts);
}

// Write RefIpts to a stream
void RoomRecognition::writePointToStream(Ipoint p, ostream& s)
{
    s<<p.x<<" "<<p.y<<" "<<p.scale<<" "<<p.orientation<<" "<<p.laplacian;
    s<<" "<<p.dx<<" "<<p.dy<<" "<<p.clusterIndex;
    for(int i = 0; i < 64; i++)
        s<<" "<<p.descriptor[i];
    
}

// Read RefIPoints from a stream
Ipoint RoomRecognition::readPointFromStream(istream& s)
{
    Ipoint p = Ipoint();
    float x, y, scale, orientation, laplacian, descriptor[64], dx, dy;
    int clusterIndex;
    int sizeOfDescriptor;
    s>>x>>y>>scale>>orientation>>laplacian;
    s>>dx>>dy>>clusterIndex;
    for(int i=0; i<64; i++)
        s>>p.descriptor[i];
    
    p.x = x;
    p.y = y;
    p.scale = scale;
    p.orientation = orientation;
    p.laplacian = laplacian;
    //p.descriptor = descriptor;
    p.dx = dx;
    p.dy = dy;
    p.clusterIndex = clusterIndex;
    
    return p;
}

// Add a room image to the known rooms
void RoomRecognition::addRoom(RoomTextureGTK *room)
{
    string tempName;
    string fileName;
    char s[200];
    // Extract reference object Ipoints
    IpVec ref_ipts;
    surfDetDes(room->getImage(), ref_ipts, false, 4, 4, 2, 0.0006f);

    cout <<"ADD ROOM: "<<nRooms<<endl;
    
    // Set the standard image size
    src_corners[0] = cvPoint(0,0);
    src_corners[1] = cvPoint(room->getImage()->width,0);
    src_corners[2] = cvPoint(room->getImage()->width, room->getImage()->height);
    src_corners[3] = cvPoint(0, room->getImage()->height);

    // Add to the rooms vector
    roomRefIpts.push_back(ref_ipts);
    
    // Check if this is a new room name
    bool newName = true;
    tempName = room->getName();
    transform(tempName.begin(), tempName.end(), tempName.begin(), ::tolower);
    for(int i = 0; i < nNames; i++)
    {
        if(tempName == roomNames[i])
        {
            roomLookUp.push_back(i);
            nImgsPerRoom[i] = nImgsPerRoom[i]+1;
            newName = false;
            
            sprintf( s, "%s%d", (char*)room->getName().c_str(), nImgsPerRoom[i]);
            cout<<"NEW NUMBER: "<<s<<endl;
            fileName = s;//room->getName() + nImgsPerRoom[i];
            
            break;
        }
    }
    if(newName)
    {
        roomLookUp.push_back(nNames);
        roomNames.push_back(room->getName());
        nImgsPerRoom.push_back(0);
        sprintf( s, "%s%d", (char*)room->getName().c_str(), 0 );
        fileName = s;
        nNames++;
    }
    nRooms++;
    // Save the texture as a jpg
    room->saveRoomAsJPG(fileName);
    fileNames.push_back(fileName);
    
#ifdef DEBUG_ROOMS
 	// Draw box around object
     for(int i = 0; i < 4; i++ )
    {
        CvPoint r1 = src_corners[i%4];
        CvPoint r2 = src_corners[(i+1)%4];
        cvLine( room->getImage(), cvPoint(r1.x, r1.y),
               cvPoint(r2.x, r2.y), cvScalar(255,255,255), 3 );
    }
    
    for (unsigned int i = 0; i < ref_ipts.size(); ++i)
        drawIpoint(room->getImage(), ref_ipts[i]);

    cvShowImage((char*)fileName.c_str(), room->getImage());
#endif

    saveToXml();
}

string RoomRecognition::recognizeRoom(IplImage *image)
{
    // Declare Ipoints and other stuff
    IpPairVec matches;
    IpVec ipts;
    CvPoint dst_corners[4];
    
    // Detect and describe interest points in the frame
    surfDetDes(image, ipts, false, 4, 4, 2, 0.0006f);

#ifdef DEBUG_ROOMS
 	// Draw box around object
     for(int i = 0; i < 4; i++ )
    {
        CvPoint r1 = src_corners[i%4];
        CvPoint r2 = src_corners[(i+1)%4];
        cvLine( image, cvPoint(r1.x, r1.y),
               cvPoint(r2.x, r2.y), cvScalar(255,255,255), 3 );
    }
    
    for (unsigned int i = 0; i < ipts.size(); ++i)
        drawIpoint(image, ipts[i]);

    cvShowImage("REC ROOM", image);
#endif

	// Compare to the previously detected room first
	getMatches(ipts,roomRefIpts[prevRoom],matches);
	int cornersTranslated = translateCorners(matches, src_corners, dst_corners);
	if(cornersTranslated)
	{
		prevCount++;
    	return roomNames[roomLookUp[prevRoom]];
	}

    // Compare to all the rooms
    for( int i = 0; i < nRooms; i++)
    {
		if(i != prevRoom)
		{
		    // Fill match vector
		    getMatches(ipts,roomRefIpts[i],matches);
		    int cornersTranslated = translateCorners(matches, src_corners, dst_corners);
		    // This call finds where the object corners should be in the frame
		    if (cornersTranslated)
		    {

 // Draw box around object 	
        for(int j = 0; j < 4; j++ )
            {
                CvPoint r1 = dst_corners[j%4];
                CvPoint r2 = dst_corners[(j+1)%4];
                cvLine( image, cvPoint(r1.x, r1.y),
                       cvPoint(r2.x, r2.y), cvScalar(255,255,255), 3 );
     }

				prevRoom = i;
				prevCount = 0;
		        return roomNames[roomLookUp[i]];
		    }
		}
    }
	
	prevCount = 0;
    return "...";
}
