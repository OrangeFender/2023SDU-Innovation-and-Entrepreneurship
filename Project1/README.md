# implement the naïve birthday attack of reduced SM3

## 生日攻击原理

假设有一个函数f，它的输出范围是[0,2^n]，那么我们的攻击就是找到两个不同的x，y，让f(x)=f(y)。这时候，我们可以称x和y发生了碰撞。根据概率论的公式，我们想要达到50%的几率，那么需要尝试的次数是2^(n/2)。 
## 实现思路

我们寻找hash函数值的28位的碰撞。首先创建一个大小为2^(28)的数组：

```c++
unsigned short arr[ 2 << 28];
```

将其全都初始化为0。

然后对unsigned int 类型的变量i进行hash。实现方式如下：

```c++
std::string str = std::to_string(i);//转化为字符串
unsigned int message_len = strlen(str.c_str());
unsigned char hash[EVP_MAX_MD_SIZE];
unsigned int hash_len;
EVP_Digest(str.c_str(), message_len, hash, &hash_len, EVP_sm3(), NULL);
```

hash值结果存于hash[]中。

unsigned char 类型大小为8位，为得到28位，我们取hash[0],hash[1],hash[2],hash[3],将其合并为一个32位usigned int类型的数，并和28位1进行与运算，得到28位的hash值。实现方式如下：

```c++
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

	int mask = (1 << 28) - 1;  // 28 位全为 1 的数
	int tem2 = tem1 & mask;
	return tem2;
}
```

我们采取空间换取时间的方式，每对一个i进行hash，得到28位值x，就令arr[x]=i。这样我们每得到一个值，只需要检测hash[x]是否等于0即可。如果不等于0，那么得到碰撞，两个原像分别为i和arr[x]。如果等于0，就令arr[x]=i。实现过程如下：

```c++
int tem = hash28(hash[0], hash[1], hash[2], hash[3]);
if (arr[tem] != 0)
{
	cout << "找到碰撞，原像为" << i << "和" << arr[tem] << endl;
	cout << "hash值的28位";
	//cout<<tem << endl;
	std::bitset<32> binary5(tem);

	std::cout << binary5 << std::endl;
	clock_t end = clock();

	double endtime = (double)(end - start) / CLOCKS_PER_SEC;

	cout << "寻找hash值28位碰撞花费时间:" << endtime * 1000 << "ms(忽略前四位全0)" << endl;    //ms为单位
	system("pause");
	return 0;
}
else
{
	arr[tem] = i;
}
```

## 实验结果

![image-20230706201550880](C:\Users\lzdwy\AppData\Roaming\Typora\typora-user-images\image-20230706201550880.png)



|   位数   |  28bits  |
| :------: | :------: |
| **时间** | **53ms** |



