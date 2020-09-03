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

TrieNode::TrieNode(Character value): value(value) {}

TrieNode::TrieNode(Character value, TrieNode* child, TrieNode* next): value(value), child(child), next(next) {}

TrieNode::~TrieNode()
{
    delete child;
    delete next;
}

/**
 * This method is used to get a node representing the
 * null character. Null character will signify that
 * there are no further characters after this node.
 */
TrieNode* getNullNode() {}
