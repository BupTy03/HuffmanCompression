#ifndef HTREE_HPP
#define HTREE_HPP

#include <vector>
#include <array>

using BytesBuffer = std::vector<std::uint8_t>;
using BitsBuffer = std::vector<bool>;
using HuffmanDict = std::array<BitsBuffer, 256>;
using CharFrequencies = std::array<std::size_t, 256>;

struct HTreeNode
{
    int weight = 0;
    std::uint8_t sign = 0;
    int leftNodeID = -1;
    int rightNodeID = -1;
    int parentNodeID = -1;
};

class HTree
{
public: // types
    using Node = HTreeNode;
    using Nodes = std::vector<Node>;
    using NodeIDs = std::vector<int>;

public:
    explicit HTree() = default;
    explicit HTree(const BytesBuffer& message);
    const HuffmanDict& huffmanDict() const { return huffmanDict_; }

    void setMessage(const BytesBuffer& message);

private:
    int makeNode() { nodes_.emplace_back(); return nodes_.size() - 1; }
    Node& getNode(int nodeID) { return nodes_.at(nodeID); }
    NodeIDs fillNodes(const CharFrequencies& frequencies);
    void buildTree(const NodeIDs& freeNodes);
    void buildHuffmanDictFromTree(HuffmanDict& dict, const NodeIDs& leafs);

private:
    Nodes nodes_;
    HuffmanDict huffmanDict_;
};

#endif // !HTREE_HPP