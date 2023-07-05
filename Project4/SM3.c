#include <stdlib.h>
#include <stdio.h>
#include <immintrin.h>

#define LeftRotate(word, bits)( (word) << (bits) | (word) >> (32 - (bits)) )

#define p0(x) (x^LeftRotate(x,9)^LeftRotate(x,17))
#define T1 0x79cc4519
#define T2 0x7a879d8a
#define FF1(x,y,z) (x^y^z)
#define FF2(x,y,z) ((x&y)|(x&z)|(y&z))
#define GG1(x,y,z) (x^y^z)
#define GG2(x,y,z) ((x&y)|((~x)&z))

typedef unsigned int UINT32;

void Expand(UINT32 B[16],UINT32 W[68+3], UINT32 Ww[64]){//W长度为68但是需要加三以确保足够SIMD寄存器存放
__m256i b;
b=_mm256_load_si256(B);
_mm256_store_si256(W,b);
__m128i va, vb, vc,vd,ve,vl,vr;
for(int j=16;j<68;j+=3){
va=_mm_load_si128(W+j-16);
vb=_mm_load_si128(W+j-9);
vc=_mm_load_si128(W+j-3);
vd=_mm_load_si128(W+j-13);
vl=_mm_rol_epi32 (vc,15);
//vl=_mm_slli_epi32(vc,15);
//vr=_mm_srli_epi32(vc,32-15);
va=_mm_xor_si128(va,vl);
//vb=_mm_xor_si128(vb,vr);
ve=_mm_load_si128(W+j-6);
vc=_mm_xor_si128(va,vb);//vc=Wj−16 ⊕ Wj−9 ⊕ (Wj−3 <<< 15)
vl=_mm_rol_epi32 (vc,15);
//vl=_mm_slli_epi32(vc,15);
//vr=_mm_srli_epi32(vc,32-15);
va=_mm_rol_epi32 (vc,23);
//va=_mm_slli_epi32(vc,23);
//vl=_mm_xor_si128(vl,vr);
//vb=_mm_srli_epi32(vc,32-23);
vc=_mm_xor_si128(vc,vl);
//va=_mm_xor_si128(vb,va);
vl=_mm_rol_epi32 (vd,7);
//vl=_mm_slli_epi32(vd,7);
vc=_mm_xor_si128(va,vc);
//vr=_mm_srli_epi32(vd,32-7);
vc=_mm_xor_si128(vc,vl);
//ve=_mm_xor_si128(ve,vr);
vc=_mm_xor_si128(ve,vc);
_mm_store_si128(W+j,vc);
}
__m512i w1,w2;
for (int i = 0; i < 64; i+=16)
{
w1=_mm512_load_si512(W+i);
w2=_mm512_load_si512(W+i+4);
w1=_mm512_xor_epi32(w1,w2);
_mm512_store_si512(Ww,w1);
}
}
//
void compress(UINT32 V[8],UINT32 W[68], UINT32 Ww[64]){
int A,B,C,D,E,F,G,H,SS1,SS2,TT1,TT2;
A=V[0];B=V[1];C=V[2];D=V[3];E=V[4];F=V[5];G=V[6];H=V[7];
for(int j=0;j<16;j++){
SS1=LeftRotate((LeftRotate(A,12)+E+LeftRotate(T1,12)),7);
SS2=SS1^LeftRotate(A,12);
TT1=FF1(A,B,C)+D+SS2+Ww[j];
TT2=GG1(A,B,C)+H+SS1+W[j];
D=C;
C=LeftRotate(B,9);
B=A;
A=TT1;
H=G;
G=LeftRotate(F,19);
F=E;
E=p0(TT2);
}
for(int j=16;j<64;j++){
SS1=LeftRotate((LeftRotate(A,12)+E+LeftRotate(T2,12)),7);
SS2=SS1^LeftRotate(A,12);
TT1=FF2(A,B,C)+D+SS2+Ww[j];
TT2=GG2(A,B,C)+H+SS1+W[j];
D=C;
C=LeftRotate(B,9);
B=A;
A=TT1;
H=G;
G=LeftRotate(F,19);
F=E;
E=p0(TT2);
}
V[0]^=A;V[1]^=B;V[2]^=C;V[3]^=D;V[4]^=E;V[5]^=F;V[6]^=G;V[7]^=H;
}

int SM3(int length,unsigned char *m,unsigned char hash[32]){
    int n=((length+1+8+63)/64);//填充后长度为64字节的倍数
    int l=64*n;
    unsigned char*M=malloc(l);
    int k=64-(length+1+8%64);
    for(int i=0;i<length;i++){
        M[i]=m[i];
    }
    M[length]=0x80;
    *(int *)&M[l-8]=length*8;//末尾填充长度，单位是bit
    
}