#include <iostream>
#include <openssl/evp.h>
#include <string>
#include <bitset>

#include<cstdlib>
#include<ctime>

using namespace std;
#define collision_length 28

unsigned short arr[ 2 << 28];//ȫ�ֱ�������0

int hash28(char a, char b, char c, char d)
{
	std::bitset<8> binary0(a);
	//std::cout << binary0 << std::endl;
	std::bitset<8> binary1(b);
	//std::cout << binary1 << std::endl;
	std::bitset<8> binary2(c);
	//std::cout << binary2 << std::endl;
	std::bitset<8> binary3(d);
	//std::cout << binary3 << std::endl;

	unsigned int num0 = static_cast<unsigned int>(binary0.to_ulong());
	unsigned int num1 = static_cast<unsigned int>(binary1.to_ulong());
	unsigned int num2 = static_cast<unsigned int>(binary2.to_ulong());
	unsigned int num3 = static_cast<unsigned int>(binary3.to_ulong());

	int tem1 = (num0 << 24) | (num1 << 16) | (num2 << 8) | num3;

	int mask = (1 << 28) - 1;  // 28 λȫΪ 1 ����
	int tem2 = tem1 & mask;
	return tem2;

}

int main()
{
	clock_t start = clock();
	for (unsigned short i = 1; i > 0; i++)
	{

		std::string str = std::to_string(i);//ת��Ϊ�ַ���
		unsigned int message_len = strlen(str.c_str());

		unsigned char hash[EVP_MAX_MD_SIZE];
		unsigned int hash_len;

		EVP_Digest(str.c_str(), message_len, hash, &hash_len, EVP_sm3(), NULL);

		/*std::cout << "SM3 hash value: ";
		for (unsigned int i = 0; i < hash_len; i++) {
			std::cout << std::hex << (unsigned int)hash[i];
		}
		std::cout << std::endl;*/

		int tem = hash28(hash[0], hash[1], hash[2], hash[3]);
		if (arr[tem] != 0)
		{
			cout << "�ҵ���ײ��ԭ��Ϊ" << i << "��" << arr[tem] << endl;
			cout << "hashֵ��28λ";
			//cout<<tem << endl;
			std::bitset<32> binary5(tem);

			std::cout << binary5 << std::endl;
			clock_t end = clock();

			double endtime = (double)(end - start) / CLOCKS_PER_SEC;

			cout << "Ѱ��hashֵ28λ��ײ����ʱ��:" << endtime * 1000 << "ms(����ǰ��λȫ0)" << endl;    //msΪ��λ
			system("pause");
			return 0;
		}
		else
		{
			arr[tem] = i;
		}
	}
	cout << "��Ѱ��ײʧ�ܣ�" << endl;
	clock_t end = clock();

	double endtime = (double)(end - start) / CLOCKS_PER_SEC;

	cout << "Total time:" << endtime * 1000 << "ms" << endl;    //msΪ��λ
	system("pause");
	return 0;
}



