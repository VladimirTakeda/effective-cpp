#include <boost/crc.hpp>

#include "crc.h"

namespace {
    uint32_t CalculateCRC(const std::vector<char> &data) {
        boost::crc_32_type crc32;
        crc32.process_bytes(data.data(), data.size());
        return crc32.checksum();
    }
}

bool CheckCRC(const std::vector<char>& data, uint32_t givenCrc){
    uint32_t crc = CalculateCRC(data);
    return crc == givenCrc;
}
