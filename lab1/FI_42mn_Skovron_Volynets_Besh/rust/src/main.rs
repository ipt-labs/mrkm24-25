use std::time::Instant;
use num_bigint::BigInt;
use num_traits::{One, Zero};
use rand::Rng;
use rand::thread_rng;

fn generate_random_bigint(bit_length: usize) -> BigInt {
    let mut rng = thread_rng();
    let num_bytes = (bit_length + 7) / 8;
    let mut bytes = Vec::with_capacity(num_bytes);

    for _ in 0..num_bytes {
        bytes.push(rng.gen::<u8>());
    }

    BigInt::from_bytes_le(num_bigint::Sign::Plus, &bytes)
}

fn add(a: BigInt, b: BigInt) -> BigInt {
    a + b
}

fn sub(a: BigInt, b: BigInt) -> BigInt {
    a - b
}

fn mul(a: BigInt, b: BigInt) -> BigInt {
    a * b
}

fn div(a: BigInt, b: BigInt) -> BigInt {
    a / b
}

fn pow(a: BigInt, exp: u32) -> BigInt {
    a.pow(exp)
}

fn pow_optimized_int(base: BigInt, mut exp: u32) -> BigInt {
    let mut result = BigInt::one();
    let mut current_base = base;

    while exp > 0 {
        if exp & 1 == 1 {
            result = &result * &current_base;
        }
        current_base = &current_base * &current_base;
        exp >>= 1;
    }
    result
}

fn pow_optimized_big(base: BigInt, mut exp: BigInt) -> BigInt {
    let mut result = BigInt::one();
    let mut current_base = base;

    while exp > BigInt::zero() {
        if &exp & BigInt::one() == BigInt::one() {
            result = &result * &current_base;
        }
        current_base = &current_base * &current_base;
        exp >>= 1;
    }
    result
}

fn add_mod(a: BigInt, b: BigInt, modulus: &BigInt) -> BigInt {
    (a + b) % modulus
}

fn sub_mod(a: BigInt, b: BigInt, modulus: &BigInt) -> BigInt {
    (a - b) % modulus
}

fn mul_mod(a: BigInt, b: BigInt, modulus: &BigInt) -> BigInt {
    (a * b) % modulus
}

fn div_mod(a: BigInt, b: BigInt, modulus: &BigInt) -> BigInt {
    let result = &a / &b;
    result % modulus
}

fn pow_mod(a: BigInt, exp: usize, modulus: &BigInt) -> BigInt {
    a.modpow(&BigInt::from(exp), modulus)
}

fn mod_pow(a: BigInt, exp: usize, modulus: &BigInt) -> BigInt {
    a.modpow(&BigInt::from(exp), modulus)
}

fn pow_mod_optimized(a: BigInt, mut exp: usize, modulus: &BigInt) -> BigInt {
    let mut result = BigInt::one();
    let mut base = a;

    while exp > 0 {
        if exp & 1 == 1 {
            result = (result * &base) % modulus;
        }
        base = &base * &base;
        exp >>= 1;
    }

    result
}

fn pow_mod_optimized_bigint(base: BigInt, mut exp: BigInt, modulus: &BigInt) -> BigInt {
    let mut result = BigInt::one();
    let mut current_base = base;

    while exp > BigInt::zero() {
        if &exp & BigInt::from(1) == BigInt::one() {
            result = (result * &current_base) % modulus;
        }
        current_base = &current_base * &current_base;
        exp = exp >> 1;
    }

    result
}

fn measure_average_execution_time<F>(method_name: &str, method_to_run: F, iterations: usize)
where
    F: Fn() -> (),
{
    let mut total_duration_in_nano = 0u128;

    for _ in 0..iterations {
        let start_time = Instant::now();

        method_to_run();

        let end_time = Instant::now();
        total_duration_in_nano += end_time.duration_since(start_time).as_nanos();
    }

    let avg_duration_in_nano = total_duration_in_nano / iterations as u128;
    let avg_duration_in_millis = avg_duration_in_nano / 1_000_000;

    // Output the results with the method name
    println!("Method called: {}", method_name);
    println!("Average Execution time (ns): {}", avg_duration_in_nano);
    println!("Average Execution time (ms): {}", avg_duration_in_millis);
    println!();
}

fn add_metric() {
    measure_average_execution_time("add_metric", || {
        let a = generate_random_bigint(BIT_LENGTH);
        let b = generate_random_bigint(BIT_LENGTH);

        add(a, b);
    }, 1000);
}

fn sub_metric() {
    measure_average_execution_time("sub_metric", || {
        let a = generate_random_bigint(BIT_LENGTH);
        let b = generate_random_bigint(BIT_LENGTH);

        sub(a, b);
    }, 1000);
}

fn mul_metric() {
    measure_average_execution_time("mul_metric", || {
        let a = generate_random_bigint(BIT_LENGTH);
        let b = generate_random_bigint(BIT_LENGTH);

        mul(a, b);
    }, 1000);
}

fn div_metric() {
    measure_average_execution_time("div_metric", || {
        let a = generate_random_bigint(BIT_LENGTH);
        let b = generate_random_bigint(BIT_LENGTH);

        div(a, b);
    }, 1000);
}

fn pow_metric(exp: u32) {
    measure_average_execution_time("pow_metric", || {
        let a = generate_random_bigint(BIT_LENGTH);

        pow(a, exp);
    }, 10);
}

fn pow_optimized_int_metric(exp: u32) {
    measure_average_execution_time("pow_optimized_int_metric", || {
        let a = generate_random_bigint(BIT_LENGTH);

        pow_optimized_int(a, exp);
    }, 10);
}

fn pow_optimized_big_metric(exp: BigInt) {
    measure_average_execution_time("pow_optimized_big_metric", || {
        let a = generate_random_bigint(BIT_LENGTH);

        pow_optimized_big(a, exp.clone());
    }, 10);
}

fn add_mod_metric() {
    measure_average_execution_time("add_mod_metric", || {
        let a = generate_random_bigint(BIT_LENGTH);
        let b = generate_random_bigint(BIT_LENGTH);
        let modulus = generate_random_bigint(BIT_LENGTH);

        add_mod(a, b, &modulus);
    }, 1000);
}

fn sub_mod_metric() {
    measure_average_execution_time("sub_mod_metric", || {
        let a = generate_random_bigint(BIT_LENGTH);
        let b = generate_random_bigint(BIT_LENGTH);
        let modulus = generate_random_bigint(BIT_LENGTH);

        sub_mod(a, b, &modulus);
    }, 1000);
}

fn mul_mod_metric() {
    measure_average_execution_time("mul_mod_metric", || {
        let a = generate_random_bigint(BIT_LENGTH);
        let b = generate_random_bigint(BIT_LENGTH);
        let modulus = generate_random_bigint(BIT_LENGTH);

        mul_mod(a, b, &modulus);
    }, 1000);
}

fn div_mod_metric() {
    measure_average_execution_time("div_mod_metric", || {
        let a = generate_random_bigint(BIT_LENGTH);
        let b = generate_random_bigint(BIT_LENGTH);
        let modulus = generate_random_bigint(BIT_LENGTH);

        div_mod(a, b, &modulus);
    }, 1000);
}

fn mod_pow_metric(exp: usize) {
    measure_average_execution_time("mod_pow_metric", || {
        let a = generate_random_bigint(BIT_LENGTH);
        let modulus = generate_random_bigint(BIT_LENGTH);

        mod_pow(a, exp, &modulus);
    }, 10);
}

fn pow_mod_metric(exp: usize) {
    measure_average_execution_time("pow_mod_metric", || {
        let a = generate_random_bigint(BIT_LENGTH);
        let modulus = generate_random_bigint(BIT_LENGTH);

        pow_mod(a, exp, &modulus);
    }, 10);
}

fn pow_mod_optimized_metric(exp: usize) {
    measure_average_execution_time("pow_mod_optimized_metric", || {
        let a = generate_random_bigint(BIT_LENGTH);
        let modulus = generate_random_bigint(BIT_LENGTH);

        pow_mod_optimized(a, exp, &modulus);
    }, 10);
}

fn pow_mod_optimized_bigint_metric(exp: BigInt) {
    measure_average_execution_time("pow_mod_optimized_bigint_metric", || {
        let a = generate_random_bigint(BIT_LENGTH);
        let modulus = generate_random_bigint(BIT_LENGTH);

        pow_mod_optimized_bigint(a, exp.clone(), &modulus);
    }, 10);
}

const BIT_LENGTH: usize = 1028;

fn main() {
    let exp: u32 = 12743;
    let exp2: usize = 12743;
    let rand: BigInt = generate_random_bigint(14);

    add_metric();
    sub_metric();
    mul_metric();
    div_metric();
    pow_metric(exp.clone());
    pow_optimized_int_metric(exp.clone());
    pow_optimized_big_metric(rand.clone());

    add_mod_metric();
    sub_mod_metric();
    mul_mod_metric();
    div_mod_metric();

    mod_pow_metric(exp2);
    pow_mod_metric(exp2);
    pow_mod_optimized_metric(exp2);
    pow_mod_optimized_bigint_metric(rand.clone());
}
