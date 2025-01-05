import time
import pandas as pd
from optimus import *

trash = []
param_vals = [128, 256, 512, 768, 1024, 1536, 2048]
tN = dict(zip(param_vals, [100, 100, 100, 75, 75, 50, 50]))

data = pd.DataFrame(columns=["gen", "length", "time"])


print("Miller-Rabin gen")
for param, N in tN.items():
    print(f'N length = {param}')
    for i in range(N):
        start = time.time()
        p1 = generate_prime_MR(param)
        end = time.time()

        trash.append(p1)
        new_entry = pd.DataFrame([("Miller-Rabin", param, end - start)],
                                 columns=["gen", "length", "time"])
        data = pd.concat([data, new_entry], ignore_index=True)


print("\nMaurer gen")
for param, N in tN.items():
    print(f'N length = {param}')
    for i in range(N):
        start = time.time()
        p2 = generate_prime_maurer(param)
        end = time.time()

        trash.append(p2)
        new_entry = pd.DataFrame([("Maurer", param, end - start)],
                                 columns=["gen", "length", "time"])
        data = pd.concat([data, new_entry], ignore_index=True)


data.to_csv('save.csv', index=False)  
