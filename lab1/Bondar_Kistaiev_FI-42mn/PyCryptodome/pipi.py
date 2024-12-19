import numpy as np
import time
import timeit
from Crypto.Random import get_random_bytes
from Crypto.Hash import SHA256


def precision(n):
    if n < 10e-8:
        return 0
    else:
        p = 1 + np.ceil(np.log10((1 / n) + 1))
        return np.around(n, int(p))
    
    
def print_stats(params, data, title = ""):
    print('----------------------')
    print(title + '\n')

    for p in params:
        print(f'mean for {p}:  \t{precision(np.mean(data[p]))} sec')
        print(f'median for {p}:\t{precision(np.median(data[p]))} sec')
        print(f'std for {p}:   \t{precision(np.std(data[p]))} sec')

        print('\n')

    print('----------------------')




# ---------------------------- ASYM ------------------------------------
from Crypto.PublicKey import RSA
from Crypto.PublicKey import DSA

from Crypto.Signature import pkcs1_15
from Crypto.Signature import DSS

# ___RSA___

trash = []
param_vals = [1024, 2048, 4096]
tN = dict(zip(param_vals, [200, 75, 25]))

times_gen = {p: [] for p in param_vals}
times_sign = {p: [] for p in param_vals}
times_verif = {p: [] for p in param_vals}

for param, N in tN.items():
    for i in range(N):
        start = time.time()
        key = RSA.generate(param)
        end = time.time()

        times_gen[param].append(end - start)

        txt = get_random_bytes(param // 8)
        h = SHA256.new(txt)

        t2 = timeit.timeit(lambda: pkcs1_15.new(key).sign(h), number=1)
        times_sign[param].append(t2)

        signature = pkcs1_15.new(key).sign(h)

        t3 = timeit.timeit(lambda: pkcs1_15.new(key).verify(h, signature), number=1)
        times_verif[param].append(t3)


print_stats(param_vals, times_gen, "RSA_keygen")
print_stats(param_vals, times_sign, "RSA_sign")
print_stats(param_vals, times_verif, "RSA_sign_verif")


# ___DSA___

trash = []
param_vals = [1024, 2048, 3072]
tN = dict(zip(param_vals, [150, 75, 20]))

times_gen = {p: [] for p in param_vals}
times_sign = {p: [] for p in param_vals}
times_verif = {p: [] for p in param_vals}

for param, N in tN.items():
    for i in range(N):
        start = time.time()
        key = DSA.generate(param)
        end = time.time()
        print(param, N)
        times_gen[param].append(end - start)

        txt = get_random_bytes(param // 8)
        h = SHA256.new(txt)

        t2 = timeit.timeit(lambda: DSS.new(key, 'fips-186-3').sign(h), number=1)
        times_sign[param].append(t2)

        signature = DSS.new(key, 'fips-186-3').sign(h)

        t3 = timeit.timeit(lambda: DSS.new(key, 'fips-186-3').verify(h, signature), number=1)
        times_verif[param].append(t3)


print_stats(param_vals, times_gen, "DSA_keygen")
print_stats(param_vals, times_sign, "DSA_sign")
print_stats(param_vals, times_verif, "DSA_sign_verif")




# # ---------------------------- HASH ------------------------------------
from Crypto.Hash import SHA256
from Crypto.Hash import SHA3_512


# ___SHA256___

trash = []
param_vals = [10*2**20]
tN = dict(zip(param_vals, [2500]))

times = {p: [] for p in param_vals}

for param, N in tN.items():
    for i in range(N):
        text = get_random_bytes(param)

        start = time.time()
        hash_object = SHA256.new(data=text)
        trash.append(hash_object.digest())
        end = time.time()

        
        times[param].append(end - start)

print_stats(param_vals, times, "SHA256_10MB")


# ___SHA3-512___

trash = []
param_vals = [10*2**20]
tN = dict(zip(param_vals, [2500]))

times = {p: [] for p in param_vals}

for param, N in tN.items():
    for i in range(N):
        text = get_random_bytes(param)

        start = time.time()
        hash_object = SHA3_512.new(data=text)
        trash.append(hash_object.digest())
        end = time.time()

        
        times[param].append(end - start)

print_stats(param_vals, times, "SHA3-512_10MB")




# ---------------------------- SYM ------------------------------------
from Crypto.Cipher import ChaCha20
from Crypto.Cipher import AES


# ___ChaCha20___

trash = [1]
param_vals = [8]
tN = dict(zip(param_vals, [5000]))

times = {p: [] for p in param_vals}

for param, N in tN.items():
    for i in range(N):
        text = get_random_bytes(10*2**20)
        key = get_random_bytes(32)
        nonce = get_random_bytes(param)
        start = time.time()
        cipher = ChaCha20.new(key=key, nonce=nonce)
        trash[0] = cipher.encrypt(text)
        end = time.time()

        times[param].append(end - start)

print_stats(param_vals, times, "ChaCha20_10MB")


# ___AES___

trash = [1]
param_vals = [128, 192, 256]
tN = dict(zip(param_vals, [2500, 2500, 2500]))


times = {p: [] for p in param_vals}

for param, N in tN.items():
    for i in range(N):
        text = get_random_bytes(10*2**20)
        key = get_random_bytes(param // 8)
        start = time.time()
        cipher = AES.new(key, AES.MODE_CBC)
        trash[0] = cipher.encrypt(text)
        end = time.time()
        
        times[param].append(end - start)

print_stats(param_vals, times, "AES_10MB")

