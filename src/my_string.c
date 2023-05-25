#include <my_string.h>

size_t my_strlen(const char *s) {
    const char *s2;
    for (s2 = s; *s2; ++s2);
    return (s2 - s);
}

bool string_init_with_capacity(string *s, const size_t start_cap) {
    s->capacity = start_cap;
    s->size = 0;
    if (start_cap == 0) 
        return true;
    s->buffer = (char *) malloc(s->capacity * sizeof(char));
    if (s->buffer == NULL) 
        return false;
    return true;
}

bool string_init_from_cstr(string* s, const char* cs) {
    const size_t start_cap = NEXT_MEM_SIZE(my_strlen(cs), MEM_BLOCK_SIZE);
    if (!string_init_with_capacity(s, start_cap)) {
        return false;
    }
    s->size = my_strlen(cs);
    strncpy(&s->buffer[0], &cs[0], s->size);
    memset(&s->buffer[s->size], '\0', s->capacity - s->size);
    return true;
}

bool string_init_unallocated(string *s) {
    return string_init_with_capacity(s, 0);
}

bool string_init_from_string(string* s, string s2) {
    return string_init_from_cstr(s, s2.buffer);
}

bool string_init(string* s) {
    return string_init_with_capacity(s, INITIAL_STRING_CAP);
} 

void string_init_no_return(string *s) {
    string_init(s);
}

void string_free(string* s) {
    if (s->buffer != NULL) {
        free(s->buffer);
        s->buffer = NULL;
    }
    s->size = 0;
    s->capacity = 0;
}

bool string_clear(string* s) {
    s->size = 0;
    s->buffer[0] = '\0';
    memset(&s->buffer[0], '\0', s->capacity);
    return true; // TODO realloc into smaller buffer if size is large
}

bool string_resize(string* s, size_t new_cap) {
    s->capacity = new_cap;
    s->buffer = (char *) realloc(s->buffer, s->capacity * sizeof(char));
    memset(&s->buffer[s->size], '\0', s->capacity - s->size);
    return s->buffer != NULL;
}

bool string_push_back_c(string* s, const char c) {
    if ((s->size >= s->capacity) && 
        !string_resize(s, NEXT_MEM_SIZE(s->capacity, MEM_BLOCK_SIZE))) 
    {
        return false;
    }
    s->buffer[s->size] = c;
    s->buffer[++s->size] = '\0';
    return true;
}

bool string_push_back_cstr(string* s, const char *s2) {
    const size_t len = my_strlen(s2);
    size_t new_cap = s->capacity;
    while (new_cap < s->size + len)
        new_cap = NEXT_MEM_SIZE(new_cap, MEM_BLOCK_SIZE);
    if (!string_resize(s, new_cap)) {
        return false;
    }
    bool fail = (NULL == memcpy(&s->buffer[s->size], s2, len * sizeof(char)));
    s->size += len;
    s->buffer[s->size] = '\0';
    return !fail;
}

bool string_push_back_string(string* s, string* s2) { return string_push_back_cstr(s, s2->buffer); }

bool string_replace_c(string* s, const char c, const size_t start, const size_t count) {
    if (start + count >= s->size)
    {
        if (!string_resize(s, NEXT_MEM_SIZE(start + count, MEM_BLOCK_SIZE))) 
            return false;
        s->size += start + count - s->size;
    }
    for (size_t i = start; i < start + count; ++i) s->buffer[i] = c;
    return true;
}

#if 0
bool string_replace_cstr(string* s, const char* s2, const size_t start, const size_t count) { 
    if (start + count >= s->size)
    {
        if (!string_resize(s, NEXT_MEM_SIZE(start + count, MEM_BLOCK_SIZE))) 
            return false;
        s->size += start + count - s->size;
    }
    return (NULL != memcpy(&s->buffer[start], s2, count * sizeof(char)));
}
#endif


bool string_replace_cstr(string* s, const char* s2, const size_t start, const size_t count) { 
    if (start + count >= s->capacity &&
        !string_resize(s, NEXT_MEM_SIZE(start + count, MEM_BLOCK_SIZE)))
            return false;
    bool ret = (NULL != memcpy(&s->buffer[start], s2, count * sizeof(char)));
    if (start + count >= s->size)
        s->size += start + count - s->size;
    return ret;
}

bool string_replace_string(string* s, string s2, const size_t start, const size_t count) { 
    return string_replace_cstr(s, s2.buffer, start, count); 
}

bool string_replace_contents_c(string* s, const char c, const size_t count) {
    if (!string_clear(s))
        return false;

    if (count > s->capacity && 
        !string_resize(s, NEXT_MEM_SIZE(count, MEM_BLOCK_SIZE)))
    {
        return false;
    }

    memset(s->buffer, c, count * sizeof(char));
    if ( !(s->buffer) ) return false;
    memset(&s->buffer[count], '\0', (s->capacity - count) * sizeof(char));
    s->size = count;
    return (bool) (s->buffer);
}

bool string_replace_contents_cstr(string* s, const char* s2, const size_t count) {
    if (!string_clear(s)) {
        return false;
    }

    if (count > s->capacity && 
        !string_resize(s, NEXT_MEM_SIZE(count, MEM_BLOCK_SIZE)))
    {
        return false;
    }

    return string_replace_cstr(s, s2, 0, count);
}

bool string_replace_contents_string(string* s, string s2, const size_t count) {
    return string_replace_contents_cstr(s, s2.buffer, count);
}

bool string_equals(string lhs, string rhs) {
    if (lhs.buffer == NULL || rhs.buffer == NULL) return false;
    return !strcmp(lhs.buffer, rhs.buffer);
}

void string_copy(string *lhs, string rhs) {
    lhs->size = rhs.size;
    lhs->capacity = rhs.capacity;
    lhs->buffer = (char *) malloc(lhs->capacity * sizeof(char));
    strncpy(lhs->buffer, rhs.buffer, rhs.capacity);
}

char *rand_cstr(const size_t size) {
    char *ret = (char*) malloc(size * sizeof(char));
    if (!ret) return NULL;
    struct timeval _tval;
    for (size_t i = 0; i < size; ++i) {
        gettimeofday(&_tval, NULL);
        srand(_tval.tv_usec * _tval.tv_sec);
        const char c = (char) (rand() % 96 + 32);
        ret[i] = c;
    }
    return ret;
}

