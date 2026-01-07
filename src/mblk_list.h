// Corresponds to mblk_list.c
// Provides an interface for managing free & drop mblk_list(s)

#pragma once

#include "mblk.h"

#ifndef NULL
#define NULL
#endif

// Holds an initial mblk
typedef struct mblk_list {
  void *init;
  void *curr;
  void *prev;
} mblk_list_t;

void mblk_list_new(mblk_list_t *list);
void mblk_list_reset(mblk_list_t *list);
void mblk_list_put(mblk_list_t *list, void *mblk);
void mblk_list_put_back(mblk_list_t *list, void *mblk);
void *mblk_list_prev(mblk_list_t *list);
void *mblk_list_curr(mblk_list_t *list);
void *mblk_list_next(mblk_list_t *list);
void *mblk_list_jump(mblk_list_t *list);
void mblk_list_push(mblk_list_t *list, void *mblk);
void *mblk_list_pop(mblk_list_t *list);
