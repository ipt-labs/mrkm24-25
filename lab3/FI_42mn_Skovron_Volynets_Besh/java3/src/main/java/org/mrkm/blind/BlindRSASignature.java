package org.mrkm.blind;

import java.math.BigInteger;
import java.security.SecureRandom;

public class BlindRSASignature {

    private static final SecureRandom RANDOM = new SecureRandom();

    public static void main(String[] args) {
        // Alice generates the RSA key pair (public key e, n and private key d)
        BigInteger[] keys = generateRSAKeys(256);
        BigInteger n = keys[0];  // Modulus, public and private key use n
        BigInteger e = keys[1];  // Public exponent (Alice will share this with Bob)
        BigInteger d = keys[2];  // Private exponent (Alice will keep this secret)

        // Alice generates a random blinding factor r
        BigInteger r = generateBlindingFactor(n);

        // Alice's message to sign
        BigInteger msg = BigInteger.valueOf(123);

        // Alice blinds the message and sends it to Bob for signing
        BigInteger blindedMsg = blindMessage(msg, r, e, n);
        System.out.println("Blinded message to send to Bob: " + blindedMsg);

        // Bob signs the blinded message using the private key d and returns the signed message
        BigInteger signedBlindedMsg = signBlindedMessage(blindedMsg, d, n);
        System.out.println("Bob's signed blinded message: " + signedBlindedMsg);

        // Alice unblinds the signature to retrieve the actual signature
        BigInteger signature = unblindSignature(signedBlindedMsg, r, n);

        // Alice verifies the signature using her public key e
        if (verifySignature(signature, e, n, msg)) {
            System.out.println("Signature is valid!");
        } else {
            System.out.println("Signature is invalid!");
        }
    }

    private static BigInteger[] generateRSAKeys(final int bitLength) {
        final BigInteger p = new BigInteger(bitLength, 100, RANDOM);
        final BigInteger q = new BigInteger(bitLength, 100, RANDOM);
        final BigInteger n = p.multiply(q);
        BigInteger e = new BigInteger("10001", 16);
        final BigInteger phiN = (p.subtract(BigInteger.ONE)).multiply(q.subtract(BigInteger.ONE));  // Euler's Totient

        while (e.gcd(phiN).compareTo(BigInteger.ONE) != 0) {
            e = e.add(BigInteger.TWO);
        }

        final BigInteger d = e.modInverse(phiN);

        return new BigInteger[]{n, e, d};
    }

    private static BigInteger generateBlindingFactor(final BigInteger n) {
        BigInteger r;
        do {
            r = new BigInteger(256, RANDOM);
        } while (r.compareTo(BigInteger.ONE) <= 0 || r.compareTo(n) >= 0 || r.gcd(n).compareTo(BigInteger.ONE) != 0);
        System.out.println("Generated blinding factor r: " + r);
        return r;
    }

    private static BigInteger blindMessage(final BigInteger msg, final BigInteger r, final BigInteger e, final BigInteger n) {
        return msg.multiply(r.modPow(e, n)).mod(n);  // Blinded message = msg * r^e mod n
    }

    private static BigInteger signBlindedMessage(final BigInteger blindedMsg, final BigInteger d, final BigInteger n) {
        return blindedMsg.modPow(d, n);
    }

    private static BigInteger unblindSignature(final BigInteger signedBlindedMsg, final BigInteger r, final BigInteger n) {
        return signedBlindedMsg.multiply(r.modInverse(n)).mod(n);  // Unblind the signature = signedBlindedMsg * r^-1 mod n
    }

    private static boolean verifySignature(final BigInteger signature, final BigInteger e, final BigInteger n, final BigInteger msg) {
        final BigInteger leftSide = signature.modPow(e, n);
        final BigInteger rightSide = msg.mod(n);

        return leftSide.compareTo(rightSide) == 0;
    }
}
