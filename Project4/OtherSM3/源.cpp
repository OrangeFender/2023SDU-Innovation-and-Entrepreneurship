#include <iostream>
#include <openssl/evp.h>
#include <string>
#include <windows.h>

unsigned char mtest[33554432];
int main ()
{
	unsigned int message_len = 3;

	unsigned char hash[EVP_MAX_MD_SIZE];
	unsigned int hash_len;

	EVP_Digest("abc", 3, hash, &hash_len, EVP_sm3(), NULL);
	for (int i = 0; i < 32; i++)
		printf("%x", hash[i]);
	printf("\n");
	




    LARGE_INTEGER frequency, start,end;
    double elapsed_time;

    // ��ȡƵ��
    QueryPerformanceFrequency(&frequency);

    // ��ȡ��ʼʱ��
    QueryPerformanceCounter(&start);

	EVP_Digest(mtest, 33554432, hash, &hash_len, EVP_sm3(), NULL);
    QueryPerformanceCounter(&end);
    for (int i = 0; i < 32; i++)
        printf("%x", hash[i]);
    printf("\n");

    // ��������ʱ�䣨�Ժ���Ϊ��λ��
    elapsed_time = (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

    // �������ʱ��
    printf("openssl��������ʱ�䣺%.2f ����\n", elapsed_time);


	return 0;
}


