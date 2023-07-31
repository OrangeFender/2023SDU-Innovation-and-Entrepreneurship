typedef unsigned int UINT32;
#include <immintrin.h>
#include<string.h>
void Expand(UINT32 B[16], UINT32 W[68 + 3], UINT32 Ww[64]) {//W长度为68但是需要加三以确保足够SIMD寄存器存放

    {
        memcpy(W, B, 64);
        
    }
    register __m128i va, vb, vc, vd, ve, vl, vr; 
    //register __m128i va, vb;
#pragma loop(hint_parallel(17))
    for (int j = 16; j < 68; j += 3) {

    //    va = _mm_xor_si128(_mm_srli_epi32(*((__m128i*)W + j - 3), 32 - 15) , _mm_slli_epi32(*((__m128i*)W + j - 3), 15));
    //    vb = _mm_xor_si128(*((__m128i*)W + j - 9), *((__m128i*)W + j - 16));
    //    va = _mm_xor_si128(va, vb);

    //    vb = _mm_xor_si128(_mm_slli_epi32(va, 15),_mm_srli_epi32(va, 32 - 15));
    //    vb = _mm_xor_si128(vb, _mm_srli_epi32(va, 32 - 23));
    //    va = _mm_xor_si128(va, _mm_srli_epi32(va, 32 - 23));
    //    va = _mm_xor_si128(va,vb);
    //    vb = _mm_xor_si128(_mm_slli_epi32(*((__m128i*)W + j - 13), 7), _mm_srli_epi32(*((__m128i*)W + j - 13), 32 - 7));
    //    va = _mm_xor_si128(va, *((__m128i*)W + j - 6));
    //     vb= _mm_xor_si128(va,vb);
    //     *(__m128i*)(W + j) = vb;
        
        va = _mm_loadu_si128((__m128i*)(W + j - 16));
        vb = _mm_loadu_si128((__m128i*)(W + j - 9));
        vc = _mm_loadu_si128((__m128i*)(W + j - 3));
        vd = _mm_loadu_si128((__m128i*)(W + j - 13));
        vl = _mm_slli_epi32(vc, 15);
        vr = _mm_srli_epi32(vc, 32 - 15);
        va = _mm_xor_si128(va, vl);
        vr = _mm_xor_si128(vr, vb);//(vc<<<15)⊕va⊕vb
        ve = _mm_load_si128((__m128i*)(W + j - 6));
        vc = _mm_xor_si128(va, vr);//vc=Wj−16 ⊕ Wj−9 ⊕ (Wj−3 <<< 15)
        vl = _mm_slli_epi32(vc, 15);
        vr = _mm_srli_epi32(vc, 32 - 15);
        va = _mm_slli_epi32(vc, 23);
        vl = _mm_xor_si128(vl, vr);
        vb = _mm_srli_epi32(vc, 32 - 23);
        vc = _mm_xor_si128(vc, vl);
        va = _mm_xor_si128(vb, va);
        vl = _mm_slli_epi32(vd, 7);
        vc = _mm_xor_si128(va, vc);
        vr = _mm_srli_epi32(vd, 32 - 7);
        vc = _mm_xor_si128(vc, vl);
        ve = _mm_xor_si128(ve, vr);
        vc = _mm_xor_si128(ve, vc);
        _mm_storeu_si128((__m128i*)(W + j), vc);
        
    }
    {
        __m256i w1, w2;
        for (int i = 0; i < 64; i += 8)
        {
            w1 = _mm256_load_si256(W + i);
            w2 = _mm256_load_si256(W + i + 4);
            w1 = _mm256_xor_si256(w1, w2);
            _mm256_store_si256((__m256*)(Ww + i), w1);
        }
    }
}