#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

#include "../image.h"

//https://github.com/Intervention/image/issues/893
//https://purepng.readthedocs.io/en/latest/chunk.html

Image ReadPng(std::string_view filename);

enum class PNG_Filter : uint8_t {
    None = 0,
    Sub = 1,
    Up = 2,
    Average = 3,
    Paeth = 4
};

#pragma pack(push, 1)
struct PNG_IHDR_Header {
    int32_t width;
    int32_t height;
    uint8_t bitDepth;
    uint8_t colorType;
    uint8_t compressionMethod;
    PNG_Filter filterMethod;
    uint8_t interlaceMethod;
};

struct PNG_PLTE_Entry {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};
#pragma pack(pop)

struct PngChunk{
    PngChunk();
    virtual void ReadData(std::ifstream& in, int length);
    bool CheckCRC() const;
    virtual ~PngChunk() = default;
    std::string chunkType;
    std::vector<char> data;
    uint32_t crc;
};

struct PLTEChunk : public PngChunk{
    void ReadData(std::ifstream& in, int length) override;
    std::vector<char> m_metaData;
};

struct IHDRChunk : public PngChunk{
    void ReadData(std::ifstream& in, int length) override;
    PNG_IHDR_Header m_hdr;
};

class PngDecoder {
    void ReadSignature();
    void Defiltering(std::vector<unsigned char>& R, size_t width, size_t height, size_t bitDepth);
    void DefilteringInterlaced(std::vector<unsigned char>& R, size_t width, size_t height, size_t bitDepth);

    void ReadChunks();

    std::vector<unsigned char> IDAT;
    std::ifstream m_in;
public:
    explicit PngDecoder(std::string_view filename);

    void FillInterlacedCustom(Image& image) const;
    void FillNonInterlaced(Image& image) const;

    std::vector<unsigned char> uncompressedData;
    std::shared_ptr<IHDRChunk> IHDR;
    std::shared_ptr<PLTEChunk> PLTE;
};
