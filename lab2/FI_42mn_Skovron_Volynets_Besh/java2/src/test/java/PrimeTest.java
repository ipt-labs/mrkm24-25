import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import java.math.BigInteger;
import java.util.Arrays;

public class PrimeTest {

    private final org.mrkm.PrimeTest test = new org.mrkm.PrimeTest();

    @Test
    void surePrimeTest1() {
        boolean[] expected = new boolean[]{
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true
        };
        boolean[] actual = new boolean[expected.length];

        BigInteger p = BigInteger.valueOf(13);
        for (int i = 0; i < expected.length; i++) {
            actual[i] = test.isPrime(p);
        }

        Assertions.assertArrayEquals(expected, actual);
    }

    @Test
    void surePrimeTest2() {
        boolean[] expected = new boolean[]{
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true
        };
        boolean[] actual = new boolean[expected.length];

        BigInteger p = BigInteger.valueOf(113);
        for (int i = 0; i < expected.length; i++) {
            actual[i] = test.isPrime(p);
        }

        Assertions.assertArrayEquals(expected, actual);
    }

    @Test
    void surePrimeTest3() {
        boolean[] expected = new boolean[]{
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true
        };
        boolean[] actual = new boolean[expected.length];

        BigInteger p = BigInteger.valueOf(99607);
        for (int i = 0; i < expected.length; i++) {
            actual[i] = test.isPrime(p);
        }

        Assertions.assertArrayEquals(expected, actual);
    }

    @Test
    void noSurePrimeTest1() {
        boolean[] expected = new boolean[]{
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true
        };
        boolean[] actual = new boolean[expected.length];

        BigInteger p = BigInteger.valueOf(15);
        for (int i = 0; i < expected.length; i++) {
            actual[i] = test.isPrime(p);
        }

        Assertions.assertFalse(Arrays.equals(expected, actual));
    }

    @Test
    void noSurePrimeTest2() {
        boolean[] expected = new boolean[]{
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true
        };
        boolean[] actual = new boolean[expected.length];

        BigInteger p = BigInteger.valueOf(112);
        for (int i = 0; i < expected.length; i++) {
            actual[i] = test.isPrime(p);
        }

        Assertions.assertFalse(Arrays.equals(expected, actual));
    }

    @Test
    void noSurePrimeTest3() {
        boolean[] expected = new boolean[]{
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true
        };
        boolean[] actual = new boolean[expected.length];

        BigInteger p = BigInteger.valueOf(39121);
        for (int i = 0; i < expected.length; i++) {
            actual[i] = test.isPrime(p);
        }

        Assertions.assertFalse(Arrays.equals(expected, actual));
    }

    @Test
    void noSurePrimeTest4() {
        boolean[] expected = new boolean[]{
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true
        };
        boolean[] actual = new boolean[expected.length];

        BigInteger p = BigInteger.valueOf(196183);
        for (int i = 0; i < expected.length; i++) {
            actual[i] = test.isPrime(p);
        }

        Assertions.assertFalse(Arrays.equals(expected, actual));
    }

    @Test
    void noSurePrimeTest5() {
        boolean[] expected = new boolean[]{
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true
        };
        boolean[] actual = new boolean[expected.length];

        BigInteger p = BigInteger.valueOf(122923);
        for (int i = 0; i < expected.length; i++) {
            actual[i] = test.isPrime(p);
        }

        Assertions.assertFalse(Arrays.equals(expected, actual));
    }

    @Test
    void noSurePrimeTest6() {
        boolean[] expected = new boolean[]{
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true,
                true, true, true, true, true, true, true, true, true, true
        };
        boolean[] actual = new boolean[expected.length];

        BigInteger p = new BigInteger("2500744714570633849");
        for (int i = 0; i < expected.length; i++) {
            actual[i] = test.isPrime(p);
        }

        System.out.println(Arrays.toString(actual));
        Assertions.assertFalse(Arrays.equals(expected, actual));
    }
}
