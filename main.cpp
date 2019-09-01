#include "HTree.hpp"

#include <iostream>
#include <algorithm>
#include <iterator>

void print(const BitsBuffer& bits) { std::copy(std::cbegin(bits), std::cend(bits), std::ostream_iterator<bool>(std::cout)); }
void print(const BytesBuffer& bytes) { std::copy(std::cbegin(bytes), std::cend(bytes), std::ostream_iterator<std::uint8_t>(std::cout)); }
void error(const char* message) { std::cerr << "Error: " << message << std::endl; }

BitsBuffer encode(const HuffmanDict& dict, const BytesBuffer& message)
{
    BitsBuffer result;
    for(auto ch : message) {
        const auto& bits = dict.at(static_cast<std::size_t>(ch));
        std::copy(std::cbegin(bits), std::cend(bits), std::back_inserter(result));
    }
    return result;
}

BytesBuffer decode(const HuffmanDict& dict, const BitsBuffer& bitsBuffer)
{
    BytesBuffer result;
    for(std::size_t bitIndex = 0; bitIndex < bitsBuffer.size(); ) {
        bool bitSequenceFound = false;
        for(std::size_t byteIndex = 0; byteIndex < dict.size(); ++byteIndex) {
            auto& codeBits = dict.at(byteIndex);
            if(codeBits.empty()) continue;

            auto byteBitsIt = std::mismatch(std::cbegin(codeBits), std::cend(codeBits), std::cbegin(bitsBuffer) + bitIndex);
            if(byteBitsIt.first == std::cend(codeBits)) {
                result.push_back(static_cast<std::uint8_t>(byteIndex));
                bitIndex = std::distance(std::cbegin(bitsBuffer), byteBitsIt.second);
                bitSequenceFound = true;
                break;
            }
        }

        if(!bitSequenceFound) {
            error("Bits sequence is not found!");
            break;
        }
    }
    return result;
}

int main(int argc, char* argv[])
{
    const char str[] = "very very very big message";
    BytesBuffer message(std::cbegin(str), std::cbegin(str) + std::size(str) - 1);

    // building tree
    HTree tree(message);
    const auto& huffmanCodes = tree.huffmanDict();

    // print huffman codes
    for(std::size_t signByte = 0; signByte < huffmanCodes.size(); ++signByte) {
        const auto& bitCode = huffmanCodes.at(signByte);
        if(bitCode.empty()) continue;

        std::cout << "'" << static_cast<char>(signByte) << "' = ";
        print(bitCode);
        std::cout << std::endl; 
    }

    // encoding
    const auto bits = encode(huffmanCodes, message);
    std::cout << "Encoded(size: " << bits.size() / 8 << " bytes): ";
    print(bits);
    std::cout << std::endl;

    // decoding
    const auto bytes = decode(huffmanCodes, bits);
    std::cout << "Decoded(size: " << bytes.size() << " bytes): ";
    print(bytes);
    std::cout << std::endl;
    return 0;
}