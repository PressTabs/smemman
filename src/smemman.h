// Header file for smalloc.c
// Provides s-methods for s-memory management with analogs to <stdlib.h>

#pragma once

#include <stddef.h>

void smemman_init(void);
void *smalloc(size_t size);
void *scalloc(size_t nitems, size_t size);
void *srealloc(void *ptr, size_t size);
void sfree(void *ptr);
