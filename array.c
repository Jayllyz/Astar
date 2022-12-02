/*
Listes de fonctions pour gérer les tableaux
*/
#include "fonction.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void array_reserve_more(array_t *array) // Pour augmenter la capacité d'un tableau
{
    if (array->_capacity < 65536)
        array_reserve(array, array->_capacity * 2);
    else
        array_reserve(array, array->_capacity + 65536);
}

static void array_free_item(array_t *array, size_t loc) // clear l'adresse d'un element 
{
    if (array->_should_free_items != 0)
        free(array->_storage[loc]);
}

void array_new(array_t *array, size_t capacity, int should_free_items) //faire un nouveau tableau
{
    array->_storage = malloc(sizeof(void*) * capacity); //On alloue la mémoire pour l'array
    array->_capacity = capacity; // on indique la capacity
    array->_size = 0; // Nombre "d'élèments" rempli de l'array
    array->_should_free_items = should_free_items; // on indique si le tableau peut clear ou pas
}

void array_destroy(array_t *array) // destruction d'un tableau
{
    if (array == NULL || array->_storage == NULL) // vérification que le tab n'est pas vide
        return;

    if (array->_should_free_items != 0)//on vérifie qu'il peut s'auto free
        for (int i = 0; i < array->_size; i++)
            array_free_item(array, i);

    free(array->_storage); // free l'adresse de l'array
    array->_storage = NULL; // on set a NULL la valeur de l'adresse (sécurité)
    array->_capacity = 0; // on set a 0 la valeur de la capacity (sécurité)
    array->_size = 0; // on set a 0 la size (sécurité)
}

void array_reserve(array_t *array, size_t newCapacity) // augmente la capacité d'un tab
{
    assert(NULL != array);
    if (newCapacity <= array->_capacity) // vérification que la capacité est pas < à l'ancienne
        return;
    
    void **newAddr = realloc(array->_storage, sizeof(void*) * newCapacity); // init un nouveau tab
    array->_storage = newAddr; // update de l'adresse
    array->_capacity = newCapacity; // update de la capacité
}


void array_add(array_t *array, void *elem) // Ajouter un élements à la fin de l'array
{
    assert(NULL != array);
    if (array->_size + 1 > array->_capacity) // On vérifie la capacité
        array_reserve_more(array); // sinon on augmente avec la fonction en question
    
    size_t idx = array->_size; // récupération de la taille <=> l'index de l'elem a add (car on ajoute à la fin)
    array->_storage[idx] = elem; // add de l'elem
    array->_size++; // update de la size
}

void array_reverse(array_t *array)//Inverser un tableau
{
    assert(NULL != array);
    
    for (size_t i = 0, j = array->_size - 1; i < array->_size / 2; i++, j--)
    {
        void *tmp = array->_storage[i];
        array->_storage[i] = array->_storage[j];
        array->_storage[j] = tmp;
    }
}

void *array_get(array_t *array, size_t index) // recup un element du rang index
{
    assert(NULL != array);
    assert(index < array->_size);
    return array->_storage[index];
}

size_t array_get_size(array_t *array) // recup la taille du tableau
{
    assert(NULL != array);
    return array->_size;
}


