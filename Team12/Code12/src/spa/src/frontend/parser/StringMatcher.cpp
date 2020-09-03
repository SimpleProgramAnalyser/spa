/**
 * Implementation of a trie of characters,
 * to determine whether a given string can
 * be found within a previously specified
 * set of strings.
 *
 * Reference:
 * https://en.wikipedia.org/wiki/Trie
 */

#include "StringMatcher.h"

using namespace str_match;

template <typename T>
TrieNode<T>::~TrieNode()
{
    if (child != nullptr) {
        delete child;
    }

    if (next != nullptr) {
        delete next;
    }
}

template <typename T>
TrieNode<T>::TrieNode(Character value, TrieNode* child, TrieNode* next, T* reference):
    value(value), child(child), next(next), reference(reference)
{}

template <typename T>
Trie<T>::Trie(): firstCharMap(), ownedNodes() {}

template <typename T>
Trie<T>::~Trie()
{
    for (TrieNode<T>* node : ownedNodes) {
        delete node;
    }
}

/**
 * Helper function to check whether a TrieNode
 * pointer is the nullNode, and return a new
 * node for the given character if it is.
 * Also, store this new nodes in the ownedNodes.
 *
 * If it is not, do not do anything as the node
 * already exists in the trie. Return the original node.
 *
 * @param c The character for the new node.
 * @param location The original TrieNode pointer.
 * @param ref The reference to T to link with the node.
 *            (default value is nullptr)
 */
template <typename T>
TrieNode<T>* Trie<T>::getNodeIfNull(const char c, TrieNode<T>* node, T* ref)
{
    if (node == nullptr) {
        auto* newNode = new TrieNode<T>(c, nullptr, nullptr, ref);
        ownedNodes.push_back(newNode);
        return newNode;
    } else {
        // node already exists
        return node;
    }
}

template <typename T>
Void Trie<T>::addEntryToTrie(const String& str, T* newReference)
{
    const char* strChar = str.c_str();
    // ignore empty strings
    if (*strChar == '\0') {
        return;
    }
    // navigate hash table for first character
    firstCharMap.at(*strChar) = getNodeIfNull(*strChar, firstCharMap.at(*strChar));
    // traverse the trie
    TrieNode<T>* currentNode = firstCharMap.at(*strChar);
    strChar++;
    while (*strChar != '\0') {
        currentNode->child = getNodeIfNull(*strChar, currentNode->child);
        currentNode = currentNode->child;
        if (currentNode->value != *strChar) {
            // loop through "next"s
            bool matchFound = false;
            while (!matchFound && currentNode->next != nullptr) {
                if (currentNode->next->value == *strChar) {
                    matchFound = true;
                }
                currentNode = currentNode->next;
            }
            if (!matchFound) {
                // currentNode->next == nullptr
                currentNode->next = getNodeIfNull(*strChar, currentNode->next);
                currentNode = currentNode->next;
            }
        }
        strChar++;
    }
    // store the end of the string
    if (currentNode->child == nullptr) {
        currentNode->child = getNodeIfNull(*strChar, currentNode->child, newReference);
    } else {
        currentNode = currentNode->child;
        // loop through "next"s
        bool matchFound = false;
        while (currentNode->next != nullptr) {
            if (currentNode->next->value == *strChar) {
                // could be an error, string is already stored in trie
                matchFound = true;
                break;
            }
            currentNode = currentNode->next;
        }
        if (!matchFound) {
            currentNode->next = getNodeIfNull(*strChar, currentNode->next, newReference);
        } else {
            // string was already stored in the trie
            // just replace old value with new one
            currentNode->next->reference = newReference;
        }
    }
}

template <typename T>
T* Trie<T>::matchString(const String& str)
{
    const char* strChar = str.c_str();
    // ignore empty strings
    if (*strChar == '\0') {
        return nullptr;
    }
    // check for first character
    TrieNode<T>* firstNode = firstCharMap.at(*strChar);
    if (firstNode == nullptr) {
        return nullptr;
    }
    // traverse the trie
    TrieNode<T>* currentNode = firstNode->child;
    strChar++;
    while (*strChar != '\0') {
        if (currentNode == nullptr) {
            return nullptr;
        }
        bool currentMatches = currentNode->value == *strChar;
        if (!currentMatches) {
            currentNode = currentNode->next;
        } else {
            currentNode = currentNode->child;
            strChar++;
        }
    }
    // check last character
    if (currentNode->value == *strChar) {
        return currentNode->reference;
    } else {
        // loop through "next"s
        while (currentNode->next != nullptr) {
            if (currentNode->next->value == *strChar) {
                return currentNode->next->reference;
            }
            currentNode = currentNode->next;
        }
        return nullptr;
    }
}
