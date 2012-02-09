//
//  Utility.h
//  Collage
//
//  Created by Michelle Alexander on 10/02/11.
//  Copyright 2011 ??? . All rights reserved.
//
//  Static utitlty functions available throughout the program
//  Bitmap font display (non rotateable or scaleable)
//  Define a color object that can be used to pass red, green, blue and alpha

#ifndef Utility_h
#define Utility_h

#include <string>
#include <sys/stat.h>

using namespace std;

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 262

// Check if a file exists
static bool fexist( char *filename ) {
    struct stat buffer ;
    if ( stat( filename, &buffer ) == 0 ) return true ;
    return false ;
}

// Flip the y-axis
static void correctCoords(int &aX, int &aY)
{	
    aY=WINDOW_HEIGHT-aY;
}

// Define an object to define Color
class BaseColor{

private:
    struct Color{
        float red;
        float green;
        float blue;
        float alpha;
    };    
    
public:
    BaseColor(float r, float g, float b, float a)
    {
        color.red=r;color.green=g;color.blue=b;color.alpha=a;
    }
    Color color;
};

#endif