//
//  Eigenfaces.h
//  PiMP
//
//  Created by Michelle Alexander on 9/27/11.
//  Copyright 2011 ??? . All rights reserved.
//

#ifndef Eigenfaces_h
#define Eigenfaces_h

#include <cv.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>

#include "FaceTextureGTK.h"

class Eigenfaces
{
private:
    // File Names
    std::string textFileName;
    std::string xmlFileName;
    std::string averageImageFileName;
    std::string eigenfacesImageFileName;
    
    // Properties
    IplImage ** faceImgArr;         // array of face images
    CvMat    *  personNumTruthMat;  // array of person numbers
    int nTrainFaces;                // the number of training images
    int nEigens;                    // the number of eigenvalues
    IplImage * pAvgTrainImg;        // the average image
    IplImage ** eigenVectArr;       // eigenvectors
    CvMat * eigenValMat;            // eigenvalues
    CvMat * projectedTrainFaceMat;  // projected training faces
    std::vector<std::string> personNames;		// array of person names (indexed by the person number). Added by Shervin.
    std::vector<int> nImagesPerson;
    int nPersons;
    bool empty;
    
    // Training Data
    CvMat * trainPersonNumMat;
                      // the number of people in the training set
    
    // Helper Functions
    void storeTrainingData();
    int  loadTrainingData(CvMat ** pTrainPersonNumMat);
    void doPCA();
    int loadFaceImgArray(char * filename);
    void storeEigenfaceImages();
    IplImage* convertFloatImageToUcharImage(const IplImage *srcImg);
    int  findNearestNeighbor(float * projectedTestFace);
    int findNearestNeighbor(float * projectedTestFace, float *pConfidence);
    void createTextFile();
    
public:
    Eigenfaces(std::string filename);
    void learn(char * filename);
    void update();
    std::string recognizeFace(IplImage* preProcessedFace);
    void addFace(FaceTextureGTK *face);
};

#endif
