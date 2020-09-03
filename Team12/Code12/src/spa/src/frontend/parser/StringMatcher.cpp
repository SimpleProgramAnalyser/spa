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

TrieNode::TrieNode(Character value): value(value), child(nullptr), next(nullptr) {}

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

/**
 * There should only be one null node created for each trie.
 *
 * Null node is a trie node representing the
 * null character. It will signify that there
 * are no further characters after this node.
 */
Trie::Trie(): nullNode(new TrieNode('\0')), firstCharMap(), ownedNodes()
{
    firstCharMap.fill(nullptr);
}

/**
 * Helper function to check whether a TrieNode
 * pointer is the nullNode, and change it to point
 * to a new node for the given character if it is.
 * Also, store this new nodes in the ownedNodes.
 *
 * If it is not, do not do anything as the node
 * already exists in the trie.
 *
 * @param c The character for the new node.
 * @param location The location of the TrieNode pointer.
 */
void Trie::replaceNullNode(const char c, TrieNode** location) {
    if (*location == nullptr) {
        *location = new TrieNode(c, nullptr, nullptr);
        ownedNodes.push_back(*location);
    } else {
        // node already exists
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
    TrieNode** firstNode = &firstCharMap.at(*strChar);
    replaceNullNode(*strChar, firstNode);
    // traverse the trie
    TrieNode** currentNode = &((*firstNode)->child);
    strChar++;
    while (*strChar != '\0') {
        replaceNullNode(*strChar, currentNode);
        if ((*currentNode)->value == *strChar) {
            *currentNode = (*currentNode)->child;
        } else {
            // (*currentNode)->value != *strChar
            *currentNode = (*currentNode)->next;
        }
        strChar++;
    }
    // store the end of the string
    (*currentNode)->child = nullNode;
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
    return currentNode == nullNode;
}

Trie::~Trie()
{
    for (TrieNode* node : ownedNodes) {
        delete node;
    }
    // nullNode is deleted last
    delete nullNode;
}
