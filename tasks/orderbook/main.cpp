#include <iostream>
#include <gflags/gflags.h>
#include <lz4frame.h>
#include <lz4frame_static.h>

#include <absl/container/btree_map.h>

#include <fstream>
#include <cassert>
#include <map>

#include "pcapng.hpp"
#include "static_alocator.h"
#include "circular_orderbook.h"
#include "thread_pool.h"

#define IN_CHUNK_SIZE  (16*1024)

static int
decompress_file_internal(FILE* f_in, FILE* f_out,
                         LZ4F_dctx* dctx,
                         void* src, size_t srcCapacity, size_t filled, size_t alreadyConsumed,
                         void* dst, size_t dstCapacity)
{
    int firstChunk = 1;
    size_t ret = 1;

    assert(f_in != NULL); assert(f_out != NULL);
    assert(dctx != NULL);
    assert(src != NULL); assert(srcCapacity > 0); assert(filled <= srcCapacity); assert(alreadyConsumed <= filled);
    assert(dst != NULL); assert(dstCapacity > 0);

    /* Decompression */
    while (ret != 0) {
        /* Load more input */
        size_t readSize = firstChunk ? filled : fread(src, 1, srcCapacity, f_in); firstChunk=0;
        const void* srcPtr = (const char*)src + alreadyConsumed; alreadyConsumed=0;
        const void* const srcEnd = (const char*)srcPtr + readSize;
        if (readSize == 0 || ferror(f_in)) {
            printf("Decompress: not enough input or error reading file\n");
            return 1;
        }

        /* Decompress:
         * Continue while there is more input to read (srcPtr != srcEnd)
         * and the frame isn't over (ret != 0)
         */
        while (srcPtr < srcEnd && ret != 0) {
            /* Any data within dst has been flushed at this stage */
            size_t dstSize = dstCapacity;
            size_t srcSize = (const char*)srcEnd - (const char*)srcPtr;
            ret = LZ4F_decompress(dctx, dst, &dstSize, srcPtr, &srcSize, /* LZ4F_decompressOptions_t */ NULL);
            if (LZ4F_isError(ret)) {
                printf("Decompression error: %s\n", LZ4F_getErrorName(ret));
                return 1;
            }
            /* Flush output */
            if (dstSize != 0) fwrite(dst, 1, dstSize, f_out);
            /* Update input */
            srcPtr = (const char*)srcPtr + srcSize;
        }

        assert(srcPtr <= srcEnd);

        /* Ensure all input data has been consumed.
         * It is valid to have multiple frames in the same file,
         * but this example only supports one frame.
         */
        if (srcPtr < srcEnd) {
            printf("Decompress: Trailing data left in file after frame\n");
            return 1;
        }
    }

    /* Check that there isn't trailing data in the file after the frame.
     * It is valid to have multiple frames in the same file,
     * but this example only supports one frame.
     */
    {
        size_t const readSize = fread(src, 1, 1, f_in);
        if (readSize != 0 || !feof(f_in)) {
            printf("Decompress: Trailing data left in file after frame\n");
            return 1;
        }
    }

    return 0;
}

static size_t get_block_size(const LZ4F_frameInfo_t* info) {
    switch (info->blockSizeID) {
        case LZ4F_default:
        case LZ4F_max64KB:  return 1 << 16;
        case LZ4F_max256KB: return 1 << 18;
        case LZ4F_max1MB:   return 1 << 20;
        case LZ4F_max4MB:   return 1 << 22;
        default:
            printf("Impossible with expected frame specification (<=v1.6.1)\n");
            exit(1);
    }
}

static int decompress_file_allocDst(FILE* f_in, FILE* f_out, LZ4F_dctx* dctx, void* src, size_t srcCapacity)
{
    assert(f_in != NULL); assert(f_out != NULL);
    assert(dctx != NULL);
    assert(src != NULL);
    assert(srcCapacity >= LZ4F_HEADER_SIZE_MAX);  /* ensure LZ4F_getFrameInfo() can read enough data */

    /* Read Frame header */
    size_t const readSize = fread(src, 1, srcCapacity, f_in);
    if (readSize == 0 || ferror(f_in)) {
        printf("Decompress: not enough input or error reading file\n");
        return 1;
    }

    LZ4F_frameInfo_t info;
    size_t consumedSize = readSize;
    {
        size_t const fires = LZ4F_getFrameInfo(dctx, &info, src, &consumedSize);
        if (LZ4F_isError(fires)) {
            printf("LZ4F_getFrameInfo error: %s\n", LZ4F_getErrorName(fires));
            return 1;
        }
    }

    /* Allocating enough space for an entire block isn't necessary for
     * correctness, but it allows some memcpy's to be elided.
     */
    size_t const dstCapacity = get_block_size(&info);
    void* const dst = malloc(dstCapacity);
    if (!dst)
    {
        perror("decompress_file(dst)"); return 1;
    }

    int const decompressionResult = decompress_file_internal(
            f_in, f_out,
            dctx,
            src, srcCapacity, readSize-consumedSize, consumedSize,
            dst, dstCapacity);

    free(dst);
    return decompressionResult;
}

static int decompress_file(const std::string& inputFileName, const std::string& outputFileName)
{
    FILE *f_in = fopen(inputFileName.c_str(), "r");
    if (f_in == NULL) {
        perror("Failed to open input file");
        return 1;
    }

    FILE *f_out = fopen(outputFileName.c_str(), "w+");
    if (f_out == NULL) {
        perror("Failed to open output file");
        fclose(f_in);
        return 1;
    }

    assert(f_in != NULL);
    assert(f_out != NULL);

    /* Resource allocation */
    void* const src = malloc(IN_CHUNK_SIZE);
    if (!src) { perror("decompress_file(src)"); return 1; }

    LZ4F_dctx* dctx;
    {
        size_t const dctxStatus = LZ4F_createDecompressionContext(&dctx, LZ4F_VERSION);
        if (LZ4F_isError(dctxStatus)) {
            printf("LZ4F_dctx creation error: %s\n", LZ4F_getErrorName(dctxStatus));
        }
    }

    int const result = !dctx ? 1 /* error */ :
                       decompress_file_allocDst(f_in, f_out, dctx, src, IN_CHUNK_SIZE);

    free(src);
    LZ4F_freeDecompressionContext(dctx);
    return result;
}

DEFINE_int32(t, 1, "Number");

static const size_t kMaxSize = 1e4;

template <typename T, typename V>
double runTest(T& sellMap, V& buyMap, const std::vector<Message>& messages, char * testName, const std::string& file){
    double res = 0;

    uint32_t t = 0;

    auto start_time = std::chrono::high_resolution_clock::now();
    for (const auto& message : messages){
        if (message.ts & (static_cast<uint64_t>(1) << 63)){
            // meta data
            t = pow(10, message.ts & 0b0000'1111);
            const unsigned char *coinName = reinterpret_cast<const unsigned char*>(&message) + 8;
            continue;
        }

        double currPrice = message.price / static_cast<double>(t);
        double volume = message.volume / 100'000'000.0;

        if (message.conf & 2){
            //aks (sell)
            if (volume != 0)
                sellMap[currPrice] = volume;
            else
                sellMap.erase(currPrice);
        }
        else{
            //bid (buy)
            if (volume != 0)
                buyMap[currPrice] = volume;
            else
                buyMap.erase(currPrice);
        }

        if (message.conf & 1){
            //continue to update
        }
        else{
            auto sellIter  = std::next(sellMap.begin(), 19);
            auto buyIter = std::next(buyMap.begin(), 19);
            res = std::max(res, sellIter->first * sellIter->second + buyIter->first * buyIter->second);
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::cout << "Thread ID: " << std::this_thread::get_id() << " testType: " << testName << " file: " << file << " Result: " << res << " Elapsed time: " << duration.count() << " microseconds" << std::endl;

    return res;
}

void processFile(const std::string& fileName) {
    std::vector<Message> messages;
    if (fileName.size() > 4 && fileName.substr(fileName.size() - 4) == ".lz4") {
        std::string outputFileName = fileName.substr(0, fileName.size() - 4);
        if (decompress_file(fileName, outputFileName)){
            std::cerr << "Error: Can't decompress: " << fileName << std::endl;
            return;
        }

        std::ifstream input(outputFileName, std::ios::binary);
        std::vector<char> data((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
        messages = Parse(std::string_view(data.data(), data.size()));
        if (std::remove(outputFileName.c_str()) != 0) {
            std::cerr << "Error: Can't remove file: " << outputFileName << std::endl;
        }
    } else {
        std::ifstream input("fileName", std::ios::binary);
        std::vector<char> data((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
        messages = Parse(std::string_view(data.data(), data.size()));
    }

    {
        std::map<double, double, std::less<>> sellMap;
        std::map<double, double, std::greater<>> buyMap;

        runTest(sellMap, buyMap, messages, "std::map", fileName);
    }

    {
        StaticStlAllocator<int> alloc1(kMaxSize);
        StaticStlAllocator<int> alloc2(kMaxSize);

        std::map<double, double, std::less<>, StaticStlAllocator<std::pair<const double, double>>> sellMap(alloc1);
        std::map<double, double, std::greater<>, StaticStlAllocator<std::pair<const double, double>>> buyMap(alloc2);

        runTest(sellMap, buyMap, messages, "std::map with static alocator", fileName);
    }

    {
        absl::btree_map<double, double> sellMap;
        absl::btree_map<double, double, std::greater<>> buyMap;

        runTest(sellMap, buyMap, messages, "btree_map", fileName);
    }

    {
        //CircularOrderBook sellMap;
        //CircularOrderBook buyMap;

        //runTest(sellMap, buyMap, messages);
    }
}

int main(int argc, char *argv[]){
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    std::vector<std::string> files;
    for (int i = 1; i < argc; ++i) {
        files.emplace_back(argv[i]);
    }

    int threads = FLAGS_t;

    ThreadPool pool(threads);

    for (const auto& fileName : files) {
        pool.enqueue([=]() { processFile(fileName); });
    }

    return 0;
}