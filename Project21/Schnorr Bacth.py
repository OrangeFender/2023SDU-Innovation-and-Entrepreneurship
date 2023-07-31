import gmssl.sm3
import sympy
import random
import hashlib
import time

p=0x8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3

a=0x787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498

b=0x63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A

xG=0x421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D

yG=0x0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2

n=0x8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7


#最大公约数
def get_gcd(x, y):
    if y == 0:
        return x
    else:
        return get_gcd(y, x % y)

#p+q
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

#2p
def calculate_2p(p_x, p_y, a, b, p):
    tem_x = p_x
    tem_y = p_y
    p_value = calculate_p_q(tem_x, tem_y, p_x, p_y, a, b, p)
    tem_x = p_value[0]
    tem_y = p_value[1]
    return p_value


# nP
def calculate_np(p_x, p_y, n, a, b, p):
    p_value = ["0", "0"]
    p_temp = [0, 0]
    while n != 0:
        if n & 1:
            if (p_value[0] == "0" and p_value[1] == "0"):
                p_value[0], p_value[1] = p_x, p_y
            else:
                p_value = calculate_p_q(p_value[0], p_value[1], p_x, p_y, a, b, p)
        n >>= 1
        p_temp = calculate_2p(p_x, p_y, a, b, p)
        p_x, p_y = p_temp[0], p_temp[1]
    return p_value

def schnorr_batch_sign(msg_list,sk_list):
    # 计算消息的哈希值
    h_list = [hashlib.sha256(msg).digest() for msg in msg_list]

    # 选择随机数k_list
    k_list = [random.randint(1, n-1) for _ in sk_list]

    # 计算椭圆曲线点R_list = [k*G for k in k_list]
    R_list = [calculate_np(xG, yG, k, a, b, p) for k in k_list]

    # 计算s_list = [k + x*h for k, x, h in zip(k_list, sk_list, h_list)]
    s_list = [(k + int.from_bytes(h, 'big') * x) % n for k, x, h in zip(k_list, sk_list, h_list)]


    s_sum=(sum(i for i in s_list)) % n

    tem=calculate_p_q(R_list[0][0], R_list[0][1], R_list[1][0], R_list[1][1], a, b, p)
    R_sum=calculate_p_q(tem[0],tem[1],R_list[2][0],R_list[2][1],a,b,p)

    return s_sum,R_sum

# 定义验证函数
def schnorr_batch_verify(msg_list, sig, vk_list):
    # 解析签名(r_list, s_list)
    s_sum,R_sum = sig

    # 计算消息的哈希值
    h_list = [hashlib.sha256(msg).digest() for msg in msg_list]

    tem=calculate_p_q(vk_list[0][0], vk_list[0][1], vk_list[1][0], vk_list[1][1], a, b, p)
    P_sum=calculate_p_q(tem[0],tem[1],vk_list[2][0],vk_list[2][1],a,b,p)

    sG=calculate_np(xG, yG, s_sum, a, b, p)

    h_=int.from_bytes(h_list[0], 'big')

    hP=calculate_np(P_sum[0], P_sum[1], h_, a, b, p)

    print(sG)

    print(calculate_p_q(R_sum[0],R_sum[1],hP[0],hP[1],a,b,p))

    if(sG==calculate_p_q(R_sum[0],R_sum[1],hP[0],hP[1],a,b,p)):
        print("验证成功！")

msg_list = [b'message', b'message', b'message']
sk_list = [random.randint(1, n - 1) for _ in msg_list]
vk_list = [calculate_np(xG, yG, sk, a, b, p) for sk in sk_list]
sig = schnorr_batch_sign(msg_list, sk_list)
time_start_1 = time.time()
schnorr_batch_verify(msg_list, sig, vk_list)
time_end_1 = time.time()
print("运行时间："+str(time_end_1 - time_start_1)+"秒")