#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <adwaita.h>
#include "password_generate.h"

struct _CocoPasswordGenerate {
    AdwBin parent_instance;
    GtkSwitch * char_diff;
    GtkSwitch * number_dict;
    GtkSwitch * lowercase_dict;
    GtkSwitch * uppercase_dict;
    GtkSwitch * special_dict;
    GtkSpinButton * generate_length;
    GtkSpinButton * generate_count;
    GtkButton * generate_button;
    GtkTextView * generate_output;
    char full_dict[128];
};

G_DEFINE_TYPE ( CocoPasswordGenerate, coco_password_generate, ADW_TYPE_BIN)

// 重写随机整数 return [0, n)
int rand_n(int n) {
    if (n <= 0 || n > RAND_MAX)
        //throw domain_error("抛出异常");
        return 0;
    const int _size = RAND_MAX / n;
    int r;

    do r = rand() / _size;
    while (r >= n);
    return r;
}

char *generate_random_string(char *dict, int no_repeat, int pwd_len) {
    int i;
    int random;
    char *password = (char *) malloc(pwd_len + 1);
    char dict_t [128];
    strcpy(dict_t, dict);
    int dict_size = strlen(dict_t);
    for (i = 0; i < pwd_len; ++i) {
        if (dict_size == 0) {
            break;
        }
        random = rand_n(dict_size);
        *(password + i) = dict_t[random];
        if (no_repeat) {
            for (int j = random; j < dict_size - 1; ++j) {
                dict_t[j] = dict_t[j + 1];
            }
            dict_t[dict_size - 1] = '\0';
            dict_size -= 1;
        }
    }
    *(password + i) = '\0';
    return password;
}

static void generate_go(GtkButton *button, CocoPasswordGenerate *self)
{
    memset (self->full_dict, 0, sizeof self->full_dict);
    const char *number_charset = "0123456789";
    const char *lowercase_charset = "abcdefghijklmnopqrstuvwxyz";
    const char *uppercase_charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *special_charset = "!@#$%^&*-_+={}[]|\\;:'\"<>,./?`~";
    if (gtk_switch_get_state (self->number_dict) == TRUE) {
        strcat(self->full_dict, number_charset);
    }
    if (gtk_switch_get_state (self->lowercase_dict) == TRUE) {
        strcat(self->full_dict, lowercase_charset);
    }
    if (gtk_switch_get_state (self->uppercase_dict) == TRUE) {
        strcat(self->full_dict, uppercase_charset);
    }
    if (gtk_switch_get_state (self->special_dict) == TRUE) {
        strcat(self->full_dict, special_charset);
    }

    printf("dict: %s\n", self->full_dict);

    GtkTextBuffer * viewer = gtk_text_view_get_buffer (self->generate_output);
    gtk_text_buffer_set_text(viewer, "", -1);
    GtkAdjustment * generate_count = gtk_spin_button_get_adjustment (self->generate_count);
    gint64 count = gtk_adjustment_get_value (generate_count);
    GtkAdjustment * generate_length = gtk_spin_button_get_adjustment (self->generate_length);
    gint64 length = gtk_adjustment_get_value (generate_length);
    for (int i = 0; i < count; ++i) {
        char *pwd = generate_random_string(self->full_dict, gtk_switch_get_state (self->char_diff), length);
        printf("generate: %s\n", pwd);
        GtkTextIter end_char;
        gtk_text_buffer_get_end_iter(viewer, &end_char);
        gtk_text_buffer_insert(viewer, &end_char, pwd, strlen(pwd));
        gtk_text_buffer_get_end_iter(viewer, &end_char);
        gtk_text_buffer_insert(viewer, &end_char, "\n", 1);
    }
}

static void coco_password_generate_class_init(CocoPasswordGenerateClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    gtk_widget_class_set_template_from_resource(widget_class, "/cc/xuthus/coco/components/password_generate/password_generate.ui");
    gtk_widget_class_bind_template_child(widget_class, CocoPasswordGenerate, char_diff);
    gtk_widget_class_bind_template_child(widget_class, CocoPasswordGenerate, number_dict);
    gtk_widget_class_bind_template_child(widget_class, CocoPasswordGenerate, lowercase_dict);
    gtk_widget_class_bind_template_child(widget_class, CocoPasswordGenerate, uppercase_dict);
    gtk_widget_class_bind_template_child(widget_class, CocoPasswordGenerate, special_dict);
    gtk_widget_class_bind_template_child(widget_class, CocoPasswordGenerate, generate_length);
    gtk_widget_class_bind_template_child(widget_class, CocoPasswordGenerate, generate_count);
    gtk_widget_class_bind_template_child(widget_class, CocoPasswordGenerate, generate_button);
    gtk_widget_class_bind_template_child(widget_class, CocoPasswordGenerate, generate_output);
}

static void
coco_password_generate_init(CocoPasswordGenerate *self) {
    gtk_widget_init_template(GTK_WIDGET(self));
    g_signal_connect(self->generate_button, "clicked", G_CALLBACK(generate_go), self);
}
