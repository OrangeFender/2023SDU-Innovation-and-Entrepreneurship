# implement sm2 2P decrypt with real network communication

## 项目介绍

为实现通过真实网络进行sm2的2P解密，我们采用socket编程。其中，双方所需要的公钥等信息由utils.py文件生成。此外，其中一方所需要的密文C也由utils.py生成。

## 实验内容

![image-20230730112331180](C:\Users\lzdwy\AppData\Roaming\Typora\typora-user-images\image-20230730112331180.png)

## 实现流程

client端：

* 建立连接
* 计算T1并传递
* 接收T2
* 恢复明文

sever端：

* 建立连接
* 接收T1
* 计算T2并传递

## 实验结果

![image-20230730112706274](C:\Users\lzdwy\AppData\Roaming\Typora\typora-user-images\image-20230730112706274.png)

![image-20230730112710021](C:\Users\lzdwy\AppData\Roaming\Typora\typora-user-images\image-20230730112710021.png)
