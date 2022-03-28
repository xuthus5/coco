#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define COCO_TYPE_PASSWORD_GENERATE (coco_password_generate_get_type())

G_DECLARE_FINAL_TYPE(CocoPasswordGenerate, coco_password_generate, COCO, PASSWORD_GENERATE, AdwBin)

G_END_DECLS

