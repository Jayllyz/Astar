
SDL: First of all, here is the command to run the program with SDL

gcc main_sdl.c array.c linked_list.c -o prog $(sdl2-config --cflags --libs) -lm ./prog

The SDL has several display problems, crashes, black screen, unfortunately we did not manage to solve them.
