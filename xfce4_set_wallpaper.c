#include <xfconf/xfconf.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        g_print("%s\n", "no args set!");
        return -1;
    }
    xfconf_init(NULL);
    XfconfChannel *channel = xfconf_channel_get("xfce4-desktop");
    GHashTable *props = xfconf_channel_get_properties(channel, "/backdrop");

    GList *gl = g_hash_table_get_keys(props);
    GList *current = g_list_first(gl);
    while (current)
    {
        gchar *key = (gchar *)current->data;
        if (g_str_has_suffix(key, "/last-image"))
        {
            xfconf_channel_set_string(channel, key, argv[1]);
        }
        current = g_list_next(current);
    }
    g_list_free(gl);
    g_hash_table_destroy(props);
    xfconf_shutdown();
    return 0;
}
