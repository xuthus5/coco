#include <gtk/gtk.h>
#include <adwaita.h>
#include "password_generate.h"

struct _CocoPasswordGenerate {
    AdwBin parent_instance;
};

G_DEFINE_TYPE ( CocoPasswordGenerate, coco_password_generate, ADW_TYPE_BIN
)

static void
coco_password_generate_class_init(CocoPasswordGenerateClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    gtk_widget_class_set_template_from_resource(widget_class,
                                                "/cc/xuthus/coco/components/password_generate/password_generate.ui");
}

static void
coco_password_generate_init(CocoPasswordGenerate *self) {
    gtk_widget_init_template(GTK_WIDGET(self));
}
