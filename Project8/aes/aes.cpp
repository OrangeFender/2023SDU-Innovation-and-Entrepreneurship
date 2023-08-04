#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<iomanip>
using namespace std;
int GaloisMul_2(int Num_R)
{
	if (Num_R >= 128)
	{
		Num_R = (Num_R - 128) * 2;
		return Num_R ^ 27;
	}
	else
	{
		return Num_R * 2;
	}
}
int GaloisMul_4(int Num_R)
{
	return GaloisMul_2(GaloisMul_2(Num_R));
}
int GaloisMul_8(int Num_R)
{
	return GaloisMul_2(GaloisMul_2(GaloisMul_2(Num_R)));
}
int GaloisMultiplication(int Num_L, int Num_R)//伽罗华域内的运算
{
	switch (Num_L)
	{
	case 1:return Num_R;
	case 2:return GaloisMul_2(Num_R);
	case 3:return GaloisMul_2(Num_R) ^ Num_R;
	default:return -1;
	}
}

void AddRoundKey(int* PlainArray, int* CipherArray, int* Key,int time)//time 0-10
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			CipherArray[i * 4 + j] = PlainArray[i * 4 + j] ^ Key[i * 44 + j + time*4];//subkey一行44个
		}
	}
}
void SubByte(int* PlainArray, int* CipherArray,int length,bool flag)//flag=FALSE为S盒逆运算，flag=TRUE为S盒运算
{
	if (flag == true)
	{
		int S1[256] = {
		0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
		0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
		0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
		0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
		0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
		0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
		0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
		0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
		0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
		0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
		0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
		0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
		0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
		0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
		0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
		0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16 };
		for (int i = 0; i < length; i++)
		{
			CipherArray[i] = S1[(PlainArray[i] / 16) * 16 + (PlainArray[i] % 16)];
		}
	}
	else
	{
		int S2[256] = {
			0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
			0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
			0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
			0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
			0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
			0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
			0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,	
			0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
			0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
			0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
			0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
			0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
			0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
			0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
			0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
			0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D };
		for (int i = 0; i < length; i++)
		{
			CipherArray[i] = S2[(PlainArray[i] / 16) * 16 + (PlainArray[i] % 16)];
		}
	}
}
void ShiftRow(int *PlainArray,int *CipherArray)
{
	CipherArray[0] = PlainArray[0];
	CipherArray[1] = PlainArray[1];
	CipherArray[2] = PlainArray[2];
	CipherArray[3] = PlainArray[3];

	CipherArray[4] = PlainArray[5];
	CipherArray[5] = PlainArray[6];
	CipherArray[6] = PlainArray[7];
	CipherArray[7] = PlainArray[4];

	CipherArray[8] = PlainArray[10];
	CipherArray[9] = PlainArray[11];
	CipherArray[10] = PlainArray[8];
	CipherArray[11] = PlainArray[9];

	CipherArray[12] = PlainArray[15];
	CipherArray[13] = PlainArray[12];
	CipherArray[14] = PlainArray[13];
	CipherArray[15] = PlainArray[14];

}
void MixColumn(int* ArrayTemp,int* PlainArray)//前一个为加密矩阵，后一个作为加密结果
{
	int MixArray[4][4] =
	{
	0x02, 0x03, 0x01, 0x01,
	0x01, 0x02, 0x03, 0x01,
	0x01, 0x01, 0x02, 0x03,
	0x03, 0x01, 0x01, 0x02 
	};
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++) 
		{
			PlainArray[i*4+j] = 
				GaloisMultiplication(MixArray[i][0], ArrayTemp[0 * 4 + j]) ^
				GaloisMultiplication(MixArray[i][1], ArrayTemp[1 * 4 + j]) ^
				GaloisMultiplication(MixArray[i][2], ArrayTemp[2 * 4 + j]) ^
				GaloisMultiplication(MixArray[i][3], ArrayTemp[3 * 4 + j]);
		}
	}
}
void KeyExpansion(int* Key,int* Subkey)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Subkey[i * 44 + j] = Key[i * 4 + j];
		}
	}
	int Rcon[10][4] = {
		1,0,0,0,
		2,0,0,0,
		4,0,0,0,
		8,0,0,0,
		16,0,0,0,
		32,0,0,0,
		64,0,0,0,
		128,0,0,0,
		27,0,0,0, 
		54,0,0,0 };
	for (int i = 4; i < 44; i++)
	{
		if (i % 4 == 0)
		{
			int tmp1[4] = {Subkey[44 + i - 1] ,Subkey[88 + i - 1] ,Subkey[132 + i - 1], Subkey[i - 1]  };//第一步移位
			int tmp2[4] = { 0,0,0,0 };//第二步S盒
			SubByte(tmp1, tmp2, 4, true);

			Subkey[i] = Rcon[i / 4 - 1][0] ^ tmp2[0] ^ Subkey[i-4];
			Subkey[44+i] = Rcon[i / 4 - 1][1] ^ tmp2[1] ^ Subkey[44+i - 4];
			Subkey[88+i] = Rcon[i / 4 - 1][2] ^ tmp2[2] ^ Subkey[88+i - 4];
			Subkey[132+i] = Rcon[i / 4 - 1][3] ^ tmp2[3] ^ Subkey[132+i - 4];
		}
		else
		{
			Subkey[i] = Subkey[i - 1] ^ Subkey[i - 4];
			Subkey[44 + i] = Subkey[44 + i - 1] ^ Subkey[44 + i - 4];
			Subkey[88 + i] = Subkey[88 + i - 1] ^ Subkey[88 + i - 4];
			Subkey[132 + i] = Subkey[132 + i - 1] ^ Subkey[132 + i - 4];
		}
	}
}


void AES_Encrypto(int* Plaintext, int* Ciphertext, int* Key, int length)
{
	//进来时length即为16倍数
	int num = length / 16;
	int rowlen = num * 4;
	int temptext1[16];
	int temptext2[16];
	int temptext3[16];
	int temptext4[16];
	int temptext5[16];
	int Subkey[176];
	KeyExpansion(Key, Subkey);

	//明文分块
	for (int i = 0; i < num; i++)
	{
		int Plaintext_Seperate[16];
		for (int j = 0; j < 16; j++)
		{
			Plaintext_Seperate[j] = Plaintext[i * 16 + j];
		}
		AddRoundKey(Plaintext_Seperate, temptext1, Subkey,0);
		for (int j = 0; j < 9; j++)
		{
 			SubByte(temptext1, temptext2, 16, true);
			ShiftRow(temptext2, temptext3);
			MixColumn(temptext3, temptext4);
			AddRoundKey(temptext4, temptext5, Subkey,j+1);

			for (int k = 0; k < 16; k++)
			{
				temptext1[k] = temptext5[k];
			}
		}
		SubByte(temptext1, temptext2, 16, true);
		ShiftRow(temptext2, temptext3);
		AddRoundKey(temptext3, temptext4, Subkey,10);
		
		//转置
		for (int j = 0; j < 4; j++)
		{
			Ciphertext[j*4] = temptext4[j];
			Ciphertext[j*4 + 1] = temptext4[4 + j];
			Ciphertext[2 + j*4] = temptext4[8 + j];
			Ciphertext[3 + j*4] = temptext4[12 + j];
		}
	}
}

 
int main()
{
	int Plaintext[16], Ciphertext[16],XOR[16];
	int key[16] = { 0x32,0x30,0x32,0x31 ,0x30 ,0x30 ,0x34 ,0x36 ,0x30 ,0x31 ,0x32 ,0x36 };
	srand(time(0));
	char x[17] = "202100460046";
	for (int i = 0; i < 16; i++)
	{
		//右侧填充0
		if (x[i] == '\0')
		{
			for (int j = i; j < 16; j++)
			{
				Plaintext[j % 4 * 4 + j / 4] = 0;
			}
			break;
		}
		Plaintext[i % 4 * 4 + i / 4] = (int)x[i];
	}
	

	double start = clock();
	for(int i=0; i<1000000;i++)
		AES_Encrypto(Plaintext,Ciphertext,Plaintext,16);
	double end = clock();
	cout << endl;
	for (int i = 0; i < 4; i++)
	{
		cout << hex << Ciphertext[i] << " ";
		cout << hex << Ciphertext[4+i] << " ";
		cout << hex << Ciphertext[8+i] << " ";
		cout << hex << Ciphertext[12+i] << " "<<endl;
	}
	cout <<endl<< "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << endl;



	
	return 0;
}