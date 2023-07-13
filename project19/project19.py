import math
import random
import time


def ExEucl(a, m):
    if gcd(a, m) != 1 and gcd(a, m) != -1:
        return None
    u1, u2, u3 = 1, 0, a
    v1, v2, v3 = 0, 1, m
    while v3 != 0:
        q = u3 // v3
        v1, v2, v3, u1, u2, u3 = (u1 - q * v1), (u2 - q * v2), (u3 - q * v3), v1, v2, v3
    if u1 > 0:
        return u1 % m
    else:
        return (u1 + m) % m



def gcd(m, n):
    while m != 0:
        m, n = n % m, m
    return n


def add(m, n):
    if (m == 0):
        return n
    if (n == 0):
        return m
    tp = []
    if (m != n):
        if (gcd(m[0] - n[0], p) != 1 and gcd(m[0] - n[0], p) != -1):
            return 0
        else:
            k = ((m[1] - n[1]) * ExEucl(m[0] - n[0], p)) % p
    else:
        k = ((3 * (m[0] ** 2) + a) * ExEucl(2 * m[1], p)) % p
    x = (k ** 2 - m[0] - n[0]) % p
    y = (k * (m[0] - x) - m[1]) % p
    tp.append(x)
    tp.append(y)
    return tp




def mul(n, l):
    if n == 0:
        return 0
    if n == 1:
        return l
    t = l
    while (n >= 2):
        t = add(t, l)
        n = n - 1
    return t


def sign(m, n, G, d,k):
    e = hash(m)
    R = mul(k, G)
    r = R[0] % n
    s = (ExEucl(k, n) * (e + d * r)) % n
    return r, s


def verify(m, n, G, r, s, P):
    e = hash(m)
    w = ExEucl(s, n)
    v1 = (e * w) % n
    v2 = (r * w) % n
    w = add(mul(v1, G), mul(v2, P))
    if (w == 0):
        print('false')
        return False
    else:
        if (w[0] % n == r):
            print('valid signature!!!')
            return True
        else:
            print('invalid signature!!!')
            return False




#初始化
a = 3
b = 4
p = 7
m = 'love'
G = [5, 1]
n = 19
k = 2
d = 5
P = mul(d, G)


start = time.time()


print("if check with ECDSA")
r,s=sign(m,n,G,d,k)
print("signature is:",r,s)
m=verify(m,n,G,r,s,P)
print("result is：",m)


end = time.time()
print('程序用时:',end - start,'s')
