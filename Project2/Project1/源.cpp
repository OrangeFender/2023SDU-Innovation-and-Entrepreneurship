#include <iostream>
#include <openssl/evp.h>
#include <string>
#include <bitset>

#include<cstdlib>
#include<ctime>

using namespace std;
#define collision_length 32

struct three {
	unsigned int a;
	unsigned int b;
	unsigned int c;
};

struct four {
	unsigned int a;
	unsigned int b;
	unsigned int c;
	unsigned int d;
};


// 将三个 unsigned int 类型的变量拼接为一个 unsigned int 类型的变量

unsigned int join(unsigned int a, unsigned int b, unsigned int c) {
	return ((a << 16) | (b << 8) | c);
}

unsigned int join4(unsigned int a, unsigned int b, unsigned int c, unsigned int d)
{
	return ((a << 24) | (b << 16) | (c << 8) | d);
}

three hash_(unsigned int x)
{
	three tem;
	std::string str = std::to_string(x);//转化为字符串
	//unsigned int message_len = strlen(str.c_str());

	unsigned char hash[EVP_MAX_MD_SIZE];
	unsigned int hash_len;

	EVP_Digest(str.c_str(), 256, hash, &hash_len, EVP_sm3(), NULL);
	tem.a = (unsigned int)hash[0];
	tem.b = (unsigned int)hash[1];
	tem.c = (unsigned int)hash[2];
	return tem;
}


four hash_4(unsigned int x)
{
	four tem;
	std::string str = std::to_string(x);//转化为字符串
	unsigned int message_len = strlen(str.c_str());

	unsigned char hash[EVP_MAX_MD_SIZE];
	unsigned int hash_len;

	EVP_Digest(str.c_str(), 256, hash, &hash_len, EVP_sm3(), NULL);
	tem.a = (unsigned int)hash[0];
	tem.b = (unsigned int)hash[1];
	tem.c = (unsigned int)hash[2];
	tem.d = (unsigned int)hash[3];
	return tem;
}




int main()
{
	//three tem1;
	//three tem2;
	//
	//
	//unsigned int x = 456;
	//	
	//tem1 = hash_(x);
	//tem2 = hash_(x);
	//unsigned int result1;
	//unsigned int result2;

	//clock_t start = clock();
	//
	//result1 = join(tem1.a, tem1.b, tem1.c);
	////cout << result1 << endl;
	//
	//result2 = join(tem2.a,tem2.b, tem2.c);
	////cout << result2 << endl;

	//tem2 = hash_(result2);
	//result2 = join(tem2.a, tem2.b, tem2.c);


	//for (int i = 0; i < 99999999; i++)
	//{
	//	
	//	if (tem1.a == tem2.a && tem1.b == tem2.b && tem1.c == tem2.c)
	//	{
	//		cout << "找到24位碰撞" << endl;
	//		cout << "前24位为0x" << hex << tem1.a << hex << tem1.b << hex << tem1.c << endl;
	//		cout << "前24位为0x" << hex << tem2.a << hex << tem2.b << hex << tem2.c << endl;
	//		cout << "循环" <<dec<<i<<"次"<< endl;
	//		clock_t end = clock();
	//		double endtime = (double)(end - start) / CLOCKS_PER_SEC;
	//		cout << "寻找hash值24位碰撞花费时间:" << endtime * 1000 << "ms" << endl;    //ms为单位
	//		system("pause");
	//		return 0;
	//	}
	//	else
	//	{
	//		tem1 = hash_(result1);
	//		result1 = join(tem1.a, tem1.b, tem1.c);
	//		

	//		tem2 = hash_(result2);
	//		result2 = join(tem2.a, tem2.b, tem2.c);
	//		tem2 = hash_(result2);
	//		result2 = join(tem2.a, tem2.b, tem2.c);
	//		
	//		/*if (i % 100 == 0)
	//		{
	//			cout << result1 << endl << result2 << endl;
	//		}*/
	//	}

	//}
	//cout << "循环99999999次未找到12位碰撞" << endl;
	//

	four tem1;
	four tem2;


	unsigned int x = 45654;

	tem1 = hash_4(x);
	tem2 = hash_4(x);
	unsigned int result1;
	unsigned int result2;

	clock_t start = clock();

	result1 = join4(tem1.a, tem1.b, tem1.c,tem1.d);
	//cout << result1 << endl;

	result2 = join4(tem2.a, tem2.b, tem2.c,tem2.d);
	//cout << result2 << endl;

	tem2 = hash_4(result2);
	result2 = join4(tem2.a, tem2.b, tem2.c,tem2.d);


	for (int i = 0; i < 99999999; i++)
	{

		if (tem1.a == tem2.a && tem1.b == tem2.b && tem1.c == tem2.c&&tem1.d==tem2.d)
		{
			cout << "找到32位碰撞" << endl;
			cout << "前32位为0x" << hex << tem1.a << hex << tem1.b << hex << tem1.c << hex << tem1.d << endl;
			cout << "前32位为0x" << hex << tem2.a << hex << tem2.b << hex << tem2.c << hex << tem2.d << endl;
			cout << "循环" << dec << i << "次" << endl;
			clock_t end = clock();
			double endtime = (double)(end - start) / CLOCKS_PER_SEC;
			cout << "寻找hash值32位碰撞花费时间:" << endtime * 1000 << "ms" << endl;    //ms为单位
			//system("pause");
			return 0;
		}
		else
		{
			tem1 = hash_4(result1);
			result1 = join4(tem1.a, tem1.b, tem1.c,tem1.d);


			tem2 = hash_4(result2);
			result2 = join4(tem2.a, tem2.b, tem2.c,tem2.d);
			tem2 = hash_4(result2);
			result2 = join4(tem2.a, tem2.b, tem2.c,tem2.d);

			/*if (i % 100 == 0)
			{
				cout << result1 << endl << result2 << endl;
			}*/
		}

	}
	cout << "循环99999999次未找到12位碰撞" << endl;


	return 0;
}


