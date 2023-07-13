#include<iostream>
#include "openssl/sha.h"

#define HashLen 32

void sign(int V,unsigned char*PublicHash, unsigned char* Seed) {//���Ż�����ǩ������
	unsigned char h[HashLen];
	memcpy(h,Seed,HashLen);
	int i;
	for (i = 0; i < V; i++) {
		SHA256(h, HashLen, h);
	}
	memcpy(PublicHash, h, HashLen);
}

void prover(int V,int v, unsigned char* Seed,unsigned char* Proverhash) {//֤�����ṩ����
	unsigned char h[HashLen];
	memcpy(h, Seed, HashLen);
	for (int i = v; i < V; i++) {
		SHA256(h, HashLen, h);
	}
	memcpy(Proverhash, h, HashLen);
}

bool prove(int v, unsigned char* Proverhash, unsigned char* Publichash) {//��֤�������֤������
	unsigned char h[HashLen];
	memcpy(h, Proverhash, HashLen);
	for (int i = 0; i < v; i++) {
		SHA256(h, HashLen, h);
	}
	return memcmp(Publichash, h, HashLen)==0;
}

int main() {
	unsigned char seed[HashLen] = "abcdefgh12345678ABCDEFGH8765432";//���ɵ��������
	int V = 21;//֤����ӵ�е�����Ϊ21
	unsigned char Pub[HashLen];
	sign(V,Pub,seed );//���Ż����Լ�����ǩ����ǩ���ԣ�
	unsigned char Proverhash[HashLen];
	int v = 12;//��֤����Ҫ��֤����֤���Լ����������ڵ���12
	prover(V, v, seed, Proverhash);//֤�����ṩ֤������
	bool ret=prove(v, Proverhash, Pub);//��֤�������֤������
	std::cout << ret;
}