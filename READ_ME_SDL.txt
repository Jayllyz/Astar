Concernant la SDL : 
Tout d'abord voici la commande pour exécuter le programme avec SDL

gcc main_sdl.c array.c linked_list.c -o prog $(sdl2-config --cflags --libs) -lm
./prog

La SDL à plusieurs problèmes d'affichage, crash, écran noir, malheureusement nous n'avons pas réussi à les résoudre.

Pour la version sans SDL il vous suffit d'écrire "make", et tout se fera dans le terminal.

Pierrick GRATON & Antony DAVID.