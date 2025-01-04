package org.mrkm;

import java.util.Scanner;

public class CryptoTerminal {

    private static String currentUser = null;

    public static void main(String[] args) {
        final Scanner scanner = new Scanner(System.in);

        boolean running = true;

        while (running) {
            if (currentUser == null) {
                System.out.println("--- Криптографічний Термінал ---");
                System.out.print("Введіть ваше ім'я користувача: ");
                currentUser = scanner.nextLine();

                if (!CryptoUtils.getUserPasswords().containsKey(currentUser)) {
                    System.out.println("Перше введення пароля для користувача " + currentUser);
                    System.out.print("Введіть новий пароль: ");
                    String password = scanner.nextLine();
                    System.out.print("Повторіть пароль: ");
                    String confirmPassword = scanner.nextLine();

                    if (!password.equals(confirmPassword)) {
                        System.out.println("Паролі не співпадають! Спробуйте знову.");
                        continue;
                    }

                    CryptoUtils.setPassword(currentUser, password);
                    System.out.println("Пароль встановлено успішно.");

                    try {
                        CryptoUtils.initKey(currentUser, 128);
                        System.out.println("Ключ для AES 128 біт успішно згенеровано за замовчуванням.");
                    } catch (Exception e) {
                        System.out.println("Помилка при ініціалізації ключа: " + e.getMessage());
                    }
                } else {
                    System.out.print("Введіть пароль: ");
                    String password = scanner.nextLine();

                    if (!CryptoUtils.checkPassword(currentUser, password)) {
                        System.out.println("Невірний пароль! Спробуйте знову.");
                        continue;
                    } else {
                        System.out.println("Вхід успішний.");
                    }
                }
            }

            System.out.println("\n--- Криптографічний Термінал ---");
            System.out.println("Вхід як: " + currentUser);
            System.out.println("1. Шифрувати текст");
            System.out.println("2. Дешифрувати текст");
            System.out.println("3. Показати ключ шифрування (Base64)");
            System.out.println("4. Налаштування AES шифрування (розмір ключа)");
            System.out.println("5. Вийти як користувач");
            System.out.println("6. Завершити програму");

            System.out.print("Виберіть операцію: ");
            final String choice = scanner.nextLine();

            switch (choice) {
                case "1":
                    System.out.print("Введіть текст для шифрування: ");
                    String textToEncrypt = scanner.nextLine();
                    try {
                        String encryptedText = CryptoUtils.encrypt(currentUser, textToEncrypt);
                        System.out.println("Зашифрований текст: " + encryptedText);
                    } catch (Exception e) {
                        System.out.println("Помилка при шифруванні: " + e.getMessage());
                    }
                    break;

                case "2":
                    System.out.print("Введіть зашифрований текст для дешифрування: ");
                    String textToDecrypt = scanner.nextLine();
                    try {
                        String decryptedText = CryptoUtils.decrypt(currentUser, textToDecrypt);
                        System.out.println("Розшифрований текст: " + decryptedText);
                    } catch (Exception e) {
                        System.out.println("Помилка при дешифруванні: " + e.getMessage());
                    }
                    break;

                case "3":
                    String keyBase64 = CryptoUtils.getSecretKeyBase64(currentUser);
                    if (keyBase64 != null) {
                        System.out.println("Ключ шифрування (Base64): " + keyBase64);
                    } else {
                        System.out.println("Ключ не знайдений!");
                    }
                    break;

                case "4":
                    System.out.println("Вибір розміру ключа для AES:");
                    System.out.println("1. 128 біт");
                    System.out.println("2. 192 біт");
                    System.out.println("3. 256 біт");
                    System.out.print("Виберіть розмір ключа: ");
                    String keySizeChoice = scanner.nextLine();

                    try {
                        int keySize = 128;
                        switch (keySizeChoice) {
                            case "1": keySize = 128; break;
                            case "2": keySize = 192; break;
                            case "3": keySize = 256; break;
                            default: System.out.println("Невірний вибір, використано 128 біт."); break;
                        }

                        CryptoUtils.initKey(currentUser, keySize);
                        System.out.println("Ключ для AES " + keySize + " біт успішно згенеровано.");
                    } catch (Exception e) {
                        System.out.println("Помилка при налаштуванні ключа: " + e.getMessage());
                    }
                    break;

                case "5":
                    System.out.println("Вихід як користувач " + currentUser);
                    currentUser = null;
                    break;

                case "6":
                    running = false;
                    System.out.println("Завершення програми...");
                    break;

                default:
                    System.out.println("Невірний вибір. Спробуйте ще раз.");
            }
        }

        scanner.close();
    }
}
