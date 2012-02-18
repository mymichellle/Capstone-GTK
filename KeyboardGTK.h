#ifndef KeyboardGTK_h
#define KeyboardGTK_h

#include <glib.h>

G_BEGIN_DECLS
	
#define KEYBOARDGTK_TYPE            (keyboardGTK_get_type ())
#define KEYBOARDGTK(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), KEYBOARDGTK_TYPE, KeyboardGTK))
#define KEYBOARDGTK_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), KEYBOARDGTK_TYPE, KeyboardGTKClass))
#define IS_KEYBOARDGTK(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), KEYBOARDGTK_TYPE))
#define IS_KEYBOARDGTK_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), KEYBOARDGTK_TYPE))

#define ALPHA_KEYS 26
#define SPECIAL_KEYS 3
	
typedef struct _KeyboardGTK			KeyboardGTK;
typedef struct _KeyboardGTKClass 	KeyboardGTKClass;

struct _KeyboardGTK
{
	GtkVBox vbox;
	
	GtkWidget *alphaButtons[ALPHA_KEYS];
	GtkWidget *deleteButton;

	gchar* activeKey;
};

struct _KeyboardGTKClass
{
	GtkVBoxClass parent_class;
	
	void (* keyboardGTK)(KeyboardGTK *kb);
};

GType 		keyboardGTK_get_type	(void);
GtkWidget* 	keyboardGTK_new			(void);
void 		keyboardGTK_clear		(KeyboardGTK *kb);

G_END_DECLS
#endif /*KeyboardGTK_h*/
