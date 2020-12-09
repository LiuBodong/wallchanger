#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <glib.h>

typedef struct dirent _dirent;

GList *list = NULL; 

void list_dir_recursively(char *path);
bool ends_with(char *s, char *r);

int main(int argc, char const *argv[])
{
    char *path = "/home/liubodong/Pictures/EnterDeskBeauty";
    list_dir_recursively(path);
    GList *ptr = list;
    while (ptr != NULL)
    {
        g_print("%s\n", (gchar *)ptr->data);
        free(ptr->data);
        ptr = ptr->next;
    }
    g_list_free(list);
    free(ptr);
    return 0;
}

void list_dir_recursively(char *path)
{
    DIR *dir = opendir(path);
    _dirent *d;
    while ((d = readdir(dir)) != NULL)
    {
        if (strcmp(d->d_name, ".") == 0 ||
            strcmp(d->d_name, "..") == 0)
        {
            continue;
        }
        char p[256];
        strcpy(p, path);
        strcat(p, "/");
        strcat(p, d->d_name);
        struct stat st;
        stat(p, &st);
        if (S_ISDIR(st.st_mode))
        {
            list_dir_recursively(p);
        }
        else if (S_ISREG(st.st_mode))
        {
            if (ends_with(p, ".jpg"))
            {
                char *file_path = calloc(sizeof(char), strlen(p) + 1);
                strcpy(file_path, p);
                list = g_list_append(list, file_path);
            }
        }
    }
    closedir(dir);
}

bool ends_with(char *s, char *r)
{
    size_t s_l = strlen(s);
    size_t r_l = strlen(r);
    if (s_l < r_l)
    {
        return false;
    }
    else
    {
        return strcmp(s + (s_l - r_l), r) == 0;
    }
}
