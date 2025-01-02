package org.mrkm;

import java.math.BigInteger;


public class Main {
    private static final MathUtil mathUtil = new MathUtil(4096);

    public static void measureAverageExecutionTime(final Runnable methodToRun, int iterations) {
        long totalDurationInNano = 0;

        for (int i = 0; i < iterations; i++) {
            final long startTime = System.nanoTime();

            methodToRun.run();

            final long endTime = System.nanoTime();
            totalDurationInNano += (endTime - startTime);
        }

        long avgDurationInNano = totalDurationInNano / iterations;
        long avgDurationInMillis = avgDurationInNano / 1_000_000;

        System.out.println("Method called: " + Thread.currentThread().getStackTrace()[2].getMethodName());
        System.out.println("Average Execution time (ns): " + avgDurationInNano);
        System.out.println("Average Execution time (ms): " + avgDurationInMillis);
        System.out.println();
    }

    public static void main(String[] args) {
        addMetric();
        subMetric();
        mulMetric();
        divMetric();

        powMetric(12743);
        powAdvMetric1(12743);
        powAdvMetric2(14);

        addModMetric();
        subModMetric();
        mulModMetric();
        divModMetric();

        powModMetric1(12743);
        powModMetric2(12743);
        powModAdvMetric1(12743);
        powModAdvMetric2(14);
    }

    private static void addMetric() {
        measureAverageExecutionTime(() -> {
            BigInteger a = mathUtil.generateRandomBigInteger();
            BigInteger b = mathUtil.generateRandomBigInteger();

            mathUtil.add(a, b);
        }, 100);
    }


    private static void subMetric() {
        measureAverageExecutionTime(() -> {
            BigInteger a = mathUtil.generateRandomBigInteger();
            BigInteger b = mathUtil.generateRandomBigInteger();

            mathUtil.sub(a, b);
        }, 100);
    }

    private static void mulMetric() {
        measureAverageExecutionTime(() -> {
            BigInteger a = mathUtil.generateRandomBigInteger();
            BigInteger b = mathUtil.generateRandomBigInteger();

            mathUtil.mul(a, b);
        }, 100);
    }

    private static void divMetric() {
        measureAverageExecutionTime(() -> {
            BigInteger a = mathUtil.generateRandomBigInteger();
            BigInteger b = mathUtil.generateRandomBigInteger();

            mathUtil.div(a, b);
        }, 100);
    }

    private static void powMetric(int exponent) {
        measureAverageExecutionTime(() -> {
            BigInteger a = mathUtil.generateRandomBigInteger();

            mathUtil.pow(a, exponent);
        }, 3);
    }

    private static void powAdvMetric1(int exponent) {
        measureAverageExecutionTime(() -> {
            BigInteger a = mathUtil.generateRandomBigInteger();

            mathUtil.powOptimized(a, exponent);
        }, 3);
    }

    private static void powAdvMetric2(int bitLength) {
        measureAverageExecutionTime(() -> {
            BigInteger a = mathUtil.generateRandomBigInteger();
            BigInteger b = mathUtil.generateRandomBigInteger(bitLength);

            mathUtil.powOptimized(a, b);
        }, 3);
    }

    private static void addModMetric() {
        measureAverageExecutionTime(() -> {
            BigInteger a = mathUtil.generateRandomBigInteger();
            BigInteger b = mathUtil.generateRandomBigInteger();
            BigInteger c = mathUtil.generateRandomBigInteger();

            mathUtil.addMod(a, b, c);
        }, 100);
    }

    private static void subModMetric() {
        measureAverageExecutionTime(() -> {
            BigInteger a = mathUtil.generateRandomBigInteger();
            BigInteger b = mathUtil.generateRandomBigInteger();
            BigInteger c = mathUtil.generateRandomBigInteger();

            mathUtil.subMod(a, b, c);
        }, 100);
    }

    private static void mulModMetric() {
        measureAverageExecutionTime(() -> {
            BigInteger a = mathUtil.generateRandomBigInteger();
            BigInteger b = mathUtil.generateRandomBigInteger();
            BigInteger c = mathUtil.generateRandomBigInteger();

            mathUtil.mulMod(a, b, c);
        }, 100);
    }

    private static void divModMetric() {
        measureAverageExecutionTime(() -> {
            BigInteger a = mathUtil.generateRandomBigInteger();
            BigInteger b = mathUtil.generateRandomBigInteger();
            BigInteger c = mathUtil.generateRandomBigInteger();

            mathUtil.divMod(a, b, c);
        }, 100);
    }

    private static void powModMetric1(int exponent) {
        measureAverageExecutionTime(() -> {
            BigInteger a = mathUtil.generateRandomBigInteger();
            BigInteger c = mathUtil.generateRandomBigInteger();

            mathUtil.powMod(a, exponent, c);
        }, 3);
    }


    private static void powModMetric2(int exponent) {
        measureAverageExecutionTime(() -> {
            BigInteger a = mathUtil.generateRandomBigInteger();
            BigInteger c = mathUtil.generateRandomBigInteger();

            mathUtil.modPow(a, exponent, c);
        }, 3);
    }


    private static void powModAdvMetric1(int exponent) {
        measureAverageExecutionTime(() -> {
            BigInteger a = mathUtil.generateRandomBigInteger();
            BigInteger c = mathUtil.generateRandomBigInteger();

            mathUtil.powModOptimized(a, exponent, c);
        }, 3);
    }

    private static void powModAdvMetric2(int bitLength) {
        measureAverageExecutionTime(() -> {
            BigInteger a = mathUtil.generateRandomBigInteger();
            BigInteger b = mathUtil.generateRandomBigInteger(bitLength);
            BigInteger c = mathUtil.generateRandomBigInteger();

            mathUtil.powModOptimized(a, b, c);
        }, 3);
    }
}
