#include <gtk/gtk.h>
#include <libadwaita-1/adwaita.h>
#include <json.h>
#include <curl/curl.h>
#include "upload.h"

struct _UploadUpyun
{
    AdwBin parent_instance;
    GtkButton *go_home;
    GtkButton *go_to;
    GtkButton *upload_button;
    GtkListBox *file_list;
    GtkEntry *current_path;
};

G_DEFINE_TYPE(UploadUpyun, upload_upyun, ADW_TYPE_BIN)

static void upload_upyun_class_init(UploadUpyunClass *klass)
{
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    gtk_widget_class_set_template_from_resource(widget_class, "/com/github/xuthus5/uploader/upload.ui");
    gtk_widget_class_bind_template_child(widget_class, UploadUpyun, go_home);
    gtk_widget_class_bind_template_child(widget_class, UploadUpyun, go_to);
    gtk_widget_class_bind_template_child(widget_class, UploadUpyun, upload_button);
    gtk_widget_class_bind_template_child(widget_class, UploadUpyun, current_path);
    gtk_widget_class_bind_template_child(widget_class, UploadUpyun, file_list);
}

char *get_current_path(GtkEntry *current_path)
{
    GtkEntryBuffer *buffer = gtk_entry_get_buffer(current_path);

    if (buffer == NULL)
    {
        return "/";
    }
    return gtk_entry_buffer_get_text(buffer);
}

void set_current_path(GtkEntry *current_path, const char *path)
{
    GtkEntryBuffer *buffer = gtk_entry_get_buffer(current_path);
    gtk_entry_buffer_set_text(buffer, path, -1);
}

void mkdir_to_server(const char *path)
{
    json_object *_tbody = json_object_new_object();

    json_object_object_add(_tbody, "path", json_object_new_string(path));
    const char *payload = json_object_to_json_string(_tbody);

    struct curl_slist *headers = NULL;

    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json; charset: utf-8");
    char *mkdir_response = post_response("https://central.xuthus.cc/api/storage/upyun/mkdir", payload, headers);
    /* curl_slist_free_all(headers); */
    printf("mkdir %s\n", mkdir_response);
}

static void get_file_list_from_path(GtkButton *button, UploadUpyun *self)
{
    while (1)
    {
        GtkListBoxRow *row = gtk_list_box_get_row_at_index(self->file_list, 0);
        if (row == NULL)
        {
            break;
        }
        gtk_list_box_remove(self->file_list, (GtkWidget *)row);
    }
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json; charset: utf-8");
    char url[100];
    sprintf(url, "https://central.xuthus.cc/api/storage/upyun/list?path=%s", get_current_path(self->current_path));
    char *list_response = get_response(url, headers);
    curl_slist_free_all(headers);

    if (list_response == NULL)
    {
        printf("接口调用出错,程序退出.\n");
        return;
    }

    printf("response: %s.\n", list_response);

    if (strlen(list_response) == 0) {
        printf("strlen == 0.\n");
        return;
    }

    json_object *response_json = json_tokener_parse(list_response);
    free(list_response);

    json_object *errcode;
    json_object *errmsg;
    json_object_object_get_ex(response_json, "err_code", &errcode);
    json_object_object_get_ex(response_json, "err_msg", &errmsg);
    if (json_object_get_int64(errcode) != 0)
    {
        printf("接口调用出错: %s,程序退出.\n", json_object_get_string(errmsg));
        return;
    }
    json_object *top_data;
    json_object_object_get_ex(response_json, "data", &top_data);
    json_object *list_data;

    json_bool exist = json_object_object_get_ex(top_data, "data", &list_data);
    if (exist == 0)
    {
        // create directory
        char *now_path = get_current_path(self->current_path);
        mkdir_to_server(now_path);
        return;
    }

    int len = json_object_array_length(list_data);
    for (int i = 0; i < len; i++)
    {
        json_object *item = json_object_array_get_idx(list_data, i);
        json_object *file_name;
        json_object *file_type;
        json_object *file_size;
        json_object *file_time;
        json_object_object_get_ex(item, "name", &file_name);
        json_object_object_get_ex(item, "is_dir", &file_type);
        json_object_object_get_ex(item, "size", &file_size);
        json_object_object_get_ex(item, "time", &file_time);
        AdwActionRow *file_node = adw_action_row_new();
        adw_preferences_row_set_title(file_node, json_object_get_string(file_name));
        if (json_object_get_boolean(file_type))
        {
            adw_action_row_set_icon_name(file_node, "folder");
        }
        else
        {
            adw_action_row_set_icon_name(file_node, "text-html");
        }
        GtkLabel *trigger = gtk_label_new("");
        adw_action_row_set_activatable_widget(file_node, trigger);
        gtk_list_box_append(self->file_list, file_node);
    }
}

static void goto_home_path(GtkButton *button, UploadUpyun *self)
{
    set_current_path(self->current_path, "/");
    get_file_list_from_path(NULL, self);
}

static void on_choose_file_response(GtkNativeDialog *native, int response, UploadUpyun *self)
{
    if (response == GTK_RESPONSE_ACCEPT)
    {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(native);
        g_autoptr(GFile) file = gtk_file_chooser_get_file(chooser);
        char *filename = g_file_get_basename(file);
        char *file_path = g_file_get_path(file);
        printf("filename %s, path: %s\n", filename, file_path);
        GFileInfo *file_info = g_file_query_info(file, G_FILE_ATTRIBUTE_STANDARD_SIZE, G_FILE_QUERY_INFO_NONE, NULL, NULL);
        guint64 file_size = g_file_info_get_attribute_uint64(file_info, G_FILE_ATTRIBUTE_STANDARD_SIZE);
        printf("file size %d\n", file_size);
        if (file_size >= 1024000)
        {
            // large size return
            return;
        }
        printf("now path %s\n", get_current_path(self->current_path));

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: multipart/form-data");

        struct curl_httppost *formpost = NULL;
        struct curl_httppost *lastptr = NULL;
        curl_formadd(&formpost,
                     &lastptr,
                     CURLFORM_COPYNAME, "file",
                     CURLFORM_FILE, file_path,
                     CURLFORM_END);

        curl_formadd(&formpost,
                     &lastptr,
                     CURLFORM_COPYNAME, "path",
                     CURLFORM_COPYCONTENTS, get_current_path(self->current_path),
                     CURLFORM_END);
        char *upload_response = post_form_response("https://central.xuthus.cc/api/storage/upyun/upload", formpost, headers);
        printf("upload %s\n", upload_response);
        curl_slist_free_all(headers);
        /* curl_formfree(formpost); */
        g_object_unref(file);
    }

    g_object_unref(native);
}

static void upload_to_path(GtkButton *button, UploadUpyun *self)
{
    GtkFileChooserNative *native = gtk_file_chooser_native_new("Open File", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, "_Open", "_Cancel");
    g_signal_connect(native, "response", G_CALLBACK(on_choose_file_response), self);
    gtk_native_dialog_show(GTK_NATIVE_DIALOG(native));
}

static void activate_file_item(GtkListBox *list, AdwActionRow *row, UploadUpyun *self)
{
    char *icon_name = adw_action_row_get_icon_name(row);
    char *filename = adw_preferences_row_get_title(row);
    printf("is_dir: %d, filename: %s\n", icon_name == "folder", filename);
    // if (strcmp(icon_name, "folder") == 0)
    // {
    //     char *t_path;
    //     char *current_path = get_current_path(self->current_path);
    //     if (strcmp(current_path, "/") == 0)
    //     {
    //         sprintf(t_path, "%s%s", get_current_path(self->current_path), filename);
    //     }
    //     else
    //     {
    //         sprintf(t_path, "%s/%s", get_current_path(self->current_path), filename);
    //     }
    //     printf("file path: %s\n", t_path);
    //     gtk_entry_set_buffer(self->current_path, gtk_entry_buffer_new("/golang", 8));
    //     // gtk_entry_buffer_set_text(buffer, t_path, -1);
    //     get_file_list_from_path(NULL, self);
    //     return;
    // }
    // copy link
    char *current_path = get_current_path(self->current_path);
    printf("file path: %s\n", current_path);
    const char *website = "https://central-storage.xuthus.cc";
    int buff_size = strlen(website) + strlen(current_path) + strlen(filename) + 2;
    char link[buff_size];
    if (strcmp(current_path, "/") == 0)
    {
        sprintf(link, "%s/%s", website, filename);
    }
    else
    {
        sprintf(link, "%s%s/%s", website, current_path, filename);
    }
    GdkClipboard *clipboard = gtk_widget_get_clipboard((GtkWidget *)row);
    gdk_clipboard_set_text(clipboard, link);
    printf("copied: %s\n", link);
    return;
}

static void upload_upyun_init(UploadUpyun *self)
{
    gtk_widget_init_template(GTK_WIDGET(self));
    g_signal_connect(self->go_home, "clicked", G_CALLBACK(goto_home_path), self);
    g_signal_connect(self->go_to, "clicked", G_CALLBACK(get_file_list_from_path), self);
    g_signal_connect(self->upload_button, "clicked", G_CALLBACK(upload_to_path), self);
    g_signal_connect(self->file_list, "row-activated", G_CALLBACK(activate_file_item), self);

    // default root path
    set_current_path(self->current_path, "/");
    get_file_list_from_path(NULL, self);
}
