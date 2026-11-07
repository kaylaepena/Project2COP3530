#include "Trie.h"

#include <algorithm>

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

Trie::~Trie() {
    // Deletes all pointers to children in trie.
    clearHelper(root);
}

void Trie::collectPrefixIds(Entry *currentNode, vector<int> &prefixIds) {
    // Recursively appends all logIds below prefix.
    if (currentNode == nullptr) {return;} // Node is empty.

    if (currentNode->endOfKey) {
        // Append key's logIds to prefixIds vector.
        prefixIds.insert(prefixIds.end(), currentNode->logIds.begin(), currentNode->logIds.end());
    }

    for (auto child : currentNode->children) {
        // Continue searching through all the currentNode's children.
        collectPrefixIds(child.second, prefixIds);
    }
}

void Trie::clearHelper(Entry *currentNode) {
    // Recursively removes all nodes and stored logIds from trie.
    if (currentNode == nullptr) {return;}
    for (auto& child : currentNode->children) {
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
    // Check endOfKey status before appending logId.
    if (currentNode->endOfKey) {
        // Key is already in trie, so only append the logId.
        currentNode->logIds.insert(logId);
    }
    else {
        // Key is new.
        currentNode->endOfKey = true; // Establish as end node.
        currentNode->logIds.insert(logId); // Append logId.
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
        vector<int> results(currentNode->logIds.begin(), currentNode->logIds.end());
        return results; // Returns logIds stored at end node of key.
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

vector<int> Trie::searchPrefix(string prefix) {
    // Finds all logIds for keys matching the given prefix.
    vector<int> prefixIds = {};
    if (!startsWith(prefix)) {
        // Prefix does not exist in trie.
        return prefixIds; // Return empty vector.
    }

    Entry* currentNode = root; // Start at root.
    for (char c : prefix) {
        // Iterate through all the characters in prefix.
        currentNode = currentNode->children[c];
    }
    collectPrefixIds(currentNode, prefixIds); // Collect at and below prefix end node.
    sort(prefixIds.begin(), prefixIds.end()); // Sort IDs.
    prefixIds.erase(unique(prefixIds.begin(), prefixIds.end()), prefixIds.end()); // Delete consecutive duplicates.
    return prefixIds;
}

void Trie::clear() {
    // Removes all nodes and stored logIds from trie.
    if (numKeys == 0) {return;} // Trie is already clear.
    clearHelper(root);
    root = new Entry();
    numKeys = 0;
}
