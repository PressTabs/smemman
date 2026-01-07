// Corresponds to drop_list.c
#pragma once

#include "mblk_list.h"

typedef mblk_list_t drop_list_t;

void drop_list_new(drop_list_t *list);
void drop_list_push(drop_list_t *list, void *mblk);
void *drop_list_pop(drop_list_t *list);
