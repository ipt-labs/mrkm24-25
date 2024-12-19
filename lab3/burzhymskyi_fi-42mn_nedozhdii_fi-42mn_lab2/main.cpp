#include <cstdio>
#include <iostream>
#include <openssl/bn.h>
#include <openssl/core_dispatch.h>
#include <stdio.h>
#include <string>

#define PRIME_BIT 512
void generateHardPrime(BIGNUM* p, BIGNUM* q, BN_CTX* ctx)
{
    BIGNUM* p_t = BN_new();

    while (true)
    {
        BN_generate_prime_ex(q, PRIME_BIT, 1, NULL, NULL,
                             NULL);
        BN_copy(p, q);
        BN_mul_word(p, 2);
        BN_add_word(p, 1);
        if (BN_is_prime_ex(p, 10000, ctx, NULL))
        {
            return;
        }
    }
    BN_free(p_t);
}

void findGenerator(BIGNUM* p, BIGNUM* q, BIGNUM* g, BN_CTX* ctx)
{
    BIGNUM* tmp = BN_new();
    BIGNUM* one = BN_new();
    BN_set_word(one, 1);
    BIGNUM* two = BN_new();
    BN_set_word(two, 2);

    while (true)
    {
        BN_rand_range(g, p);
        BN_mod_exp(tmp, g, two, p, ctx);
        if (BN_cmp(tmp, one) == 0)
            continue;
        BN_mod_exp(tmp, g, q, p, ctx);

        if (BN_cmp(tmp, one) == 0)
            continue;
        return;
    }
}

void generate_keys(BIGNUM* p, BIGNUM* g, BIGNUM* x, BIGNUM* y, BN_CTX* ctx)
{
    BIGNUM* q = BN_new();

    generateHardPrime(p, q, ctx);
    findGenerator(p, q, g, ctx);
    std::cout << "g: \n";
    BN_print_fp(stdout, g);
    std::cout << "\n";
    std::cout << "p:\n";
    BN_print_fp(stdout, p);
    std::cout << "\n";


    BN_rand_range(x, p);
    BN_mod_exp(y, g, x, p, ctx);
    std::cout << "y: \n";
    BN_print_fp(stdout, y);
    std::cout << "\n";
    BN_free(q);
}

void encrypt(BIGNUM* c1, BIGNUM* c2, BIGNUM* m, BIGNUM* p, BIGNUM* g, BIGNUM* y,
             BN_CTX* ctx)
{
    BIGNUM* k = BN_new();
    BN_rand_range(k, p);
    BN_mod_exp(c1, g, k, p, ctx);
    BIGNUM* tmp = BN_new();
    BN_mod_exp(tmp, y, k, p, ctx);
    BN_mod_mul(c2, m, tmp, p, ctx);
    BN_free(tmp);
    BN_free(k);
}

void decrypt(BIGNUM* m, BIGNUM* c1, BIGNUM* c2, BIGNUM* x, BIGNUM* p,
             BN_CTX* ctx)
{
    BIGNUM* tmp = BN_new();
    BN_mod_exp(tmp, c1, x, p, ctx);
    BN_mod_inverse(tmp, tmp, p, ctx);
    BN_mod_mul(m, c2, tmp, p, ctx);
    BN_free(tmp);
}

void sign_message(BIGNUM* r, BIGNUM* s, BIGNUM* m, BIGNUM* p, BIGNUM* g,
                  BIGNUM* x, BN_CTX* ctx)
{
    BIGNUM* k = BN_new();
    BIGNUM* k_inv = BN_new();
    BIGNUM* tmp = BN_new();


    BIGNUM* p_minus_1 = BN_new();
    BN_copy(p_minus_1, p);
    BN_sub_word(p_minus_1, 1);
    do
    {
        BN_rand_range(k, p_minus_1);
        BN_gcd(tmp, k, p_minus_1, ctx);
    } while (!BN_is_one(tmp));


    BN_mod_exp(r, g, k, p, ctx);


    BN_mod_inverse(k_inv, k, p_minus_1, ctx);


    BN_mod_mul(tmp, x, r, p_minus_1, ctx);
    BN_mod_sub(tmp, m, tmp, p_minus_1, ctx);
    BN_mod_mul(s, tmp, k_inv, p_minus_1, ctx);

    BN_free(k);
    BN_free(k_inv);
    BN_free(tmp);
    BN_free(p_minus_1);
}

int verify_signature(BIGNUM* r, BIGNUM* s, BIGNUM* m, BIGNUM* p, BIGNUM* g,
                     BIGNUM* y, BN_CTX* ctx)
{
    BIGNUM* p_minus_1 = BN_new();
    BN_copy(p_minus_1, p);
    BN_sub_word(p_minus_1, 1);

    if (BN_is_zero(r) || BN_cmp(r, p) >= 0 || BN_is_zero(s) ||
        BN_cmp(s, p_minus_1) >= 0)
    {
        return 0;
    }

    BIGNUM* v1 = BN_new();
    BIGNUM* v2 = BN_new();
    BIGNUM* tmp1 = BN_new();
    BIGNUM* tmp2 = BN_new();


    BN_mod_exp(v1, g, m, p, ctx);


    BN_mod_exp(tmp1, y, r, p, ctx);
    BN_mod_exp(tmp2, r, s, p, ctx);
    BN_mod_mul(v2, tmp1, tmp2, p, ctx);

    int result = BN_cmp(v1, v2) == 0;

    BN_free(p_minus_1);
    BN_free(v1);
    BN_free(v2);
    BN_free(tmp1);
    BN_free(tmp2);
    return result;
}

#include <cstdlib>
#include <term.h>
#include <unistd.h>

void clear()
{
#ifdef _WIN32
    std::system("cls");
#elif defined(__unix__) || defined(__APPLE__) || defined(__linux__)
    std::system("clear");
#else
    std::cout << "Unsupported operating system" << std::endl;
#endif
}

void encrypt() {}

void decrypt() {}

int main()
{
    bool exit = false;
    BN_CTX* ctx = BN_CTX_new();

    BIGNUM *p = BN_new(), *g = BN_new(), *x = BN_new(), *y = BN_new();


    generate_keys(p, g, x, y, ctx);

    while (!exit)
    {
        std::cout << "[0]. Вихід\n";
        std::cout << "[1]. Encrypt()\n";
        std::cout << "[2]. Decrypt()\n";
        std::cout << "[3]. Sign()\n";
        std::cout << "[4]. Verify()\n";
        std::cout << "[5]. PublicKey()\n";
        std::cout << "Enter number of operation.\n";
        int i;
        std::cin >> i;
        switch (i)
        {
            case 0:
                exit = true;
                break;
            case 1:
            {
                clear();
                std::cout << "Encryption (HEX)\n";
                std::cout << "Input primitive element of group (g):\n";
                std::string g_s;
                std::cin >> g_s;

                std::cout << "Input prime mod (p):\n";
                std::string p_s;
                std::cin >> p_s;

                std::cout << "y:\n";
                std::string y_s;
                std::cin >> y_s;
                BIGNUM *p_t = BN_new(), *g_t = BN_new(), *y_t = BN_new();
                BN_hex2bn(&g_t, g_s.c_str());
                BN_hex2bn(&p_t, p_s.c_str());
                BN_hex2bn(&y_t, y_s.c_str());

                BIGNUM *c1 = BN_new(), *c2 = BN_new(), *m = BN_new();

                std::cout << "Input message:\n";
                std::string m_s;
                std::cin >> m_s;
                BN_hex2bn(&m, m_s.c_str());
                encrypt(c1, c2, m, p_t, g_t, y_t, ctx);

                std::cout << "(c1,c2) = \n";
                BN_print_fp(stdout, c1);
                std::cout << '\n';
                BN_print_fp(stdout, c2);
                std::cout << "\n";
                BN_free(c1);

                BN_free(c2);
                BN_free(g_t);
                BN_free(p_t);
                BN_free(y_t);
                BN_free(m);
                std::string tmp;
                std::cout << "Enter smth:\n";
                std::cin >> tmp;
                std::cout << "\n";
            }

            break;
            case 2:
            {
                clear();
                std::cout << "Decryption (HEX)\n";
                std::cout << "Input cyphertext:\n";
                std::string C1, C2;
                std::cout << "Input C1:\n";
                std::cin >> C1;
                std::cout << "Input C2:\n";
                std::cin >> C2;
                BIGNUM *c1 = BN_new(), *c2 = BN_new(), *m = BN_new();
                BN_hex2bn(&c1, C1.c_str());
                BN_hex2bn(&c2, C2.c_str());
                decrypt(m, c1, c2, x, p, ctx);
                std::cout << "Plaintext:\n";
                BN_print_fp(stdout, m);
                BN_free(c1);
                BN_free(c2);
                BN_free(m);
                std::string tmp;
                std::cout << "\nEnter smth:\n";
                std::cin >> tmp;
                std::cout << "\n";
            }
            break;
            case 3:
            {
                clear();
                std::cout << "Sign (HEX):\n";
                std::cout << "Enter message:\n";
                std::string M;
                std::cin >> M;
                BIGNUM* m = BN_new();
                BN_hex2bn(&m, M.c_str());
                BIGNUM *r = BN_new(), *s = BN_new();

                sign_message(r, s, m, p, g, x, ctx);
                std::cout << "SIGN (r,s): \n";
                BN_print_fp(stdout, r);
                std::cout << '\n';
                BN_print_fp(stdout, s);
                std::cout << '\n';
                BN_free(m);
                BN_free(r);
                BN_free(s);
                std::string tmp;
                std::cout << "Enter smth:\n";
                std::cin >> tmp;
                std::cout << "\n";
            }

            break;
            case 4:
            {

                clear();
                std::string R, S, M;
                std::cout << "Verify (HEX):\n";
                std::cout << "Enter r:\n";
                std::cin >> R;
                std::cout << "Enter s:\n";
                std::cin >> S;
                std::cout << "Enter m:\n";
                std::cin >> M;
                BIGNUM* m = BN_new();
                BN_hex2bn(&m, M.c_str());
                BIGNUM *r = BN_new(), *s = BN_new();
                BN_hex2bn(&r, R.c_str());
                BN_hex2bn(&s, S.c_str());

                std::cout << "Input primitive element of group (g):\n";
                std::string g_s;
                std::cin >> g_s;

                std::cout << "Input prime mod (p):\n";
                std::string p_s;
                std::cin >> p_s;

                std::cout << "y:\n";
                std::string y_s;
                std::cin >> y_s;
                BIGNUM *p_t = BN_new(), *g_t = BN_new(), *y_t = BN_new();
                BN_hex2bn(&g_t, g_s.c_str());
                BN_hex2bn(&p_t, p_s.c_str());
                BN_hex2bn(&y_t, y_s.c_str());

                int res = verify_signature(r, s, m, p_t, g_t, y_t, ctx);

                std::cout << "Verify: " << res << "\n";

                BN_free(m);
                BN_free(r);
                BN_free(s);
                BN_free(g_t);
                BN_free(p_t);
                BN_free(y_t);
                std::string tmp;
                std::cout << "Enter smth:\n";
                std::cin >> tmp;
                std::cout << "\n";
            }
            break;
            case 5:
            {
                clear();
                std::cout << "g: \n";
                BN_print_fp(stdout, g);
                std::cout << "\n";
                std::cout << "p:\n";
                BN_print_fp(stdout, p);
                std::cout << "\n";

                std::cout << "y: \n";
                BN_print_fp(stdout, y);
                std::cout << "\n";
                std::string tmp;
                std::cout << "Enter smth:\n";
                std::cin >> tmp;
                std::cout << "\n";
            }
            default:
                std::cout << "You entered wrong number\n";
        }
        clear();
    }
    BN_free(p);
    BN_free(g);
    BN_free(x);
    BN_free(y);

    BN_CTX_free(ctx);
    return 0;
}
