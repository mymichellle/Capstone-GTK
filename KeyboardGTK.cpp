#include "KeyboardGTK.h"

#define TOP_ROW_COUNT 10
#define MID_ROW_COUNT 19
#define BUTTON_DIM 40
#define BTM_ROW_HEIGHT BUTTON_DIM/2 + BUTTON_DIM
#define MID_ROW_HEIGHT BTM_ROW_HEIGHT + BUTTON_DIM
#define TOP_ROW_HEIGHT MID_ROW_HEIGHT + BUTTON_DIM
#define SPACE_KEY 0
#define BACKSPACE_KEY 1
#define CLOSE_KEY 2

const char* enumText[] = { "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "A", "S", "D", "F", "G", "H", "J", "K", "L", "Z", "X", "C", "V", "B", "N", "M", " ", "*"};


guint keyboardGTK_get_type ()
{
	static guint kb_type = 0;
	
	if(!kb_type)
	{
		GtkTypeInfo kb_info = 
		{
			"KeyboardGTK",
			sizeof (KeyboardGTK),
			sizeof (KeyboardGTKClass),
			(GtkClassInitFunc) keyboardGTK_class_init,
			(GtkObjectInitFunc) keyboardGTK_init,
			(GtkArgSetFunc) NULL,
			(GtkArgGetFunc) NULL
		};
		
		kb_type = gtk_type_unique (gtk_vbox_get_type (), &kb_info);
	}
	return kb_type;
}

enum {
	KEYBOARDGTK_SIGNAL,
	LAST_SIGNAL
};

static gint keyboardGTK_signals[LAST_SIGNAL] = {0};

static void keyboardGTK_class_init (KeyboardGTKClass *class)
{
	GtkObjectClass *object_class;
	
	object_class = (GtkObjectClass*) class;
	
	keyboardGTK_signals[KEYBOARDGTK_SIGNAL] = gtk_signal_new ("keyboardGTK",
															  GTK_RUN_FIRST,
															  object_class->type,
															  GTK_SIGNAL_OFFSET (KeyboardGTKClass, keyboardGTK),
															  gtk_signal_default_marshaller, GTK_TYPE_NONE, 1,GTK_TYPE_CHAR);
	
	gtk_object_class_add_signals (object_class, keyboardGTK_signals, LAST_SIGNAL);
	
	class->keyboardGTK = NULL;
}

static void keyboardGTK_init (KeyboardGTK *kb)
{
	GtkWidget *vbox;
	gint i;
	
	hbox1 = gtk_hbox_new (TRUE,1);
	hbox2 = gtk_hbox_new (TRUE,1);
	hbox3 = gtk_hbox_new (TRUE,1);
	
	gtk_box_pack_start( GTK_BOX(kb), hbox1, TRUE, TRUE, 0);
	gtk_box_pack_start( GTK_BOX(kb), hbox2, TRUE, TRUE, 0);
	gtk_box_pack_start( GTK_BOX(kb), hbox3, TRUE, TRUE, 0);
	
	gtk_widget_show (hbox1);
	gtk_widget_show (hbox2);
	gtk_widget_show (hbox3);
	
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
		gtk_signal_connect (GTK_OBJECT (kb->alphaButtons[i]), "clicked",
							GTK_SIGNAL_FUNC (alpha_clicked), kb);
    }
}

GtkWidget* keyboardGTK_new ()
{
	return GTK_WIDGET ( gtk_type_new (keyboardGTK_get_type ()));
}

void keyboardGTK_clear (KeyboardGTK *kb)
{
	int i;
	
	for (i=0;i<ALPHA_KEYS;i++)
	{
		gtk_signal_handler_block_by_data (GTK_OBJECT(kb->alphaButtons[i]), kb);
		//gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (kb->alphaButtons[i]),FALSE);
		gtk_signal_handler_unblock_by_data (GTK_OBJECT(kb->alphaButtons[i]), kb);
	}
}

static void alpha_clicked (GtkWidget *widget, KeyboardGTK *kb)
{
	int i;
	
	for (i=0;i<ALPHA_KEYS;i++)
	{
		if(kb->alphaButtons[i] == widget)
		{
			// emit the signal with the char pressed
			gtk_signal_emit (GTK_OBJECT (kb),
							 keyboardGTK_signals[KEYBOARDGTK_SIGNAL], 
							 enumText[i]);
			break;
		}
	}    
}