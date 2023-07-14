#include<iostream>
#include "openssl/sha.h"

#define HashLen 32

void sign(int V,unsigned char*PublicHash, unsigned char* Seed) {//可信机构的签发过程
	unsigned char h[HashLen];
	memcpy(h,Seed,HashLen);
	int i;
	for (i = 0; i < V; i++) {
		SHA256(h, HashLen, h);
	}
	memcpy(PublicHash, h, HashLen);
}

void prover(int V,int v, unsigned char* Seed,unsigned char* Proverhash) {//证明者提供数据
	unsigned char h[HashLen];
	memcpy(h, Seed, HashLen);
	for (int i = v; i < V; i++) {
		SHA256(h, HashLen, h);
	}
	memcpy(Proverhash, h, HashLen);
}

bool prove(int v, unsigned char* Proverhash, unsigned char* Publichash) {//被证明者完成证明过程
	unsigned char h[HashLen];
	memcpy(h, Proverhash, HashLen);
	for (int i = 0; i < v; i++) {
		SHA256(h, HashLen, h);
	}
	return memcmp(Publichash, h, HashLen)==0;
}

int main() {
	unsigned char seed[HashLen] = "abcdefgh12345678ABCDEFGH8765432";//生成的随机种子
	int V = 21;//证明者拥有的数量为21
	unsigned char Pub[HashLen];
	sign(V,Pub,seed );//可信机构对计算结果签名（签名略）
	unsigned char Proverhash[HashLen];
	int v = 12;//被证明者要求证明者证明自己的数量大于等于12
	prover(V, v, seed, Proverhash);//证明者提供证明数据
	bool ret=prove(v, Proverhash, Pub);//被证明者完成证明过程
	std::cout << ret;
}