/*
    This file is part of tgl-library

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

    Copyright Vitaly Valtman 2013-2015
*/

#ifndef __TOOLS_H__
#define __TOOLS_H__
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
//#include "tgl.h"
#include "crypto/err.h"

struct tgl_allocator {
  void *(*alloc)(size_t size);
  void *(*realloc)(void *ptr, size_t old_size, size_t size);
  void (*free)(void *ptr, int size);
  void (*check)(void);
  void (*exists)(void *ptr, int size);
};

#define talloc tgl_allocator->alloc
#define talloc0 tgl_alloc0
#define tfree tgl_allocator->free
#define tfree_str tgl_free_str
#define tfree_secure tgl_free_secure
#define trealloc tgl_allocator->realloc
#define tcheck tgl_allocator->check
#define texists tgl_allocator->exists
#define tstrdup tgl_strdup
#define tmemdup tgl_memdup
#define tstrndup tgl_strndup
#define tasprintf tgl_asprintf
#define tsnprintf tgl_snprintf


struct tgl_allocator *tgl_allocator;
double tglt_get_double_time (void);

int tgl_inflate (void *input, int ilen, void *output, int olen);
//void ensure (int r);
//void ensure_ptr (void *p);

static inline void out_of_memory (void) {
  fprintf (stderr, "Out of memory\n");
  exit (1);
}

static inline void ensure (int r) {
  if (!r) {
    fprintf (stderr, "Crypto error\n");
    TGLC_err_print_errors_fp (stderr);
    assert (0);
  }
}

static inline void ensure_ptr (void *p) {
  if (p == NULL) {
    out_of_memory ();
  }
}

void *tgl_alloc_debug (size_t size);
void *tgl_alloc_release (size_t size);

void *tgl_realloc_debug (void *ptr, size_t old_size, size_t size);
void *tgl_realloc_release (void *ptr, size_t old_size, size_t size);

void *tgl_alloc0 (size_t size);
char *tgl_strdup (const char *s);
char *tgl_strndup (const char *s, size_t n);

void tgl_free_debug (void *ptr, int size);
void tgl_free_release (void *ptr, int size);
//void tgl_free_str (void *ptr);
//void tgl_free_secure (void *ptr, int size);

void tgl_check_debug (void);
void tgl_exists_debug (void *ptr, int size);
void tgl_check_release (void);
void tgl_exists_release (void *ptr, int size);

void *tgl_memdup (const void *s, size_t n);

int tgl_snprintf (char *buf, int len, const char *format, ...) __attribute__ ((format (printf, 3, 4)));
int tgl_asprintf (char **res, const char *format, ...) __attribute__ ((format (printf, 2, 3)));

void tglt_secure_random (void *s, int l);
void tgl_my_clock_gettime (int clock_id, struct timespec *T);

static inline void tgl_free_str (void *ptr) {
  if (!ptr) { return; }
  tfree (ptr, strlen ((const char *)ptr) + 1);
}

static inline void tgl_free_secure (void *ptr, int size) {
  memset (ptr, 0, size);
  tfree (ptr, size);
}

static inline void hexdump (void *ptr, void *end_ptr) {
  int total = 0;
  unsigned char *bptr = (unsigned char *)ptr;
  while (bptr < (unsigned char *)end_ptr) {
    fprintf (stderr, "%02x", (int)*bptr);
    bptr ++;
    total ++;
    if (total == 16) {
      fprintf (stderr, "\n");
      total = 0;
    }
  }
  if (total) { fprintf (stderr, "\n"); }
}


#endif
