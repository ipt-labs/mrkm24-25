package org.mrkm;

import java.math.BigInteger;
import java.security.SecureRandom;

public class PrimeTest {
    private static final BigInteger FOUR = BigInteger.valueOf(4);
    private static final BigInteger EIGHT = BigInteger.valueOf(8);

    private final SecureRandom random = new SecureRandom();

    public boolean isPrime(final BigInteger p) {
        if (p.mod(BigInteger.TWO).equals(BigInteger.ZERO)) {
            return false;
        }

        for (int k = 0; k < 10; k++) {
            BigInteger x = new BigInteger(p.bitLength(), random).mod(p);

            if (x.equals(BigInteger.ZERO)) {
                x = BigInteger.ONE;
            }

            if (x.equals(p)) {
                x = p.subtract(BigInteger.ONE);
            }

            if (gcd(p, x).compareTo(BigInteger.ONE) > 0) {
                return false;
            }

            BigInteger jacobiRes = jacobi(x, p);
            if (jacobiRes.compareTo(BigInteger.ZERO) < 0) {
                jacobiRes = p.subtract(BigInteger.ONE);
            }

            if (!jacobiRes.equals(x.modPow(p.subtract(BigInteger.ONE).divide(BigInteger.TWO), p))) {
                return false;
            }
        }

        return true;
    }

    public BigInteger jacobi(BigInteger a, BigInteger n) {
        if (a.compareTo(BigInteger.ZERO) < 0 || n.mod(BigInteger.TWO).equals(BigInteger.ZERO)) {
            throw new IllegalArgumentException("Invalid arguments: (a, n) -- (" + a + ", " + n + ")");
        }

        if (a.equals(BigInteger.ZERO)) {
            return BigInteger.ZERO;
        }

        if (a.equals(BigInteger.ONE)) {
            return BigInteger.ONE;
        }

        int e = 0;
        int s = 0;

        while (a.mod(BigInteger.TWO).equals(BigInteger.ZERO)) {
            e++;
            a = a.divide(BigInteger.TWO);
        }

        final BigInteger modEight = n.mod(EIGHT);
        if (e % 2 == 0 || modEight.equals(BigInteger.ONE) || modEight.equals(BigInteger.valueOf(7))) {
            s = 1;
        } else if (modEight.equals(BigInteger.valueOf(3)) || modEight.equals(BigInteger.valueOf(5))) {
            s = -1;
        }

        if (n.mod(FOUR).equals(BigInteger.valueOf(3)) && a.mod(FOUR).equals(BigInteger.valueOf(3))) {
            s *= -1;
        }

        n = n.mod(a);

        return a.equals(BigInteger.ONE) ? BigInteger.valueOf(s) : BigInteger.valueOf(s).multiply(jacobi(n, a));
    }

    public BigInteger gcd(final BigInteger a, final BigInteger b) {
        if (b.equals(BigInteger.ZERO)) {
            return a;
        }

        return gcd(b, a.mod(b));
    }

}
