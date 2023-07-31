import hashlib
import random
import ecdsa
import time


# 选择椭圆曲线secp256k1
curve = ecdsa.curves.SECP256k1

# 定义签名函数
def schnorr_batch_sign(msg_list, sk_list):
    # 计算消息的哈希值
    h_list = [hashlib.sha256(msg).digest() for msg in msg_list]

    # 选择随机数k_list
    k_list = [random.randint(1, curve.order - 1) for _ in sk_list]

    # 计算椭圆曲线点R_list = [k*G for k in k_list]
    G = curve.generator
    R_list = [k * G for k in k_list]

    # 计算s_list = [k + x*h for k, x, h in zip(k_list, sk_list, h_list)]
    s_list = [(k + int.from_bytes(h, 'big') * x) % curve.order for k, x, h in zip(k_list, sk_list, h_list)]

    # 生成签名(r_list, s_list)
    r_list = [R.x() for R in R_list]
    return (r_list, s_list)

# 定义验证函数
def schnorr_batch_verify(msg_list, sig, vk_list):
    # 解析签名(r_list, s_list)
    r_list, s_list = sig

    # 计算消息的哈希值
    h_list = [hashlib.sha256(msg).digest() for msg in msg_list]

    # 计算椭圆曲线点R_list = [s*G - h*P for s, P, h in zip(s_list, P_list, h_list)]
    G = curve.generator
    P_list = [ecdsa.VerifyingKey.from_string(vk.to_string(), curve=curve).pubkey.point for vk in vk_list]
    R_list = [s * G + (-int.from_bytes(h, 'big') * P) for s, P, h in zip(s_list, P_list, h_list)]

    # 验证r_list是否等于R_list的横坐标

    if(all([r == R.x() for r, R in zip(r_list, R_list)])):
        print("验证成功！")

    return all([r == R.x() for r, R in zip(r_list, R_list)])



# 测试签名和验证函数
msg_list = [b'message 1', b'message 2', b'message 3']
sk_list = [random.randint(1, curve.order - 1) for _ in msg_list]
vk_list = [ecdsa.SigningKey.from_secret_exponent(sk, curve=curve).get_verifying_key() for sk in sk_list]
sig = schnorr_batch_sign(msg_list, sk_list)

time_start_1 = time.time()
assert schnorr_batch_verify(msg_list, sig, vk_list)
time_end_1 = time.time()
print("运行时间："+str(time_end_1 - time_start_1)+"秒")