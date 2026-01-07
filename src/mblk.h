// Header file for mblk.s
#pragma once

#include <stddef.h>
#include <stdint.h>

#ifndef NULL_PTR
#define NULL_PTR 0
#endif

#ifndef MBLK_HEADER_SIZE
#define MBLK_HEADER_SIZE 16
#endif

#ifndef MBLK_MIN_SPLIT_SIZE
#define MBLK_MIN_SPLIT_SIZE (MBLK_HEADER_SIZE + 1)
#endif

// TODO: Replace void* stuff with a typedef (void*) mblk

extern void mblk_new(void *addr, void *next, size_t size);
extern uintptr_t *mblk_next(void *addr);
extern size_t mblk_size(void *addr);
extern void *mblk_data(void *addr);
extern void *mblk_merge(void *addr);
extern void *mblk_split(void *addr, size_t resize);
extern void *mblk_zero(void *addr);
