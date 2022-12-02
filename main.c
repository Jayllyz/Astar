#include "fonction.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "map/map.h"

#define adj_number 8
#define possible_walk 1
#define wall 0
#define DEFAULT_CHESTS 0 //1 if you want default spawn setup

elements vertex_map[MAP_HEIGHT][MAP_WIDTH] = {};

int get_tile_cost(int tile)
{
    switch (tile)
    {
    case possible_walk:
        return 1;
    case wall:
        return 0;
    }
    return 0;
}

void spawn_scanf(int map[MAP_HEIGHT][MAP_WIDTH], points *p_spawn, points chests_coord[4]) // get spawn point value
{
    bool good_coord = false;
    do
    {
        printf("Coordonées de votre spawn : ");
        scanf("%d %d", &p_spawn->x, &p_spawn->y);
        if (map[p_spawn->y][p_spawn->x] != 1)
        {
            printf("C'est dans un obstacle\n");
            continue;
        }

        if (p_spawn->x < 0 || p_spawn->x >= 40 || p_spawn->y < 0 || p_spawn->y >= 40)//verif des coord du spawn
        {
            printf("Vous etes en dehors de la map\n");
            continue;
        }
        good_coord = true;
        for (int i = 0; i < 4; i++)
        {
            if (p_spawn->x == chests_coord[i].x && p_spawn->y == chests_coord[i].y)
            { //on vérifie que le spawn n'est pas sur un coffre
                good_coord = false;
                break;
            }
        }
        if (good_coord == false)
            printf("Spawn impossible\n");
    } while (false == good_coord); //tant que le spawn est pas ok on redemande des coords
}

void chest_spawn(int map[MAP_HEIGHT][MAP_WIDTH], points s_chest[4])
{
    bool test = true;
    for (int j = 0; j < 4; j++)
    {
        bool good_coord = false;
        do
        {
            printf("Coffre numero %d, saisir coordonnées : ", j + 1);
            scanf("%d %d", &s_chest[j].x, &s_chest[j].y);
            if (map[s_chest[j].y][s_chest[j].x] != 1) // on verifie si on est sur un obstacle ou pas
            {
                printf("C'est dans un obstacle\n");
                continue;
            }
            if (s_chest[j].x < 0 || s_chest[j].x >= 40 || s_chest[j].y < 0 || s_chest[j].y >= 40) //on verifie qu'on est dans la map
            {
                printf("Vous etes en dehors de la map\n");
                continue;
            }
            for (int x = 0; x < j; x++)
            {
                if ((s_chest[x].y == s_chest[j].y) && (s_chest[x].x == s_chest[j].x))//on verifie l'emplacements des coffres
                {
                    printf("2 Coffres au même endroits, impossible !\n");
                    test = false;
                    break;
                }
            }
            if (test == true)
            {
                good_coord = true;
            }
        } while (false == good_coord); //tant que le spawn est pas ok on redemande des coords
    }
}

points neighboors_coords[adj_number] =
    {
        {.x = 0, .y = -1},
        {.x = -1, .y = 0},
        {.x = +1, .y = 0},
        {.x = 0, .y = +1},
        {.x = -1, .y = -1},
        {.x = +1, .y = -1},
        {.x = -1, .y = +1},
        {.x = +1, .y = +1},
};

bool astar(int map[MAP_HEIGHT][MAP_WIDTH], points origin, points destination, array_t *found_path)
{
    // source du pseudo code : https://fr.wikipedia.org/wiki/Algorithme_A*
    for (int x = 0; x < MAP_WIDTH; x++)
    {
        for (int y = 0; y < MAP_HEIGHT; y++)
        { //on init les poids,  coord etc
            vertex_map[y][x].cost = 0;
            vertex_map[y][x].heuristic = 0;
            vertex_map[y][x].parent = NULL;
            vertex_map[y][x].coord = point_init(x, y);
            vertex_map[y][x].self_cost = get_tile_cost(map[y][x]);
        }
    }
    bool closed_list[MAP_HEIGHT][MAP_WIDTH] = {{false}}; //on init à faux toute la closed list
    linked_list_t *open_list = ll_new(); // liste prioritaire
    ll_insert_heuristic(open_list, &vertex_map[origin.y][origin.x]);
    // tant que open_list n'est pas vide
    while (ll_is_empty(open_list) == false)
    {
        elements *u = ll_head_data(open_list);
        // si la position est égale à la destination
        if (point_equals(u->coord, destination))
        { // algo termine car on est au bon point
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
        for (int i = 0; i < adj_number; i++) // pour chaque voisin on fait :
        {
            points math_var = neighboors_coords[i]; //on recupere la valeurs à additionner pour avoir les voisins
            points neighboor = point_init(u->coord.x + math_var.x, u->coord.y + math_var.y);
            if (neighboor.x < 0 || neighboor.x >= MAP_WIDTH || neighboor.y < 0 || neighboor.y >= MAP_HEIGHT)
                continue; // hors map

            if (map[neighboor.y][neighboor.x] == 0)
                continue; // obstacle

            if (closed_list[neighboor.y][neighboor.x] == true)
                continue; //déjà visité

            elements *v = ll_find(open_list, neighboor);
            if (v != NULL && v->cost < (u->cost + v->self_cost))
                continue;
            bool in_open_list = v != NULL;
            v = &vertex_map[neighboor.y][neighboor.x];
            v->cost = u->cost + v->self_cost;//on addiditone les poids
            v->heuristic = v->cost + point_distance(v->coord, destination); //on calcule l'heuristic
            v->parent = u;
            if (false == in_open_list)
            {
                ll_insert_heuristic(open_list, v);
            }
        }
        closed_list[u->coord.y][u->coord.x] = true;
    }
    ll_destroy(open_list);
    return false;
}

int main(int argc, const char *argv[]){
    system("clear");
    printf("A* INFORMATIONS:\nmap 40*40 (dossier map, 1 signfie que vous pouvez marcher et 0 est un obstacle), 4 coffres possibles, 1 vrai coffre ! Bonne chance.\n\n");
    array_t road[4];
    points chests[4] = {
        // on définit les coord des coffres (mais le joueur va les choisirs)
        {.x = 6, .y = 15},
        {.x = 25, .y = 9},
        {.x = 0, .y = 37},
        {.x = 38, .y = 36},
    };
    srand(time(NULL));
    int good_chest = rand() % 4;
    points spawn;
    if(DEFAULT_CHESTS == 0)
        chest_spawn(map, chests);    
    spawn_scanf(map, &spawn, chests); //on demande au joueur son spawn
    bool chest_find = false;
    int i = 0;
    int test_first = 0;
    array_t alreadyTry[5];
    array_new(alreadyTry, 5, false);
    do
    {
        printf("Choisissez le coffre que vous voulez tester (0,1,2,3): ");
        scanf("%d", &i);
        if( test_first != 0){//permet de vérifier si c'est notre premier essai
            spawn = chests[i];//sinon on update le spawn avec le coffre cherché avant
        }
        if (false == astar(map, spawn, chests[i], &road[i])){ //si astar renvoi false, la route est impossible
            printf("Erreur de coordonnées\n");
            return 1;
        }
        //besoin de "bloquer" les coffres déjà validé avec genre un tab qui regroupent les truc déjà test
        size_t road_size = array_get_size(&road[i]); //else on recup la taille de la liste fermé
        printf("\nCoffre numéro %d, %ld mouvements nécessaires, voici les coordonnées du chemin à parcourir :\n", i, road_size);
        printf("Depart -> (%d,%d)\n", spawn.x,spawn.y );
        printf("Objectif -> (%d,%d)\n\n", chests[i].x,chests[i].y);
        for (int j = 0; j < road_size; ++j) //puis on affiche toutes les coord de la route
        {
            elements *vertex = (elements *)array_get(&road[i], j);
            printf("(%d,%d)\n", vertex->coord.x, vertex->coord.y);
        }

        array_destroy(&road[i]); // on "détruit" les routes pour la mémoire et pour pouvoir eviter les bug quand on recommence
        if (i == good_chest){
            printf("Bravo c'est le bon coffre !\n");
            chest_find = true;
        }
        else{
            printf("Dommage ce n'est pas le bon coffre !\n");
            array_add(alreadyTry,&i);
        }
    test_first ++;
    } while (chest_find == false);
    return 0;
}
