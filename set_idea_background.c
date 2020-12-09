#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#include <libxml/parser.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

void modify_setting_file(const char *xml_file_path,
                         const char *bg_path,
                         const char *opacity,
                         const bool flipH,
                         const bool flipV);

int main(int argc, char const *argv[])
{

    char *pic_path = calloc(sizeof(char), 256);
    char *opacity = calloc(sizeof(char), 256);
    bool flipH = false;
    bool flipV = false;
    for (int i = 1; i < argc; i++)
    {
        const char *arg = argv[i];
        if (strcmp(arg, "--path") == 0)
        {
            strcpy(pic_path, argv[++i]);
        }
        else if (strcmp(arg, "--opacity") == 0)
        {
            strcpy(opacity, argv[++i]);
        }
        else if (strcmp(arg, "--flipH") == 0)
        {
            flipH = true;
        }
        else if (strcmp(arg, "--flipV") == 0)
        {
            flipV = true;
        }
        else
        {
            printf("Unknown argument %s\n", arg);
            free(pic_path);
            free(opacity);
            exit(1);
        }
    }

    char *path = calloc(256, sizeof(char));
    char *home = getenv("HOME");
    strcat(path, home);
    strcat(path, "/.config/JetBrains");
    if (access(path, F_OK) == 0)
    {
        struct dirent *dirent;
        DIR *dir = opendir(path);
        while ((dirent = readdir(dir)) != NULL)
        {
            if (strcmp(dirent->d_name, ".") != 0 &&
                strcmp(dirent->d_name, "..") != 0)
            {
                char *real_path = calloc(256, sizeof(char));
                strcpy(real_path, path);
                strcat(real_path, "/");
                strcat(real_path, dirent->d_name);
                strcat(real_path, "/options/other.xml");
                if (access(real_path, F_OK) != 0)
                {
                    printf("%s not exist!\n", real_path);
                }
                else
                {
                    if (access(pic_path, F_OK) != 0)
                    {
                        printf("dest pic not exist\n");
                    }
                    else
                    {
                        printf("modifying file %s with args path=%s, opacity=%s, flipH=%d, flipV=%d\n",
                               real_path, pic_path, opacity, flipH, flipV);
                        modify_setting_file(real_path, pic_path, opacity, flipH, flipV);
                    }
                }
                free(real_path);
            }
        }
        free(dirent);
        closedir(dir);
    }
    free(path);
    free(pic_path);
    free(opacity);
}

void modify_setting_file(const char *xml_file_path,
                         const char *bg_path,
                         const char *opacity,
                         const bool flipH,
                         const bool flipV)
{
    xmlDocPtr doc;
    xmlNodePtr current_node;

    doc = xmlParseFile(xml_file_path);
    if (doc == NULL)
    {
        printf("Parse failed");
        return;
    }
    current_node = xmlDocGetRootElement(doc);
    if (current_node == NULL)
    {
        printf("No root found\n");
        xmlFree(doc);
        return;
    }
    // 查找application节点
    if (xmlStrcmp(current_node->name, (const xmlChar *)"application") != 0)
    {
        printf("Root node \"application\" not found\n");
        xmlFree(doc);
        return;
    }

    // 查找component
    current_node = current_node->children;
    while (current_node != NULL)
    {
        if (
            xmlStrcmp(current_node->name, (const xmlChar *)"component") == 0 &&
            xmlHasProp(current_node, (const xmlChar *)"name"))
        {
            xmlChar *name = xmlGetProp(current_node, (const xmlChar *)"name");
            if (xmlStrcmp(name, (const xmlChar *)"PropertiesComponent") == 0)
            {
                xmlFree(name);
                break;
            }
            xmlFree(name);
        }
        current_node = current_node->next;
    }
    if (current_node == NULL)
    {
        printf("No node named component and property \"name\"=\"PropertiesComponent\" found\n");
        xmlFree(doc);
        return;
    }
    current_node = current_node->children;
    while (current_node != NULL)
    {
        if (
            xmlStrcmp(current_node->name, (const xmlChar *)"property") == 0 &&
            xmlHasProp(current_node, (const xmlChar *)"name"))
        {
            xmlChar *name = xmlGetProp(current_node, (const xmlChar *)"name");
            if (xmlStrcmp(name, (const xmlChar *)"idea.background.editor") == 0)
            {
                xmlFree(name);
                break;
            }
            xmlFree(name);
        }
        current_node = current_node->next;
    }
    if (current_node == NULL)
    {
        printf("No node named \"property\" and property \"name\"=\"idea.background.editor\" found\n");
        xmlFree(doc);
        return;
    }
    char res[512];
    char *flip = calloc(sizeof(char), 8);
    if (!flipV && !flipH)
    {
        sprintf(res, "%s,%s,%s,%s", bg_path, opacity, "scale", "center");
    }
    else
    {
        strcpy(flip, "flip");
        if (flipH)
        {
            strcat(flip, "H");
        }
        if (flipV)
        {
            strcat(flip, "V");
        }
        sprintf(res, "%s,%s,%s,%s,%s", bg_path, opacity, "scale", "center", flip);
    }
    printf("set value: %s\n", res);
    xmlSetProp(current_node, (const xmlChar *)"value", (const xmlChar *)res);
    xmlSaveFormatFile(xml_file_path, doc, 1);
    xmlFree(doc);
    xmlCleanupParser();
    xmlCleanupMemory();
    free(flip);
}
