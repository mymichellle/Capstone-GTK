//
//  BasePageGTK.cpp
//  Collage
//
//  Created by Michelle Alexander on 9/23/11.
//  Copyright 2011 ??? . All rights reserved.
//
//  Template for a GTK display page

#include "BasePageGTK.h"

#include <stdlib.h>

BasePageGTK::BasePageGTK()
{
    window = gtk_vbox_new (TRUE,1);
}
void BasePageGTK::display(void)
{
    
}

GtkWidget* BasePageGTK::getWindow()
{
    return window;
}

