#include "HTree.hpp"
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <iterator>

void printBits(const BitsBuffer& bits)
{
    std::copy(std::cbegin(bits), std::cend(bits), std::ostream_iterator<bool>(std::cout));
}

BitsBuffer encode(const HuffmanDict& dict, const BytesBuffer& message)
{
    BitsBuffer result;
    for(auto ch : message) {
        std::copy(std::cbegin(dict.at(ch)), std::cend(dict.at(ch)), std::back_inserter(result));
    }
    return result;
}

BytesBuffer decode(const HuffmanDict& dict, std::vector<bool> bits)
{
    BytesBuffer result;
    while(!bits.empty()) {
        bool found = false;
        for(const auto& charBits : dict) {
            auto itPair = std::mismatch(std::cbegin(charBits.second), std::cend(charBits.second), std::cbegin(bits));
            if(itPair.first == std::cend(charBits.second))
            {
                result += charBits.first;
                bits.erase(std::cbegin(bits), itPair.second);
                found = true;
                break;
            }
        }
        if(!found) {
            std::cout << "Byte not found!" << std::endl;
            break;
        }
    }
    return result;
}

int main(int argc, char* argv[])
{
    const BytesBuffer message = "very very very big message";

    HTree tree(message);
    const auto huffmanCodes = tree.huffmanDict();
    const auto bits = encode(huffmanCodes, message);
    std::cout << "Encoded(size: " << bits.size() / 8 << " bytes): ";
    printBits(bits);
    std::cout << std::endl;
    const auto bytes = decode(huffmanCodes, bits);
    std::cout << "Decoded(size: " << bytes.size() << " bytes): " << bytes << std::endl;

    return 0;
}