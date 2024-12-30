import random
import time

def generate_random_big_integer(bit_length):
    """Генерація випадкового великого числа заданої довжини в бітах."""
    return random.getrandbits(bit_length)

def add(a, b):
    return a + b

def sub(a, b):
    return a - b

def mul(a, b):
    return a * b

def div(a, b):
    return a // b

def pow_simple(a, exponent):
    return a ** exponent

def pow_optimized(a, b):
    result = 1
    current_base = a

    while b > 0:
        if b & 1:
            result *= current_base
        current_base *= current_base
        b >>= 1
    return result


def add_mod(a, b, c):
    return (a + b) % c

def sub_mod(a, b, c):
    return (a - b) % c

def mul_mod(a, b, c):
    return (a * b) % c

def div_mod(a, b, c):
    return (a // b) % c

def pow_mod(a, exponent, c):
    return pow(a, exponent, c)

def pow_mod_optimized(a, b, c):
    result = 1
    current_base = a % c

    while b > 0:
        if b & 1:
            result = (result * current_base) % c
        current_base = (current_base * current_base) % c
        b >>= 1
    return result

def measure_average_execution_time(method_to_run, iterations, method_name):
    total_duration_in_nano = 0

    for _ in range(iterations):
        start_time = time.time_ns()
        method_to_run()
        end_time = time.time_ns()
        total_duration_in_nano += (end_time - start_time)

    avg_duration_in_nano = total_duration_in_nano / iterations
    avg_duration_in_millis = avg_duration_in_nano / 1_000_000

    print(f"Метод: {method_name}")
    print(f"Середній час у (ns): {avg_duration_in_nano}")
    print(f"Середній час у (ms): {avg_duration_in_millis}\n")

def add_metric(bit_length):
    def run():
        a = generate_random_big_integer(bit_length)
        b = generate_random_big_integer(bit_length)
        add(a, b)
    measure_average_execution_time(run, 100, "add_metric")

def sub_metric(bit_length):
    def run():
        a = generate_random_big_integer(bit_length)
        b = generate_random_big_integer(bit_length)
        sub(a, b)
    measure_average_execution_time(run, 100, "sub_metric")

def mul_metric(bit_length):
    def run():
        a = generate_random_big_integer(bit_length)
        b = generate_random_big_integer(bit_length)
        mul(a, b)
    measure_average_execution_time(run, 100, "mul_metric")

def div_metric(bit_length):
    def run():
        a = generate_random_big_integer(bit_length)
        b = generate_random_big_integer(bit_length)
        div(a, b)
    measure_average_execution_time(run, 100, "div_metric")

def pow_metric(bit_length, exponent):
    def run():
        a = generate_random_big_integer(bit_length)
        pow_simple(a, exponent)
    measure_average_execution_time(run, 3, "pow_metric")

def pow_adv_metric1(bit_length, exponent):
    def run():
        a = generate_random_big_integer(bit_length)
        pow_optimized(a, exponent)
    measure_average_execution_time(run, 3, "pow_adv_metric1")

def pow_adv_metric2(bit_length):
    def run():
        a = generate_random_big_integer(bit_length)
        b = generate_random_big_integer(bit_length // 4)
        pow_optimized(a, b)
    measure_average_execution_time(run, 3, "pow_adv_metric2")

def add_mod_metric(bit_length):
    def run():
        a = generate_random_big_integer(bit_length)
        b = generate_random_big_integer(bit_length)
        c = generate_random_big_integer(bit_length)
        add_mod(a, b, c)
    measure_average_execution_time(run, 100, "add_mod_metric")

def sub_mod_metric(bit_length):
    def run():
        a = generate_random_big_integer(bit_length)
        b = generate_random_big_integer(bit_length)
        c = generate_random_big_integer(bit_length)
        sub_mod(a, b, c)
    measure_average_execution_time(run, 100, "sub_mod_metric")

def mul_mod_metric(bit_length):
    def run():
        a = generate_random_big_integer(bit_length)
        b = generate_random_big_integer(bit_length)
        c = generate_random_big_integer(bit_length)
        mul_mod(a, b, c)
    measure_average_execution_time(run, 100, "mul_mod_metric")

def div_mod_metric(bit_length):
    def run():
        a = generate_random_big_integer(bit_length)
        b = generate_random_big_integer(bit_length)
        c = generate_random_big_integer(bit_length)
        div_mod(a, b, c)
    measure_average_execution_time(run, 100, "div_mod_metric")

def pow_mod_metric1(bit_length, exponent):
    def run():
        a = generate_random_big_integer(bit_length)
        c = generate_random_big_integer(bit_length)
        pow_mod(a, exponent, c)
    measure_average_execution_time(run, 3, "pow_mod_metric1")

def pow_mod_metric2(bit_length, exponent):
    def run():
        a = generate_random_big_integer(bit_length)
        c = generate_random_big_integer(bit_length)
        pow_mod(a, exponent, c)
    measure_average_execution_time(run, 3, "pow_mod_metric2")

def pow_mod_adv_metric1(bit_length, exponent):
    def run():
        a = generate_random_big_integer(bit_length)
        c = generate_random_big_integer(bit_length)
        pow_mod_optimized(a, exponent, c)
    measure_average_execution_time(run, 3, "pow_mod_adv_metric1")

def pow_mod_adv_metric2(bit_length):
    def run():
        a = generate_random_big_integer(bit_length)
        b = generate_random_big_integer(bit_length // 4)
        c = generate_random_big_integer(bit_length)
        pow_mod_optimized(a, b, c)
    measure_average_execution_time(run, 3, "pow_mod_adv_metric2")

def main(bit_length):

    print(f"Довжина: {bit_length}")
    add_metric(bit_length)
    sub_metric(bit_length)
    mul_metric(bit_length)
    div_metric(bit_length)

    pow_metric(bit_length, 12743)
    pow_adv_metric1(bit_length, 12743)
    pow_adv_metric2(14)

    add_mod_metric(bit_length)
    sub_mod_metric(bit_length)
    mul_mod_metric(bit_length)
    div_mod_metric(bit_length)

    pow_mod_metric1(bit_length, 12743)
    pow_mod_metric2(bit_length, 12743)
    pow_mod_adv_metric1(bit_length, 12743)
    pow_mod_adv_metric2(14)
    print("")
    print("")

if __name__ == "__main__":
    bit_length1 = 1024
    bit_length2 = 2048
    bit_length3 = 4096

    main(bit_length1)
    main(bit_length2)
    main(bit_length3)
