#define _CRT_SECURE_NO_WARNINGS

#include "BIGNUM.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define MAX_BINARY_LEN 500


void getNum(Bignum* n, Bignum* u, int* t) //let n - 1 = 2^t*u
{
    Bignum temp, t_copy, two_exp, n_minus_one;
    int binary[MAX_BINARY_LEN] = {0};
    int i, first_non_zero_bit;

    n_minus_one = BignumMinus(n, &one);
    *t = 0;
    while (n_minus_one.num[n_minus_one.numlen] % 2 == 0) {
        (*t)++;
        n_minus_one = BignumDivide(&n_minus_one, &two);
    }
    *u = n_minus_one;
}

int Witness(int a, Bignum* n)
{
    int t, i;
    Bignum u, a_copy, n_minus_one;
    Bignum x[100];

    n_minus_one = BignumMinus(n, &one);
    initWithInt(&a_copy, a);

    if (BignumCmp(&a_copy, n) > 0) {
        a_copy = BignumMod(&a_copy, &n_minus_one);
        a_copy = BignumAdd(&a_copy, &one);
    }

    getNum(n, &u, &t);

    x[0] = BignumModExp(&a_copy, &u, n);
   
    for (i = 1; i <= t; i++) {
        x[i] = BignumModExp(&x[i - 1], &two, n);
        if ((BignumCmp(&x[i], &one) == 0) &&
            (BignumCmp(&x[i - 1], &one) != 0) &&
            (BignumCmp(&x[i - 1], &n_minus_one) != 0)) return 1;
    }
    if (BignumCmp(&one, &x[t]) != 0) return 1;
    return 0;
}

int MillerRabin(Bignum* n, int s)
{
    int i;
    int randNum;
    Bignum randNum_Copy;
    
    if (BignumCmp(n, &zero)   == 0) return 0;
    if (BignumCmp(n, &one)    == 0) return 0;
    if (BignumCmp(n, &two)    == 0) return 0;
    if (n->num[n->numlen] % 2 == 0) return 0;

    for (i = 1; i <= s; i++) {
        srand(time(NULL));
        randNum = rand();
        if (Witness(randNum, n)) return 0;
    }
    
    return 1;
}

void updateLog(int arg, Bignum* n)
{
    FILE* fp;
    time_t time_now;

    fp = fopen("Log.txt", "a+");
    time(&time_now);
    fprintf(fp, "%s", asctime(gmtime(&time_now)));
    if (arg == 0) {
        fprintf(fp, "User inputted an invalid value.\n");
    }

    if (arg == 1) {
        fprintf(fp, "User inputted ");
        fputBignum(fp, n);

        fprintf(fp, ". not a prime\n");
    }

    if (arg == 2) {
        fprintf(fp, "User inputted ");
        fputBignum(fp, n);

        fprintf(fp, ". is a prime\n");
    }

    fprintf(fp, "\n");
}

int main()
{
    Bignum a, b, c, d, ans;
    int i, valid;
    
    //initWithDigitString(&a, "225251798594466661409915431774713195745814267044878909733007331390393510002687");

    printf("Please input a number larger than 2.\n");

    valid = getBignum(&a);

    system("cls");

    if (valid) {
        printf("Please wait a moment, I'm working on it :)");
    } else {
        printf("Invalid input :(\n");
        system("pause");
        updateLog(0, &a);
        return 0;
    }

    if (MillerRabin(&a, 5)) {
        system("cls");
        updateLog(2, &a);
        printf("isPrime\n");
    } else {
        system("cls");
        updateLog(1, &a);
        printf("isNotPrime\n");
    }
    getchar();

    return 0;
}