#pragma once
#include <glib-object.h>
#include <gtk/gtk.h>
#include <libadwaita-1/adwaita.h>

G_BEGIN_DECLS

#define UPLOAD_TYPE_UPYUN (upload_upyun_get_type())

G_DECLARE_FINAL_TYPE(UploadUpyun, upload_upyun, UPLOAD, UPYUN, AdwBin)

G_END_DECLS
