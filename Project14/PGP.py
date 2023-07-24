import base64
import binascii
from gmssl import sm2, func

import os
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import padding

m = b"Hello, World!"
#16进制的公钥和私钥，运行creat_pk_sk.py文件得到两对16进制的公钥和私钥
s_private_key = 'c0936685b2cd410bbc3ff428a327fc32e7613d315bc123d065ae7e5aa60269e0'
s_public_key = 'c1a4aef3aab26db6ee7e6140c68b483132632497a48df433fa55c29ff50e7a12f3a1824689f2f31c019696ac64be38f5e7c2ebe431d38db2112b41e091737d2a'
r_private_key='d8d01f5ccb5eeb0e2d4a0a8e1613a0e601a0cfb27d0da5b5eb2ea7d4547f4f22'
r_public_key='d4eed75b3826806b908aba08f5773c33b369b1c6d39936584d356970ba5f2db3b884ff9e586b2e8c54104fed30c5d584778f46a1dfd4b3cf60490d4b2f5054e2'


#RSA部分
## 生成128位随机密钥
key = os.urandom(16)

## 使用AES-128算法创建对称加密器
cipher = Cipher(algorithms.AES(key), modes.ECB(), backend=default_backend())

def AES_128_EN(data):
    padder = padding.PKCS7(algorithms.AES.block_size).padder()
    padded_data = padder.update(data) + padder.finalize()
    encryptor = cipher.encryptor()
    encrypted_data = encryptor.update(padded_data) + encryptor.finalize()
    #print("密钥k：",key)
    print("待RSA加密的明文m：", data,'\n')
    #print("填充后的数据：", padded_data)
    print("加密后的密文m_en：", encrypted_data,'\n')
    return encrypted_data,padded_data

def AES_128_DE(encrypted_data):
    decryptor = cipher.decryptor()
    unpadder = padding.PKCS7(algorithms.AES.block_size).unpadder()
    decrypted_data = unpadder.update(decryptor.update(encrypted_data)) + unpadder.finalize()
    #print("密钥k：",key)
    
    #print("加密后的数据：", encrypted_data)
    print("对密文m_en进行解密，得到明文m：", decrypted_data,'\n')
    return decrypted_data

#sm2部分
def sm2_en(key):
    sm2_crypt = sm2.CryptSM2(
    public_key=r_public_key, private_key=r_private_key)
    k_en = sm2_crypt.encrypt(key)
    print("RSA的密钥k：",key,'\n')
    print("sm2对k加密后，k_en：",k_en,'\n')
    return k_en

def sm2_de(k_en):
    sm2_crypt = sm2.CryptSM2(
    public_key=r_public_key, private_key=r_private_key)
    dec_data =sm2_crypt.decrypt(k_en)
    print("对k_en解密,得到k：",dec_data,'\n')
    return dec_data
    
def SE(m,s_private_key,s_public_key,r_private_key,r_public_key,key):
    k_en=sm2_en(key)
    encrypted_data,padded_dat=AES_128_EN(m)
    sm2_crypt = sm2.CryptSM2(
    public_key=s_public_key, private_key=s_private_key)
    random_hex_str = func.random_hex(sm2_crypt.para_len)
    sign = sm2_crypt.sign(k_en, random_hex_str)
    print("对k_en签名，签名值：",sign,'\n')
    return k_en,encrypted_data,sign

def RE(k_en,sign,encrypted_data):
    jk=sm2_de(k_en)
    sm2_crypt = sm2.CryptSM2(
    public_key=s_public_key, private_key=s_private_key)
    print("签名是否验证成功：")
    if(sm2_crypt.verify(sign, k_en)==True):
        print("验签成功！",'\n')
    else:
        print("验证失败！",'\n')
    AES_128_DE(encrypted_data)

print("发送者公钥：",s_public_key)
print("发送者私钥：",s_private_key,'\n')
print("接收者公钥：",r_public_key)
print("接收者私钥：",r_private_key,'\n')
k_en,encrypted_data,sign=SE(m,s_private_key,s_public_key,r_private_key,r_public_key,key)
RE(k_en,sign,encrypted_data)
