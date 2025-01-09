package org.mrkm.classic;

import java.security.*;
import java.security.spec.ECGenParameterSpec;
import java.util.Base64;

public class ECDSASignature {

    public static KeyPair generateKeyPair() throws Exception {
        KeyPairGenerator keyPairGenerator = KeyPairGenerator.getInstance("EC");
        ECGenParameterSpec ecSpec = new ECGenParameterSpec("secp256r1");
        keyPairGenerator.initialize(ecSpec, new SecureRandom());
        return keyPairGenerator.generateKeyPair();
    }

    public static String signMessage(String message, PrivateKey privateKey) throws Exception {
        Signature signature = Signature.getInstance("SHA256withECDSA");
        signature.initSign(privateKey);
        signature.update(message.getBytes());
        byte[] signedMessage = signature.sign();
        return Base64.getEncoder().encodeToString(signedMessage);
    }

    public static boolean verifySignature(String message, String signedMessage, PublicKey publicKey) throws Exception {
        Signature signature = Signature.getInstance("SHA256withECDSA");
        signature.initVerify(publicKey);
        signature.update(message.getBytes());
        byte[] signedBytes = Base64.getDecoder().decode(signedMessage);
        return signature.verify(signedBytes);
    }

    public static void main(String[] args) throws Exception {
        KeyPair keyPair = generateKeyPair();
        PrivateKey privateKey = keyPair.getPrivate();
        PublicKey publicKey = keyPair.getPublic();

        String message = "This is a secret message.";

        String signedMessage = signMessage(message, privateKey);
        System.out.println("Signed Message: " + signedMessage);

        boolean isVerified = verifySignature(message, signedMessage, publicKey);
        System.out.println("Signature Verified: " + isVerified);
    }
}
