
import sys
sys.path.append('D:\sm3')
from sm3 import sm3hash

#寻找x比特长度的碰撞
def birth_bit(x):#x必须为4的倍数
    ans=[-1]*(2**x)
    y=x/2
    p=2**y
    #print(p)
    for i in range(2**128):#生日攻击的复杂度为1.17*sqrt(n)
        temp=int(sm3hash(hex(i))[0:int(x/4)],16)#要注意python中整数默认为float格式
        if ans[temp]==-1:#如果数组值发生了改变，说明原来至少有一个i和现在的i发生了碰撞
            ans[temp]=i;
        else:
            print(i,ans[temp])#ans数组的下标表示hash值，值表示hash值的原像

#寻找x比特长度的碰撞
def birth_30bit(x):#x必须为4的倍数
    ans=[-1]*(2**x)
    y=x/2
    p=2**y
    #print(p)
    for i in range(2**128):#生日攻击的复杂度为1.17*sqrt(n)
        temp=int(bin(int(sm3hash(hex(i))[0:8],16))[2:x+2],2)#要注意python中整数默认为float格式
        if ans[temp]==-1:#如果数组值发生了改变，说明原来至少有一个i和现在的i发生了碰撞
            ans[temp]=i;
        else:
            print(i,ans[temp])#ans数组的下标表示hash值，值表示hash值的原像


#birth_bit(28)
#birth_30bit(30)
#print(int(sm3hash(hex(21))[0:4],16))
#print(sm3hash(hex(33175)))
#print(sm3hash(hex(30844)))
print(sm3hash(hex(41207)))
print(sm3hash(hex(17079)))
