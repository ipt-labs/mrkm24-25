package org.mrkm.classic;

import javax.crypto.Cipher;
import java.security.*;
import java.math.BigInteger;

public class RSASignature {

    public static KeyPair generateKeyPair() throws NoSuchAlgorithmException {
        KeyPairGenerator keyGen = KeyPairGenerator.getInstance("RSA");
        keyGen.initialize(2048);
        return keyGen.generateKeyPair();
    }

    public static BigInteger sign(String message, PrivateKey privateKey) throws Exception {
        byte[] messageBytes = message.getBytes();
        MessageDigest digest = MessageDigest.getInstance("SHA-256");
        byte[] hash = digest.digest(messageBytes);
        BigInteger messageHash = new BigInteger(1, hash);
        Cipher cipher = Cipher.getInstance("RSA");
        cipher.init(Cipher.ENCRYPT_MODE, privateKey);
        return new BigInteger(cipher.doFinal(messageHash.toByteArray()));
    }

    public static boolean verify(String message, BigInteger signature, PublicKey publicKey) throws Exception {
        byte[] messageBytes = message.getBytes();
        MessageDigest digest = MessageDigest.getInstance("SHA-256");
        byte[] hash = digest.digest(messageBytes);
        BigInteger messageHash = new BigInteger(1, hash);
        Cipher cipher = Cipher.getInstance("RSA");
        cipher.init(Cipher.DECRYPT_MODE, publicKey);
        byte[] decryptedSignature = cipher.doFinal(signature.toByteArray());
        BigInteger decryptedHash = new BigInteger(1, decryptedSignature);
        return messageHash.equals(decryptedHash);
    }

    public static void main(String[] args) throws Exception {
        KeyPair keyPair = generateKeyPair();
        PrivateKey privateKey = keyPair.getPrivate();
        PublicKey publicKey = keyPair.getPublic();

        String message = "Hello, RSA!";

        BigInteger signature = sign(message, privateKey);
        System.out.println("Signature: " + signature);

        boolean isValid = verify(message, signature, publicKey);
        System.out.println("Is the signature valid? " + isValid);
    }
}
