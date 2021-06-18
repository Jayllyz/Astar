#include "fonction.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "map/map.h"
#include <SDL.h>

#define adj_number 8
#define possible_walk 1
#define wall 0

#define WHITE 200, 200, 200, 255
#define BLACK 40, 40, 40, 255
#define RED 220, 12, 4, 255
#define GREEN 28, 180, 14, 255
#define TILE_SIZE 10

bool running = true;

elements vertex_map[MAP_HEIGHT][MAP_WIDTH] = {};

int get_tile_cost(int tile) {
    switch (tile) {
        case possible_walk: return 1;
        case wall: return 0;
    }
    return 0; // ça ne devrait jamais arriver
}

void spawn_scanf(int map[MAP_HEIGHT][MAP_WIDTH], points *p_spawn, points chests_coord[4]) // get spawn point value
{
    bool good_coord = false;
    do {
        printf("Coordonées de votre spawn : ");
        scanf("%d %d", &p_spawn->x, &p_spawn->y);
        if (map[p_spawn->y][p_spawn->x] != 1) 
        {
            printf("C'est dans un obstacle\n");
            continue;
        }
        
        if (p_spawn->x < 0 || p_spawn->x >= 40 || p_spawn->y < 0 || p_spawn->y >= 40)
        {
            printf("Vous etes en dehors de la map\n");
            continue;
        }
        good_coord = true;
        for(int i =0; i<4; i++){
        if(p_spawn->x == chests_coord[i].x && p_spawn->y == chests_coord[i].y) 
                { //on vérifie que le spawn n'est pas sur un coffre
                    good_coord = false;
                    break;
                } 
        }
        if (good_coord == false)
            printf("Spawn impossible\n");
    } while (false == good_coord);//tant que le spawn est pas ok on redemande des coords
}

void chest_spawn(int map[MAP_HEIGHT][MAP_WIDTH], points s_chest[4]) 
{
    bool test=true;
    for(int j=0; j < 4; j++){
        bool good_coord = false;
        do {
            printf("Coffre numero %d, saisir coordonnées : ",j+1);
            scanf("%d %d", &s_chest[j].x, &s_chest[j].y);
            if (map[s_chest[j].y][s_chest[j].x] != 1) // on verifie si on est sur un obstacle ou pas
            {
                printf("C'est dans un obstacle\n");
                continue;
            }         
            if (s_chest[j].x < 0 || s_chest[j].x >= 40 || s_chest[j].y < 0 || s_chest[j].y >= 40)//on verifie qu'on est dans la map
            {
                printf("Vous etes en dehors de la map\n");
                continue;
            }
            for(int x=0; x<j; x++){
                if((s_chest[x].y == s_chest[j].y) && (s_chest[x].x == s_chest[j].x) ){
                    printf("2 Coffres au même endroits, impossible !\n");
                    test = false;
                    break;
                }
            }
            if(test == true){
            good_coord = true;}
        } while (false == good_coord);//tant que le spawn est pas ok on redemande des coords
    }
}

points neighboors_coords[adj_number] = 
{
    {.x = -1, .y = -1},
    {.x = +1, .y = -1},
    {.x = -1, .y = +1},
    {.x = +1, .y = +1},
    {.x =  0, .y = -1},
    {.x = -1, .y =  0},
    {.x = +1, .y =  0},
    {.x =  0, .y = +1},
};

void on_sdl_event(SDL_Event *event){
    switch(event->type){
        case SDL_WINDOWEVENT:{
            switch(event->window.event){
                case SDL_WINDOWEVENT_CLOSE: 
                    running = false;
                    break;
            }
        }
    }
}

bool astar(int map[MAP_HEIGHT][MAP_WIDTH], points origin, points destination, array_t *found_path){
for (int x = 0; x < MAP_WIDTH; x++)
{
     for (int y = 0; y < MAP_HEIGHT; y++)
    {
        vertex_map[y][x].cost = 0;
        vertex_map[y][x].heuristic = 0;
        vertex_map[y][x].parent = NULL;
        vertex_map[y][x].coord = point_init(x, y);
        vertex_map[y][x].self_cost = get_tile_cost(map[y][x]);
    }
}
bool closed_list[MAP_HEIGHT][MAP_WIDTH] = { { false } };
linked_list_t *open_list = ll_new(); // liste prioritaire
ll_insert_heuristic(open_list, &vertex_map[origin.y][origin.x]);
// tant que open_list n'est pas vide
    while (ll_is_empty(open_list) == false)	
    {
        elements *u = ll_head_data(open_list);
        // si la position est égale à la destination
        if (point_equals(u->coord, destination)) {
            // reconstitution du chemin en partant de la fin
            array_new(found_path, 32, false);
            array_add(found_path, u);
            
            while (u->parent != NULL) 
            {
                u = u->parent;
                array_add(found_path, u);
            }
            array_reverse(found_path);
            open_list = ll_destroy(open_list);
            return true;
        }
        
        // pour chaque voisin v de u
        for (int i = 0; i < adj_number; i++)
        {
            points delta = neighboors_coords[i];
            points neighboor = point_init(u->coord.x + delta.x, u->coord.y + delta.y);
            if (neighboor.x < 0 || neighboor.x >= MAP_WIDTH || neighboor.y < 0 || neighboor.y >= MAP_HEIGHT)
                continue; // hors map
            
            if (map[neighboor.y][neighboor.x] == 0)
                continue; // obstacle
            
            if (closed_list[neighboor.y][neighboor.x] == true)
                continue;
            
            elements *v = ll_find(open_list, neighboor);
            if (v != NULL && v->cost < (u->cost + v->self_cost))
                continue;
            bool in_open_list = v != NULL; 
            v = &vertex_map[neighboor.y][neighboor.x];
            v->cost = u->cost + v->self_cost; 
            v->heuristic = v->cost + point_distance(v->coord, destination);
            v->parent = u;
           if(false == in_open_list){
               ll_insert_heuristic(open_list, v);
           }
        }
        closed_list[u->coord.y][u->coord.x] = true;
    }
    ll_destroy(open_list);
    return false;
}



int main() {
    SDL_Delay(5000);
    array_t road[4];
    points chests[4] = { // on définit les coord des coffres
        {.x = 0, .y = 39},
        {.x = 0, .y = 38},
        {.x = 0, .y = 37},
        {.x = 0, .y = 36},
    };
    srand(time(NULL));
    int good_chest = rand() % 4; 
    points spawn;
    chest_spawn(map,chests);
    spawn_scanf(map, &spawn, chests); //on demande au joueur son spawn
    bool chest_find = false;
    int i = 0;
    if( SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        SDL_Log("ERREUR : init SDL > %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    window= SDL_CreateWindow("map", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,  40*10,  40*10, SDL_WINDOW_SHOWN);
    renderer= SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  
     SDL_Event event;
    while (running){
        while(SDL_PollEvent(&event))
            on_sdl_event(&event);
        SDL_RenderClear(renderer);
        SDL_Rect r;
        r.h = 10;
        r.w = 10;
                do{
                    printf("Choisissez le coffre que vous voulez tester (0,1,2,3): ");
                    scanf("%d",&i);
                    if (false == astar(map, spawn, chests[i], &road[i]))//si astar renvoi false, la route est impossible
                    {
                        printf("Erreur de coordonnées\n");
                        return 1;
                    }
                     for(int x = 0; x < 40;x++)
                    {
                        for(int y = 0; y < 40; y++){
                            r.x = y*10;
                            r.y = x*10;
                            int value=get_tile_cost(map[x][y]);
                            if(value == 0 )
                            {
                                SDL_SetRenderDrawColor(renderer, BLACK);
                            }
                            else{
                                SDL_SetRenderDrawColor(renderer,GREEN);
                            }
                            SDL_RenderFillRect(renderer, &r);
                            
                        }
                    }
                    SDL_RenderPresent(renderer);
                    size_t road_size = array_get_size(&road[i]);//else on recup la taille de la liste fermé
                    SDL_SetRenderDrawColor(renderer,RED);
                    printf("\nCoffre numéro %d, %ld mouvements nécessaires, voici les coordonnées du chemin à parcourir :\n", i, road_size);
                    for (int j = 0; j < road_size; ++j)//puis on affiche toutes les coord de la route
                    {
                        elements *vertex = (elements*)array_get(&road[i], j);
                        printf("(%d,%d)\n", vertex->coord.x, vertex->coord.y);
                        r.x = vertex->coord.y * 10;
                        r.y= vertex->coord.x *10;
                        SDL_RenderFillRect(renderer,&r);
                        SDL_RenderPresent(renderer);   
                    }
                    array_destroy(&road[i]);// on "détruit" le chemin pour la mémoire 
                    if(i== good_chest){
                    printf("Bravo c'est le bon coffre !\n");
                    chest_find = true;
                    break;
                    }
                    else{
                        printf("Dommage ce n'est pas le bon coffre !\n");
                    }
                 } while(chest_find == false);
    }
    
    SDL_DestroyRenderer(renderer);SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
