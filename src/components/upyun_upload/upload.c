#include <gtk/gtk.h>
#include <adwaita.h>
#include <libgen.h>
#include <time.h>
#include <curl/curl.h>
#include <json.h>
#include "upload.h"
#include <request.h>

struct _CocoUpYunUpload {
	AdwBin parent_instance;
	GtkEntry * current_path;
	GtkButton * go_back;
	GtkButton * go_to;
	GtkListBox *file_list;
};

G_DEFINE_TYPE(CocoUpYunUpload, coco_upyun_upload, ADW_TYPE_BIN)

char * get_current_path(GtkEntry * current_path)
{
    GtkEntryBuffer * buffer = gtk_entry_get_buffer (current_path);
    if (buffer == NULL)
        {
            return "/";
        }
    return gtk_entry_buffer_get_text (buffer);
}

static void get_file_list_from_path(GtkButton *button, CocoUpYunUpload * self )
{
    // rebuild
    while ( 1 ) {
        GtkListBoxRow *row = gtk_list_box_get_row_at_index ( self->file_list, 0 );
        if ( row == NULL ) {
            break;
        }
        gtk_list_box_remove ( self->file_list, ( GtkWidget * ) row );
    }

    struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json; charset: utf-8");
    char url[100];
    sprintf (url, "https://central.xuthus.cc/api/storage/upyun/list?path=%s", get_current_path(self->current_path));
	char *list_response = get_response(url, headers);

	printf("%s\n", list_response);

	if (list_response == NULL) {
		printf("接口调用出错,程序退出.\n");
		return;
	}
	json_object *response_json = json_tokener_parse(list_response);

	free(list_response);

	json_object *errcode;
	json_object *errmsg;

	json_object_object_get_ex(response_json, "err_code", &errcode);
	json_object_object_get_ex(response_json, "err_msg", &errmsg);
	if (json_object_get_int64(errcode) != 0) {
		printf("接口调用出错: %s,程序退出.\n", json_object_get_string(errmsg));
		return;
	}
	json_object * top_data;

	json_object_object_get_ex(response_json, "data", &top_data);
	json_object * list_data;

	json_object_object_get_ex(top_data, "data", &list_data);
	int len = json_object_array_length(list_data);

	for (int i = 0; i < len; i++) {
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
		if (json_object_get_boolean(file_type)) {
			adw_action_row_set_icon_name(file_node, "folder");
		} else {
			adw_action_row_set_icon_name(file_node, "text-x-generic");
		}
		/* GtkButton * suffix_copy = gtk_button_new(); */
		/* gtk_button_set_icon_name(suffix_copy, "edit-copy"); */
		/* adw_action_row_add_suffix(file_node, suffix_copy); */
		/* GtkStyleContext * style = gtk_widget_get_style_context(suffix_copy); */
		/* gtk_style_context_add_class(style, "flat"); */
		/* adw_action_row_set_activatable_widget(file_node, suffix_copy); */
		gtk_list_box_append(self->file_list, file_node);
	}
}

void set_current_path(GtkEntry * current_path, const char * path)
{
    GtkEntryBuffer * buffer = gtk_entry_get_buffer (current_path);
    gtk_entry_buffer_set_text (buffer, path, -1);
}

static void get_and_set_parent_path(GtkButton *button, CocoUpYunUpload * self)
{
    char now_path[100];
    GtkEntryBuffer * buffer = gtk_entry_get_buffer (self->current_path);
    if (buffer == NULL)
        {
            strcpy (now_path, "/");
        } else {
            strcpy (now_path, gtk_entry_buffer_get_text (buffer));
        }

    const char * parent_path = dirname(now_path);

    set_current_path (self->current_path, parent_path);
}

static void
coco_upyun_upload_class_init(CocoUpYunUploadClass *klass)
{
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

	gtk_widget_class_set_template_from_resource(widget_class, "/cc/xuthus/coco/components/upyun_upload/upload.ui");
	gtk_widget_class_bind_template_child( widget_class, CocoUpYunUpload, file_list );
	gtk_widget_class_bind_template_child( widget_class, CocoUpYunUpload, current_path );
	gtk_widget_class_bind_template_child( widget_class, CocoUpYunUpload, go_back );
	gtk_widget_class_bind_template_child( widget_class, CocoUpYunUpload, go_to );
}

static void
coco_upyun_upload_init(CocoUpYunUpload *self)
{
	gtk_widget_init_template(GTK_WIDGET(self));
    g_signal_connect ( self->go_back, "clicked", G_CALLBACK ( get_and_set_parent_path ), self );
    g_signal_connect ( self->go_to, "clicked", G_CALLBACK ( get_file_list_from_path ), self );

	set_current_path (self->current_path, "/");

    get_file_list_from_path (NULL, self);
}

