package org.mrkm;

import java.math.BigInteger;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.util.ArrayList;
import java.util.List;

public class PrimeGeneratorTime {
    private static final SecureRandom RANDOM;
    private static int counter = 0;

    static {
        try {
            RANDOM = SecureRandom.getInstanceStrong();
        } catch (NoSuchAlgorithmException e) {
            throw new RuntimeException(e);
        }
    }

    public static long measureAverageExecutionTime(final Runnable methodToRun) {
        long totalDurationInNano = 0;
        final long startTime = System.nanoTime();

        methodToRun.run();

        final long endTime = System.nanoTime();
        totalDurationInNano += (endTime - startTime);

        return totalDurationInNano / 1_000_000;
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

        return primes;
    }

    public static BigInteger generateRandomBigInteger(final int bitLength) {
        return new BigInteger(bitLength , RANDOM);
    }

    public static BigInteger generatePrime(final int bitLength, final List<Long> divisors) {
        BigInteger prime;

        while (true) {
            counter++;
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
        final List<Long> divisors = generatePrimes(250_000);
        final PrimeTest test = new PrimeTest();

        final List<Integer> counter1 = new ArrayList<>();
        final List<Integer> counter2 = new ArrayList<>();

        int iteration = 5;
        int iterationTry = 1_000;
        int bitLength = 1024;

        System.out.println("Divisors number: " + divisors.size());

        long time1 = measureAverageExecutionTime(() -> {
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

        System.out.println("----------------");

        long time2 = measureAverageExecutionTime(() -> {
            for (int j = 0; j < iteration; j++) {
                int count = 0;

                for (int i = 0; i < (counter / iteration); i++) {
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

        System.out.println("Odd rand numbers: " + counter);

        System.out.println("Average time1 (ms): " + time1);
        System.out.println("Average time2 (ms): " + time2);

        System.out.println("Average amount1 (ms): " + average1);
        System.out.println("Average amount2 (ms): " + average2);

        System.out.println("amount1/amount2 diff: " + (average2 / average1));
        System.out.println("time1/time2     diff: " + (time2 / time1));
    }

    public static void main(String[] args) {
        metrics();
    }
}