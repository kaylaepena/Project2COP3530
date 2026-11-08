#ifndef PROJECT2COP3530_TRIE_H
#define PROJECT2COP3530_TRIE_H

#include <string>
#include <unordered_map>
#include <set>
#include <vector>
using namespace std;

class Trie {
    // ================== ENTRY STRUCT ================== //
    struct Entry {
        bool endOfKey; // Identifies node as the end of an inserted key.
        unordered_map<char, Entry*> children; // Stores key-value pairs of characters and entry nodes.
        set<int> logIds; // Stores all IDs that map to the key.
        
        Entry(); // Constructor
    };

    // ================== TRIE ATTRIBUTES ================== //
    Entry* root; // Empty root node.
    int numKeys; // Total number of keys that have been inserted.

    // ================== HELPER FUNCTIONS ================== //
    void collectPrefixIds(Entry* currentNode, vector<int>& prefixIds); // Recursively appends all IDs below prefix. Called by searchPrefix(string prefix).
    void clearHelper(Entry* currentNode); // Recursively deletes all nodes. Called by clear().

    public:
        // ================== CONSTRUCTOR / DESTRUCTOR ================== //
        Trie(); // Constructor
        ~Trie(); // Destructor
        // ================== OPERATIONS ================== //
        void insert(string key, int logId); // Inserts key and ID into trie.
        vector<int> search(string key); // Searches for key and returns list of IDs.
        int size(); // Gets number of keys in trie.
        bool startsWith(string prefix); // Returns whether there is a key that starts with prefix.
        vector<int> searchPrefix(string prefix); // Returns IDs associated with prefix.
        void clear(); // Clears trie of all nodes.
};


#endif //PROJECT2COP3530_TRIE_H