#include <gtk/gtk.h>

static void
create_list(GtkWidget *window, gpointer data)
{
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_margin_start(vbox, 20);
    gtk_widget_set_margin_end(vbox, 20);
    gtk_widget_set_margin_top(vbox, 20);
    gtk_widget_set_margin_bottom(vbox, 20);
    gtk_window_set_child(window, vbox);

}

static void
do_list_store(GtkApplication *app, gpointer data)
{
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "List");
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);

    g_signal_connect(window, "activate", G_CALLBACK(create_list), data);

    gtk_widget_show(window);
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    app = gtk_application_new("sdu.edu.list", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(do_list_store), NULL);
    g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return 0;
}
