//
//  DisplayPageGTK.h
//  Pimp
//
//  Created by Michelle Alexander on 12/30/11.
//  Copyright 2011 ??? . All rights reserved.
//

#ifndef DisplayPageGTK_h
#define DisplayPageGTK_h

#include "BasePageGTK.h"
#include "Pimp.h"

class DisplayPageGTK : public BasePageGTK
{
private:
    // Title
    std::string title;
    std::string personTitle;
    std::string roomTitle;
    
    // Display Text
    std::string personName;
    std::string roomName;
    
    // Buttons
    GtkWidget *btn_back;
        
public:
    DisplayPageGTK();
    
    // Display
    void display();
};

#endif