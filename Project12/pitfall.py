import ecdsa
import random
import string
import hashlib
import binascii
from sympy import mod_inverse
#初始化，调库生成ECDSA签名方案的各项参数
def init_():
    sk = ecdsa.SigningKey.generate(curve=ecdsa.NIST256p) #生成私钥
    vk = sk.verifying_key  # 生成公钥,横纵坐标级联
    d = int(sk.to_string().hex(), 16) #d，私钥
    Q = int(vk.to_string().hex(), 16) #Q，公钥
    n = sk.curve.order #模数
    K = random.randint(1, n)
    mes = b'hello' #待签名的消息
    sign = sk.sign(mes, k=K, hashfunc=hashlib.sha256) #获取签名值
    r = sign.hex()[:64]  #r
    s = sign.hex()[64:]  #s
    s = int(s, 16)
    r = int(r, 16)
    e = hashlib.sha256()
    e.update(mes)
    e = int(e.hexdigest(), 16)
    return sk,vk,d,Q,n,K,mes,sign,r,s,e

def leaking_k_to_d(K,s,r,n,e):
    r_inv = mod_inverse(r, n)
    d=(r_inv*(s*K-e))%n
    print("d =",d,"\n")

def  reusing_k_to_d(K,s,r,sk,e,n):
    mes_2=b'word'
    sign_2=sk.sign(mes_2, k=K, hashfunc=hashlib.sha256)
    r2 = sign_2.hex()[:64]
    s_2 = sign_2.hex()[64:]
    e_2 = hashlib.sha256()
    e_2.update(mes_2)
    e_2 = int(e_2.hexdigest(), 16)
    s_2 = int(s_2, 16)
    r2 = int(r2, 16)
    d_2=(s * e_2 - s_2 * e) * mod_inverse(s_2 * r - s * r, n) % n
    print('d=', d_2, '\n')

def reusing_k_to_each_d(K,n,s,e,r,d):
    sk_2 = ecdsa.SigningKey.generate(curve=ecdsa.NIST256p)  # 用户2再生成一个d
    vk_2 = sk_2.verifying_key  # P的横坐标纵坐标级联
    mes_2 = b'word'
    sign_2 = sk_2.sign(mes_2, k=K, hashfunc=hashlib.sha256)
    r2 = sign_2.hex()[:64]
    s2 = sign_2.hex()[64:]
    e2 = hashlib.sha256()
    e2.update(mes_2)
    e2 = int(e2.hexdigest(), 16)
    s2 = int(s2, 16)
    r2 = int(r2, 16)
    d2 = (s2 * e - s * e2 + s2 * r * d) * mod_inverse(s * r, n) % n
    d1 = (s * e2 - s2 * e + s * r * d2) * mod_inverse(s2 * r, n) % n
    print('d1:', d1)
    print('d2:', d2, '\n')


def rs_to_negative_rs(s,sign,vk,mes):
    s_2 = (-s) % n
    s_2 = hex(s_2)[2:]
    if len(s_2) % 2:
        s_2 = '0' + s_2
    sign_2 = sign.hex()[:64] + s_2
    sign_2 = binascii.unhexlify(sign_2)
    if(vk.verify(sign_2, mes, hashfunc=hashlib.sha256)==True):
        print("验证成功！\n")
    else:
        print("验证失败！\n")

def ecdsa_schnorr(d,sk,mes,K,n,s,r,e):
    G = sk.curve.generator
    e_1 = int(hashlib.sha256((K * G).to_bytes() + mes).hexdigest(), 16)
    s_1 = (K + e_1 * d) % n
    d_1 = (s * s_1 - e) * mod_inverse(s * e_1 + r, n) % n
    print('计算得出d =',d_1,'\n')


sk,vk,d,P,n,K,mes,sign,r,s,e = init_()
print('Leaking  k leads to leaking of d:')
leaking_k_to_d(K,s,r,n,e)
print('Reusing k leads to leaking of d:')
reusing_k_to_d(K,s,r,sk,e,n)
print('Two users, using k leads to leaking of d, that is they can deduce each other’s d:')
reusing_k_to_each_d(K,n,s,e,r,d)
print('(r,s) and (r,-s) are both valid signatures:')
rs_to_negative_rs(s,sign,vk,mes)
print('Same d and k in ECDSA and Schnorr, leads to leaking of d:')
ecdsa_schnorr(d,sk,mes,K,n,s,r,e)
