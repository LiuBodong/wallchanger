#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <glib.h>
#include <gtk/gtk.h>

static void print_data(GtkWidget *widget,
                       gpointer data)
{
    g_log("image_viewer", G_LOG_LEVEL_DEBUG, "%s", (gchar *)data);
}

static void activate(GtkApplication *app,
                     gpointer data)
{
    GtkWidget *window;
    GtkWidget *grid;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "ImageViewer");
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);

    grid = gtk_grid_new();
    gtk_window_set_child(GTK_WINDOW(window), grid);

    GtkWidget *button1 = gtk_button_new_with_label("Button1");
    g_signal_connect(button1, "clicked", G_CALLBACK(print_data), data);

    gtk_grid_attach(GTK_GRID(grid), button1, 0, 0, 1, 1);

    GtkWidget *button2 = gtk_button_new_with_label("Button2");
    g_signal_connect(button2, "clicked", G_CALLBACK(print_data), data);

    gtk_grid_attach(GTK_GRID(grid), button2, 1, 0, 1, 1);

    GtkWidget *button3 = gtk_button_new_with_label("Quit");
    g_signal_connect_swapped(button3, "clicked", G_CALLBACK(gtk_window_destroy), window);

    gtk_grid_attach(GTK_GRID(grid), button3, 0, 1, 2, 1);

    // gtk_window_fullscreen(GTK_WINDOW(window));
    gtk_widget_show(window);
}

static void g_log_debug_handler(const char *log_domain,
                                GLogLevelFlags log_level,
                                const gchar *message,
                                gpointer user_data)
{
    g_print("[%s]Debug : %s \n", log_domain, message);
}

int main(int argc, char **argv)
{

    g_log_set_handler("image_viewer", G_LOG_LEVEL_DEBUG | G_LOG_FLAG_FATAL | G_LOG_FLAG_RECURSION,
                      g_log_debug_handler, "");

    GtkApplication *app;
    int status;
    app = gtk_application_new("sdu.edu.image_viewer", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), "NULL");
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
