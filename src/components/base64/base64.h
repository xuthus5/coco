#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define COCO_TYPE_BASE64 (coco_base64_get_type())

G_DECLARE_FINAL_TYPE (CocoBase64, coco_base64, COCO, BASE64, AdwBin)

G_END_DECLS

