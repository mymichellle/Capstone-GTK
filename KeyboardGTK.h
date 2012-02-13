#ifndef KeyboardGTK_h
#define KeyboardGTK_h

#include <gtk/gtk.h>
#include <gtk/gtkvbox.h>

#ifdef __cplusplus
extern "C"{
#endif /*__cplusplus*/
	
#define KEYBOARDGTK(obj)			GTK_CHECK_CAST (obj, keyboardGTK_get_type (), KeyboardGTK)
#define KEYBOARDGTK_CLASS(klass)	GTK_CHECK_CLASS_CAST (klass, keyboardGTK_get_type (), KeyboardGTKClass)
#define	IS_KEYBOARDGTK(obj)			GTK_CHECK_TYPE (obj, keyboardGTK_get_type ())
	
	
#define ALPHA_KEYS 26
#define SPECIAL_KEYS 3
	
typedef struct _KeyboardGTK			KeyboardGTK;
typedef struct _KeyboardGTKClass 	KeyboardGTKClass;

struct _KeyboardGTK
{
	GtkVBox vbox;
	
	GtkWidget *alphaButtons[ALPHA_KEYS];
};

struct _KeyboardGTKClass
{
	GtkVBoxClass parent_class;
	
	void (* keyboardGTK)(KeyboardGTK *kb);
};

guint 		keyboardGTK_get_type	(void);
GtkWidget* 	keyboardGTK_new			(void);
void 		keyboardGTK_clear		(KeyboardGtk *kb);
	
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif KeyboardGTK_h