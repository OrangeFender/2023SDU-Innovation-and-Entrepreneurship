# impl sm2 with RFC6979

## RFC6962

当生成ECDSA签名时，需要保证$k$是保密且唯一的。$k$值重复（弱伪随机）可能导致私钥泄露。

因此在本次实验中令k=hash(M||dA)，其中dA为私钥，hash函数为 sm3算法。

## SM2签名算法

### 签名算法生成

待签名算法为M，为得到签名（r,s）

1. $\overline{M}=Z_A||M$

2. 计算 $e=H_v(\overline{M})$ ，将 $e$ 转换为整数
3. 随机生成 $k$ ， $k\in[1,n-1]$ 
4. 计算椭圆曲线点 $（x_1,y_1）=[k]G$ ，将 $x_1$ 转换为整数
5. 计算 $r=(e+x_1) mod n$ ,若 $r=0$ 或 $r+k=n$ ，则返回3
6. 计算 $s=((1+d_A)^{-1}\cdot(k-r\cdot d_A))mod n$ ,若 $s=0$ ，则返回3
7. 将 $r、s$ 转换为字节串，签名为 $（r,s）$

### 签名验证算法

收到消息 $M'$ 及其数字签名 $（r',s'）$ ，验证

1. 检验 $r'\in [1,n-1]$
2. 检验 $s'\in [1,n-1]$
3. 置 $\overline{M'}=Z_A||M'$
4. 计算 $e'=H_v(\overline{M'})$ ，将 $e'$ 转换为整数
5. 将 $r',s'$ 转换为整数，计算 $t=(r'+s')mod n$ ，若 $t=0$ ，则验证不通过
6. 计算椭圆曲线点 $（x'_1,y'_1）=[s']G+[t]P_A$
7. 将 $x1'$ 转换为整数，计算 $R=(e'+x'_1)mod n$ ，检验 $R=r'$ 是否成立

### 参数选取

$p=0x8542D69E 4C044F18 E8B92435 BF6FF7DE 45728391 5C45517D 722EDB8B 08F1DFC3$

$a=0x787968B4 FA32C3FD 2417842E 73BBFEFF 2F3C848B 6831D7E0 EC65228B 3937E498  $

$b=0x63E4C6D3 B23B0C84 9CF84241 484BFE48 F61D59A5 B16BA06E 6E12D1DA 27C5249A  $

$Gx=0x421DEBD6 1B62EAB6 746434EB C3CC315E 32220B3B ADD50BDC 4C4E6C14 7FEDD43D  $

$Gy=0x0680512B CBB42C07 D47349D2 153B70C4 E5D7FDFC BFA36EA1 A85841B9 E46E09A2 $

$n=0x8542D69E 4C044F18 E8B92435 BF6FF7DD 29772063 0485628D 5AE74EE7 C32E79B7 $

$message='hello'$

$ID_A=0x414C494345313233405941484F4F2E434F4D$

### 实验结果

![image-20230719171922513](C:\Users\lzdwy\AppData\Roaming\Typora\typora-user-images\image-20230719171922513.png)
