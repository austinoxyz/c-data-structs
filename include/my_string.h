#ifndef MY_STRING_
#define MY_STRING_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>

size_t my_strlen(const char *s);

#define MEM_BLOCK_SIZE (16)
#define INITIAL_STRING_CAP (MEM_BLOCK_SIZE)
#define MAX_STRING_CAP ((INITIAL_STRING_CAP) * 32)

#define NEXT_MEM_SIZE(curr, block_size) ((block_size) * (((curr) / (block_size)) + 1))

typedef struct string {
    size_t capacity;
    size_t size;
    char *buffer;
} string;

void string_free(string* s);

string new_string();

bool string_increase_capacity(string* s, const size_t new_cap);

bool string_init_with_capacity(string *s, const size_t start_cap);
bool string_init_unallocated(string *s);
bool string_init(string* s);
void string_init_no_return(string *s);
bool string_init_from_cstr(string* s, const char* cs);
bool string_init_from_string(string* s, string* s2);

bool string_clear(string* s);

bool string_push_back_c(string* s, const char c);
bool string_push_back_cstr(string* s, const char *s2);
bool string_push_back_string(string* s, string* s2);

bool string_replace_c(string* s, const char c, size_t idx);
bool string_replace_cstr(string* s, const char* s2, const size_t start, const size_t count);
bool string_replace_string(string* s, string* s2, const size_t start, const size_t count);

bool string_replace_contents_cstr(string* s, const char* s2, const size_t count);
bool string_replace_contents_string(string* s, string* s2, const size_t count);

bool string_equals(string lhs, string rhs);
void string_copy(string *lhs, string rhs);

char *rand_cstr(const size_t size);

#endif // MY_STRING_
