/**
 * Methods for determining whether a given string
 * exists within a previously specified set of
 * strings. This set of strings must be known
 * beforehand to set up the trie data structure.
 */

#ifndef SPA_FRONTEND_STRING_MATCHER_H
#define SPA_FRONTEND_STRING_MATCHER_H

namespace str_match {

typedef char Character;

/**
 * Object representing a single node for the trie.
 */
class TrieNode {
public:
    const Character value;
    TrieNode* child{}; // points to next character in string
    TrieNode* next{};  // points to an alternative character

    explicit TrieNode(Character value);
    TrieNode(Character value, TrieNode* child, TrieNode* next);
    ~TrieNode();
    TrieNode(const TrieNode&) = delete;
    TrieNode& operator=(const TrieNode&) = delete;
    TrieNode(TrieNode&&) = delete;
    TrieNode& operator=(TrieNode&&) = delete;
};

/**
 * Object representing the root node of the trie, also
 * encapsulating methods to add strings to the trie
 * and to search the entire trie.
 */
class Trie {};

} // namespace str_match

#endif // SPA_FRONTEND_STRING_MATCHER_H
