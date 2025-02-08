#include <gtk-3.0/gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONFIG_FILE "/etc/mouse_remap/config.txt"

typedef struct
{
    GtkWidget *btn_top_combo;
    GtkWidget *btn_middle_combo;
    GtkWidget *btn_wheel_combo;
    GtkWidget *btn_bottom_combo;
    GtkWidget *btn_side_top_combo;
    GtkWidget *btn_side_bottom_combo;
} Widgets;

const char *get_key_for_index(int index)
{
    const char *keys[] = {
        "0",
        "KEY_LEFT",
        "KEY_RIGHT",
        "KEY_COPY",
        "KEY_PASTE",
        "KEY_FORWARD",
        "KEY_BACK",
        "KEY_UNDO",
        "KEY_REDO"};
    return keys[index];
}

GtkWidget *create_button_combo()
{
    GtkWidget *combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "None");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "Left Click");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "Right Click");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "Copy");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "Paste");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "Forward (Browser)");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "Backward (Browser)");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "Undo");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "Redo");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
    return combo;
}

void read_config(Widgets *widgets)
{
    FILE *file = fopen(CONFIG_FILE, "r");
    if (file == NULL)
    {
        g_print("Config file not found, using default values.\n");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        char *eq = strchr(line, '=');
        if (!eq)
            continue;
        *eq = '\0';
        char *keyName = eq + 1;
        keyName[strcspn(keyName, "\r\n")] = '\0';
        if (strcmp(line, "BTN_Top") == 0)
        {
            int index = 0;
            if (strcmp(keyName, "None") != 0)
            {
                for (index = 1; index < 9; ++index)
                {
                    if (strcmp(keyName, get_key_for_index(index)) == 0)
                    {
                        break;
                    }
                }
            }
            gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->btn_top_combo), index);
        }
        else if (strcmp(line, "BTN_Middle") == 0)
        {
            int index = 0;
            if (strcmp(keyName, "None") != 0)
            {
                for (index = 1; index < 9; ++index)
                {
                    if (strcmp(keyName, get_key_for_index(index)) == 0)
                    {
                        break;
                    }
                }
            }
            gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->btn_middle_combo), index);
        }
        else if (strcmp(line, "BTN_Wheel") == 0)
        {
            int index = 0;
            if (strcmp(keyName, "None") != 0)
            {
                for (index = 1; index < 9; ++index)
                {
                    if (strcmp(keyName, get_key_for_index(index)) == 0)
                    {
                        break;
                    }
                }
            }
            gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->btn_wheel_combo), index);
        }
        else if (strcmp(line, "BTN_Bottom") == 0)
        {
            int index = 0;
            if (strcmp(keyName, "None") != 0)
            {
                for (index = 1; index < 9; ++index)
                {
                    if (strcmp(keyName, get_key_for_index(index)) == 0)
                    {
                        break;
                    }
                }
            }
            gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->btn_bottom_combo), index);
        }
        else if (strcmp(line, "BTN_Side Top") == 0)
        {
            int index = 0;
            if (strcmp(keyName, "None") != 0)
            {
                for (index = 1; index < 9; ++index)
                {
                    if (strcmp(keyName, get_key_for_index(index)) == 0)
                    {
                        break;
                    }
                }
            }
            gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->btn_side_top_combo), index);
        }
        else if (strcmp(line, "BTN_Side Bottom") == 0)
        {
            int index = 0;
            if (strcmp(keyName, "None") != 0)
            {
                for (index = 1; index < 9; ++index)
                {
                    if (strcmp(keyName, get_key_for_index(index)) == 0)
                    {
                        break;
                    }
                }
            }
            gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->btn_side_bottom_combo), index);
        }
    }
    fclose(file);
}

void save_config(GtkWidget *widget, gpointer data)
{
    Widgets *widgets = (Widgets *)data;
    FILE *file = fopen(CONFIG_FILE, "w");
    if (file == NULL)
    {
        g_print("Failed to open config file for writing\n");
        return;
    }

    fprintf(file, "BTN_Top=%s\n", get_key_for_index(gtk_combo_box_get_active(GTK_COMBO_BOX(widgets->btn_top_combo))));
    fprintf(file, "BTN_Middle=%s\n", get_key_for_index(gtk_combo_box_get_active(GTK_COMBO_BOX(widgets->btn_middle_combo))));
    fprintf(file, "BTN_Wheel=%s\n", get_key_for_index(gtk_combo_box_get_active(GTK_COMBO_BOX(widgets->btn_wheel_combo))));
    fprintf(file, "BTN_Bottom=%s\n", get_key_for_index(gtk_combo_box_get_active(GTK_COMBO_BOX(widgets->btn_bottom_combo))));
    fprintf(file, "BTN_Side Top=%s\n", get_key_for_index(gtk_combo_box_get_active(GTK_COMBO_BOX(widgets->btn_side_top_combo))));
    fprintf(file, "BTN_Side Bottom=%s\n", get_key_for_index(gtk_combo_box_get_active(GTK_COMBO_BOX(widgets->btn_side_bottom_combo))));

    fclose(file);

    int status = system("sudo systemctl restart mouse_remap.service");
    if (status == -1)
    {
        perror("System call failed");
    }
    else if (status == 0)
    {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Configuration saved successfully!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
    else
    {
        fprintf(stderr, "Error restarting service, status code: %d\n", status);
    }
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Evoluent VerticalMouse D Settings");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);
    GtkWidget *grid = gtk_grid_new();
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(window), grid);

    Widgets widgets;
    int row = 0;

    GtkWidget *label = gtk_label_new("Top Button");
    gtk_grid_attach(GTK_GRID(grid), label, 0, row, 1, 1);
    widgets.btn_top_combo = create_button_combo();
    gtk_grid_attach(GTK_GRID(grid), widgets.btn_top_combo, 1, row++, 1, 1);

    label = gtk_label_new("Middle Button");
    gtk_grid_attach(GTK_GRID(grid), label, 0, row, 1, 1);
    widgets.btn_middle_combo = create_button_combo();
    gtk_grid_attach(GTK_GRID(grid), widgets.btn_middle_combo, 1, row++, 1, 1);

    label = gtk_label_new("Wheel Button");
    gtk_grid_attach(GTK_GRID(grid), label, 0, row, 1, 1);
    widgets.btn_wheel_combo = create_button_combo();
    gtk_grid_attach(GTK_GRID(grid), widgets.btn_wheel_combo, 1, row++, 1, 1);

    label = gtk_label_new("Bottom Button");
    gtk_grid_attach(GTK_GRID(grid), label, 0, row, 1, 1);
    widgets.btn_bottom_combo = create_button_combo();
    gtk_grid_attach(GTK_GRID(grid), widgets.btn_bottom_combo, 1, row++, 1, 1);

    label = gtk_label_new("Side Top Button");
    gtk_grid_attach(GTK_GRID(grid), label, 0, row, 1, 1);
    widgets.btn_side_top_combo = create_button_combo();
    gtk_grid_attach(GTK_GRID(grid), widgets.btn_side_top_combo, 1, row++, 1, 1);

    label = gtk_label_new("Side Bottom Button");
    gtk_grid_attach(GTK_GRID(grid), label, 0, row, 1, 1);
    widgets.btn_side_bottom_combo = create_button_combo();
    gtk_grid_attach(GTK_GRID(grid), widgets.btn_side_bottom_combo, 1, row++, 1, 1);

    GtkWidget *save_button = gtk_button_new_with_label("Save Settings");
    gtk_grid_attach(GTK_GRID(grid), save_button, 0, row, 2, 1);
    g_signal_connect(save_button, "clicked", G_CALLBACK(save_config), &widgets);

    read_config(&widgets);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
