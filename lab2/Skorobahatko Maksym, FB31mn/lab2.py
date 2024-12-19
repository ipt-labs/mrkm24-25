import random
import math


def getSieve(limit):
    # Generate a list of prime numbers up to a given limit using the Sieve of Eratosthenes
    sieve = [True] * (limit + 1)
    sieve[0] = sieve[1] = False  # 0 and 1 
    for start in range(2, int(math.sqrt(limit)) + 1):
        if sieve[start]:
            for multiple in range(start * start, limit + 1, start):
                sieve[multiple] = False
    return [num for num, is_prime in enumerate(sieve) if is_prime]


def is_probably_prime(n, primes):
    #Check if a number is probably prime using trial division with known primes
    if n < 2:
        return False
    for prime in primes:
        if prime * prime > n:
            break
        if n % prime == 0:
            return False
    return True


def generatePrime(n: int, primes=None, s=None):
    #Generate a large prime number with n digits
    up_limit = 10**n
    if not primes:
        primes = getSieve(1000)
    if not s:
        s = primes[-1]
   
    while s < up_limit:
        lo, hi = (s + 1) >> 1, (s << 1) + 1


        while True:
            r = random.randint(lo, hi) << 1
            candidate = s * r + 1
            if not is_probably_prime(candidate, primes):
                continue
           
            while True:
                a = random.randint(2, candidate - 1)
                if pow(a, candidate - 1, candidate) != 1:
                    break


                d = math.gcd((pow(a, r, candidate) - 1) % candidate, candidate)
                if d != candidate:
                    if d == 1:
                        s = candidate
                    break
            if s == candidate:
                break
    return s


def main():
    n = int(input("Enter the number of digits for the prime number: "))
    prime_number = generatePrime(n)
    print(f"Generated prime number with {n} digits: {prime_number}")


if __name__ == "__main__":
    main()