/*
 * mt_arr_list.c
 *
 *  Created on: 02.05.2014
 *      Author: dhein
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "mt_arr_list.h"

/*!
 * \brief Computes the next highest power of two
 *
 * This nice little algorithm is taken from
 * http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
 */
static uint32_t round_next_power_two(uint32_t v) {
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  v+=(v==0); // handle v == 0 edge case
  return v;
}

//----------------------------------------------------------------------
static int is_power_of_two(uint32_t v) {
  return (v != 0) && ((v & (v - 1)) == 0);
}

//----------------------------------------------------------------------
static void print_hex_buffer(uint8_t* buffer, uint32_t size) {
  for (uint32_t i = 0; i < size; ++i) {
    printf("%02X", buffer[i]);
  }
}

//----------------------------------------------------------------------
mt_al_t *mt_al_create() {
  return calloc(1, sizeof(mt_al_t));
}

//----------------------------------------------------------------------
void mt_al_delete(mt_al_t *mt_al) {
  free(mt_al);
}

//----------------------------------------------------------------------
void mt_al_add(mt_al_t *mt_al, const uint8_t data[D_HASH_LENGTH]) {
  if (!mt_al) {
    // TODO Error handling
    return;
  }
  if (!data) {
    // TODO Error handling
    return;
  }
  if (mt_al->elems == 0) {
    // Add first element
    mt_al->store = malloc(HASH_LENGTH);
    if (!mt_al->store) {
      // TODO Error handling
    }
  } else if (is_power_of_two(mt_al->elems)) {
    // Need more memory
    size_t alloc = mt_al->elems * 2 * HASH_LENGTH;
    uint8_t *tmp = realloc(mt_al->store, alloc);
    if (!tmp) {
      // TODO Error Handling
      return;
    }
    printf("Allocated memory: %x, Old: %p, New: %p\n", alloc/HASH_LENGTH, mt_al->store, tmp);
    mt_al->store = tmp;
  }
  memcpy(&mt_al->store[mt_al->elems * HASH_LENGTH], data, HASH_LENGTH);
  mt_al->elems += 1;
}

//----------------------------------------------------------------------
void mt_al_truncate(mt_al_t *mt_al, uint32_t elems) {
  if (!mt_al) {
    // TODO error handling
    return;
  }
  if (elems > mt_al->elems) {
    // TODO error handling
    return;
  }
  if (elems == mt_al->elems) {
    // nothing to do
    return;
  }
  mt_al->elems = elems;
  if (elems == 0) {
    free(mt_al->store);
    return;
  }
  uint32_t alloc = round_next_power_two(elems) * HASH_LENGTH;
  uint8_t *tmp = realloc(mt_al->store, alloc);
  if (!tmp) {
    // TODO Do I need to check for a different pointer here?
    // TODO Error Handling
    return;
  }
  printf("Allocated memory: %x, Old: %p, New: %p\n", alloc/HASH_LENGTH, mt_al->store, tmp);
  mt_al->store = tmp;
}

//----------------------------------------------------------------------
uint8_t const * const mt_al_get(mt_al_t *mt_al, uint32_t offset) {
  if (!mt_al) {
    // TODO Error code handling
    return NULL;
  }
  if (offset >= mt_al->elems) {
    // TODO Error code handling (index out of bounds)
    return NULL;
  }
  return &mt_al->store[offset * HASH_LENGTH];
}

//----------------------------------------------------------------------
void mt_al_print(mt_al_t *mt_al) {
  if (!mt_al) {
    // TODO Error handling
  }
  printf("[%08X\n", mt_al->elems);
  for (uint32_t i = 0; i < mt_al->elems; ++i) {
    print_hex_buffer(&mt_al->store[i*HASH_LENGTH], HASH_LENGTH);
    printf("\n");
  }
  printf("]\n");
}
