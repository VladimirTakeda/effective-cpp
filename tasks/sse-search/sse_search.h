#pragma once

#include <immintrin.h>
#include <cstdint>
#include <cstring>


    template <typename T>
    T clear_leftmost_set(const T value) {

        assert(value != 0);

        return value & (value - 1);
    }

    template <typename T>
    unsigned get_first_bit_set(const T value) {
        assert(value != 0);
        return __builtin_ctz(value);
    }


    template <>
    unsigned get_first_bit_set<uint64_t>(const uint64_t value) {
        assert(value != 0);
        return __builtin_ctzl(value);
    }

template<class Callback>
inline size_t SseSearch(const uint8_t *str, size_t size, uint64_t word, Callback cb) {
        const __m128i pattern = _mm_set1_epi64x(word);
        const __m128i zeros  = _mm_setzero_si128();
        const uint8_t *end = str + size - 8;
        const uint8_t *start = str;

        size_t occurrences = 0;
        for (; str <= end; str += 8) {
            __m128i data;

            if (end - str >= 8) {
                // Если да, загружаем доступные данные
                data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(str));
            } else {
                // Если нет, создаем регистр с нулевыми значениями
                alignas(16) char temp[16] = {0};
                // Копируем доступные данные в начало временного буфера
                std::memcpy(temp, str, end - str + 8);
                // Загружаем данные из временного буфера
                data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(temp));
            }
            const __m128i result1 = _mm_mpsadbw_epu8(data, pattern, 0);
            const __m128i result2 = _mm_mpsadbw_epu8(data, pattern, 5);
            const __m128i cmp1    = _mm_cmpeq_epi16(result1, zeros);
            const __m128i cmp2    = _mm_cmpeq_epi16(result2, zeros);
            unsigned mask1 = _mm_movemask_epi8(cmp1) & 0b0101010101010101;
            unsigned mask2 = _mm_movemask_epi8(cmp2) & 0b0101010101010101;
            while (mask1) {
                const auto index = get_first_bit_set(mask1)/2;

                if (str + index <= str + size - 8 && (mask2 & (1 << index * 2))) {
                    ++occurrences;
                    if (!cb(str - start + index))
                        return occurrences;
                }

                mask1 = clear_leftmost_set(mask1);
            }
        }
        return occurrences;
    }