#include "mblk_list.h"
#include "mblk.h"
#include <stdint.h>

// All of the scuff with this code is for two reasons:
// 1. I didn't write setter methods
// 2. I made the list weird
// As such, a 'pop()' method must be tailored to each list.

// Initializes a mblk_list_t.
void mblk_list_new(mblk_list_t *list) {
  list->init = NULL;
  list->curr = NULL;
  list->prev = NULL;
  return;
}

// Resets the mblk_list_t to the initial.
void mblk_list_reset(mblk_list_t *list) {
  list->curr = list->init;
  list->prev = NULL;
  return;
}

// Pushes a mblk to a mblk_list_t after the curr.
// Fails if init/curr == NULL.
void mblk_list_put(mblk_list_t *list, void *mblk) {
  *mblk_next(mblk) = *mblk_next(list->curr);
  *mblk_next(list->curr) = (uintptr_t)mblk;
  list->curr = mblk;
  return;
}

// Pushes a mblk to a mblk_list_t as the init,
// or before the init, if one exists.
// Should only be used if prev == NULL.
void mblk_list_put_back(mblk_list_t *list, void *mblk) {
  *mblk_next(mblk) = (uintptr_t)list->init;
  list->init = mblk;
  list->curr = mblk;
  return;
}

// Gets prev.
void *mblk_list_prev(mblk_list_t *list) { return list->prev; }

// Gets curr.
void *mblk_list_curr(mblk_list_t *list) { return list->curr; }

// Gets next.
// Fails if init/curr == NULL.
void *mblk_list_next(mblk_list_t *list) {
  return (void *)mblk_next(list->curr);
}

// Leaps the list head forward an entry. Returns the next mblk.
// Fails if init/curr == NULL.
void *mblk_list_jump(mblk_list_t *list) {
  list->prev = list->curr;
  list->curr = mblk_list_next(list);
  return mblk_list_next(list);
}

// Pushes the mblk to the correct place in the list.
// Sets init, curr, and prev as appropriate.
void mblk_list_push(mblk_list_t *list, void *mblk) {
  if (list->init == NULL || mblk < list->init) {
    mblk_list_put_back(list, mblk);
  } else {
    void *next = mblk_list_next(list);
    while (next != NULL && mblk < next) {
      next = mblk_list_jump(list);
    }

    mblk_list_put(list, mblk);
  }
  return;
}

// Pops the current mblk from the list.
// Resets to initial.
// Fails if list->init == NULL.
void *mblk_list_pop(mblk_list_t *list) {
  void *popped = list->curr;
  if (list->prev == NULL) {
    list->init = mblk_list_next(list);
  } else {
    *mblk_next(list->prev) = (uintptr_t)mblk_list_next(list);
  }
  mblk_list_reset(list);
  return popped;
}
