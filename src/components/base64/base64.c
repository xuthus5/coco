#include <gtk/gtk.h>
#include "base64.h"

struct _CocoBase64 {
    AdwBin parent_instance;
    GtkTextView *source_data;
    GtkTextBuffer *input_buffer;
    GtkTextView *destination_data;
    GtkTextBuffer *output_buffer;
    GtkButton *encode_button;
    GtkButton *decode_button;
    GtkButton *exchange_button;
};

G_DEFINE_TYPE ( CocoBase64, coco_base64, ADW_TYPE_BIN
)

static void
encode_callback(GtkButton *button, CocoBase64 *user_data) {
    GtkTextIter input_start, input_end;
    user_data->input_buffer = gtk_text_view_get_buffer(user_data->source_data);
    gtk_text_buffer_get_bounds(user_data->input_buffer, &input_start, &input_end);
    gchar *input_text = gtk_text_buffer_get_text(user_data->input_buffer, &input_start, &input_end, TRUE);
    // encode
    char *encoded_text = g_base64_encode(input_text, strlen(input_text));
    // debug
    g_print("event: encode, input: %s, output: %s\n", input_text, encoded_text);
    // set encode data
    user_data->output_buffer = gtk_text_view_get_buffer(user_data->destination_data);
    gtk_text_buffer_set_text(user_data->output_buffer, encoded_text, -1);
}

static void
decode_callback(GtkButton *button, CocoBase64 *user_data) {
    GtkTextIter input_start, input_end;
    user_data->input_buffer = gtk_text_view_get_buffer(user_data->source_data);
    gtk_text_buffer_get_bounds(user_data->input_buffer, &input_start, &input_end);
    char *input_text = gtk_text_buffer_get_text(user_data->input_buffer, &input_start, &input_end, TRUE);
    // decode
    gsize output_len;
    unsigned char *decoded_text = g_base64_decode(input_text, &output_len);
    // debug
    g_print("event: decode, input: %s, output: %s\n", input_text, decoded_text);
    // set decode data
    user_data->output_buffer = gtk_text_view_get_buffer(user_data->destination_data);
    gtk_text_buffer_set_text(user_data->output_buffer, (gchar *) decoded_text, -1);
}

static void
exchange_callback(GtkButton *button, CocoBase64 *user_data) {
    user_data->input_buffer = gtk_text_view_get_buffer(user_data->source_data);
    user_data->output_buffer = gtk_text_view_get_buffer(user_data->destination_data);
    GtkTextIter input_start, input_end, output_start, output_end;
    gtk_text_buffer_get_bounds(user_data->input_buffer, &input_start, &input_end);
    gtk_text_buffer_get_bounds(user_data->output_buffer, &output_start, &output_end);
    gchar *encode_text = gtk_text_buffer_get_text(user_data->input_buffer, &input_start, &input_end, TRUE);
    gchar *decode_text = gtk_text_buffer_get_text(user_data->output_buffer, &output_start, &output_end, TRUE);
    gtk_text_buffer_set_text(user_data->input_buffer, decode_text, -1);
    gtk_text_buffer_set_text(user_data->output_buffer, encode_text, -1);
}

static void
coco_base64_class_init(CocoBase64Class *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/cc/xuthus/coco/components/base64/base64.ui");
    gtk_widget_class_bind_template_child(widget_class, CocoBase64, source_data);
    gtk_widget_class_bind_template_child(widget_class, CocoBase64, destination_data);
    gtk_widget_class_bind_template_child(widget_class, CocoBase64, encode_button);
    gtk_widget_class_bind_template_child(widget_class, CocoBase64, decode_button);
    gtk_widget_class_bind_template_child(widget_class, CocoBase64, exchange_button);
}

static void
coco_base64_init(CocoBase64 *self) {
    gtk_widget_init_template(GTK_WIDGET(self));

    g_signal_connect(self->encode_button, "clicked", G_CALLBACK(encode_callback), self);
    g_signal_connect(self->decode_button, "clicked", G_CALLBACK(decode_callback), self);
    g_signal_connect(self->exchange_button, "clicked", G_CALLBACK(exchange_callback), self);
}


