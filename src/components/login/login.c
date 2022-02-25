#include <gtk/gtk.h>
#include <adwaita.h>
#include "login.h"

struct _CocoLogin {
    AdwApplicationWindow parent_instance;
};

G_DEFINE_TYPE ( CocoLogin, coco_login, ADW_TYPE_APPLICATION_WINDOW )

static void
coco_login_class_init ( CocoLoginClass *klass ) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS ( klass );

    gtk_widget_class_set_template_from_resource ( widget_class, "/cc/xuthus/coco/components/coco/login.ui" );
}

static void
coco_login_init ( CocoLogin *self ) {
    gtk_widget_init_template ( GTK_WIDGET ( self ) );
}

CocoLogin * coco_login_new() {
    CocoLogin * obj = g_object_new ( COCO_TYPE_LOGIN, NULL );
    return obj;
}
