#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <adwaita.h>
#include <curl/curl.h>
#include <json.h>
#include <request.h>
#include "clipboard.h"

struct _CocoClipboard {
    AdwBin parent_instance;
    GtkButton *pull_data;
    GtkButton *push_data;
    GtkListBox *clipboard_list;
    AdwToastOverlay *toast_overlay;
};

G_DEFINE_TYPE ( CocoClipboard, coco_clipboard, ADW_TYPE_BIN
)

static void get_clipboard_list(CocoClipboard *self)
{
    printf("%s\n", "get clipboard list");
    while (1) {
        GtkListBoxRow *row = gtk_list_box_get_row_at_index(self->clipboard_list, 0);
        if (row == NULL) {
            break;
        }
        gtk_list_box_remove(self->clipboard_list, (GtkWidget *) row);
    }

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json; charset: utf-8");
    char *clipboard_response = get_response("https://central.xuthus.cc/api/clipboard/list?page_size=20", headers);
    /* curl_slist_free_all(headers); */
    if (clipboard_response == NULL) {
        printf("接口调用出错,程序退出.\n");
        return;
    }

    json_object *clipboard_response_json = json_tokener_parse(clipboard_response);
    free(clipboard_response);

    json_object *errcode;
    json_object *errmsg;
    json_object *clipboard_data;
    json_object *clipboard_list;
    json_bool exist = json_object_object_get_ex(clipboard_response_json, "err_code", &errcode);

    if (exist == 1) {
        if (json_object_get_int(errcode) != 0) {
            json_object_object_get_ex(clipboard_response_json, "err_msg", &errmsg);
            printf("接口调用出错: %s\n", json_object_get_string(errmsg));
            return;
        }
        exist = json_object_object_get_ex(clipboard_response_json, "data", &clipboard_data);
        if (exist == 1) {
            exist = json_object_object_get_ex(clipboard_data, "data", &clipboard_list);
            if (exist != 1) {
                AdwActionRow *empty_node = adw_action_row_new();
                adw_preferences_row_set_title(empty_node, "list empty...");
                gtk_list_box_append(self->clipboard_list, empty_node);
                return;
            }
            int len = json_object_array_length(clipboard_list);
            for (int i = 0; i < len; i++) {
                json_object *item = json_object_array_get_idx(clipboard_list, i);
                json_object *content_data;
                json_object_object_get_ex(item, "content", &content_data);
                AdwActionRow *clipboard_node = adw_action_row_new();
                gchar * title = g_markup_escape_text(json_object_get_string(content_data), -1);
                adw_preferences_row_set_title(clipboard_node, title);
                GtkLabel *copy_label = gtk_label_new ("");
                adw_action_row_set_activatable_widget(clipboard_node, copy_label);
                gtk_list_box_append(self->clipboard_list, clipboard_node);
            }
        }
        return;
    }
    // get empty list
    AdwActionRow *empty_node = adw_action_row_new();
    adw_preferences_row_set_title(empty_node, "list empty...");
    gtk_list_box_append(self->clipboard_list, empty_node);
    printf("接口请求内容: %s\n接口调用出错,程序退出.", clipboard_response);
    return;
}

static void clipboard_row_activate(GtkListBox *list, AdwActionRow *row, gpointer user_data) {
    const char *title = adw_preferences_row_get_title(&row->parent_instance);
    GdkClipboard *clipboard = gtk_widget_get_clipboard((GtkWidget *) row);

    gdk_clipboard_set_text(clipboard, title);
    printf("copied: %s\n", title);
}

static void pull_clipboard_data(GtkButton *button, CocoClipboard *self) {
    get_clipboard_list (self);
}

void get_text_from_clipboard(GdkClipboard *clipboard, GAsyncResult *res, CocoClipboard *self) {
    GError *error = NULL;
    char *data = gdk_clipboard_read_text_finish(clipboard, res, &error);

    if (error != NULL) {
        printf("%s\n", error->message);
        return;
    }
    if (strlen(data) == 0) {
        return;
    }

    json_object *_tbody = json_object_new_object();

    json_object_object_add(_tbody, "text", json_object_new_string(data));
    const char *payload = json_object_to_json_string(_tbody);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json; charset: utf-8");

    char *add_response = post_response("https://central.xuthus.cc/api/clipboard/add", payload, headers);
    /* curl_slist_free_all(headers); */
    printf("%s\n", add_response);

    json_object *add_response_json = json_tokener_parse(add_response);
    json_object *errcode;
    json_object *errmessag;
    json_bool errcode_exist = json_object_object_get_ex(add_response_json, "err_code", &errcode);
    json_bool errmsg_exist = json_object_object_get_ex(add_response_json, "err_msg", &errmessag);
    AdwToast *toast;
    if (errcode_exist == 1) {
        if (json_object_get_int64(errcode) == 0) {
            toast = (AdwToast *) adw_toast_new("推送成功! 请刷新数据.");
        } else {
            if (errmsg_exist == 1) {
                const char *errmsg_string = json_object_get_string(errmessag);
                toast = (AdwToast *) adw_toast_new(errmsg_string);
            } else {
                toast = (AdwToast *) adw_toast_new("未知错误!请稍后重试...");
            }
        }
    } else {
        toast = (AdwToast *) adw_toast_new("网络异常!请稍后重试...");
    }
    adw_toast_set_timeout(toast, 3);
    adw_toast_overlay_add_toast(self->toast_overlay, toast);
}

static void push_clipboard_data(GtkButton *button, CocoClipboard *self) {
    GdkClipboard *clipboard = gtk_widget_get_clipboard(button);

    gdk_clipboard_read_text_async(clipboard, NULL, get_text_from_clipboard, self);
}

static void
coco_clipboard_class_init(CocoClipboardClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/cc/xuthus/coco/components/clipboard/clipboard.ui");
    gtk_widget_class_bind_template_child(widget_class, CocoClipboard, pull_data);
    gtk_widget_class_bind_template_child(widget_class, CocoClipboard, push_data);
    gtk_widget_class_bind_template_child(widget_class, CocoClipboard, clipboard_list);
    gtk_widget_class_bind_template_child(widget_class, CocoClipboard, toast_overlay);
}

static void
coco_clipboard_init(CocoClipboard *self) {
    gtk_widget_init_template(GTK_WIDGET(self));
    g_signal_connect(self->pull_data, "clicked", G_CALLBACK(pull_clipboard_data), self);
    g_signal_connect(self->push_data, "clicked", G_CALLBACK(push_clipboard_data), self);
    g_signal_connect(self->clipboard_list, "row-activated", G_CALLBACK(clipboard_row_activate), self);

    get_clipboard_list (self);
    printf("%s\n", "clipboard init");
}
