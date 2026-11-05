#ifndef PROJECT2COP3530_TRIE_H
#define PROJECT2COP3530_TRIE_H

#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class Trie {
    struct Entry {
        bool endOfKey; // Identifies node as the end of an inserted key.
        unordered_map<char, Entry*> children; // Stores pointers to all child nodes.
        vector<int> logIds; // Stores all log Ids that map to the key.
        
        Entry(); // Initializes trie node.
    };
    Entry* root; // Empty root node.
    int numKeys; // Total number of keys that have been inserted.

    public:
        // ================== CONSTRUCTOR ================== //
        Trie(); // Initializes trie with root.
        // ================== HELPER FUNCTIONS ================== //
        void clearHelper(Entry* currentNode); // Recursively deletes all nodes. Called by clear().
        // ================== OPERATIONS ================== //
        void insert(string key, int logId); // Inserts key and log Id into trie.
        vector<int> search(string key); // Searches for key and returns list of log Ids.
        int size(); // Gets number of keys in trie.
        bool startsWith(string prefix); // Checks and returns if there is a key that starts with prefix.
        void clear(); // Clears trie of all nodes.
};


#endif //PROJECT2COP3530_TRIE_H