#include <memory>
#include <bitset>
#include <map>
#include <exception>

#include "png_decoder.h"
#include "inflater.h"
#include "crc.h"

namespace{
    int colIncrement[] = {8, 8, 4, 4, 2, 2, 1};
    int rowIncrement[] = {8, 8, 8, 4, 4, 2, 2};
    int passOffsetX[] = {0, 4, 0, 2, 0, 1, 0};
    int passOffsetY[] = {0, 0, 4, 0, 2, 0, 1};
}

PngChunk::PngChunk(){
    chunkType.resize(4);
}

void PngChunk::ReadData(std::ifstream& in, int length){
    data.resize(length);
    if (!in.read(data.data(), length))
        throw "unexpected eof";
}

void PLTEChunk::ReadData(std::ifstream& in, int length) {
    PngChunk::ReadData(in, length);
    m_metaData = data;
}

void IHDRChunk::ReadData(std::ifstream& in, int length) {
    if (sizeof(PNG_IHDR_Header) != length)
        throw "wrong IHDR chunk length";
    PngChunk::ReadData(in, length);
    m_hdr = *reinterpret_cast<PNG_IHDR_Header *>(data.data());
    std::reverse(reinterpret_cast<char *>(&m_hdr.height), reinterpret_cast<char *>(&m_hdr.height) + 4);
    std::reverse(reinterpret_cast<char *>(&m_hdr.width), reinterpret_cast<char *>(&m_hdr.width) + 4);
}

void ReadData(std::ifstream& in, char* buffer, size_t length) {
    if (!in.read(buffer, length)) {
        if (in.eof())
            throw std::runtime_error("Unexpected end of file");
        else
            throw std::runtime_error("Error while reading file");
    }
}

std::string ReadString(std::ifstream& in, size_t length) {
    std::vector<char> buffer(length);
    ReadData(in, buffer.data(), length);
    return {buffer.data(), length};
}

std::shared_ptr<PngChunk> CreateChunk(const std::string& type) {
    if (type == "IHDR")
        return std::make_shared<IHDRChunk>();
    else if (type == "PLTE")
        return std::make_shared<PLTEChunk>();
    else
        return std::make_shared<PngChunk>();
}

std::shared_ptr<PngChunk> ReadChunk(std::ifstream& in) {
    int32_t length;
    ReadData(in, reinterpret_cast<char*>(&length), 4);
    std::reverse(reinterpret_cast<char*>(&length), reinterpret_cast<char*>(&length) + 4);

    std::string type = ReadString(in, 4);
    std::shared_ptr<PngChunk> currChunk = CreateChunk(type);
    currChunk->chunkType = type;

    currChunk->ReadData(in, length);

    ReadData(in, reinterpret_cast<char*>(&currChunk->crc), 4);
    std::reverse(reinterpret_cast<char*>(&currChunk->crc), reinterpret_cast<char*>(&currChunk->crc) + 4);

    if (!currChunk->CheckCRC())
        throw std::runtime_error("CRC check failed");

    return currChunk;
}

bool PngChunk::CheckCRC() const {
    std::vector<char> toCheck;
    toCheck.insert(toCheck.end(), chunkType.begin(), chunkType.end());
    toCheck.insert(toCheck.end(), data.begin(), data.end());
    return ::CheckCRC(toCheck, crc);
}

size_t numberOfColorsPerPixel(uint8_t colorType) {
    switch (colorType) {
        case 0:
            return 1;
        case 3:
            return 1;
        case 2:
            return 3;
        case 4:
            return 2;
        case 6:
            return 4;
        default:
            throw std::runtime_error("Unknown color type");
    }
}

std::map<int, std::pair<int, int>> CalculatePassSizes(int width, int height) {
    std::map<int, std::pair<int, int>> passSizes;

    // Adam7 passes
    int passWidth[7];
    int passHeight[7];

    for (int i = 0; i < 7; ++i) {
        passWidth[i] = (width - passOffsetX[i] + colIncrement[i] - 1) / colIncrement[i];
        passHeight[i] = (height - passOffsetY[i] + rowIncrement[i] - 1) / rowIncrement[i];
        passSizes[i] = std::make_pair(passWidth[i], passHeight[i]);
    }

    return passSizes;
}

PngDecoder::PngDecoder(std::string_view filename) : m_in(filename.data(), std::ios::binary) {
    if (!m_in.is_open())
        throw "can't open file";
    ReadSignature();
    ReadChunks();
}

void PngDecoder::ReadSignature(){
    std::vector<unsigned char> signature = {137, 80, 78, 71, 13, 10, 26, 10};
    std::vector<unsigned char> data(8);
    m_in.read(reinterpret_cast<char*>(data.data()), 8);
    if (data != signature)
        throw "invalid png signature";
}

void PngDecoder::ReadChunks(){
    std::string type;
    do {
        auto chunk = ReadChunk(m_in);
        if (chunk->chunkType == "IDAT"){
            IDAT.insert(IDAT.end(), chunk->data.begin(), chunk->data.end());
        }
        else if (chunk->chunkType == "IHDR"){
            IHDR = std::dynamic_pointer_cast<IHDRChunk>(chunk);
        }
        else if (chunk->chunkType == "PLTE"){
            PLTE = std::dynamic_pointer_cast<PLTEChunk>(chunk);
        }
        type = chunk->chunkType;
    }
    while (type != "IEND");

    uncompressedData = inflate(IDAT);

    if (IHDR->m_hdr.interlaceMethod)
        DefilteringInterlaced(uncompressedData, IHDR->m_hdr.width, IHDR->m_hdr.height, IHDR->m_hdr.bitDepth);
    else
        Defiltering(uncompressedData, IHDR->m_hdr.width, IHDR->m_hdr.height, IHDR->m_hdr.bitDepth);
}

void PngDecoder::DefilteringInterlaced(std::vector<unsigned char>& R, size_t width, size_t height, size_t bitDepth) {
    auto passSizes = CalculatePassSizes(width, height);

    std::vector<std::vector<unsigned char>> passes;
    size_t passStart = 0;
    for (int pass = 0; pass < 7; ++pass) {
        int passWidth = passSizes[pass].first;
        int passHeight = passSizes[pass].second;
        int passTotalPixels = passWidth * passHeight;

        size_t passBytes = passTotalPixels * numberOfColorsPerPixel(IHDR->m_hdr.colorType) + passHeight;
        std::vector<unsigned char> passData(R.begin() + passStart, R.begin() + passStart + passBytes);

        passStart += passBytes;
        Defiltering(passData, passWidth, passHeight, bitDepth);
        passes.push_back(std::move(passData));
    }

    R.clear();
    for (const auto& passData : passes) {
        R.insert(R.end(), passData.begin(), passData.end());
    }
}

// Each data row in PNG must be aligned by byte.
// If the number of bits per pixel is not a multiple of 8, then the last byte of each row contains the remaining bits, filled with zeros, to align the row by byte.

void PngDecoder::Defiltering(std::vector<unsigned char>& R, size_t width, size_t height, size_t bitDepth) {
    // Вычисляем количество байтов на строку (округленное вверх)
    std::vector<unsigned char> RNoFilter;
    size_t bytesPerRow = (width * bitDepth * numberOfColorsPerPixel(IHDR->m_hdr.colorType) + 7) / 8 + 1;
    size_t bytesPerPixel = numberOfColorsPerPixel(IHDR->m_hdr.colorType) * bitDepth % 8 == 0 ?
                           numberOfColorsPerPixel(IHDR->m_hdr.colorType) * bitDepth / 8 :
                           numberOfColorsPerPixel(IHDR->m_hdr.colorType) * bitDepth / 8 + 1;

    auto Raw = [&](int currIndex, int index) -> unsigned char {
        if (index < 0)
            return 0;
        if (index % bytesPerRow == 0)
            return 0;
        if (index % bytesPerRow > currIndex % bytesPerRow)
            return 0;
        return R[index];
    };
    size_t filterTypeBytePos = 0;

    for (size_t y = 0; y < height; ++y) {
        size_t rowStart = y * bytesPerRow;

        uint8_t filterTypeByte = R[rowStart + filterTypeBytePos];

        // Apply a filter to each row, starting from the second byte (the first byte contains the filter type)
        for (size_t x = 0; x < bytesPerRow - 1; ++x) {
            uint8_t byte = R[rowStart + x + 1];

            // Apply the inverse transformation depending on the filter type
            switch (filterTypeByte) {
                case 0:  // None
                    break;
                case 1:  // Sub
                    byte = (byte + Raw(rowStart + x + 1, rowStart + x + 1 - bytesPerPixel) + 256) % 256;
                    break;
                case 2:  // Up
                    byte = (byte + Raw(rowStart + x + 1, rowStart + x + 1 - bytesPerRow) + 256) % 256;
                    break;
                case 3:  // Average
                    byte = (byte + (Raw(rowStart + x + 1, rowStart + x + 1 - bytesPerRow) +
                                    Raw(rowStart + x + 1, rowStart + x + 1 - bytesPerPixel)) / 2) % 256;
                    break;
                case 4:  // Paeth
                    auto Paeth = [&]() {
                        int a = Raw(rowStart + x + 1, rowStart + x + 1 - bytesPerPixel);
                        int b = Raw(rowStart + x + 1, rowStart + x + 1 - bytesPerRow);
                        int c = Raw(rowStart + x + 1, rowStart + x + 1 - bytesPerRow - bytesPerPixel);
                        int p = a + b - c;
                        int pa = abs(p - a);
                        int pb = abs(p - b);
                        int pc = abs(p - c);
                        if (pa <= pb && pa <= pc) {
                            return a;
                        } else if (pb <= pc) {
                            return b;
                        } else {
                            return c;
                        }
                    };
                    byte = (byte + Paeth()) % 256;
                    break;
            }

            R[rowStart + x + 1] = byte;
        }
        RNoFilter.insert(RNoFilter.end(), R.begin() + rowStart + 1, R.begin() + rowStart + bytesPerRow);
    }
    R = RNoFilter;
}

// Algorithm
// Given an array of bytes representing the image data:
// [FirstPassPixel1, FirstPassPixel2, ..., FirstPassPixelN, SecondPassPixel1, ..., SecondPassPixelN, .... ,SeventhPassPixel1, ..., SeventhPassPixelN]
// What is required to decode this image?
// 1) The number of pixels in each pass.
// 2) The number of pixels in each row of each pass.

void PngDecoder::FillInterlacedCustom(Image& image) const {
    auto passCount = CalculatePassSizes(IHDR->m_hdr.width, IHDR->m_hdr.height);
    size_t bitsPerPixel = IHDR->m_hdr.bitDepth * numberOfColorsPerPixel(IHDR->m_hdr.colorType);
    size_t rowStart = 0;
    for (int pass = 0; pass < 7; pass++){
        auto [passWidth, passHeight] = passCount[pass];
        size_t bytesPerRow = (passWidth * IHDR->m_hdr.bitDepth * numberOfColorsPerPixel(IHDR->m_hdr.colorType) + 7) / 8;
        int row = passOffsetY[pass];
        int col = passOffsetX[pass];
        for (int pixelRow = 0; pixelRow < passHeight; ++pixelRow) {
            size_t bitIndex = rowStart;
            for (int pixelIdx = 0; pixelIdx < passWidth; ++pixelIdx){
                RGB pixel;
                for (size_t c = 0; c < numberOfColorsPerPixel(IHDR->m_hdr.colorType); ++c) {
                    uint8_t pixelData = 0;
                    for (size_t b = 0; b < IHDR->m_hdr.bitDepth; ++b) {
                        pixelData = (pixelData << 1) | (uncompressedData[bitIndex / 8] >> (7 - (bitIndex % 8)) & 1);
                        bitIndex++;
                    }
                    switch (c) {
                        case 0:
                            pixel.r = pixelData;
                            break;
                        case 1:
                            pixel.g = pixelData;
                            break;
                        case 2:
                            pixel.b = pixelData;
                            break;
                        case 3:
                            pixel.a = pixelData;
                            break;
                    }
                    if (IHDR->m_hdr.colorType == 3) {
                        image(row, col).r = static_cast<unsigned char>(PLTE->m_metaData[pixelData * 3]);
                        image(row, col).g = static_cast<unsigned char>(PLTE->m_metaData[pixelData * 3 + 1]);
                        image(row, col).b = static_cast<unsigned char>(PLTE->m_metaData[pixelData * 3 + 2]);
                    }
                }
                if (IHDR->m_hdr.colorType == 2 || IHDR->m_hdr.colorType == 6) {
                    image(row, col) = pixel;
                }
                if (IHDR->m_hdr.colorType == 0 || IHDR->m_hdr.colorType == 4) {
                    image(row, col).r = pixel.r;
                    image(row, col).g = pixel.r;
                    image(row, col).b = pixel.r;
                    if (IHDR->m_hdr.colorType == 4)
                        image(row, col).a = pixel.g; //because alpha chahhel goes to pixel.g above
                }
                //calc next col index;
                col += colIncrement[pass];
                if (col >= IHDR->m_hdr.width){
                    row += rowIncrement[pass];
                    col = passOffsetX[pass];
                }
            }
            rowStart += bytesPerRow * 8;
        }
    }
}

void PngDecoder::FillNonInterlaced(Image& image) const {
    size_t bitsPerChannel = IHDR->m_hdr.bitDepth;
    size_t bitIndex = 0;
    for (int i = 0; i < IHDR->m_hdr.height; ++i) {
        for (int j = 0; j < IHDR->m_hdr.width; ++j) {
            RGB pixel;
            for (size_t c = 0; c < numberOfColorsPerPixel(IHDR->m_hdr.colorType); ++c) {
                uint8_t pixelData = 0;
                for (size_t b = 0; b < bitsPerChannel; ++b) {
                    pixelData = (pixelData << 1) | (uncompressedData[bitIndex / 8] >> (7 - (bitIndex % 8)) & 1);
                    bitIndex++;
                }
                switch (c) {
                    case 0:
                        pixel.r = pixelData;
                        break;
                    case 1:
                        pixel.g = pixelData;
                        break;
                    case 2:
                        pixel.b = pixelData;
                        break;
                    case 3:
                        pixel.a = pixelData;
                        break;
                }
                if (IHDR->m_hdr.colorType == 3) {
                    image(i, j).r = static_cast<unsigned char>(PLTE->m_metaData[pixelData * 3]);
                    image(i, j).g = static_cast<unsigned char>(PLTE->m_metaData[pixelData * 3 + 1]);
                    image(i, j).b = static_cast<unsigned char>(PLTE->m_metaData[pixelData * 3 + 2]);
                }
            }
            if (IHDR->m_hdr.colorType == 2 || IHDR->m_hdr.colorType == 6) {
                image(i, j) = pixel;
            }
            if (IHDR->m_hdr.colorType == 0 || IHDR->m_hdr.colorType == 4) {
                image(i, j).r = pixel.r;
                image(i, j).g = pixel.r;
                image(i, j).b = pixel.r;
                if (IHDR->m_hdr.colorType == 4)
                    image(i, j).a = pixel.g; //because alpha chahhel goes to pixel.g above
            }
        }
        size_t bitsToSkip = (8 - (bitIndex % 8)) % 8;
        bitIndex += bitsToSkip;
    }
}

Image ReadPng(std::string_view filename) {
    PngDecoder decoder(filename);
    Image image(decoder.IHDR->m_hdr.height, decoder.IHDR->m_hdr.width);
    if (decoder.IHDR->m_hdr.interlaceMethod){
        decoder.FillInterlacedCustom(image);
    }
    else{
        decoder.FillNonInterlaced(image);
    }
    return image;
}
