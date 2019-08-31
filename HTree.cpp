#include "HTree.hpp"

#include <algorithm>
#include <cassert>
#include <list>

namespace { // impl
    static CharFrequencies CalculateFrequencies(const BytesBuffer& message)
    {
        CharFrequencies frequencies;
        for(auto ch : message) {
            const auto it = frequencies.find(ch);
            if(it != std::end(frequencies)) {
                ++(it->second);
            }
            else {
                frequencies.emplace(ch, 1);
            }
        }
        return frequencies;
    }

    static int GetChild(const HTree::Nodes& nodes, std::list<int>& freeIds)
    {
        const auto childIt = std::min_element(std::cbegin(freeIds), std::cend(freeIds),
        [&nodes](const auto left, const auto right)
        {
            return nodes.at(left).weight < nodes.at(right).weight;
        });

        assert(childIt != std::cend(freeIds));

        const int childID = *childIt;
        freeIds.erase(childIt);
        return childID;
    }

    static HuffmanDict BuildHuffmanDictFromTree(const HTree::Nodes& nodes, const HTree::NodeIDs& leafs)
    {
        HuffmanDict result;
        for(const int leafID : leafs) {
            const auto sign = nodes.at(leafID).sign;
            result.emplace(sign, BitsBuffer());
            auto& bits = result[sign];
            for(int parentID = leafID; nodes.at(parentID).parentNodeID >= 0; parentID = nodes.at(parentID).parentNodeID) {
                bits.push_back(nodes.at(nodes.at(parentID).parentNodeID).rightNodeID == parentID);
            }
            std::reverse(std::begin(bits), std::end(bits));
        }
        return result;
    }
}

HTree::HTree(const BytesBuffer& message) { setMessage(message); }

void HTree::setMessage(const BytesBuffer& message)
{
    const auto frequencies = CalculateFrequencies(message);
    const auto leafs = fillNodes(frequencies);
    buildTree(leafs);
    huffmanDict_ = BuildHuffmanDictFromTree(nodes_, leafs);
}

int HTree::makeNode()
{
    nodes_.emplace_back();
    return nodes_.size() - 1;
}

HTree::NodeIDs HTree::fillNodes(const CharFrequencies& frequencies)
{
    NodeIDs leafsIDs;
    leafsIDs.reserve(frequencies.size());
    
    nodes_.reserve(frequencies.size());
    for(const auto& signWeight : frequencies) {
        const int currNodeID = makeNode();

        auto& lastElem = nodes_.at(currNodeID);
        lastElem.sign = signWeight.first;
        lastElem.weight = signWeight.second;
        leafsIDs.push_back(currNodeID);
    }
    return leafsIDs;
}

void HTree::buildTree(const NodeIDs& leafs)
{
    std::list<int> freeNodes(leafs.cbegin(), leafs.cend());
    while(freeNodes.size() > 1) {
        const int leftChildID = GetChild(nodes_, freeNodes);
        const int rightChildID = GetChild(nodes_, freeNodes);

        const int parentID = makeNode();
        auto& parent = nodes_.at(parentID);

        auto& leftChild = nodes_.at(leftChildID);
        auto& rightChild = nodes_.at(rightChildID);

        parent.leftNodeID = leftChildID;
        parent.rightNodeID = rightChildID;
        parent.weight = leftChild.weight + rightChild.weight;

        leftChild.parentNodeID = parentID;
        rightChild.parentNodeID = parentID;

        freeNodes.push_back(parentID);
    }
}