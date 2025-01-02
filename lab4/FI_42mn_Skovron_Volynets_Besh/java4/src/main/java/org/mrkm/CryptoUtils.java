package org.mrkm;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Base64;
import java.util.HashMap;
import java.util.Map;

public class CryptoUtils {

    private static final Map<String, String> USER_PASSWORDS = new HashMap<>();
    private static final Map<String, SecretKey> USER_KEYS = new HashMap<>();

    public static Map<String, String> getUserPasswords() {
        return USER_PASSWORDS;
    }

    public static void setPassword(final String username, final String password) {
        try {
            final MessageDigest digest = MessageDigest.getInstance("SHA-256");
            final byte[] hash = digest.digest(password.getBytes());
            final String passwordHash = Base64.getEncoder().encodeToString(hash);

            USER_PASSWORDS.put(username, passwordHash);
        } catch (NoSuchAlgorithmException _) { }
    }

    public static boolean checkPassword(final String username, final String password) {
        try {
            final MessageDigest digest = MessageDigest.getInstance("SHA-256");
            final byte[] hash = digest.digest(password.getBytes());
            final String passwordHash = Base64.getEncoder().encodeToString(hash);

            return passwordHash.equals(USER_PASSWORDS.get(username));
        } catch (NoSuchAlgorithmException e) {
            return false;
        }
    }

    public static void initKey(final String username, final int keySize) throws Exception {
        try {
            final KeyGenerator keyGenerator = KeyGenerator.getInstance("AES");
            keyGenerator.init(keySize);

            final SecretKey secretKey = keyGenerator.generateKey();
            USER_KEYS.put(username, secretKey);
        } catch (NoSuchAlgorithmException e) {
            throw new Exception("Алгоритм AES не підтримується в цьому середовищі: " + e.getMessage());
        }
    }

    public static String encrypt(final String username, final String plainText) throws Exception {
        try {
            final SecretKey secretKey = USER_KEYS.get(username);

            if (secretKey == null) {
                throw new Exception("Ключ користувача не знайдено!");
            }

            final Cipher cipher = Cipher.getInstance("AES");
            cipher.init(Cipher.ENCRYPT_MODE, secretKey);

            final byte[] encryptedBytes = cipher.doFinal(plainText.getBytes());

            return Base64.getEncoder().encodeToString(encryptedBytes);
        } catch (Exception e) {
            throw new Exception("Помилка при шифруванні: " + e.getMessage());
        }
    }

    public static String decrypt(final String username, final String encryptedText) throws Exception {
        try {
            final SecretKey secretKey = USER_KEYS.get(username);

            if (secretKey == null) {
                throw new Exception("Ключ користувача не знайдено!");
            }

            final Cipher cipher = Cipher.getInstance("AES");
            cipher.init(Cipher.DECRYPT_MODE, secretKey);

            final byte[] decodedBytes = Base64.getDecoder().decode(encryptedText);
            final byte[] decryptedBytes = cipher.doFinal(decodedBytes);

            return new String(decryptedBytes);
        } catch (Exception e) {
            throw new Exception("Помилка при дешифруванні: " + e.getMessage());
        }
    }

    public static String getSecretKeyBase64(final String username) {
        final SecretKey secretKey = USER_KEYS.get(username);

        if (secretKey != null) {
            return Base64.getEncoder().encodeToString(secretKey.getEncoded());
        }

        return null;
    }
}
