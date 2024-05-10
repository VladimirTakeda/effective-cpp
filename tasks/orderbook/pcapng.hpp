#pragma once

#include <vector>
#include <string>
#include <iomanip>
#include <cstring>

#include "types.hpp"

struct Block{

};

struct HeaderBlock : public Block{
    int32_t BlockType = 0x0a0d0d0a;
    int32_t BlockLength = 0;
    int32_t ByteOrderMagic = 0x1a2b3c4d;
    int16_t  MajorVersion = 0x0001;
    int16_t MinorVersion = 0x0000;
    uint64_t SectionLength = 0xffffffffffffffff;
    std::vector<uint8_t> Options;
    int32_t BlockLengthDuplicate = 0;

};

struct DescriptionBlock : public Block{
    int32_t BlockType= 0x00000001;
    int32_t BlockLength = 0;
    int16_t LinkType = 0;
    int16_t Reserved = 0x0000;
    int32_t SnapLength = 0;
    std::vector<uint8_t> Options;
    int32_t BlockLengthDuplicate = 0;
};

struct PacketBlock : public Block{
    int32_t BlockType = 0x00000006;
    int32_t BlockLength = 0;
    int32_t InterfaceID = 0;
    int32_t TimestampUpper = 0;
    int32_t TimestampLower = 0;
    int32_t CapturedPacketLength = 0;
    int32_t OriginalPacketLength = 0;
    std::vector<uint8_t> PacketData;
    std::vector<uint8_t> Options;
    int32_t BlockLengthDuplicate = 0;
};

void parseEthernetFrame(const std::vector<uint8_t>& ethernetData) {
    if (ethernetData.size() < 14) {
        std::cerr << "Ethernet frame is too short" << std::endl;
        return;
    }

    // Destination MAC Address (6 bytes)
    std::array<uint8_t, 6> destinationMAC;
    std::copy(ethernetData.begin(), ethernetData.begin() + 6, destinationMAC.begin());

    // Source MAC Address (6 bytes)
    std::array<uint8_t, 6> sourceMAC;
    std::copy(ethernetData.begin() + 6, ethernetData.begin() + 12, sourceMAC.begin());

    // EtherType (2 bytes)
    uint16_t etherType = (ethernetData[12] << 8) | ethernetData[13];

    // Output parsed information
    std::cout << "Ethernet Frame:" << std::endl;
    std::cout << "  Destination MAC Address: ";
    for (const auto& byte : destinationMAC) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << ":";
    }
    std::cout << std::endl;

    std::cout << "  Source MAC Address: ";
    for (const auto& byte : sourceMAC) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << ":";
    }
    std::cout << std::endl;

    std::cout << "  EtherType: " << std::hex << std::setw(4) << std::setfill('0') << etherType << std::endl;
}

void parseIPv4Packet(const std::vector<uint8_t>& ipData) {
    if (ipData.size() < 20) {
        std::cerr << "IPv4 packet is too short" << std::endl;
        return;
    }

    // Version (4 bits) and IHL (4 bits)
    uint8_t versionIHL = ipData[0];
    uint8_t version = versionIHL >> 4;
    uint8_t ihl = versionIHL & 0x0F;

    // Differentiated Services Field (1 byte)
    uint8_t dscpECN = ipData[1];

    // Total Length (2 bytes)
    uint16_t totalLength = (ipData[2] << 8) | ipData[3];

    // Identification (2 bytes)
    uint16_t identification = (ipData[4] << 8) | ipData[5];

    // Flags and Fragment Offset (2 bytes)
    uint16_t flagsFragmentOffset = (ipData[6] << 8) | ipData[7];
    uint16_t flags = (flagsFragmentOffset >> 13) & 0x07;
    uint16_t fragmentOffset = flagsFragmentOffset & 0x1FFF;

    // Time to Live (1 byte)
    uint8_t ttl = ipData[8];

    // Protocol (1 byte)
    uint8_t protocol = ipData[9];

    // Header Checksum (2 bytes)
    uint16_t headerChecksum = (ipData[10] << 8) | ipData[11];

    // Source IP Address (4 bytes)
    std::array<uint8_t, 4> sourceIP;
    std::copy(ipData.begin() + 12, ipData.begin() + 16, sourceIP.begin());

    // Destination IP Address (4 bytes)
    std::array<uint8_t, 4> destinationIP;
    std::copy(ipData.begin() + 16, ipData.begin() + 20, destinationIP.begin());

    // Output parsed information
    std::cout << "IPv4 Packet:" << std::endl;
    std::cout << "  Version: " << static_cast<int>(version) << std::endl;
    std::cout << "  IHL: " << static_cast<int>(ihl) << " words" << std::endl;
    std::cout << "  Differentiated Services Field: " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(dscpECN) << std::endl;
    std::cout << "  Total Length: " << totalLength << " bytes" << std::endl;
    std::cout << "  Identification: " << identification << std::endl;
    std::cout << "  Flags: " << flags << std::endl;
    std::cout << "  Fragment Offset: " << fragmentOffset << std::endl;
    std::cout << "  Time to Live: " << static_cast<int>(ttl) << std::endl;
    std::cout << "  Protocol: " << static_cast<int>(protocol) << std::endl;
    std::cout << "  Header Checksum: " << std::hex << std::setw(4) << std::setfill('0') << headerChecksum << std::endl;
    std::cout << "  Source IP Address: ";
    for (const auto& byte : sourceIP) {
        std::cout << std::dec << static_cast<int>(byte) << ".";
    }
    std::cout << std::endl;
    std::cout << "  Destination IP Address: ";
    for (const auto& byte : destinationIP) {
        std::cout << std::dec << static_cast<int>(byte) << ".";
    }
    std::cout << std::endl;
}

inline std::vector<Message> Parse(std::string_view buf) {
    std::vector<std::unique_ptr<PacketBlock>> blocks;
    auto currPtr = buf.data();
    while (currPtr < buf.data() + buf.size()){
        int blockType = *reinterpret_cast<const int*>(currPtr);
        int blockLength = *reinterpret_cast<const int*>(currPtr + 4);
        if (blockType == 0x00000006){
            std::unique_ptr<PacketBlock> packetBlock = std::make_unique<PacketBlock>();
            packetBlock->BlockType = blockType;
            packetBlock->BlockLength = blockLength;
            packetBlock->InterfaceID = *reinterpret_cast<const int32_t*>(currPtr + 8);
            packetBlock->TimestampUpper = *reinterpret_cast<const int32_t*>(currPtr + 12);
            packetBlock->TimestampLower = *reinterpret_cast<const int32_t*>(currPtr + 16);
            packetBlock->CapturedPacketLength = *reinterpret_cast<const int32_t*>(currPtr + 20);
            packetBlock->OriginalPacketLength = *reinterpret_cast<const int32_t*>(currPtr + 24);

            // Чтение данных пакета
            int dataOffset = 28; // Смещение данных пакета от начала блока
            packetBlock->PacketData.resize(packetBlock->CapturedPacketLength);
            std::copy(currPtr + dataOffset, currPtr + dataOffset + packetBlock->CapturedPacketLength, packetBlock->PacketData.begin());

            // Чтение опций
            int optionsOffset = dataOffset + packetBlock->CapturedPacketLength;
            while (optionsOffset % 4 != 0) {
                optionsOffset++;
            }
            int optionsLength = packetBlock->BlockLength - optionsOffset;
            packetBlock->Options.resize(optionsLength);
            std::copy(currPtr + optionsOffset, currPtr + optionsOffset + optionsLength, packetBlock->Options.begin());

            // Чтение BlockLengthDuplicate
            packetBlock->BlockLengthDuplicate = *reinterpret_cast<const int32_t*>(currPtr + packetBlock->BlockLength - 4);

            // Добавляем блок пакета в вектор
            blocks.push_back(std::move(packetBlock));
            currPtr += blockLength;
        }
        else {
            currPtr += blockLength;
        }
    }
    std::vector<Message> answer;
    for (const auto& elem : blocks){
        Message message;
        std::memcpy(&message, elem->PacketData.data() + elem->PacketData.size() - sizeof(Message), sizeof(Message));
        answer.push_back(message);
    }
    return answer;
}
