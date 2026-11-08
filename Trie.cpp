#include "Trie.h"

#include <algorithm>

// ================== ENTRY STRUCT ================== //

Trie::Entry::Entry() {
    // Initializes trie node, or entry.
    endOfKey = false;
    children = {};
    logIds = {};
}

// ================== TRIE CONSTRUCTOR / DESTRUCTOR ================== //

Trie::Trie() {
    // Initializes trie.
    root = new Entry(); // Empty Root
    numKeys = 0; // Number of Keys in Trie
}

Trie::~Trie() {
    // Deletes all pointers to children in trie.
    clearHelper(root);
}

// ================== HELPER FUNCTIONS ================== //

void Trie::collectPrefixIds(Entry *currentNode, vector<int> &prefixIds) {
    // Recursively appends all IDs below prefix.
    if (currentNode == nullptr) {return;} // Node is null.

    // Check if the end of a key has been reached.
    if (currentNode->endOfKey) {
        // Append key's IDs to prefixIds vector.
        prefixIds.insert(prefixIds.end(), currentNode->logIds.begin(), currentNode->logIds.end());
    }

    // Iterate across current node's children.
    for (auto child : currentNode->children) {
        // Continue collecting through child node.
        collectPrefixIds(child.second, prefixIds);
    }
}

void Trie::clearHelper(Entry *currentNode) {
    // Recursively removes all nodes and stored IDs from trie.
    if (currentNode == nullptr) {return;} // Current node is null.

    // Iterate across current node's children.
    for (auto& child : currentNode->children) {
        // Continue removing through child node.
        clearHelper(child.second);
    }
    delete currentNode; // Deletes node.
}

// ================== OPERATIONS ================== //

void Trie::insert(string key, int logId) {
    // Inserts trie node based on key and inserts ID into the key's set.
    Entry* currentNode = root; // Starts at root.
    for (char c : key) {
        // Iterate through all the characters in key.
        if (currentNode->children.find(c) == currentNode->children.end()) {
            // New character in sequence.
            currentNode->children[c] = new Entry();
        }
        currentNode = currentNode->children[c];
    }
    // Check endOfKey status before inserting ID.
    if (currentNode->endOfKey) {
        // Key is already in trie, so only append the ID.
        currentNode->logIds.insert(logId);
    }
    else {
        // Key is new.
        currentNode->endOfKey = true; // Establishes as end node.
        currentNode->logIds.insert(logId); // Inserts ID.
        numKeys++; // Increments total number of keys in trie.
    }
}

vector<int> Trie::search(string key) {
    // Searches for and returns IDs that pertain to the given key.
    // If the returned vector is empty, then the key does not exist in the trie.
    Entry* currentNode = root; // Starts at root.
    for (char c : key) {
        // Iterate through all the characters in key.
        if (currentNode->children.find(c) == currentNode->children.end()) {
            // Key is not found in trie.
            return vector<int> {}; // Returns empty vector.
        }
        currentNode = currentNode->children[c];
    }
    // Check endOfKey status.
    if (currentNode->endOfKey) {
        // Key has previously been inserted.
        vector<int> results(currentNode->logIds.begin(), currentNode->logIds.end());
        return results; // Returns IDs stored at end node of key.
    }
    // Prefix matching given key was found, but key was not inserted.
    return vector<int> {}; // Returns empty vector.
}

int Trie::size() {
    // Returns total number of keys stored in the trie.
    return numKeys;
}

bool Trie::startsWith(string prefix) {
    // Returns whether the given prefix is found within the trie.
    Entry* currentNode = root; // Starts at root.
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
    // Finds and returns all IDs for keys matching the given prefix.
    vector<int> prefixIds = {};

    // Check whether prefix exists in trie.
    if (!startsWith(prefix)) {
        // Prefix does not exist in trie.
        return prefixIds; // Returns empty vector.
    }

    Entry* currentNode = root; // Starts at root.
    for (char c : prefix) {
        // Iterate through all the characters in prefix.
        currentNode = currentNode->children[c];
    }
    collectPrefixIds(currentNode, prefixIds); // Collects IDs at and below prefix end node.
    sort(prefixIds.begin(), prefixIds.end()); // Sorts IDs in ascending order.
    prefixIds.erase(unique(prefixIds.begin(), prefixIds.end()), prefixIds.end()); // Erases consecutive duplicates.
    return prefixIds; // Returns vector of IDs.
}

void Trie::clear() {
    // Removes all nodes and stored IDs from trie.
    if (numKeys == 0) {return;} // Returns if trie is already clear.
    clearHelper(root); // Starts clearing at root node.
    root = new Entry(); // Creates new root with no children.
    numKeys = 0; // Restarts total key count at zero.
}
