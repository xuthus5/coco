#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <adwaita.h>
#include <curl/curl.h>
#include <json.h>
#include "clipboard.h"

struct _CocoClipboard {
  AdwBin parent_instance;
  GtkButton *pull_data;
  GtkButton *push_data;
  GtkListBox *clipboard_list;
};

G_DEFINE_TYPE(CocoClipboard, coco_clipboard, ADW_TYPE_BIN)

typedef struct {
  char *data;
  size_t size;
} memory_response;

size_t memory_response_write_func(void *src, size_t size, size_t nmemb, memory_response *dst)
{
  size_t data_size = dst->size + size * nmemb;

  dst->data = realloc(dst->data, data_size + 1);
  if (dst->data == NULL)
    {
      fprintf(stderr, "realloc() failed\n");
      exit(EXIT_FAILURE);
    }
  memcpy(dst->data + dst->size, src, size * nmemb);
  dst->data[data_size] = '\0';
  dst->size = data_size;
  return size * nmemb;
}

void init_memory_response(memory_response *s)
{
  s->size = 0;
  s->data = malloc(s->size + 1);
  if (s->data == NULL)
    {
      fprintf(stderr, "malloc() failed\n");
      exit(EXIT_FAILURE);
    }
  s->data[0] = '\0';
}

char *get_response(char *url)
{
  CURL *curl;
  CURLcode res;
  memory_response resp;

  init_memory_response(&resp);
  curl = curl_easy_init();
  if (curl)
    {
      curl_easy_setopt(curl, CURLOPT_URL, url);
      curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
      curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, memory_response_write_func);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resp);

      res = curl_easy_perform(curl);
      curl_easy_cleanup(curl);
      return resp.data;
    }
  return NULL;
}

char *post_response(char *url, char * payload)
{
  CURL *curl;
  CURLcode res;
  memory_response resp;

  init_memory_response(&resp);
  curl = curl_easy_init();
  struct curl_slist *headers = NULL;

  headers = curl_slist_append(headers, "Accept: application/json");
  headers = curl_slist_append(headers, "Content-Type: application/json; charset: utf-8");
  if (curl)
    {
      curl_easy_setopt(curl, CURLOPT_URL, url);
      curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1);
      curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, memory_response_write_func);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resp);
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);

      res = curl_easy_perform(curl);
      curl_easy_cleanup(curl);
      return resp.data;
    }
  return NULL;
}

static void pull_clipboard_data(GtkButton *button, CocoClipboard *self)
{
  // rebuild  //
  while (1)
    {
      GtkListBoxRow *row = gtk_list_box_get_row_at_index(self->clipboard_list, 0);
      if (row == NULL)
        {
          break;
        }
      gtk_list_box_remove(self->clipboard_list, row);
    }
  char *clipboard_response = get_response("https://central.xuthus.cc/api/clipboard/list");

  if (clipboard_response == NULL)
    {
      printf("接口调用出错,程序退出.");
      return;
    }
  json_object *clipboard_response_json = json_tokener_parse(clipboard_response);

  free(clipboard_response);

  printf("%s\n", json_object_to_json_string(clipboard_response_json));

  json_object *errcode;
  json_object *errmsg;
  json_object *clipboard_data;
  json_object *clipboard_list;
  json_bool exist = json_object_object_get_ex(clipboard_response_json, "err_code", &errcode);

  if (exist == 1)
    {
      if (json_object_get_int(errcode) != 0)
        {
          json_object_object_get_ex(clipboard_response_json, "err_msg", &errmsg);
          printf("接口调用出错: %s\n", json_object_get_string(errmsg));
          return;
        }
      else
        {
          exist = json_object_object_get_ex(clipboard_response_json, "data", &clipboard_data);
          if (exist == 1)
            {
              exist = json_object_object_get_ex(clipboard_data, "data", &clipboard_list);
              if (exist != 1)
                {
                  AdwActionRow *empty_node = adw_action_row_new();
                  adw_action_row_set_title_lines(empty_node, "list empty...");
                  gtk_list_box_append(self->clipboard_list, empty_node);
                  return;
                }

              int len = json_object_array_length(clipboard_list);
              for (int i = 0; i < len; i++)
                {
                  json_object *item = json_object_array_get_idx(clipboard_list, i);
                  json_object *content_data;
                  json_object_object_get_ex(item, "content", &content_data);
                  AdwActionRow *clipboard_node = adw_action_row_new();
                  adw_preferences_row_set_title(clipboard_node, json_object_get_string(content_data));
                  gtk_list_box_append(self->clipboard_list, clipboard_node);
                }
            }
        }
    }
  else
    {
      printf("接口请求内容: %s\n接口调用出错,程序退出.", clipboard_response);
      return;
    }
}



void get_text_from_clipboard(GdkClipboard *clipboard, GAsyncResult *res, gpointer user_data)
{
  GError *error = NULL;
  char *data = gdk_clipboard_read_text_finish(clipboard, res, &error);

  if (error != NULL)
    {
      printf("%s\n", error->message);
      return;
    }
  if (strlen(data) == 0)
    {
      return;
    }

  json_object * _tbody = json_object_new_object();

  json_object_object_add(_tbody, "text", json_object_new_string(data));
  const char * payload = json_object_to_json_string(_tbody);

  char *add_response = post_response("https://central.xuthus.cc/api/clipboard/add", payload);

  printf("%s\n", add_response);
}

void set_text_to_clipboard()
{
}

static void push_clipboard_data(GtkButton *button, CocoClipboard *self)
{
  GdkClipboard *clipboard = gtk_widget_get_clipboard(button);

  gdk_clipboard_read_text_async(clipboard, NULL, get_text_from_clipboard, NULL);
}

static void
coco_clipboard_class_init(CocoClipboardClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

  gtk_widget_class_set_template_from_resource(widget_class, "/cc/xuthus/coco/components/clipboard/clipboard.ui");
  gtk_widget_class_bind_template_child(widget_class, CocoClipboard, pull_data);
  gtk_widget_class_bind_template_child(widget_class, CocoClipboard, push_data);
  gtk_widget_class_bind_template_child(widget_class, CocoClipboard, clipboard_list);
}

static void
coco_clipboard_init(CocoClipboard *self)
{
  gtk_widget_init_template(GTK_WIDGET(self));
  g_signal_connect(self->pull_data, "clicked", G_CALLBACK(pull_clipboard_data), self);
  g_signal_connect(self->push_data, "clicked", G_CALLBACK(push_clipboard_data), self);

  g_signal_connect(self,"key-press-event",G_CALLBACK(),NULL);
  g_signal_connect(self,"key-release-event",G_CALLBACK(gtk_window_propagate_key_event),NULL);

  char *clipboard_response = get_response("https://central.xuthus.cc/api/clipboard/list");

  if (clipboard_response == NULL)
    {
      printf("接口调用出错,程序退出.\n");
      return;
    }
  json_object *clipboard_response_json = json_tokener_parse(clipboard_response);

  free(clipboard_response);

  printf("%s\n", json_object_to_json_string(clipboard_response_json));

  json_object *errcode;
  json_object *errmsg;
  json_object *clipboard_data;
  json_object *clipboard_list;
  json_bool exist = json_object_object_get_ex(clipboard_response_json, "err_code", &errcode);

  if (exist == 1)
    {
      if (json_object_get_int(errcode) != 0)
        {
          json_object_object_get_ex(clipboard_response_json, "err_msg", &errmsg);
          printf("接口调用出错: %s\n", json_object_get_string(errmsg));
          return;
        }
      else
        {
          exist = json_object_object_get_ex(clipboard_response_json, "data", &clipboard_data);
          if (exist == 1)
            {
              exist = json_object_object_get_ex(clipboard_data, "data", &clipboard_list);
              if (exist != 1)
                {
                  AdwActionRow *empty_node = adw_action_row_new();
                  adw_action_row_set_title_lines(empty_node, "list empty...");
                  gtk_list_box_append(self->clipboard_list, empty_node);
                  return;
                }

              int len = json_object_array_length(clipboard_list);
              for (int i = 0; i < len; i++)
                {
                  json_object *item = json_object_array_get_idx(clipboard_list, i);
                  json_object *content_data;
                  json_object_object_get_ex(item, "content", &content_data);
                  AdwActionRow *clipboard_node = adw_action_row_new();
                  adw_preferences_row_set_title(clipboard_node, json_object_get_string(content_data));
                  gtk_list_box_append(self->clipboard_list, clipboard_node);
                }
            }
        }
    }
  else
    {
      AdwActionRow *empty_node = adw_action_row_new();
      adw_action_row_set_title_lines(empty_node, "list empty...");
      gtk_list_box_append(self->clipboard_list, empty_node);
      printf("接口请求内容: %s\n接口调用出错,程序退出.", clipboard_response);
      return;
    }

  return;
}
