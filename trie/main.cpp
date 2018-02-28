#include <iostream>
#include <array>
#include <memory>

struct TrieNode {
    bool is_word{false};
    std::array<std::unique_ptr<TrieNode>, 26> children;
};

struct TrieNodeWithLeakage {
    bool is_word{false};
    std::array<TrieNodeWithLeakage*, 26> children;

    TrieNodeWithLeakage() : is_word(false) {
        children.fill(nullptr);
    }
};

class TrieWithLeakage {
public:
    TrieWithLeakage() : root_(new TrieNodeWithLeakage()) { }
    ~TrieWithLeakage() = default;

    void insert(const std::string& word) {
        TrieNodeWithLeakage *curr = root_;
        for (const auto& ch : word) {
            if (!curr->children[ch - 'a']) {
                curr->children[ch - 'a'] = new TrieNodeWithLeakage();
            }
            curr = curr->children[ch - 'a'];
        }
        curr->is_word = true;
    }

    bool search(const std::string& word) {
        TrieNodeWithLeakage *curr = root_;
        for (const auto& ch : word) {
            if (curr->children[ch - 'a']) {
                curr = curr->children[ch - 'a'];
            } else {
                return false;
            }
        }

        return curr->is_word;
    }
private:
    TrieNodeWithLeakage *root_;
};

class Trie {
public:
    Trie() : root_(new TrieNode()) { }
    ~Trie() = default;

    void insert(const std::string& word) {
        TrieNode *curr = root_.get();
        for (const auto& ch : word) {
            if (!curr->children[ch - 'a']) {
                curr->children[ch - 'a'] = std::unique_ptr<TrieNode>(new TrieNode());
            }
            curr = curr->children[ch - 'a'].get();
        }
        curr->is_word = true;
    }

    bool search(const std::string& word) {
        TrieNode *curr = root_.get();
        for (const auto& ch : word) {
            if (curr->children[ch - 'a']) {
                curr = curr->children[ch - 'a'].get();
            } else {
                return false;
            }
        }

        return curr->is_word;
    }

private:
    std::unique_ptr<TrieNode> root_;
};

int main() {
    TrieWithLeakage trie;
    trie.insert("abc");
    trie.insert("bca");

    return 0;
}