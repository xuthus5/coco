#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define COCO_TYPE_LOGIN (coco_login_get_type())

G_DECLARE_FINAL_TYPE ( CocoLogin, coco_login, COCO, LOGIN, AdwBin )

G_END_DECLS

CocoLogin * coco_login_new();
