package org.mrkm;

import java.math.BigInteger;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.util.ArrayList;
import java.util.List;

public class PrimeGenerator {
    private static final SecureRandom RANDOM;

    static {
        try {
            RANDOM = SecureRandom.getInstanceStrong();
        } catch (NoSuchAlgorithmException e) {
            throw new RuntimeException(e);
        }
    }

    public static void measureAverageExecutionTime(final Runnable methodToRun) {
        long totalDurationInNano = 0;
        final long startTime = System.nanoTime();

        methodToRun.run();

        final long endTime = System.nanoTime();
        totalDurationInNano += (endTime - startTime);

        long avgDurationInMillis = totalDurationInNano / 1_000_000;

        System.out.println("Average Execution time (ms): " + avgDurationInMillis);
    }

    private static List<Long> generatePrimes(final int limit) {
        boolean[] isPrime = new boolean[limit + 1];
        List<Long> primes = new ArrayList<>();

        for (int i = 2; i <= limit; i++) {
            isPrime[i] = true;
        }

        for (int i = 2; i * i <= limit; i++) {
            if (isPrime[i]) {
                for (int j = i * i; j <= limit; j += i) {
                    isPrime[j] = false;
                }
            }
        }

        for (int i = 2; i <= limit; i++) {
            if (isPrime[i]) {
                primes.add((long) i);
            }
        }

        primes.removeFirst();

        return primes;
    }

    public static BigInteger generateRandomBigInteger(final int bitLength) {
        BigInteger randomBigInt = new BigInteger(bitLength - 1, RANDOM);
        randomBigInt = randomBigInt.setBit(bitLength - 1);
        return randomBigInt;
    }

    public static BigInteger generatePrime(final int bitLength, final List<Long> divisors) {
        BigInteger prime;

        while (true) {
            prime = generateRandomBigInteger(bitLength);

            boolean divisible = false;
            for (Long divisor : divisors) {
                if (prime.mod(BigInteger.valueOf(divisor)).equals(BigInteger.ZERO)) {
                    divisible = true;
                    break;
                }
            }

            if (divisible) {
                continue;
            }

            break;
        }

        return prime;
    }

    private static void metrics() {
        final List<Long> divisors = generatePrimes(10);
        final PrimeTest test = new PrimeTest();

        final List<Integer> counter1 = new ArrayList<>();
        final List<Integer> counter2 = new ArrayList<>();

        int iteration = 5;
        int iterationTry = 10_000;
        int bitLength = 128;

        System.out.println(divisors.size());

        measureAverageExecutionTime(() -> {
            for (int j = 0; j < iteration; j++) {
                int count = 0;

                for (int i = 0; i < iterationTry; i++) {
                    if (test.isPrime(generatePrime(bitLength, divisors))) {
                        count++;
                    }
                }
                counter1.add(count);
            }
        });

        measureAverageExecutionTime(() -> {
            for (int j = 0; j < iteration; j++) {
                int count = 0;

                for (int i = 0; i < iterationTry; i++) {
                    if (test.isPrime(generateRandomBigInteger(bitLength))) {
                        count++;
                    }
                }
                counter2.add(count);
            }
        });

        double average1 = counter1.stream()
                .mapToInt(Integer::intValue)
                .average()
                .orElse(0.0);

        double average2 = counter2.stream()
                .mapToInt(Integer::intValue)
                .average()
                .orElse(0.0);

        System.out.println(average1 + " " + counter1);
        System.out.println(average2 + " " + counter2);
    }

    public static void main(String[] args) {
        metrics();
    }
}