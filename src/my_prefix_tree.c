
#include "my_prefix_tree.h"

// ===================================
// TrieNode

bool new_TrieNode(TrieNode *node, const char symbol) {
    node->symbol = symbol;
    node->children = NULL;
    if ( !(node->children = (TrieNode *) malloc(N_SYMBOLS * sizeof(TrieNode))) )
        return false;
    return true;
}

void destroy_TrieNode(TrieNode *node) {
    if (!node->children) return;
    for (size_t i = 0; i < N_SYMBOLS; ++i)
        destroy_TrieNode(&node->children[i]);
}

// ===================================
// Trie

void new_Trie(Trie *trie) {
    new_TrieNode(trie->head, '\0');
}

void destroy_Trie(Trie *trie) {
    destroy_TrieNode(trie->head);
}

void insert_Trie(Trie *trie, const char *value) {

}

void remove_trie(Trie *trie, const char *value) {

}

