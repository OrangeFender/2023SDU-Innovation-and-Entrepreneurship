# verify the above pitfalls with proof-of-concept code

## 项目介绍

ECDMA、Schnorr、SM2-sig存在共同的陷阱，主要陷阱如下：

$\sdot$ Leaking  $k$ leads to leaking of $d$

$\sdot$ Reusing $k$ leads to leaking of $d$

$\sdot$ Two users, using $k$ leads to leaking of $d$, that is they can deduce each other’s $d$

$\sdot$ Malleability, e.g. $(r,s)$ and $(r,-s)$ are both valid signatures, lead to blockchain network split

$\sdot$ Same $d$ and $k$ with ECDSA, leads to leaking of $d$

$\sdot$ Ambiguity of DER encode could lead to blockchain network split

$\sdot$ One can forge signature if the verification does not check $m$

本项目以ECDSA为例，实现了前五个陷阱攻击。

## 结果展示

![image-20230723160825408](C:\Users\lzdwy\AppData\Roaming\Typora\typora-user-images\image-20230723160825408.png)