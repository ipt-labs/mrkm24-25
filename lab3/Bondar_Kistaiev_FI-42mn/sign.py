import optimus
from Crypto.Hash import SHA256
import math

class Zq:
    def __init__(self, val: int, q: int):
        self.val = val % q
        self.q = q

    @staticmethod
    def rand_ring(n):
        q, orders = optimus.generate_prime_maurer(n)
        # WRONG GENERATOR
        g = optimus.rand_int(n)
        while not optimus.check_generator(g, q, orders):
            g = optimus.rand_int(n)

        return Zq(g, q)

    @staticmethod
    def rand_element(q):
        e = optimus.rand_int(q.bit_length())
        while e >= q:
           e = optimus.rand_int(q.bit_length())

        return Zq(e, q)
    
    def __add__(self, other):
        if not (self.q == other.q):
            raise RuntimeError("Cannot add elements from different rings")

        val = (self.val + other.val) % self.q
        return Zq(val, self.q)

    def __mul__(self, other):
        if not (self.q == other.q):
            raise RuntimeError("Cannot multiply elements from different rings")

        val = (self.val * other.val) % self.q
        return Zq(val, self.q)
    
    def __pow__(self, other):
        if isinstance(other, Zq):
            exp = other.val
        else:
            exp = other

        if not isinstance(exp, int):
            raise RuntimeError("Incorrect exponent type")

        val = pow(self.val, exp, self.q)

        return Zq(val, self.q)
    
    def __eq__(self, other):
        if not isinstance(other, Zq):
            other_val = other
        else:
            other_val = other.val

        return self.val == other_val

    def to_bytes(self):
        return self.val.to_bytes(int(math.ceil(self.q.bit_length() / 8)))

    def __str__(self):
        return str(self.val)


def H(d: Zq, m: bytes):
    return Zq(int.from_bytes(SHA256.new(data=(d.to_bytes() + m)).digest()), d.q - 1)


class SchnorSigner:
    def __init__(self, n):
        g = Zq.rand_ring(n)
        self.g = g

        x = Zq.rand_element(g.q - 1)
        h = g ** x

        self.x = x
        self.h = h
        

    def blind_sign_first(self):
        r = Zq.rand_element(self.g.q - 1)
        self.r = r

        return self.g, self.h, self.g ** r
    
    def blind_sign_thrid(self, c: Zq):
        return self.r + c*self.x
    


class SchnorUser:
    def __init__(self, m: bytes):
        self.m = m

    def blind_sign_second(self, ring, h, gr):
        a = Zq.rand_element(ring.q - 1)
        b = Zq.rand_element(ring.q - 1)
        self.a = a
        self.b = b

        self.c = H(gr * (ring**a) * (h**b), self.m)

        return self.c + b


    
    def blind_sign_finalize(self, z):
        sign = (self.c, z + self.a)

        return self.m, sign

        

def schnor_check_sign(m, sign, h, g):
    c, z = sign
    return H((g**z) * (h**(-c.val)), m) == c 

