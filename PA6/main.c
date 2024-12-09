/* COP 3502C Programming Assignment 6
This program is written by: Victor Urdaneta
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26


typedef struct TrieNode {
    int nodeFreq;                
    int sumPrefixFreq;           
    int maxChildFreq;            
    struct TrieNode* children[26]; 
} TrieNode;

TrieNode* createNode() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->nodeFreq = 0;
    node->sumPrefixFreq = 0;
    node->maxChildFreq = 0;
    for (int i = 0; i < 26; i++) {
        node->children[i] = NULL;
    }
    return node;
}

// Traverses the trie and creates nodes as needed to add a new word.
void insert(TrieNode* root, const char* word, int freq) {
    TrieNode* current = root;

    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';

        // Create a new child node if necessary
        if (current->children[index] == NULL) {
            current->children[index] = createNode();
        }

        current = current->children[index];

        // Update sumPrefixFreq for cumulative frequency
        current->sumPrefixFreq += freq;
    }

    // Update the node frequency at the last character
    current->nodeFreq += freq;

    // Update maxChildFreq along the path
    TrieNode* parent = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        parent = parent->children[index];

        // Update maxChildFreq dynamically
        parent->maxChildFreq = 0;
        for (int j = 0; j < ALPHABET_SIZE; j++) {
            if (parent->children[j] != NULL) {
                if (parent->children[j]->sumPrefixFreq > parent->maxChildFreq) {
                    parent->maxChildFreq = parent->children[j]->sumPrefixFreq;
                }
            }
        }
    }
}

// Query the trie for the most likely next letters
// This function traverses the trie to the node representing the given prefix.
void query(TrieNode* root, const char* prefix) {
    TrieNode* current = root;

    // Traverse to the node corresponding to the prefix
    for (int i = 0; prefix[i] != '\0'; i++) {
        int index = prefix[i] - 'a';

        if (current->children[index] == NULL) {
            printf("unrecognized prefix\n");
            return;
        }
        current = current->children[index];
    }

    // Find the most likely next letters
    char result[ALPHABET_SIZE];
    int resultCount = 0;

    // Check all children of the current node
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (current->children[i] != NULL &&
            current->children[i]->sumPrefixFreq == current->maxChildFreq) {
            result[resultCount++] = 'a' + i;
        }
    }

    // Handle cases with no matching children
    if (resultCount == 0) {
        printf("unrecognized prefix\n");
    } else {
        result[resultCount] = '\0'; 
        printf("%s\n", result);
    }
}

void freeTrie(TrieNode* root) {
    if (root == NULL) return;

    for (int i = 0; i < 26; i++) {
        freeTrie(root->children[i]);
    }

    free(root);
}

int main() {
    TrieNode* root = createNode(); 
    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        int command;
        scanf("%d", &command);

        if (command == 1) { 
            char word[101];
            int freq;
            scanf("%s %d", word, &freq);
            insert(root, word, freq);
        } else if (command == 2) { 
            char prefix[101];
            scanf("%s", prefix);
            query(root, prefix);
        }
    }

    freeTrie(root); 
    return 0;
}
