
#include "util/includestd.h"
#include "util/tokenpaste.h"

#define N_SYMBOLS 26

#define TrieNode(DTYPE) PASTE2(DTYPE, _TrieNode)
typedef struct TrieNode {
    char symbol;
    struct TrieNode *children;
} TrieNode;

bool new_TrieNode(TrieNode *node, const char symbol);
void destroy_TrieNode(TrieNode *node);

typedef struct Trie {
    TrieNode *head;
} Trie;

void new_Trie(Trie *trie);
void destroy_Trie(Trie *trie);
void insert_Trie(Trie *trie, const char *value);
void remove_trie(Trie *trie, const char *value);

