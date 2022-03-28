#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <adwaita.h>
#include "hash.h"

struct _CocoHash {
    AdwBin parent_instance;
};

G_DEFINE_TYPE ( CocoHash, coco_hash, ADW_TYPE_BIN)

static void coco_hash_class_init(CocoHashClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    gtk_widget_class_set_template_from_resource(widget_class, "/cc/xuthus/coco/components/hash/hash.ui");
}

static void
coco_hash_init(CocoHash *self) {
    gtk_widget_init_template(GTK_WIDGET(self));
}
