#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <adwaita.h>
#include "hash.h"

struct _CocoHash
{
    AdwBin parent_instance;
    GtkTextView *input_source_text;
    GtkButton *open_from_file;
    GtkButton *clear_input;
    GtkButton *hash_sum;
    GtkEntry *md5_16;
    GtkEntry *md5_32;
    GtkEntry *sha1;
    GtkEntry *sha256;
    GtkEntry *sha512;
    GBytes * source_contents;
    gboolean is_file;
};

G_DEFINE_TYPE(CocoHash, coco_hash, ADW_TYPE_BIN)

static void on_choose_file_response(GtkNativeDialog *native, int response, CocoHash *self)
{
	if (response == GTK_RESPONSE_ACCEPT) {
		GtkFileChooser *chooser = GTK_FILE_CHOOSER(native);
		g_autoptr(GFile) file = gtk_file_chooser_get_file(chooser);
        char * filename = g_file_get_basename(file);
        GtkTextBuffer * buffer = gtk_text_view_get_buffer(self->input_source_text);
        gtk_text_buffer_set_text(buffer, filename, -1);
        // clear and disable editable
        gtk_text_view_set_editable(self->input_source_text, FALSE);
		// g_file_hash
        /* gsize length; */
        GError *error = NULL;
        self->source_contents = g_file_load_bytes (file, NULL, NULL, &error);
        if (error != NULL)
        {
            printf("load file error: %s\n", error->message);
            return;
        }
        if (self->source_contents == NULL)
            {
            printf("load file error: file content null\n");
            return;
            }
	}

	g_object_unref(native);
}


static void open_from_source_file(GtkButton *button, CocoHash *self)
{
    self->is_file = TRUE;
    if (self->source_contents != NULL) g_bytes_unref (self->source_contents);
    // file chooser
	GtkFileChooserNative * native = gtk_file_chooser_native_new("Open File", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, "_Open", "_Cancel");
	g_signal_connect(native, "response", G_CALLBACK(on_choose_file_response), self);
	gtk_native_dialog_show(GTK_NATIVE_DIALOG(native));
}

static void clear_all_data(GtkButton *button, CocoHash *self) 
{
    GtkTextBuffer * buffer = gtk_text_view_get_buffer(self->input_source_text);
    gtk_text_buffer_set_text(buffer, "", 0);
    GtkEntryBuffer * md5_16 = gtk_entry_get_buffer(self->md5_16);
    gtk_entry_buffer_set_text(md5_16, "", 0);
    GtkEntryBuffer * md5_32 = gtk_entry_get_buffer(self->md5_32);
    gtk_entry_buffer_set_text(md5_32, "", 0);
    GtkEntryBuffer * sha1 = gtk_entry_get_buffer(self->sha1);
    gtk_entry_buffer_set_text(sha1, "", 0);
    GtkEntryBuffer * sha256 = gtk_entry_get_buffer(self->sha256);
    gtk_entry_buffer_set_text(sha256, "", 0);
    GtkEntryBuffer * sha512 = gtk_entry_get_buffer(self->sha512);
    gtk_entry_buffer_set_text(sha512, "", 0);
    gtk_text_view_set_editable(self->input_source_text, TRUE);
    self->is_file = FALSE;
    // free source_contents
    if (self->source_contents != NULL) g_bytes_unref (self->source_contents);
}

static void hash_data(GtkButton *button, CocoHash *self) 
{
    if (!self->is_file)
    {
        GtkTextBuffer * buffer = gtk_text_view_get_buffer(self->input_source_text);
        GtkTextIter input_start, input_end;
        gtk_text_buffer_get_bounds(buffer,&input_start, &input_end);
        gchar *input_data = gtk_text_buffer_get_text(buffer, &input_start, &input_end, TRUE);
        printf("input data : %s\n", input_data);
        self->source_contents = g_bytes_new (input_data, strlen(input_data));
    }

    if (self->source_contents == NULL) return;

    gchar * md5_32_sum = sum_md5_32 (self->source_contents);
    GtkEntryBuffer * md5_32_buffer = gtk_entry_buffer_new(md5_32_sum, strlen(md5_32_sum));
    gtk_entry_set_buffer(self->md5_32, md5_32_buffer);
    char cipher_substring [16];
    strncpy(cipher_substring, &md5_32_sum[8], 16);
    GtkEntryBuffer * md5_16_buffer = gtk_entry_buffer_new(cipher_substring, 16);
    gtk_entry_set_buffer(self->md5_16, md5_16_buffer);

    gchar * sha1_sum = sum_sha1(self->source_contents);
    GtkEntryBuffer * sha1_buffer = gtk_entry_buffer_new(sha1_sum, strlen(sha1_sum));
    gtk_entry_set_buffer(self->sha1, sha1_buffer);

    gchar * sha256_sum = sum_sha256(self->source_contents);
    GtkEntryBuffer * sha256_buffer = gtk_entry_buffer_new(sha256_sum, strlen(sha256_sum));
    gtk_entry_set_buffer(self->sha256, sha256_buffer);

    gchar * sha512_sum = sum_sha512(self->source_contents);
    GtkEntryBuffer * sha512_buffer = gtk_entry_buffer_new(sha512_sum, strlen(sha512_sum));
    gtk_entry_set_buffer(self->sha512, sha512_buffer);
}

static void coco_hash_class_init(CocoHashClass *klass)
{
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    gtk_widget_class_set_template_from_resource(widget_class, "/cc/xuthus/coco/components/hash/hash.ui");
    gtk_widget_class_bind_template_child(widget_class, CocoHash, input_source_text);
    gtk_widget_class_bind_template_child(widget_class, CocoHash, open_from_file);
    gtk_widget_class_bind_template_child(widget_class, CocoHash, clear_input);
    gtk_widget_class_bind_template_child(widget_class, CocoHash, hash_sum);
    gtk_widget_class_bind_template_child(widget_class, CocoHash, md5_16);
    gtk_widget_class_bind_template_child(widget_class, CocoHash, md5_32);
    gtk_widget_class_bind_template_child(widget_class, CocoHash, sha1);
    gtk_widget_class_bind_template_child(widget_class, CocoHash, sha256);
    gtk_widget_class_bind_template_child(widget_class, CocoHash, sha512);
}

static void
coco_hash_init(CocoHash *self)
{
    gtk_widget_init_template(GTK_WIDGET(self));
    g_signal_connect(self->hash_sum, "clicked", G_CALLBACK(hash_data), self);
    g_signal_connect(self->clear_input, "clicked", G_CALLBACK(clear_all_data), self);
    g_signal_connect(self->open_from_file, "clicked", G_CALLBACK(open_from_source_file), self);
}
