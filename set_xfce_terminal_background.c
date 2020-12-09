#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

void set_bg(const char *file_path, const char *bg_path);
void do_set_bg(FILE *src, FILE *dest, const char *bg_path);
bool starts_with(char *s, const char *m);

int main(int argc, char const *argv[])
{
    char *home_path = getenv("HOME");
    char *file_path = calloc(sizeof(char), 256);
    strcpy(file_path, home_path);
    strcat(file_path, "/.config/xfce4/terminal/terminalrc");
    set_bg(file_path, argv[1]);
    free(file_path);
    return 0;
}

void set_bg(const char *file_path, const char *bg_path)
{
    // source file
    FILE *src_file;
    // dest file
    FILE *dest_file;
    char *dest_path = calloc(sizeof(char), 256);
    strcat(dest_path, file_path);
    strcat(dest_path, ".bak");
    if ((src_file = fopen(file_path, "r")) == NULL)
    {
        return;
    }
    if ((dest_file = fopen(dest_path, "w")) == NULL)
    {
        return;
    }
    do_set_bg(src_file, dest_file, bg_path);
    fclose(src_file);
    fclose(dest_file);
    rename(dest_path, file_path);
    free(dest_path);
    return;
}

void do_set_bg(FILE *src, FILE *dest, const char *bg_path)
{
    char line[1025];
    while (fgets(line, 1024, src) != NULL)
    {
        if (starts_with(line, "BackgroundImageFile"))
        {
            strcpy(line, "BackgroundImageFile=");
            strcat(line, bg_path);
            strcat(line, "\n");
        }
        fputs(line, dest);
    }
}

bool starts_with(char *s, const char *m)
{
    int m_len = strlen(m);
    int s_len = strlen(s);
    if (s_len < m_len)
    {
        return false;
    }

    for (int i = 0; i < m_len; i++)
    {
        if (m[i] != s[i])
        {
            return false;
        }
    }
    return true;
}
