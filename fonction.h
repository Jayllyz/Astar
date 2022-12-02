#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>

#ifndef FONCTION_H
#define FONCTION_H

typedef struct points {
    int x;
    int y;
} points;

typedef struct elements {
    points coord; //coordonnées du point
    struct elements *parent; // les "parents" du point
    int self_cost; //Poids de l'element
    int cost; //poids depuis le début de l'algo
    int heuristic; //estimation de la solution par A*
} elements;

//static inline est utile pour optimisé des fonctions très courtes ! (on avait des soucis sans ca)
static inline points point_init(int x, int y)
{
    points point = {.x = x, .y = y};
    return point;
}
//Pour vérifier si un point est identique à un autre
static inline bool point_equals(points left, points right)
{
    return left.x == right.x && left.y == right.y;
}
//Calcul de la distance entre 2 points
static inline int point_distance(points left, points right)
{
    return sqrt(pow(left.x - right.x, 2) + pow(right.y - right.y, 2));
}

//DEBUT ARRAY.C
typedef struct array_t {
    void **_storage;
    size_t _capacity; //size_t = unsigned int ET unsigned long int
    size_t _size; // Nombre "d'élèments" rempli de l'array
    int _should_free_items; //peut on clear des élèments de l'array
} array_t;

void array_new(array_t *array, size_t capacity, int should_free_items);
void array_destroy(array_t *array);
void array_reserve(array_t *array, size_t capacity);
void array_add(array_t *array, void *elem);
void array_reverse(array_t *array);
void *array_get(array_t *array, size_t index);
size_t array_get_size(array_t *array);
//FIN ARRAY.C

//DEBUT linked_list
typedef struct linked_node_t { // Structure pour naviguer dans la liste
    struct linked_node_t *prev;
    struct linked_node_t *next;
    elements *data;
} linked_node_t;

typedef struct linked_list_t { // Tête de la liste + queue de la liste
    struct linked_node_t *head;
    struct linked_node_t *tail;
} linked_list_t;

linked_list_t *ll_new(void);
linked_list_t *ll_destroy(linked_list_t *list);
void ll_append(linked_list_t *list, elements *data);
elements *ll_find(linked_list_t *list, points point);
void ll_insert_before(linked_list_t *list, linked_node_t *before, elements *data);
bool ll_is_empty(linked_list_t *list);
void ll_prepend(linked_list_t *list, elements *data);
elements *ll_head_data(linked_list_t *list);
void ll_insert_heuristic(linked_list_t *list, elements *data);

//FIN linked_list

#endif