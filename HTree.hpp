#ifndef HTREE_HPP
#define HTREE_HPP

#include <vector>
#include <unordered_map>

struct HTreeNode
{
    int weight = 0;
    char sign = '\0';
    int leftNodeID = -1;
    int rightNodeID = -1;
    int parentNodeID = -1;
};

using BytesBuffer = std::string;
using BitsBuffer = std::vector<bool>;
using HuffmanDict = std::unordered_map<char, BitsBuffer>;
using CharFrequencies = std::unordered_map<char, int>;

class HTree
{
public: // types
    using Node = HTreeNode;
    using Nodes = std::vector<Node>;
    using NodeIDs = std::vector<int>;

public:
    explicit HTree() = default;
    explicit HTree(const BytesBuffer& message);
    HuffmanDict huffmanDict() const { return huffmanDict_; }

    void setMessage(const BytesBuffer& message);

private:
    int makeNode();
    NodeIDs fillNodes(const CharFrequencies& frequencies);
    void buildTree(const NodeIDs& freeNodes);

private:
    Nodes nodes_;
    HuffmanDict huffmanDict_;
};

#endif