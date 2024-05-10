#include <immintrin.h>
#include <iostream>
#include <cstring>

template<int M, int N, int K>
void AvxMatmul(const float *a, const float *raw_b, float *c) {
    float *b_packed = static_cast<float*>(std::aligned_alloc(32,  (N + 7) / 8 * K * 8 * sizeof(float)));
    memset(b_packed, 0, (N + 7) / 8 * K * 8 * sizeof(float));

    int idx = 0;
    for (int i = 0; i < N; i += 8) {
        for (int j = 0; j < K; ++j) {
            for (int z = 0; z < 8; z++) {
                if ((i + z) * K + j < N * K)
                    b_packed[idx++] = raw_b[(i + z) * K + j];
                else
                    idx++; //leave zero
            }
        }
    }

    for (int i = 0; i < M; ++i) {
        for (int j_outer = 0; j_outer < N; j_outer += 8) {
            __m256 sum = _mm256_setzero_ps();
            for (int k = 0; k < K; ++k) {
                __m256 broadcasted_value = _mm256_set1_ps(a[i * K + k]);
                __m256 b_vals = _mm256_load_ps(&b_packed[(j_outer * K) + k * 8]);
                __m256 res = _mm256_mul_ps(broadcasted_value, b_vals);
                sum = _mm256_add_ps(res, sum);
            }
            memcpy(&c[i * N + j_outer], reinterpret_cast<char *>(&sum), std::min(8, N - j_outer) * sizeof(float));
            //_mm256_storeu_ps(&c[i * N + j_outer], sum);
        }
    }

    std::free(b_packed);
}
