#include "Trie.h"

Trie::Entry::Entry() {
    // Initializes trie node.
    endOfKey = false;
    children = {};
    logIds = {};
}

Trie::Trie() {
    // Initializes trie.
    root = new Entry(); // Empty Root
    numKeys = 0; // Number of Keys in Trie
}

void Trie::clearHelper(Entry *currentNode) {
    // Recursively removes all nodes and stored logIds from trie.
    if (currentNode == nullptr) {return;}
    for (auto child : currentNode->children) {
        clearHelper(child.second);
    }
    delete currentNode;
}

void Trie::insert(string key, int logId) {
    // Inserts trie node based on key, and appends logId to the key's list.
    Entry* currentNode = root; // Start at root.
    for (char c : key) {
        // Iterate through all the characters in key.
        if (currentNode->children.find(c) == currentNode->children.end()) {
            // New character in sequence
            currentNode->children[c] = new Entry();
        }
        currentNode = currentNode->children[c];
    }
    if (currentNode->endOfKey) {
        // Key is already in trie, so only append the logId.
        currentNode->logIds.push_back(logId);
    }
    else {
        // Key is new.
        currentNode->endOfKey = true; // Establish as end node.
        currentNode->logIds.push_back(logId); // Append logId.
        numKeys++; // Increment total number of keys in trie.
    }
}

vector<int> Trie::search(string key) {
    // Searches for log Ids that pertain to the given key.
    // If the returned vector is empty, then the key does not exist in the trie.
    Entry* currentNode = root; // Start at root.
    for (char c : key) {
        // Iterate through all the characters in key.
        if (currentNode->children.find(c) == currentNode->children.end()) {
            // Key is not found in trie.
            return vector<int> {}; // Returns empty vector.
        }
        currentNode = currentNode->children[c];
    }
    if (currentNode->endOfKey) {
        return currentNode->logIds; // Returns logIds stored at end node of key.
    }
    return vector<int> {}; // Returns empty vector otherwise.
}

int Trie::size() {
    // Returns total number of keys stored in the trie.
    return numKeys;
}

bool Trie::startsWith(string prefix) {
    // Returns whether the given prefix is found within the trie.
    Entry* currentNode = root; // Start at root.
    for (char c : prefix) {
        // Iterate through all the characters in prefix.
        if (currentNode->children.find(c) == currentNode->children.end()) {
            // Prefix does not exist in trie.
            return false;
        }
        currentNode = currentNode->children[c];
    }
    // Prefix exists in trie.
    return true;
}

void Trie::clear() {
    // Removes all nodes and stored logIds from trie.
    if (numKeys == 0) {return;} // Trie is already clear.
    clearHelper(root);
    root = new Entry();
    numKeys = 0;
}
