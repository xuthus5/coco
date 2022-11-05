#pragma once

#include <gtk/gtk.h>
#include <libadwaita-1/adwaita.h>

G_BEGIN_DECLS

#define UPLOAD_TYPE_WINDOW (upload_window_get_type())

G_DECLARE_FINAL_TYPE(UploadWindow, upload_window, UPLOAD, WINDOW, AdwApplicationWindow)

G_END_DECLS
