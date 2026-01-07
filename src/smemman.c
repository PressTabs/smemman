
#include "smemman.h"
#include "drop_list.h"
#include "free_list.h"
#include "mblk.h"
#include "sbrk.h"
#include <stdint.h>

free_list_t free_list;
drop_list_t drop_list;

// Initializes the library.
void smemman_init(void) {
  free_list_new(&free_list);
  drop_list_new(&drop_list);
}

// Same as malloc.
void *smalloc(size_t size) {
  void *mblk = free_list.curr;

  while (mblk != NULL) {
    size_t blksize = mblk_size(mblk);
    if (blksize >= size) {
      if (blksize >= size + MBLK_MIN_SPLIT_SIZE) {
        free_list_split(&free_list, size);
      }
      drop_list_push(&drop_list, free_list_pop(&free_list));
      return mblk_data(mblk);
    }
    mblk = mblk_list_next((mblk_list_t *)&free_list);
    free_list.prev = free_list.curr;
    free_list.curr = mblk;
  }

  mblk = sbrk(MBLK_HEADER_SIZE + size);
  mblk_new(mblk, NULL, size);
  drop_list_push(&drop_list, mblk);
  return mblk_data(mblk);
}

// Same as calloc.
void *scalloc(size_t nitems, size_t size) {
  size_t blksize = nitems * size;
  void *mblk = smalloc(blksize);
  mblk_zero(mblk);
  return mblk_data(mblk);
}

// Same as realloc.
// Could be made smarter by checking if in-place reallocation is possible.
void *srealloc(void *ptr, size_t size) {
  void *remblk = smalloc(size);
  size_t old_size = mblk_size(ptr);
  uint8_t *old_data = (uint8_t *)mblk_data(ptr);
  uint8_t *new_data = (uint8_t *)mblk_data(remblk);

  for (size_t idx = 0; idx < old_size; idx++) {
    new_data[idx] = old_data[idx];
  }

  sfree(ptr);

  return (void *)new_data;
}

// Same as free.
void sfree(void *ptr) {
  if (ptr == NULL) {
    return;
  }
  void *mblk = ptr - MBLK_HEADER_SIZE;
  while (mblk != NULL && drop_list.curr != mblk) {
    drop_list.prev = drop_list.curr;
    drop_list.curr = mblk_list_next((mblk_list_t *)&drop_list);
  }

  // if (mblk == NULL) { panic(); }
  // Handles an improper free.

  free_list_push(&free_list, drop_list_pop(&drop_list));
  return;
}
