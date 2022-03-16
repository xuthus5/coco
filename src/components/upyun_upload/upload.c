#include <gtk/gtk.h>
#include <adwaita.h>
#include <time.h>
#include <curl/curl.h>
#include <json.h>
#include "upload.h"
#include <request.h>

struct _CocoUpYunUpload {
	AdwBin parent_instance;
	char * current_path;
	GtkListBox *file_list;
};

G_DEFINE_TYPE(CocoUpYunUpload, coco_upyun_upload, ADW_TYPE_BIN)

static void
coco_upyun_upload_class_init(CocoUpYunUploadClass *klass)
{
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

	gtk_widget_class_set_template_from_resource(widget_class, "/cc/xuthus/coco/components/upyun_upload/upload.ui");
    gtk_widget_class_bind_template_child ( widget_class, CocoUpYunUpload, file_list );
}

static void
coco_upyun_upload_init(CocoUpYunUpload *self)
{
	gtk_widget_init_template(GTK_WIDGET(self));

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json; charset: utf-8");
	char *list_response = get_response("https://central.xuthus.cc/api/storage/upyun/list", headers);

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
		GtkButton * suffix_copy = gtk_button_new();
		gtk_button_set_icon_name(suffix_copy, "edit-copy");
		adw_action_row_add_suffix(file_node, suffix_copy);
		GtkStyleContext * style = gtk_widget_get_style_context(suffix_copy);
		gtk_style_context_add_class(style, "flat");
		adw_action_row_set_activatable_widget(file_node, suffix_copy);
		gtk_list_box_append(self->file_list, file_node);
	}
}

