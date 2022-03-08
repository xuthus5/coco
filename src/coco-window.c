/* coco-window.c
 *
 * Copyright 2022 xuthus5
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name(s) of the above copyright
 * holders shall not be used in advertising or otherwise to promote the sale,
 * use or other dealings in this Software without prior written
 * authorization.
 */

#include "coco-config.h"
#include "coco-window.h"
#include "components/base64/base64.h"
#include "components/time_convert/time_convert.h"
#include "components/clipboard/clipboard.h"
#include "components/upyun_upload/upyun.h"
#include <adwaita.h>

struct _CocoWindow {
    AdwApplicationWindow parent_instance;
    AdwFlap            *flap;
};

G_DEFINE_TYPE ( CocoWindow, coco_window, ADW_TYPE_APPLICATION_WINDOW )

static void
coco_window_class_init ( CocoWindowClass *klass ) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS ( klass );

    gtk_widget_class_set_template_from_resource ( widget_class, "/cc/xuthus/coco/coco-window.ui" );
    gtk_widget_class_bind_template_child ( widget_class, CocoWindow, flap );
}

static void
coco_window_init ( CocoWindow *self ) {
    g_type_ensure ( COCO_TYPE_BASE64 );
    g_type_ensure ( COCO_TYPE_TIME_CONVERT );
    g_type_ensure ( COCO_TYPE_CLIPBOARD );
    g_type_ensure ( COCO_TYPE_UPYUN_UPLOAD );
    gtk_widget_init_template ( GTK_WIDGET ( self ) );
}
