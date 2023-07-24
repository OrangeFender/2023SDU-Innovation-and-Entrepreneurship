# Implement the above ECMH scheme

## ECMH简介

Elliptic curve MultiSet Hash (ECMH)是一种基于椭圆曲线的多集哈希算法，用于将多个元素映射到一个椭圆曲线上，并将曲线上的点作为哈希值。

ECMH算法的基本思想是，将每个元素编码成一个椭圆曲线上的点，然后将这些点相加得到一个新的点，最终将这个点的坐标作为哈希值。具体来说，ECMH算法的步骤如下：

1. 选择一个椭圆曲线和一个基点，作为哈希算法的参数。
2. 将每个元素编码成一个点，并将所有点相加得到一个新的点。
3. 将新点的X坐标作为哈希值。

ECMH算法的优点是，在保证安全性的前提下，具有较高的哈希速度和较小的哈希值长度。另外，ECMH算法还具有一些其他的特性，如支持动态更新、支持批量哈希等。

ECMH算法已经被应用到了一些密码学应用中，如区块链、数字签名等。但是，由于其相对较新的性质和相对较少的应用经验，目前还没有被广泛使用。

## 实现过程

* 依赖库及参数选取

```python
import numpy as np
import sympy
import hashlib

p=int('0x8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3',16)
a=int('0x787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498',16)
b=int('0x63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A',16) 
```

* 椭圆曲线上加法实现

```python
def calculate_p_q(x1, y1, x2, y2, a, b, p):
    flag = 1

    if x1 == x2 and y1 == y2:
        member = 3 * (x1 ** 2) + a  
        denominator = 2 * y1  

    else:
        member = y2 - y1
        denominator = x2 - x1
        if member * denominator < 0:
            flag = 0
            member = abs(member)
            denominator = abs(denominator)

    gcd_value = get_gcd(member, denominator)
    member = member // gcd_value
    denominator = denominator // gcd_value

    inverse_value = sympy.mod_inverse(denominator, p)
    k = (member * inverse_value)
    if flag == 0:
        k = -k
    k = k % p

    x3 = (k ** 2 - x1 - x2) % p
    y3 = (k * (x1 - x3) - y1) % p
    return [x3, y3]
```

* hash函数选取为sha256

```python
def sha256(data):
    hash_obj = hashlib.sha256()
    hash_obj.update(data)
    return hash_obj.digest()
```

* 多集hash

```python
def multi_hash(set):
    point_0=[0,0]
    for item in set:
        hash_result = sha256(item)
        x=int(hash_result.hex(),16)  
        tem=np.mod(x**2+a*x+b,p)
        y = sympy.ntheory.residue_ntheory.nthroot_mod(tem,2,p)
        while(y==None):
            tem=tem+1
            y = sympy.ntheory.residue_ntheory.nthroot_mod(tem,2,p)
        point=[x,y]
        calculate_p_q(point[0], point[1], point_0[0], point_0[1], a, b, p)
    return point
```

## 实验结果

![image-20230724155045612](C:\Users\lzdwy\AppData\Roaming\Typora\typora-user-images\image-20230724155045612.png)
