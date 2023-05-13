#include <my_string.h>

#define free_if_allocated(s)  \
    if (s->buffer != NULL) {  \
        string_free(s);       \
    }

size_t my_strlen(const char *s) {
    const char *s2;
    for (s2 = s; *s2; ++s2);
    return (s2 - s);
}

bool string_init_with_capacity(string *s, const size_t start_cap) {
    s->capacity = start_cap;
    s->buffer = (char *) malloc(s->capacity * sizeof(char));
    if (s->buffer == NULL) 
        return false;
    s->size = 0;
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

bool string_init_from_string(string* s, string* s2) {
    return string_init_from_cstr(s, s2->buffer);
}

bool string_init(string* s) {
    return string_init_with_capacity(s, INITIAL_STRING_CAP);
} 

void string_free(string* s) {
    if (s->buffer != NULL) {
        free(s->buffer);
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

bool string_increase_capacity(string* s, size_t new_cap) {
    s->capacity = new_cap;
    s->buffer = (char *) realloc(s->buffer, s->capacity * sizeof(char));
    memset(&s->buffer[s->size], '\0', s->capacity - s->size);
    return s->buffer != NULL;
}

bool string_push_back_c(string* s, const char c) {
    if ((s->size >= s->capacity) && 
        !string_increase_capacity(s, NEXT_MEM_SIZE(s->capacity, MEM_BLOCK_SIZE))) 
    {
        return false;
    }
    s->buffer[s->size++] = c;
    s->buffer[s->size] = '\0';
    return true;
}

bool string_push_back_cstr(string* s, const char *s2) {
    const size_t len = my_strlen(s2);
    size_t new_cap = s->capacity;
    while ((new_cap = NEXT_MEM_SIZE(new_cap, MEM_BLOCK_SIZE)) <= s->size + len);
    if (!string_increase_capacity(s, new_cap)) {
        return false;
    }
    bool fail = (NULL == memcpy(&s->buffer[s->size], s2, len * sizeof(char)));
    s->size += len;
    s->buffer[s->size] = '\0';
    return !fail;
}

bool string_push_back_string(string* s, string* s2) { return string_push_back_cstr(s, s2->buffer); }

bool string_replace_c(string* s, const char c, size_t idx) {
    if (idx >= s->size) {
        return false;
    }
    return (c == (s->buffer[idx] = c));
}

bool string_replace_cstr(string* s, const char* s2, const size_t start, const size_t count) { 
    if (start + count >= s->capacity && 
        !string_increase_capacity(s, NEXT_MEM_SIZE(start + count, MEM_BLOCK_SIZE))) 
    {
        return false;
    }
    return (NULL != memcpy(&s->buffer[start], s2, count * sizeof(char)));
}

bool string_replace_string(string* s, string* s2, const size_t start, const size_t count) { 
    return string_replace_cstr(s, s2->buffer, start, count); 
}

bool string_replace_contents_cstr(string* s, const char* s2, const size_t count) {
    if (!string_clear(s)) {
        return false;
    }

    if (count > s->capacity && 
        !string_increase_capacity(s, NEXT_MEM_SIZE(count, MEM_BLOCK_SIZE)))
    {
        return false;
    }

    return string_replace_cstr(s, s2, 0, count);

//    return string_clear(s) 
//        && (count > s->capacity ? string_increase_capacity(s, NEXT_MEM_SIZE(count, MEM_BLOCK_SIZE)) : true)
//        && string_replace_cstr(s, s2, 0, count);
}

bool string_replace_contents_string(string* s, string* s2, const size_t count) {
    return string_replace_contents_cstr(s, s2->buffer, count);
}

bool string_equals(string lhs, string rhs) {
    return !strcmp(lhs.buffer, rhs.buffer);
}

# if 1
int main(void) {
    string s;
    string_init(&s);
    for (char c = 'A'; c <= 'Z'; ++c) {
        string_push_back_c(&s, c);
    }

    const char* s2 = "hello";
    string_push_back_cstr(&s, s2);

    string s3;
    string_init_from_cstr(&s3, "world");
    string_push_back_string(&s, &s3);

    string_replace_string(&s, &s3, 2, s3.size);
    string_replace_cstr(&s, s2, 8, 5);

    string_replace_c(&s, 'b', 12);
    string_replace_c(&s, 'a', 13);
    string_replace_c(&s, 'r', 14);

    printf("%s\n", s.buffer);
    printf("Length: %zu\n", my_strlen(s.buffer));

    string_clear(&s);
    string_push_back_cstr(&s, s2);
    string_push_back_string(&s, &s3);

    string test_str; string_init_from_string(&test_str, &s);

    printf("\n\n\n");

    for (int i = 0; i < 100; ++i)
        string_push_back_string(&test_str, &s);

    printf("s: %s\n", s.buffer);
    printf("Length: %zu\n", my_strlen(s.buffer));
    printf("test_str: %s\n", test_str.buffer);
    printf("Length: %zu\n", my_strlen(test_str.buffer));

    printf("\n\n\n");

    const char *replace_contents_test_str = "This is a string that has replaced test_str.";

    bool result = string_replace_contents_cstr(&test_str, 
            replace_contents_test_str, 
            my_strlen(replace_contents_test_str));
    printf("test_str: %s\n", test_str.buffer);
    printf("return val: %s\n", result ? "true" : "false");
    printf("\n");

    const char *replace_contents_s = "Replaced contents of s.";
    result = string_replace_contents_cstr(&s, 
            replace_contents_s, 
            my_strlen(replace_contents_s));
    printf("s: %s\n", s.buffer);
    printf("return val: %s\n", result ? "true" : "false");

    string_free(&s);
    string_free(&s3);
    string_free(&test_str);

    string lhs, rhs;
    string_init_from_cstr(&lhs, "Hello, World\n");
    string_init_from_cstr(&rhs, "Hello, World\n");
    printf("equals: %s\n", string_equals(lhs, rhs) ? "true" : "false");

    string_init_from_cstr(&s, "This is a new s!");
    printf("new s: %s\n", s.buffer);
}
#endif // main
