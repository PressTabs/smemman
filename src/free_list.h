// Header file for free_list.c
#pragma once

#include "mblk_list.h"

typedef mblk_list_t free_list_t;

void free_list_new(free_list_t *list);
void free_list_push(free_list_t *list, void *mblk);
void free_list_merge_if_able(free_list_t *list);
void *free_list_pop(free_list_t *list);
void free_list_split(free_list_t *list, size_t resize);
