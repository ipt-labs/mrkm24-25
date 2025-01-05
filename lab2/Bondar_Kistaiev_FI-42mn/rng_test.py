import time
import pandas as pd
from optimus import *

trash = []
param_vals = [bl for bl in range(128, 2049, 128)]
tN = dict(zip(param_vals, [500 for _ in range(128, 2049, 128)]))

data = pd.DataFrame(columns=["gen", "length", "time"])


print("SHA256_DRBG gen")
for param, N in tN.items():
    print(f'N length = {param}')
    rng1 = SHA256_DRBG(random.randint(123, 12432543))

    start = time.time()
    for i in range(N):
        # sol = random.randint(123, 12432543)
        rn1 = rng1.generate_bytes(param, 2567)
        trash.append(rn1)
    end = time.time()

    new_entry = pd.DataFrame([("SHA256_DRBG", param, end - start)],
                                 columns=["gen", "length", "time"])
    data = pd.concat([data, new_entry], ignore_index=True)

print("\nBBS gen")
for param, N in tN.items():
    print(f'N length = {param}')
    rng2 = BBS()

    start = time.time()
    for i in range(N):
        # sol = random.randint(123, 12432543)
        rn2 = rng2.generate_bytes(param)
        trash.append(rn2)
    end = time.time()
    
    new_entry = pd.DataFrame([("BBS", param, end - start)],
                                 columns=["gen", "length", "time"])
    data = pd.concat([data, new_entry], ignore_index=True)

data.to_csv('save_rng.csv', index=False)  
