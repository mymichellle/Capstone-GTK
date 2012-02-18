#include <gtk/gtksignal.h>
#include "KeyboardGTK.h"
#include <iostream>

#define TOP_ROW_COUNT 10
#define MID_ROW_COUNT 19
#define BUTTON_DIM 40
#define BTM_ROW_HEIGHT BUTTON_DIM/2 + BUTTON_DIM
#define MID_ROW_HEIGHT BTM_ROW_HEIGHT + BUTTON_DIM
#define TOP_ROW_HEIGHT MID_ROW_HEIGHT + BUTTON_DIM
#define SPACE_KEY 0
#define BACKSPACE_KEY 1
#define CLOSE_KEY 2

using namespace std;

const char* enumText[] = { "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "A", "S", "D", "F", "G", "H", "J", "K", "L", "Z", "X", "C", "V", "B", "N", "M", " ", "*"};

enum {
	KEYBOARDGTK_SIGNAL,
	DELETE_SIGNAL,
	LAST_SIGNAL
};

static void keyboardGTK_class_init	(KeyboardGTKClass *klass);
static void keyboardGTK_init		(KeyboardGTK *kb);
static void alpha_clicked 			(GtkWidget *widget, KeyboardGTK *kb);
static void delete_clicked			(GtkWidget *widget, KeyboardGTK *kb);

static gint keyboardGTK_signals[LAST_SIGNAL] = {0};

GType keyboardGTK_get_type (void)
{
	static GType kb_type = 0;
	
	if(!kb_type)
	{
		const GTypeInfo kb_info = 
		{
			sizeof(KeyboardGTKClass),
			NULL,
			NULL,
			(GClassInitFunc) keyboardGTK_class_init,
			NULL,
			NULL,
			sizeof(KeyboardGTK),
			0,
			(GInstanceInitFunc) keyboardGTK_init,
		};
		
		kb_type = g_type_register_static (GTK_TYPE_VBOX, "KeyboardGTK", &kb_info, GTypeFlags(0));
	}
	return kb_type;
}

static void keyboardGTK_class_init (KeyboardGTKClass *klass)
{
	keyboardGTK_signals[KEYBOARDGTK_SIGNAL] = g_signal_new ("keyboardGTK",
                                           		G_TYPE_FROM_CLASS(klass),
												G_SIGNAL_RUN_FIRST,
												G_STRUCT_OFFSET (KeyboardGTKClass, keyboardGTK),
												NULL,
												NULL,
												g_cclosure_marshal_VOID__VOID, 
												G_TYPE_NONE, 
												0);
	keyboardGTK_signals[DELETE_SIGNAL] = g_signal_new ("keyboardGTK_delete",
                                           		G_TYPE_FROM_CLASS(klass),
												G_SIGNAL_RUN_FIRST,
												G_STRUCT_OFFSET (KeyboardGTKClass, keyboardGTK),
												NULL,
												NULL,
												g_cclosure_marshal_VOID__VOID, 
												G_TYPE_NONE, 
												0);
}

static void keyboardGTK_init (KeyboardGTK *kb)
{
	gint i;
	
	GtkWidget *hbox1 = gtk_hbox_new (TRUE,1);
	GtkWidget *hbox2 = gtk_hbox_new (TRUE,1);
	GtkWidget *hbox3 = gtk_hbox_new (TRUE,1);
	
	gtk_box_pack_start( GTK_BOX(kb), hbox1, TRUE, TRUE, 0);
	gtk_box_pack_start( GTK_BOX(kb), hbox2, TRUE, TRUE, 0);
	gtk_box_pack_start( GTK_BOX(kb), hbox3, TRUE, TRUE, 0);
		
	// Alpha keys
    for(int i = 0; i < ALPHA_KEYS; i++)
    {
		kb->alphaButtons[i] = gtk_button_new_with_label(enumText[i]);
		
		// Place in the correct HBox
        if(i < TOP_ROW_COUNT)
			gtk_box_pack_start( GTK_BOX(hbox1), kb->alphaButtons[i], TRUE, TRUE, 0);
        else if(i < MID_ROW_COUNT)
			gtk_box_pack_start( GTK_BOX(hbox2), kb->alphaButtons[i], TRUE, TRUE, 0);
        else
			gtk_box_pack_start( GTK_BOX(hbox3), kb->alphaButtons[i], TRUE, TRUE, 0);
		
		// Attach a signal
		g_signal_connect (GTK_OBJECT (kb->alphaButtons[i]), "clicked",
							G_CALLBACK (alpha_clicked),(gpointer) kb);

		gtk_widget_show(kb->alphaButtons[i]);
    }

	kb->deleteButton = gtk_button_new_with_label("<--");
	gtk_box_pack_start( GTK_BOX(hbox3), kb->deleteButton, TRUE, TRUE, 0);
	g_signal_connect(GTK_OBJECT(kb->deleteButton), "clicked",
						G_CALLBACK (delete_clicked),(gpointer)kb);
}


GtkWidget* keyboardGTK_new ()
{
	return GTK_WIDGET ( g_object_new (keyboardGTK_get_type (), NULL));
}

void keyboardGTK_clear (KeyboardGTK *kb)
{
	int i;
	
	for (i=0;i<ALPHA_KEYS;i++)
	{
		g_signal_handlers_block_matched (G_OBJECT (kb->alphaButtons[i]), 
                                         G_SIGNAL_MATCH_DATA,
                                         0, 0, NULL, NULL, kb);		
		//gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (kb->alphaButtons[i]),FALSE);
		g_signal_handlers_unblock_matched (G_OBJECT (kb->alphaButtons[i]),
                                           G_SIGNAL_MATCH_DATA,
                                           0, 0, NULL, NULL, kb);
	}
}

static void alpha_clicked (GtkWidget *widget, KeyboardGTK *kb)
{
	int i;
	
	if(kb->deleteButton == widget)
	{
		kb->activeKey = const_cast <char*>(enumText[ALPHA_KEYS+1]);
		g_signal_emit ( kb,
						keyboardGTK_signals[KEYBOARDGTK_SIGNAL], 0);
		return;
	}

	for (i=0;i<ALPHA_KEYS;i++)
	{
		if(kb->alphaButtons[i] == widget)
		{
			kb->activeKey =const_cast <char*>(enumText[i]);
			// emit the signal with the char pressed
			g_signal_emit ( kb,
							keyboardGTK_signals[KEYBOARDGTK_SIGNAL], 0);
			break;
		}
	}    
}

static void delete_clicked (GtkWidget *widget, KeyboardGTK *kb)
{
	g_signal_emit ( kb,
					keyboardGTK_signals[DELETE_SIGNAL], 0);
	    
}
