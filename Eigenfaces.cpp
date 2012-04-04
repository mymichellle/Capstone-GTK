//
//  Eigenfaces.cpp
//  PiMP
//
//  adapted from  http://eigenfacesinferencia.googlecode.com/svn-history/r3/trunk/eigenface.c
//  Copyright 2011 ??? . All rights reserved.
//

#include "Eigenfaces.h"
#include "Pimp.h"
#include "cvaux.h"
#include "highgui.h"
#include <fstream>
#include "Utility.h"

#define CV_LOAD_IMAGE_GRAYSCALE 0

using namespace std;

Eigenfaces::Eigenfaces(string filename)
{
    // Create filenames
    textFileName = filename + ".txt";
    xmlFileName = filename + ".xml";
    averageImageFileName = filename + "_average.bmp";
    eigenfacesImageFileName = filename + "_eigenfaces.bmp";
    
    faceImgArr                  = 0; // array of face images
    personNumTruthMat           = 0; // array of person numbers
    nTrainFaces                 = 0; // the number of training images
    nEigens                     = 0; // the number of eigenvalues
    nPersons                    = 0; // the number of people in the training set
    pAvgTrainImg                = 0; // the average image
    eigenVectArr                = 0; // eigenvectors
    eigenValMat                 = 0; // eigenvalues
    projectedTrainFaceMat       = 0; // projected training faces
    
	int prevPerson = 0; // Number of the previously identified person
	int prevCount = 0;
	
    CvMat * trainPersonNumMat = 0;  // the person numbers during training
    empty = true;
    
    // Check if the file exists, if so assume it is not empty..
    if( fexist((char*)textFileName.c_str()) )
       empty = false;
    
    
    learn((char *)textFileName.c_str());
    
    // load the saved training data
	if( !empty && !loadTrainingData( &trainPersonNumMat ) )
    {
        cout<<"NO TRAINING DATA"<<endl;
        // Learn the data from the designated file
        learn((char *)textFileName.c_str());
    }
    
    // Initialize the projectedTestFace
	projectedTestFace = (float *)cvAlloc( nEigens*sizeof(float) );
    //createTextFile();
}

void Eigenfaces::update()
{
    learn((char*)textFileName.c_str());	
	projectedTestFace = (float *)cvAlloc( nEigens*sizeof(float) );
}

//////////////////////////////////
// learn()
//
// http://eigenfacesinferencia.googlecode.com/svn-history/r3/trunk/eigenface.c
//
void Eigenfaces::learn(char * filename)
{
	int i, offset;
    
	// load training data
	nTrainFaces = loadFaceImgArray(filename);
	if( nTrainFaces < 2 )
	{
		fprintf(stderr,
		        "Need 2 or more training faces\n"
		        "Input file contains only %d\n", nTrainFaces);
        cout<<"NUMBER TRAINS: "<<nTrainFaces<<endl;
        if(nTrainFaces == 0)
            empty = true;
		return;
	}
    
	// do PCA on the training faces
	doPCA();
    
	// project the training images onto the PCA subspace
	projectedTrainFaceMat = cvCreateMat( nTrainFaces, nEigens, CV_32FC1 );
	offset = projectedTrainFaceMat->step / sizeof(float);
	for(i=0; i<nTrainFaces; i++)
	{
		//int offset = i * nEigens;
		cvEigenDecomposite(
                           faceImgArr[i],
                           nEigens,
                           eigenVectArr,
                           0, 0,
                           pAvgTrainImg,
                           //projectedTrainFaceMat->data.fl + i*nEigens);
                           projectedTrainFaceMat->data.fl + i*offset);
	}
    
	// store the recognition data as an xml file
	storeTrainingData();
    
    storeEigenfaceImages();
    
    // update the training data
	if( !loadTrainingData( &trainPersonNumMat ) )
    {
        cout<<"NO TRAINING DATA"<<endl;
    }
}

// Open the training data from the xml file
int Eigenfaces::loadTrainingData(CvMat ** pTrainPersonNumMat)
{
	CvFileStorage * fileStorage;
	int i;
    
	// create a file-storage interface
	fileStorage = cvOpenFileStorage( (char*)xmlFileName.c_str(), 0, CV_STORAGE_READ );
	if( !fileStorage ) {
        cout<<"Can't open training database file "<<xmlFileName<<endl;
		return 0;
	}
    
	// Load the person names. Added by Shervin.
	personNames.clear();	// Make sure it starts as empty.
    nImagesPerson.clear();  // Make sure it starts as empty
	nPersons = cvReadIntByName( fileStorage, 0, "nPersons", 0 );
	if (nPersons == 0) {
        cout <<"No people found in the training database "<<xmlFileName<<endl;
		return 0;
	}
	// Load each person's name.
	for (i=0; i<nPersons; i++) {
		std::string sPersonName;
        int num;
		char varname[200];
		sprintf( varname, "personName_%d", (i) );
		sPersonName = cvReadStringByName(fileStorage, 0, varname );
		personNames.push_back( sPersonName );
		sprintf( varname, "nImagesPerson_%d", (i) );
		num = cvReadIntByName(fileStorage, 0, varname );
		nImagesPerson.push_back( num );
	}
    
	// Load the data
	nEigens = cvReadIntByName(fileStorage, 0, "nEigens", 0);
	nTrainFaces = cvReadIntByName(fileStorage, 0, "nTrainFaces", 0);
	*pTrainPersonNumMat = (CvMat *)cvReadByName(fileStorage, 0, "trainPersonNumMat", 0);
	eigenValMat  = (CvMat *)cvReadByName(fileStorage, 0, "eigenValMat", 0);
	projectedTrainFaceMat = (CvMat *)cvReadByName(fileStorage, 0, "projectedTrainFaceMat", 0);
	pAvgTrainImg = (IplImage *)cvReadByName(fileStorage, 0, "avgTrainImg", 0);
	eigenVectArr = (IplImage **)cvAlloc(nTrainFaces*sizeof(IplImage *));
	for(i=0; i<nEigens; i++)
	{
		char varname[200];
		sprintf( varname, "eigenVect_%d", i );
		eigenVectArr[i] = (IplImage *)cvReadByName(fileStorage, 0, varname, 0);
	}
    
	// release the file-storage interface
	cvReleaseFileStorage( &fileStorage );
    
    // DEBUG
/*    for(int i = 0; i < nPersons; i++)
    {
        cout<<"Person #"<<i<<" is: "<<personNames[i]<<endl;
    }*/
    // DEBUG
    
    
	return 1;
}

// Save the training data to the file xml file
void Eigenfaces::storeTrainingData()
{
	CvFileStorage * fileStorage;
	int i;
    
	// create a file-storage interface
	fileStorage = cvOpenFileStorage( (char*)xmlFileName.c_str(), 0, CV_STORAGE_WRITE );
    
	// Store the person names. Added by Shervin.
	cvWriteInt( fileStorage, "nPersons", nPersons );
	for (i=0; i<nPersons; i++) {
		char varname[200];
		sprintf( varname, "personName_%d", i );
		cvWriteString(fileStorage, varname, personNames[i].c_str(), 0);
        sprintf( varname, "nImagesPerson_%d",i);
        cvWriteInt(fileStorage, varname, nImagesPerson[i]);
	}
    
	// store all the data
	cvWriteInt( fileStorage, "nEigens", nEigens );
	cvWriteInt( fileStorage, "nTrainFaces", nTrainFaces );
	cvWrite(fileStorage, "trainPersonNumMat", personNumTruthMat, cvAttrList(0,0));
	cvWrite(fileStorage, "eigenValMat", eigenValMat, cvAttrList(0,0));
	cvWrite(fileStorage, "projectedTrainFaceMat", projectedTrainFaceMat, cvAttrList(0,0));
	cvWrite(fileStorage, "avgTrainImg", pAvgTrainImg, cvAttrList(0,0));
	for(i=0; i<nEigens; i++)
	{
		char varname[200];
		sprintf( varname, "eigenVect_%d", i );
		cvWrite(fileStorage, varname, eigenVectArr[i], cvAttrList(0,0));
	}
    
	// release the file-storage interface
	cvReleaseFileStorage( &fileStorage );
}

//////////////////////////////////
// doPCA()
// http://eigenfacesinferencia.googlecode.com/svn-history/r3/trunk/eigenface.c
//
void Eigenfaces::doPCA()
{
	int i;
	CvTermCriteria calcLimit;
	CvSize faceImgSize;
    
	// set the number of eigenvalues to use
	nEigens = nTrainFaces-1;
    
	// allocate the eigenvector images
	faceImgSize.width  = faceImgArr[0]->width;
	faceImgSize.height = faceImgArr[0]->height;
	eigenVectArr = (IplImage**)cvAlloc(sizeof(IplImage*) * nEigens);
	for(i=0; i<nEigens; i++)
		eigenVectArr[i] = cvCreateImage(faceImgSize, IPL_DEPTH_32F, 1);
    
	// allocate the eigenvalue array
	eigenValMat = cvCreateMat( 1, nEigens, CV_32FC1 );
    
	// allocate the averaged image
	pAvgTrainImg = cvCreateImage(faceImgSize, IPL_DEPTH_32F, 1);
    
	// set the PCA termination criterion
	calcLimit = cvTermCriteria( CV_TERMCRIT_ITER, nEigens, 1);
    
	// compute average image, eigenvalues, and eigenvectors
	cvCalcEigenObjects(
                       nTrainFaces,
                       (void*)faceImgArr,
                       (void*)eigenVectArr,
                       CV_EIGOBJ_NO_CALLBACK,
                       0,
                       0,
                       &calcLimit,
                       pAvgTrainImg,
                       eigenValMat->data.fl);
    
	cvNormalize(eigenValMat, eigenValMat, 1, 0, CV_L1, 0);
}


//////////////////////////////////
// loadFaceImgArray()
// http://eigenfacesinferencia.googlecode.com/svn-history/r3/trunk/eigenface.c
//
int Eigenfaces::loadFaceImgArray(char * filename)
{
    ifstream imgListFile;
    imgListFile.open(filename);
    string imgFilename;
    string faceName;
	int iFace, nFaces=0;
    int personNumber;
    
	// open the input file
	if( !imgListFile.is_open())
	{
        cout<<"ERROR: Cannot open file "<<filename<<endl;\
		return 0;
	}
    
    
	// read in the number of faces from the first line
    imgListFile >> nFaces;
    if(nFaces <= 1)
        return 0;
    
	// allocate the face-image array and person number matrix
	faceImgArr        = (IplImage **)cvAlloc( nFaces*sizeof(IplImage *) );
	personNumTruthMat = cvCreateMat( 1, nFaces, CV_32SC1 );
    
    // Clear the current personNames and nImagesPerson stacks 
    personNames.clear();
    nImagesPerson.clear();
    nPersons = 0;
    
    iFace = 0;
    while(!imgListFile.eof())
    {
		// read person number and name of image file
        imgListFile >> personNumber >> faceName >> imgFilename;
        
        // If the face is new add a new entry to personNames
        if(personNumber == nPersons)
        {
            personNames.push_back( faceName );
            nImagesPerson.push_back( 1 );
            nPersons++;
        }
        else
        {
            // Increment the number of images of the current face
            nImagesPerson[personNumber] = nImagesPerson[personNumber]+1;
        }
        
        // Keep the data
        personNumTruthMat->data.i[iFace] = personNumber;
        
        // load the image
        faceImgArr[iFace] = cvLoadImage((char*)imgFilename.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
        if( !faceImgArr[iFace] )
        {
            cout<<"ERROR: cannot load the image from "<<imgFilename<<endl;
			return 0;
		}
        
        
        // Incriment the current face
        iFace++;
    }
    // Off by one error at the end...
    //nImagesPerson[personNumber] = nImagesPerson[personNumber]-1;
    
    // DEBUG
    /*cout<<endl<<"INFORMATION READ IN FROM TXT"<<endl;
    for(int i = 0; i < nPersons; i++)
    {
        cout<<i<<" "<<personNames[i]<<" "<<nImagesPerson[i]<<endl;
    }
    cout<<endl;*/
    // DEBUG
    
	imgListFile.close();
    
	return nFaces;
}


// Save all the eigenvectors as images, so that they can be checked.
//OnlineFaceRec.cpp, by Shervin Emami (www.shervinemami.co.cc) on 2nd June 2010.
// Online Face Recognition from a camera using Eigenfaces.
void Eigenfaces::storeEigenfaceImages()
{
	// Store the average image to a file
	cvSaveImage((char *)averageImageFileName.c_str(), pAvgTrainImg);
	
    // Create a large image made of many eigenface images.
	// Must also convert each eigenface image to a normal 8-bit UCHAR image instead of a 32-bit float image.
	if (nEigens > 0) {
		// Put all the eigenfaces next to each other.
		int COLUMNS = 8;	// Put upto 8 images on a row.
		int nCols = min(nEigens, COLUMNS);
		int nRows = 1 + (nEigens / COLUMNS);	// Put the rest on new rows.
		int w = eigenVectArr[0]->width;
		int h = eigenVectArr[0]->height;
		CvSize size;
		size = cvSize(nCols * w, nRows * h);
		IplImage *bigImg = cvCreateImage(size, IPL_DEPTH_8U, 1);	// 8-bit Greyscale UCHAR image
		for (int i=0; i<nEigens; i++) {
			// Get the eigenface image.
			IplImage *byteImg = convertFloatImageToUcharImage(eigenVectArr[i]);
			// Paste it into the correct position.
			int x = w * (i % COLUMNS);
			int y = h * (i / COLUMNS);
			CvRect ROI = cvRect(x, y, w, h);
			cvSetImageROI(bigImg, ROI);
			cvCopyImage(byteImg, bigImg);
			cvResetImageROI(bigImg);
			cvReleaseImage(&byteImg);
		}
		cvSaveImage((char *)eigenfacesImageFileName.c_str(), bigImg);
		cvReleaseImage(&bigImg);
	}
}
// Get an 8-bit equivalent of the 32-bit Float image.
// Returns a new image, so remember to call 'cvReleaseImage()' on the result.
IplImage* Eigenfaces::convertFloatImageToUcharImage(const IplImage *srcImg)
{
	IplImage *dstImg = 0;
	if ((srcImg) && (srcImg->width > 0 && srcImg->height > 0)) {
        
		// Spread the 32bit floating point pixels to fit within 8bit pixel range.
		double minVal, maxVal;
		cvMinMaxLoc(srcImg, &minVal, &maxVal);
        
		//cout << "FloatImage:(minV=" << minVal << ", maxV=" << maxVal << ")." << endl;
        
		// Deal with NaN and extreme values, since the DFT seems to give some NaN results.
		if (cvIsNaN(minVal) || minVal < -1e30)
			minVal = -1e30;
		if (cvIsNaN(maxVal) || maxVal > 1e30)
			maxVal = 1e30;
		if (maxVal-minVal == 0.0f)
			maxVal = minVal + 0.001;	// remove potential divide by zero errors.
        
		// Convert the format
		dstImg = cvCreateImage(cvSize(srcImg->width, srcImg->height), 8, 1);
		cvConvertScale(srcImg, dstImg, 255.0 / (maxVal - minVal), - minVal * 255.0 / (maxVal-minVal));
	}
	return dstImg;
}

// Find the most likely person based on a detection. Returns the index, and stores the confidence value into pConfidence.
int Eigenfaces::findNearestNeighbor(float * pprojectedTestFace, float *pConfidence)
{
	//double leastDistSq = 1e12;
	double leastDistSq = DBL_MAX;
	int i, iTrain, iNearest = 0;
    
	for(iTrain=0; iTrain<nTrainFaces; iTrain++)
	{
		double distSq=0;
        
		for(i=0; i<nEigens; i++)
		{
			float d_i = pprojectedTestFace[i] - projectedTrainFaceMat->data.fl[iTrain*nEigens + i];
#ifdef USE_MAHALANOBIS_DISTANCE
			distSq += d_i*d_i / eigenValMat->data.fl[i];  // Mahalanobis distance (might give better results than Eucalidean distance)
#else
			distSq += d_i*d_i; // Euclidean distance.
#endif
		}
        
		if(distSq < leastDistSq)
		{
			leastDistSq = distSq;
			iNearest = iTrain;
		}
	}
    
	// Return the confidence level based on the Euclidean distance,
	// so that similar images should give a confidence between 0.5 to 1.0,
	// and very different images should give a confidence between 0.0 to 0.5.
	*pConfidence = 1.0f - sqrt( leastDistSq / (float)(nTrainFaces * nEigens) ) / 255.0f ;
    
	// Return the found index.
	return iNearest;
}

// Recognize the face in the preprocessed face
std::string Eigenfaces::recognizeFace(IplImage* preProcessedFace)
{
    float confidence;
    int iNearest, nearest;
    
    // make sure the trainPersonNumMat is not null
	if( trainPersonNumMat == 0 )
    {
        loadTrainingData( &trainPersonNumMat );
        cout<<"NO TRAINING DATA"<<endl;
    }
    
    // project the preProcessedFace onto the PCA subspace
    cvEigenDecomposite(preProcessedFace, nEigens, eigenVectArr, 0, 0, pAvgTrainImg, projectedTestFace);
    iNearest = findNearestNeighbor(projectedTestFace, &confidence);
    nearest  = trainPersonNumMat->data.i[iNearest];
    
	if(nearest == prevPerson)
	{
		prevCount++;
	}
	else
	{
		prevPerson = nearest;
		prevCount = 0;
	}
    //cout<<"nearest = "<< personNames[nearest]  << " Confidence = " << confidence << endl;
    
    std::stringstream output;
    output << personNames[nearest]; //<<" "<< confidence;

    if(confidence > FACE_RECOGNITION_CONFIDENCE)
        return output.str();
    else
	{
		prevPerson = nearest;
		prevCount = 0;
        return "...";
	}	
}

void Eigenfaces::addFace(FaceTextureGTK *face)
{
    // Find the people with the same name
    bool found = false;
    stringstream imageFileName;
    
    if(!empty)
    {
        //cout<<"addFace not Empty nPersons: "<<nPersons<<endl;
        for(int i = 0; i < nPersons; i++)
        {
            if(personNames[i] == face->getName())
            {
                //cout<<"found a match "<< i <<endl;
                imageFileName.str("");
                imageFileName << face->getName() << (nImagesPerson[i]);
                nImagesPerson[i] = nImagesPerson[i] + 1;
                found = true;
            }
        }
    }
    
    // If the name doesnt already exist, add a person
    if(!found)
    {
        //Special case if there are no people
        if(nPersons == 0)
        {
            personNames.clear();
            nImagesPerson.clear();
        }
        
        //cout<<"Name was not found"<<endl<<endl;
        //cout<<"  nPersons start: "<<nPersons<<endl;
        personNames.push_back(face->getName());
        nImagesPerson.push_back(0);
        imageFileName.str("");
        imageFileName << face->getName() << (nImagesPerson[nPersons]);
        nPersons++;
        
        //cout<<"  nPersons end: "<<nPersons<<endl;
        empty = false;
    }
    
    // Save the image
    face->saveProcessedFaceAsJPG(imageFileName.str());
    
    // Create new text file discribing where saved images are
    createTextFile();
}

// Create a text file of the known faces discribing where the images are saved
void Eigenfaces::createTextFile()
{    
    ofstream textFile;
    textFile.open((char *)textFileName.c_str());
    stringstream fileName;
    string dir = "Faces/Known/";
    
    // If there are no faces yet create an empty text file with 0 as number of faces
    if(empty)
    {
        cout<<"write nothing"<<endl;
        textFile << "0";
    }
    else
    {
        // Calculate the number of total images
        int count = 0;
        for(int i = 0; i < nPersons; i++)
            count += nImagesPerson[i];
        textFile<< count;
        
        // Save the Data
        for(int i = 0; i < nPersons; i++)
        {
            for(int j = 0; j < nImagesPerson[i]; j++)
            {
                char entry[512];
                // person# personName fileName
                fileName.str("");
                fileName << dir <<personNames[i] <<"/"<< personNames[i]<<j<<".jpg";
                sprintf( entry, "\n%d %s %s", i, (char*)(personNames[i]).c_str(),  (char*)(fileName.str()).c_str());
                
                textFile << entry;
            }
        }
    }
    
    textFile.close();
}
