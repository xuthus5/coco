#include "clipboard.h"

struct _CocoClipboard
{
  AdwBin parent_instance;
};

G_DEFINE_TYPE (CocoClipboard, coco_clipboard, ADW_TYPE_BIN)

static void
coco_clipboard_class_init (CocoClipboardClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/cc/xuthus/coco/components/clipboard/clipboard.ui");
}

static void
coco_clipboard_init (CocoClipboard *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}

