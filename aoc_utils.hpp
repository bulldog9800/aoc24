#include "default.hpp"

#pragma once

class InputReader {
public:
    void readFile(const string &fileName);

    vector<string> &getLines();

private:
    vector<string> m_lines;
};

class StringTokenizer {
public:
    StringTokenizer() = delete;

    static vector<string> sGetTokens(string str, string delimiter);
};

template <typename ValueType>
class TreeNode {
public:
    bool isLeaf() {
        return m_children.empty();
    }

    vector<TreeNode *> &getChildren() {
        return m_children;
    }

    TreeNode() = default;

    TreeNode(ValueType val) : m_val(val){};

    ValueType getVal() {
        return m_val;
    }

    void addChild(TreeNode *pChild) {
        m_children.push_back(pChild);
    }

private:
    ValueType m_val;
    vector<TreeNode *> m_children;
};

struct PairHash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        return h1 ^ h2;
    }
};
