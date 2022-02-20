#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define COCO_TYPE_CLIPBOARD (coco_clipboard_get_type())

G_DECLARE_FINAL_TYPE (CocoClipboard, coco_clipboard, COCO, CLIPBOARD, AdwBin)

G_END_DECLS

