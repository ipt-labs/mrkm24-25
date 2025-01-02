package org.mrkm.classic;

import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.SecureRandom;

public class Schnorre {
    private static final SecureRandom RANDOM = new SecureRandom();

    public static BigInteger hash(BigInteger num1, BigInteger num2, BigInteger num3) {
        try {
            byte[] bytes1 = num1.toByteArray();
            byte[] bytes2 = num2.toByteArray();
            byte[] bytes3 = num3.toByteArray();

            byte[] combinedBytes = new byte[bytes1.length + bytes2.length + bytes3.length];
            System.arraycopy(bytes1, 0, combinedBytes, 0, bytes1.length);
            System.arraycopy(bytes2, 0, combinedBytes, bytes1.length, bytes2.length);
            System.arraycopy(bytes3, 0, combinedBytes, bytes1.length + bytes2.length, bytes3.length);

            MessageDigest sha256 = MessageDigest.getInstance("SHA-256");
            byte[] hashBytes = sha256.digest(combinedBytes);

            return new BigInteger(1, hashBytes);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public static void main(String[] args) {
        BigInteger n = new BigInteger(130, 100, RANDOM);

        BigInteger g = BigInteger.TWO;

        BigInteger sk = new BigInteger(130, 100, RANDOM);
        BigInteger pk = sk.multiply(g).mod(n);

        BigInteger r = new BigInteger(130, 100, RANDOM);
        BigInteger R = r.multiply(g).mod(n);

        BigInteger msg = BigInteger.valueOf(123);

        BigInteger e = hash(msg, R, pk);

        BigInteger s = r.add(sk.multiply(e)).mod(n);

        // verify
        boolean equals = s.multiply(g).equals(R.add(pk.multiply(e)).mod(n));
        System.out.println(equals);
    }
}
