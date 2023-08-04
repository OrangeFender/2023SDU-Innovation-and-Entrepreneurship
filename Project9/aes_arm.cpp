#include<iostream>
#include <arm_neon.h>

void aes128_enc_armv8(const uint8_t in[16], uint8_t ou[16] ,const uint32_t rk[44]) 
{
uint8x16_t block = vld1q_u8(in);
uint8_t* p8 = (uint8_t*)rk; 
block = vaesmcq_u8(vaeseq_u8(block, vld1q_u8(p8 + 16 * 0)));
block = vaesmcq_u8 (vaeseq_u8(block,vld1q_u8(p8 + 16 * 1)));
block = vaesmcq_u8 (vaeseq_u8(block,vld1q_u8(p8 + 16 * 2)));
block = vaesmcq_u8 (vaeseq_u8(block, vld1q_u8(p8 + 16 * 3)));
block = vaesmcq_u8(vaeseq_u8(block, vld1q_u8(p8 + 16 * 4)));
block = vaesmcq_u8(vaeseq_u8(block, vld1q_u8(p8 + 16 * 5)));
block = vaesmcq_u8(vaeseq_u8(block, vld1q_u8(p8 + 16 * 6)));
block = vaesmcq_u8(vaeseq_u8(block, vld1q_u8(p8 + 16 * 7)));
block = vaesmcq_u8(vaeseq_u8(block, vld1q_u8(p8 + 16 * 8)));
block = vaeseq_u8(block, vld1q_u8(p8 + 16 * 9)); // final round
block = veorq_u8(block, vld1q_u8(p8 + 16 * 10)); // final xor subkey
vst1q_u8(ou, block);
}

void aes128_dec_armv8(const uint8_t in[16], uint8_t ou[16],const uint32_t rk[44]) 
{
uint8x16_t block = vld1q_u8(in);
uint8_t* p8 = (uint8_t*)rk;
block = vaesimcq_u8(vaesdq_8(block, vld1q_u8(p8 + 16 * 0)));
block = vaesimcq_u8(vaesdq_u8(block, vld1q_u8(p8 + 16 * 1)));
block = vaesimcq_u8(vaesdq_u8(block, vld1q_u8(p8 + 16 * 2)));
block = vaesimcq_u8(vaesdq_u8(block, vld1q_u8(p8 + 16 * 3)));
block = vaesimcq_u8(vaesdq_u8(block, vld1q_u8(p8 + 16 * 4)));
block = vaesimcq_u8(vaesdq_u8(block, vld1q_u8(p8 + 16 * 5)));
block = vaesimcq_u8(vaesdq_u8(block, vld1q_u8(p8 + 16 * 6)));
block = vaesimcq_u8(vaesdq_u8(block, vld1q_u8(p8 + 16 * 7)));
block = vaesimcq_u8(vaesdq_u8(block, vld1q_u8(p8 + 16 * 8)));
block = vaesdq_u8(block, vld1q_u8(p8 + 16 * 9)); // final round
block = veorq_u8(block, vld1q_u8(p8 + 16 * 10)); // final xor subkey

vst1q_u8(ou, block); 
}


void uint8_to_uint32(const uint8_t* in, uint32_t* out, int length) {
    for (int i = 0; i < length / 4; i++) {
        out[i] = (in[4*i] << 24) | (in[4*i + 1] << 16) | (in[4*i + 2] << 8) | in[4*i + 3];
    }
}
const static uint8_t rcon[] = {
    0x01, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00,
    0x08, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x00,
    0x20, 0x00, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00,
    0x1b, 0x00, 0x00, 0x00,
    0x36, 0x00, 0x00, 0x00
};
void aes_key_expand(const uint8_t* key, uint8_t* round_keys) {
    uint8_t round_key_temp[176];
    // 第一轮密钥直接拷贝原始密钥
    for (int i = 0; i < 16; i++) {
        round_key_temp[i] = key[i];
    }

    // 按字节方式扩展剩余10轮密钥
    for (int i = 1; i <= 10; i++) {
        uint8x16_t tmp;
        uint8x16_t round_constant = vdupq_n_u8(rcon[i - 1]);
        
        // 将前一个轮密钥的最后4个字节复制到临时变量
        tmp = vld1q_u8(round_key_temp + ((i - 1) * 16) + 12);

        // 进行字节替代
        tmp = vaesmcq_u8(tmp);
        
        // 异或轮常数
        tmp = veorq_u8(tmp, round_constant);

        // 和前一个轮密钥进行异或
        tmp = veorq_u8(tmp, vld1q_u8(round_key_temp + (i - 1) * 16));
        
        // 更新当前轮密钥
        vst1q_u8(round_key_temp + i * 16, tmp);
        
        // 按字节进行循环左移
        round_key_temp[i * 16 +  3] = round_key_temp[(i - 1) * 16 + 15];
        round_key_temp[i * 16 +  2] = round_key_temp[(i - 1) * 16 + 14];
        round_key_temp[i * 16 +  1] = round_key_temp[(i - 1) * 16 + 13];
        round_key_temp[i * 16] = round_key_temp[(i - 1) * 16 + 12];
    }
    uint8_to_uint32(round_key_temp, round_keys, 176);
}


int main()
{
uint32_t rk[44];
const static uint8_t key[16] = {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};
aes_key_expand(key, rk);
uint8_t in[16]={
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};
uint8_t ou[16];
for(int i=0;i<16;i++){
    std::cout<<std::hex<<(int)in[i];
}
aes128_enc_armv8( in[16],ou[16], rk[44]);
for(int i=0;i<16;i++){
    std::cout<<std::hex<<(int)ou[i];
}
aes128_dec_armv8( ou[16], in[16], rk[44]);
for(int i=0;i<16;i++){
    std::cout<<std::hex<<(int)in[i];
}
return 0;
}
