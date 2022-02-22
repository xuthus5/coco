#include <gtk/gtk.h>
#include <adwaita.h>
#include <time.h>
#include "time_convert.h"

struct _CocoTimeConvert {
  AdwBin parent_instance;
  GtkEntry *bj_string;
  GtkEntryBuffer *bj_string_buffer;
  GtkButton *bj_string_button;
  GtkEntry *bj_unix;
  GtkEntryBuffer *bj_unix_buffer;
  GtkButton *bj_unix_button;
  GtkEntry *input_unix_time;
  GtkButton *convert_to_string_button;
  GtkEntry *output_string_time;
  GtkEntry *input_string_time;
  GtkButton *convert_to_unix_button;
  GtkEntry *output_unix_time;
};

G_DEFINE_TYPE(CocoTimeConvert, coco_time_convert, ADW_TYPE_BIN)

static void now_bj_string_button_clicked(GtkButton * button, CocoTimeConvert * self)
{
  GDateTime *time_now = g_date_time_new_now_local();

  gtk_entry_buffer_set_text(self->bj_string_buffer, g_date_time_format(time_now, "%Y-%m-%d %H:%M:%S"), -1);
  gtk_entry_set_buffer(self->bj_string, self->bj_string_buffer);
}
static void now_bj_unix_button_clicked(GtkButton * button, CocoTimeConvert * self)
{
  GDateTime *time_now = g_date_time_new_now_local();

  gtk_entry_buffer_set_text(self->bj_unix_buffer, g_date_time_format(time_now, "%s"), -1);
  gtk_entry_set_buffer(self->bj_unix, self->bj_unix_buffer);
}

static void convert_to_unix_clicked(GtkButton * button, CocoTimeConvert * self)
{
  GtkEntryBuffer * string_buffer = gtk_entry_get_buffer(self->input_string_time);
  char *format_time = gtk_entry_buffer_get_text(string_buffer);

  if (strlen(format_time) != 19)
    {
      return;
    }
  char cpy_time[19];

  strcpy(&cpy_time, format_time);

  printf("receive time: %s\n", cpy_time);

  cpy_time[4] = cpy_time[7] = cpy_time[10] = '\0';
  cpy_time[13] = cpy_time[16] = '\0';

  struct tm time_stamp = { 0 };

  time_stamp.tm_year = atoi(&cpy_time[0]) - 1900;
  time_stamp.tm_mon = atoi(&cpy_time[5]) - 1;
  time_stamp.tm_mday = atoi(&cpy_time[8]);
  time_stamp.tm_hour = atoi(&cpy_time[11]);
  time_stamp.tm_min = atoi(&cpy_time[14]);
  time_stamp.tm_sec = atoi(&cpy_time[17]);

  printf("year: %d, mon: %d, day: %d, hour: %d, min: %d, sec: %d\n",
         atoi(&cpy_time[0]), atoi(&cpy_time[5]), atoi(&cpy_time[8]),
         atoi(&cpy_time[11]), atoi(&cpy_time[14]), atoi(&cpy_time[17]));

  time_t t = mktime(&time_stamp);

  char *unix_data[10];

  sprintf(unix_data, "%d", t);
  GtkEntryBuffer * string_time_buffer = gtk_entry_get_buffer(self->output_unix_time);

  gtk_entry_buffer_set_text(string_time_buffer, unix_data, -1);
}

static void convert_to_string_clicked(GtkButton * button, CocoTimeConvert * self)
{
  GtkEntryBuffer * unix_time_buffer = gtk_entry_get_buffer(self->input_unix_time);
  gchar *unix_time = (gchar *)gtk_entry_buffer_get_text(unix_time_buffer);
  char *_tmp[10];

  strncpy((char *)_tmp, unix_time, 10);
  int ut = strtol(_tmp, NULL, 10);
  struct tm *ts;
  char buf[80];
  time_t raw_time = ut;

  ts = localtime(&raw_time);
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ts);
  GtkEntryBuffer * string_time_buffer = gtk_entry_get_buffer(self->output_string_time);

  gtk_entry_buffer_set_text(string_time_buffer, buf, -1);
}

static void
coco_time_convert_class_init(CocoTimeConvertClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

  gtk_widget_class_set_template_from_resource(widget_class, "/cc/xuthus/coco/components/time_convert/time_convert.ui");
  gtk_widget_class_bind_template_child(widget_class, CocoTimeConvert, bj_string);
  gtk_widget_class_bind_template_child(widget_class, CocoTimeConvert, bj_string_button);
  gtk_widget_class_bind_template_child(widget_class, CocoTimeConvert, bj_unix);
  gtk_widget_class_bind_template_child(widget_class, CocoTimeConvert, bj_unix_button);
  gtk_widget_class_bind_template_child(widget_class, CocoTimeConvert, input_unix_time);
  gtk_widget_class_bind_template_child(widget_class, CocoTimeConvert, output_string_time);
  gtk_widget_class_bind_template_child(widget_class, CocoTimeConvert, convert_to_string_button);
  gtk_widget_class_bind_template_child(widget_class, CocoTimeConvert, input_string_time);
  gtk_widget_class_bind_template_child(widget_class, CocoTimeConvert, output_unix_time);
  gtk_widget_class_bind_template_child(widget_class, CocoTimeConvert, convert_to_unix_button);
}

static void
coco_time_convert_init(CocoTimeConvert *self)
{
  gtk_widget_init_template(GTK_WIDGET(self));

  g_signal_connect(self->bj_string_button, "clicked", G_CALLBACK(now_bj_string_button_clicked), self);
  g_signal_connect(self->bj_unix_button, "clicked", G_CALLBACK(now_bj_unix_button_clicked), self);
  g_signal_connect(self->convert_to_string_button, "clicked", G_CALLBACK(convert_to_string_clicked), self);
  g_signal_connect(self->convert_to_unix_button, "clicked", G_CALLBACK(convert_to_unix_clicked), self);

  GDateTime *time_now = g_date_time_new_now_local();

  self->bj_string_buffer = gtk_entry_buffer_new("", 0);
  gtk_entry_buffer_set_text(self->bj_string_buffer, g_date_time_format(time_now, "%Y-%m-%d %H:%M:%S"), -1);
  gtk_entry_set_buffer(self->bj_string, self->bj_string_buffer);

  self->bj_unix_buffer = gtk_entry_buffer_new("", 0);
  gtk_entry_buffer_set_text(self->bj_unix_buffer, g_date_time_format(time_now, "%s"), -1);
  gtk_entry_set_buffer(self->bj_unix, self->bj_unix_buffer);
}
