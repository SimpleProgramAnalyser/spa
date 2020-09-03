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

TrieNode::TrieNode(Character value, TrieNode* child, TrieNode* next): value(value), child(child), next(next) {}

TrieNode::~TrieNode()
{
    // checks to avoid deleting nullNode prematurely
    if (child != nullptr && child->value != '\0') {
        delete child;
    }

    if (next != nullptr && next->value != '\0') {
        delete next;
    }
}

Trie::Trie(): firstCharMap(), ownedNodes() {}

Trie::~Trie()
{
    for (TrieNode* node : ownedNodes) {
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
 */
TrieNode* Trie::getNodeIfNull(const char c, TrieNode* node)
{
    if (node == nullptr) {
        auto* newNode = new TrieNode(c, nullptr, nullptr);
        ownedNodes.push_back(newNode);
        return newNode;
    } else {
        // node already exists
        return node;
    }
}

Void Trie::addEntryToTrie(const String& str)
{
    const char* strChar = str.c_str();
    // ignore empty strings
    if (*strChar == '\0') {
        return;
    }
    // navigate hash table for first character
    firstCharMap.at(*strChar) = getNodeIfNull(*strChar, firstCharMap.at(*strChar));
    // traverse the trie
    TrieNode* currentNode = firstCharMap.at(*strChar);
    strChar++;
    while (*strChar != '\0') {
        currentNode->child = getNodeIfNull(*strChar, currentNode->child);
        currentNode = currentNode->child;
        if (currentNode->value != *strChar) {
            // loop through "nexts"
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
        currentNode->child = getNodeIfNull(*strChar, currentNode->child);
    } else {
        currentNode = currentNode->child;
        // loop through "nexts"
        bool matchFound = false;
        while (currentNode->next != nullptr) {
            if (currentNode->next->value == *strChar) {
                matchFound = true;
                break;
            }
            currentNode = currentNode->next;
        }
        if (!matchFound) {
            currentNode->next = getNodeIfNull(*strChar, currentNode->next);
        }
    }
}

Boolean Trie::matchString(const String& str)
{
    const char* strChar = str.c_str();
    // ignore empty strings
    if (*strChar == '\0') {
        return false;
    }
    // check for first character
    TrieNode* firstNode = firstCharMap.at(*strChar);
    if (firstNode == nullptr) {
        return false;
    }
    // traverse the trie
    TrieNode* currentNode = firstNode->child;
    strChar++;
    while (*strChar != '\0') {
        if (currentNode == nullptr) {
            return false;
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
        return true;
    } else {
        // loop through "nexts"
        while (currentNode->next != nullptr) {
            if (currentNode->next->value == *strChar) {
                return true;
            }
            currentNode = currentNode->next;
        }
        return false;
    }
}
