#include <iostream>
#include <openssl/evp.h>
#include <string>
#include <bitset>

#include<cstdlib>
#include<ctime>

#define collision_length 40;

long long hash_out5byte(unsigned int x) {

	unsigned char hash[EVP_MAX_MD_SIZE];
	unsigned int hash_len;

	EVP_Digest(&x, 4, hash, &hash_len, EVP_sm3(), NULL);

	return ((long long*)hash)[0] & 0xffffffffff000000;
}

unsigned int hash_out4byte(unsigned int x)
{
	unsigned char hash[EVP_MAX_MD_SIZE];
	unsigned int hash_len;

	EVP_Digest(&x, 4, hash, &hash_len, EVP_sm3(), NULL);

	return ((unsigned int*)hash)[0];
}

int main()
{
	

	//long long in1=1,out1,in2=1,out2;
	unsigned int in1 = 1, out1, in2 = 1, out2;

	clock_t start = clock();
	/*
	tem1 = hash_out5byte(x);
	tem2 = hash_out5byte(x);
	tem2 = hash_out5byte(tem2);*/

	for (unsigned int i = 1; i > 0; i++)
	{
		out1 = hash_out4byte(in1);
		in2 = hash_out4byte(in2);
		out2 = hash_out4byte(in2);
		if (out1 == out2)
		{
			std::cout << "找到32位碰撞" << std::endl;
			std::cout << "前32位为：" << out1 << std::endl;
			std::cout << "输入为(四字节输入)" << (unsigned int)in1 << "," << (unsigned int)in2 << std::endl;
			clock_t end = clock();
			double endtime = (double)(end - start) / CLOCKS_PER_SEC;
			std::cout << "寻找hash值32位碰撞花费时间:" << endtime * 1000 << "ms" << std::endl;    //ms为单位

			unsigned char hash[EVP_MAX_MD_SIZE];
			unsigned int hash_len;

			EVP_Digest(&in1, 4, hash, &hash_len, EVP_sm3(), NULL);

			std::cout << "SM3 hash value: ";
			for (unsigned int i = 0; i < hash_len; i++) {
				std::cout << std::hex << (unsigned int)hash[i];
			}
			std::cout << std::endl;

			EVP_Digest(&in2, 4, hash, &hash_len, EVP_sm3(), NULL);
			std::cout << "SM3 hash value: ";
			for (unsigned int i = 0; i < hash_len; i++) {
				std::cout << std::hex << (unsigned int)hash[i];
			}
			std::cout << std::endl;

			return 0;
		}


	}
	std::cout << "失败" << std::endl;


	return 0;
}


