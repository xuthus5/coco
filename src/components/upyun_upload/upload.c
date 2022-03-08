#include <gtk/gtk.h>
#include <adwaita.h>
#include <time.h>
#include "upload.h"

struct _CocoUpYunUpload {
    AdwBin parent_instance;
};

G_DEFINE_TYPE ( CocoUpYunUpload, coco_upyun_upload, ADW_TYPE_BIN )

static void
coco_upyun_upload_class_init ( CocoUpYunUploadClass *klass ) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS ( klass );

    gtk_widget_class_set_template_from_resource ( widget_class, "/cc/xuthus/coco/components/upyun_upload/upload.ui" );
}

static void
coco_upyun_upload_init ( CocoUpYunUpload *self ) {
    gtk_widget_init_template ( GTK_WIDGET ( self ) );
}
