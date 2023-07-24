# Implement a PGP scheme with SM2

## PGP

![image-20230724182230065](C:\Users\lzdwy\AppData\Roaming\Typora\typora-user-images\image-20230724182230065.png)



## 实现思路

**算法选取：**

非对称加密算法：sm2

对称加密算法：AES-128-ECB

**加密过程：**

1.发送方和接收方分别生成sm2的公钥和私钥： $(pk_s,sk_s),(pk_r,sk_r)$

2.选取对称加密算法的密钥 $K$ ,用接收方的公钥 $(pk_r)$ 加密，得到 $K_{en}$

3.发送方用 $K$ 加密信息 $m$ ，得到 $m_{en}$

4.发送方用自己的私钥 $(sk_s)$ 签名 $K_{en}$，得到签名 $sign$

5,发送 $K_{en}$， $m_{en}$， $sign$

**解密过程：**

1.对 $K_{en}$ 验签

2.接收方用自己的私钥 $(pk_s)$ 解密 $K_{en}$

3.用 $K$ 解密 $m_{en}$



## 结果展示

![image-20230724183147540](C:\Users\lzdwy\AppData\Roaming\Typora\typora-user-images\image-20230724183147540.png)