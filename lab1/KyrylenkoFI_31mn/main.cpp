#include <iostream>
#include <cryptopp/integer.h>
#include <cryptopp/osrng.h>
#include <chrono>

using namespace CryptoPP;
using namespace std;

// Функція для заміру часу виконання
template <typename Func, typename... Args>
double benchmark(Func func, Args&&... args) {
    auto start = chrono::high_resolution_clock::now();

    // Виконання функції кілька разів для заміру часу
    for (int i = 0; i < 10; ++i) {
        func(std::forward<Args>(args)...);
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    // Обчислення середнього часу виконання
    return elapsed.count() / 10;
}

// Операції для тестування
void test_addition(const Integer &a, const Integer &b) {
    Integer result = a + b;
}

void test_multiplication(const Integer &a, const Integer &b) {
    Integer result = a * b;
}

void test_modular_exponentiation(const Integer &a, const Integer &b, const Integer &mod) {
    Integer result = a_exp_b_mod_c(a, b, mod);
}

// Генерація великих чисел
pair<Integer, Integer> generate_large_numbers(int bit_size) {
    AutoSeededRandomPool rng;
    Integer a, b;

    a.Randomize(rng, bit_size);
    b.Randomize(rng, bit_size);

    return make_pair(a, b);
}

int main() {
    int bit_sizes[] = {128, 256, 512, 1024}; // Розміри чисел у бітах
    int iterations = 10; // Кількість повторень

    cout << "Тестування операцій із великими числами:" << endl;
    cout << "Бітовий розмір | Операція          | Середній час (с)" << endl;

    for (int bit_size : bit_sizes) {
        // Генерація великих чисел
        auto [a, b] = generate_large_numbers(bit_size);
        Integer mod = Integer::Power2(bit_size - 1);

        // Тестування додавання
        double avg_time = benchmark(test_addition, a, b);
        cout << bit_size << "            | Додавання        | " << avg_time << " seconds" << endl;

        // Тестування множення
        avg_time = benchmark(test_multiplication, a, b);
        cout << bit_size << "            | Множення         | " << avg_time << " seconds" << endl;

        // Тестування модульного піднесення до степеня
        avg_time = benchmark(test_modular_exponentiation, a, b, mod);
        cout << bit_size << "            | Піднесення до модуля | " << avg_time << " seconds" << endl;
    }

    return 0;
}