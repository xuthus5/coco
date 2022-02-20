#include "base64.h"

struct _CocoBase64
{
  AdwBin parent_instance;
  GtkTextView *text_view;
};

G_DEFINE_TYPE (CocoBase64, coco_base64, ADW_TYPE_BIN)

static void
coco_base64_class_init (CocoBase64Class *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/cc/xuthus/coco/components/base64/base64.ui");
  gtk_widget_class_bind_template_child(widget_class, CocoBase64, text_view);
}

static void
coco_base64_init (CocoBase64 *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}

