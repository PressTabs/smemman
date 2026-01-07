
#include "free_list.h"
#include "mblk.h"
#include "mblk_list.h"
#include <stddef.h>
#include <stdint.h>

// Makes a new free list
void free_list_new(free_list_t *list) {
  mblk_list_new((mblk_list_t *)list);
  return;
}

// Pushes any mblk to the free list, merges if possible, and resets.
void free_list_push(free_list_t *list, void *mblk) {
  mblk_list_push((mblk_list_t *)list, mblk);
  free_list_merge_if_able(list);
  return;
}

// Merges about the current head configuration. Resets to initial.
void free_list_merge_if_able(free_list_t *list) {
  // Redundant null checks, but it's fine.
  void *next = mblk_list_next((mblk_list_t *)list);
  if (next != NULL &&
      next == list->curr + MBLK_HEADER_SIZE + mblk_size(list->curr)) {
    mblk_merge(list->curr);
  }

  if (list->prev != NULL &&
      list->curr == list->prev + MBLK_HEADER_SIZE + mblk_size(list->prev)) {
    mblk_merge(list->prev);
  }

  // Necessary reset to fix the list head.
  mblk_list_reset((mblk_list_t *)list);
  return;
}

// Wrapper of mblk_list_pop(...). Fails if the list is empty.
void *free_list_pop(free_list_t *list) {
  return mblk_list_pop((mblk_list_t *)list);
}

// Splits the current mblk.
// The current mblk after execution will have the same address, but be resized.
// Handles next logic as appropriate, and the list head will be aligned to the
// resized element. Fails if the block is too small to be resized (see mblk.h)
void free_list_split(free_list_t *list, size_t resize) {
  mblk_split(list->curr, resize);
  return;
}
