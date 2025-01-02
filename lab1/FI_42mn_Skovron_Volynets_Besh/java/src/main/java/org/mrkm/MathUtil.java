package org.mrkm;

import java.math.BigInteger;
import java.security.SecureRandom;

public class MathUtil {
    private static final SecureRandom RANDOM = new SecureRandom();
    private final int BIT_LENGTH;

    public MathUtil(int bitLength) {
        this.BIT_LENGTH = bitLength;
    }

    public BigInteger generateRandomBigInteger() {
        return new BigInteger(BIT_LENGTH, RANDOM);
    }

    public BigInteger generateRandomBigInteger(int bitLength) {
        return new BigInteger(bitLength, RANDOM);
    }

    public BigInteger add(BigInteger a, BigInteger b) {
        return a.add(b);
    }

    public BigInteger sub(BigInteger a, BigInteger b) {
        return a.subtract(b);
    }

    public BigInteger mul(BigInteger a, BigInteger b) {
        return a.multiply(b);
    }

    public BigInteger div(BigInteger a, BigInteger b) {
        return a.divide(b);
    }

    public BigInteger pow(BigInteger a, int exp) {
        return a.pow(exp);
    }

    public BigInteger powOptimized(BigInteger a, int exp) {
        BigInteger result = BigInteger.ONE;
        BigInteger base = a;

        while (exp > 0) {
            if ((exp & 1) == 1) {
                result = result.multiply(base);
            }
            base = base.multiply(base);
            exp >>= 1;
        }
        return result;
    }

    public BigInteger powOptimized(BigInteger base, BigInteger exp) {
        BigInteger result = BigInteger.ONE;
        BigInteger currentBase = base;

        while (exp.compareTo(BigInteger.ZERO) > 0) {
            if (exp.and(BigInteger.ONE).equals(BigInteger.ONE)) {
                result = result.multiply(currentBase);
            }
            currentBase = currentBase.multiply(currentBase);
            exp = exp.shiftRight(1);
        }
        return result;
    }

    public BigInteger addMod(BigInteger a, BigInteger b, BigInteger modulus) {
        return add(a, b).mod(modulus);
    }

    public BigInteger subMod(BigInteger a, BigInteger b, BigInteger modulus) {
        return sub(a, b).mod(modulus);
    }

    public BigInteger mulMod(BigInteger a, BigInteger b, BigInteger modulus) {
        return mul(a, b).mod(modulus);
    }

    public BigInteger divMod(BigInteger a, BigInteger b, BigInteger modulus) {
        return div(a, b).mod(modulus);
    }

    public BigInteger powMod(BigInteger a, int exp, BigInteger modulus) {
        return pow(a, exp).mod(modulus);
    }

    public BigInteger modPow(BigInteger a, int exp, BigInteger modulus) {
        return a.modPow(BigInteger.valueOf(exp), modulus);
    }

    public BigInteger powModOptimized(BigInteger a, int exp, BigInteger modulus) {
        BigInteger result = BigInteger.ONE;
        BigInteger base = a;

        while (exp > 0) {
            if ((exp & 1) == 1) {
                result = result.multiply(base).mod(modulus);
            }
            base = base.multiply(base);
            exp >>= 1;
        }
        return result;
    }

    public BigInteger powModOptimized(BigInteger base, BigInteger exp, BigInteger modulus) {
        BigInteger result = BigInteger.ONE;
        BigInteger currentBase = base;

        while (exp.compareTo(BigInteger.ZERO) > 0) {
            if (exp.and(BigInteger.ONE).equals(BigInteger.ONE)) {
                result = result.multiply(currentBase).mod(modulus);
            }
            currentBase = currentBase.multiply(currentBase);
            exp = exp.shiftRight(1);
        }
        return result;
    }

}
