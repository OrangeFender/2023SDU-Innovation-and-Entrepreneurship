# Schnorr Bacth

## 算法介绍

**生成签名：**

* 选择一个随机数$k$, 令 $R = kG$
* 令$ s = k + H(m || R || P)*x$
* 得到签名$(R,s)$

**验证签名：**

* $sG = R + H(m || R || P)P$

**批量验证：**

* $$ \sum_{i=1}^n s_i * G = \sum_{i=1}^n R_i + H(m || R || P)* \sum_{i=1}^nP_i$$

## 项目介绍

在本项目中，实现了对多组签名进行分别验证和对多组签名进行批量验证，分别命名为`Schnorr.py`和`Schnorr Bacth.py`。本项目中hash函数选取为`sha256`，且进行简化处理，取`H（m)`。具体实现过程请见`py`文件。

## 实验结果

![image-20230730182023870](C:\Users\lzdwy\AppData\Roaming\Typora\typora-user-images\image-20230730182023870.png)

![image-20230730182030995](C:\Users\lzdwy\AppData\Roaming\Typora\typora-user-images\image-20230730182030995.png)