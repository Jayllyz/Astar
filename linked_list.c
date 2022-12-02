#include "fonction.h"
#include <assert.h> //-> https://iq.opengenus.org/assert-h-in-c/
#include <stdio.h>
#include <stdlib.h>

linked_list_t *ll_new(void) // nouvelle liste
{
  linked_list_t *list = malloc(sizeof(linked_list_t));
  list->head = list->tail = NULL;
  return list;
}

linked_node_t *ll_new_node(elements *data) // nouvelle node
{
  linked_node_t *node = malloc(sizeof(linked_node_t));
  node->data = data;
  node->next = NULL;
  node->prev = NULL;
  return node;
}

linked_list_t *ll_destroy(linked_list_t *list) // detruire une liste
{
  if (NULL == list) {
    return NULL;
  }

  linked_node_t *i = list->head;
  while (i != NULL) {
    linked_node_t *node = i;
    i = i->next;
    free(node); // on libére chaque node
  }

  free(list);
  return NULL;
}

void ll_append(linked_list_t *list, elements *data) // ajouter
{
  assert(list != NULL);
  assert(data != NULL);
  if (list->tail == NULL) {
    ll_prepend(list, data);
    return;
  }
  linked_node_t *new_node = ll_new_node(data);
  list->tail->next = new_node;
  new_node->prev = list->tail;
  list->tail = new_node;
}

elements *ll_find(linked_list_t *list,
                  points point) // chercher un point
{
  linked_node_t *i = list->head;
  while (i != NULL) {
    if (point_equals(i->data->coord, point))
      return i->data;
    i = i->next;
  }
  return NULL;
}

void ll_insert_before(linked_list_t *list, linked_node_t *before,
                      elements *data) // insérer une data
{
  assert(list != NULL);
  assert(data != NULL);

  if (list->head == NULL || before == list->head) {
    ll_prepend(list, data);
    return;
  }

  assert(before != NULL);

  linked_node_t *one = before->prev;
  linked_node_t *two = ll_new_node(data);
  linked_node_t *three = before;

  one->next = two;
  two->prev = one;
  two->next = three;
  three->prev = two;
}

bool ll_is_empty(linked_list_t *list) { return list->head == NULL; }

void ll_prepend(linked_list_t *list, elements *data) // setup la tete
{
  assert(list != NULL);
  assert(data != NULL);

  linked_node_t *one = ll_new_node(data);
  linked_node_t *two = list->head;

  list->head = one;
  if (NULL == list->tail)
    list->tail = one;

  one->next = two;
  if (NULL != two)
    two->prev = one;
}

elements *ll_head_data(linked_list_t *list) // récupérer la tete
{
  if (NULL == list)
    return NULL;

  if (NULL == list->head)
    return NULL;

  linked_node_t *node = list->head;

  list->head = list->head->next;
  if (list->head == NULL) {
    list->tail = NULL;
  } else {
    list->head->prev = NULL;
  }

  elements *data = node->data;
  free(node);
  return data;
}

void ll_insert_heuristic(
    linked_list_t *list,
    elements *data) // insérer par rapport à l'heuristic
{
  assert(list != NULL);
  assert(data != NULL);
  const int heuristic = data->heuristic;
  if (NULL == list->head ||
      heuristic <= list->head->data->heuristic) {
    ll_prepend(list, data);
    return;
  }
  // parcours chaque noeud de la liste
  for (linked_node_t *i = list->head->next; i != NULL; i = i->next) {
    // si l'heuristique est plus basse que celle du noeud, on insert
    // avant le noeud
    if (heuristic < i->data->heuristic) {
      ll_insert_before(list, i, data);
      return;
    }
  }
  ll_append(list, data);
}
