
#include "drop_list.h"
#include "mblk_list.h"

// Makes a new drop_list_t.
void drop_list_new(drop_list_t *list) {
  mblk_list_new((mblk_list_t *)list);
  return;
}

// Pushes to drop_list_t, sorting by address.
// Resets to initial.
void drop_list_push(drop_list_t *list, void *mblk) {
  mblk_list_push((mblk_list_t *)list, mblk);
  mblk_list_reset((mblk_list_t *)list);
  return;
}

// Pops from the current mblk in the drop_list_t.
void *drop_list_pop(drop_list_t *list) {
  return mblk_list_pop((mblk_list_t *)list);
}
