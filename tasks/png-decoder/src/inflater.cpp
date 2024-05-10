#include "inflater.h"

#include <zlib.h>

std::vector<unsigned char> inflate(const std::vector<unsigned char>& input){
    std::vector<unsigned char> unpacked;
    uLongf destLen = input.size() * 100;
    unpacked.resize(destLen);
    int ret = uncompress(reinterpret_cast<Bytef *>(unpacked.data()), &destLen,
                         reinterpret_cast<const Bytef *>(input.data()), input.size());
    if (ret != Z_OK) {
        throw "An error occurred during inflation: ";
    }

    unpacked.resize(destLen);
    return unpacked;
}
