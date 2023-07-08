typedef unsigned int UINT32;
#include <immintrin.h>

void Expand(UINT32 B[16], UINT32 W[68 + 3], UINT32 Ww[64]) {//W长度为68但是需要加三以确保足够SIMD寄存器存放
    __m256i b;
    const __m256i ShuffleRev = _mm256_set_epi8(
        12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3,   // first 128-bit lane
        12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3  // second 128-bit lane
    );
    b = _mm256_load_si256((__m256i*)B);
    _mm256_store_si256((__m256i*)W, b);
    b = _mm256_load_si256((__m256i*)(B + 8));
    _mm256_store_si256((__m256i*)(W + 8), b);
    __m128i va, vb, vc, vd, ve, vl, vr;


    for (int j = 16; j < 68; j += 3) {
        va = _mm_load_si128((__m128i*)(W + j - 16));
        vb = _mm_load_si128((__m128i*)(W + j - 9));
        vc = _mm_load_si128((__m128i*)(W + j - 3));
        vd = _mm_load_si128((__m128i*)(W + j - 13));
        //vl=_mm_rol_epi32 (vc,15);
        vl = _mm_slli_epi32(vc, 15);
        vr = _mm_srli_epi32(vc, 32 - 15);
        va = _mm_xor_si128(va, vl);
        vr = _mm_xor_si128(vr, vb);//(vc<<<15)⊕va⊕vb
        ve = _mm_load_si128((__m128i*)(W + j - 6));
        vc = _mm_xor_si128(va, vr);//vc=Wj−16 ⊕ Wj−9 ⊕ (Wj−3 <<< 15)
        //vl=_mm_rol_epi32 (vc,15);
        vl = _mm_slli_epi32(vc, 15);
        vr = _mm_srli_epi32(vc, 32 - 15);
        //va=_mm_rol_epi32 (vc,23);
        va = _mm_slli_epi32(vc, 23);
        vl = _mm_xor_si128(vl, vr);
        vb = _mm_srli_epi32(vc, 32 - 23);
        vc = _mm_xor_si128(vc, vl);
        va = _mm_xor_si128(vb, va);
        //vl=_mm_rol_epi32 (vd,7);
        vl = _mm_slli_epi32(vd, 7);
        vc = _mm_xor_si128(va, vc);
        vr = _mm_srli_epi32(vd, 32 - 7);
        vc = _mm_xor_si128(vc, vl);
        ve = _mm_xor_si128(ve, vr);
        vc = _mm_xor_si128(ve, vc);
        _mm_store_si128((__m128i*)(W + j), vc);
    }

    __m256i w1, w2;
    for (int i = 0; i < 64; i += 8)
    {
        w1 = _mm256_load_si256(W + i);
        w2 = _mm256_load_si256(W + i + 4);
        w1 = _mm256_xor_si256(w1, w2);
        _mm256_store_si256((__m256*)(Ww + i), w1);
    }
}