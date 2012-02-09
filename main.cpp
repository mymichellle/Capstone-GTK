#include <gtk/gtk.h>

#include "Pimp.h"
#include "MainPageGTK.h"


int main (int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *button;
    
    gtk_init (&argc, &argv);
    
    Pimp::sharedPimp().setDisplayPage(new MainPageGTK());
    Pimp::sharedPimp().draw();
    
    gtk_main ();
    
    return 0;
}