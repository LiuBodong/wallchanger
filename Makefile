CFLAGS=-g -Wall
cc=gcc

all: image_viewer xfce4_set_wallpaper set_idea_background set_xfce_terminal_background list-files
	
image_viewer: image_viewer.c
	$(cc) $(CFLAGS) -o $@ $^ `pkg-config --cflags --libs gtk4`

xfce4_set_wallpaper: xfce4_set_wallpaper.c
	$(cc) $(CFLAGS) -o $@ $^ `pkg-config --cflags --libs libxfconf-0 glib-2.0`

set_idea_background: set_idea_background.c
	gcc -o $@ $^ `pkg-config --cflags --libs libxml-2.0`

set_xfce_terminal_background: set_xfce_terminal_background.c
	gcc -o $@ $^

list-files: list-files.c
	gcc -o $@ $^ `pkg-config --cflags --libs glib-2.0`

clean:
	rm -rf image_viewer xfce4_set_wallpaper set_idea_background
