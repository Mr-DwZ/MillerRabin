#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#define MAX_NUM_LEN 1000

struct BIGNUM {
    int num[MAX_NUM_LEN];
    int numlen;
};

typedef struct BIGNUM Bignum;

const Bignum zero = { { 0 },   1 };
const Bignum one  = { { 0,1 }, 1 };
const Bignum two  = { { 0,2 }, 1 };
//Remember using '&' when calling the methods below
//------------------IO Functions--------------

void init(Bignum* number) 
{
    int i;
    number->numlen = 0;
    memset(number->num, 0, MAX_NUM_LEN*sizeof(int));
}

int getBignum(Bignum* number)
{
    char c;
    int  i;
    
    init(number);
    while (1) {
        c = getchar();
        if (isdigit(c)) {
            number->num[++number->numlen] = c - '0';
        } else {
            if (c != '\n') return 0;
            break;
        }
    }
    
    return 1;
}

void putBignum(Bignum* number)
{
    int i;
    
    if (number->numlen == 0) {
        putchar('0');
        return;
    }
    
    for (i = 1; i <= number->numlen; i++) {
        printf("%d", number->num[i]);
    }
}

void fputBignum(FILE* fp, Bignum* number)
{
    int i;

    if (number->numlen == 0) {
        fprintf(fp, "0");
        return;
    }

    for (i = 1; i <= number->numlen; i++) {
        fprintf(fp, "%d", number->num[i]);
    }
}

void initWithInt(Bignum* number, int inum)
{
    int i;
    Bignum temp;
    
    init(number);
    init(&temp);
    while (inum > 0) {
        temp.num[++temp.numlen] = inum % 10;
        inum /= 10;
    }
    number->numlen = temp.numlen;
    for (i = 1; i <= temp.numlen; i++) {
        number->num[i] = temp.num[temp.numlen - i + 1];
    }
}

int initWithDigitString(Bignum* number, char* string)
{
   int i;
   
   init(number);
   for (i = 0 ; i < strlen(string); i++) {
       if (isdigit(string[i])) {
           number->num[++number->numlen] = string[i] - '0';
       } else {
           return 0;
       }
   }
   return 1;
}
//---------------------------------------------

//-----------------Calc functions--------------
//Add up a and b
Bignum BignumAdd(Bignum* a, Bignum* b) 
{
    Bignum ret, a_copy, b_copy, ret_copy;
    int i;
    
    init(&ret);
    init(&a_copy);
    init(&b_copy);
    init(&ret_copy);

    a_copy.numlen = a->numlen;
    b_copy.numlen = b->numlen;    
    for (i = 1; i <= a->numlen; i++) {
        a_copy.num[a->numlen - i + 1] = a->num[i];
    }
    for (i = 1; i <= b->numlen; i++) {
        b_copy.num[b->numlen - i + 1] = b->num[i];
    }
    
    ret.numlen = a_copy.numlen > b_copy.numlen ? a_copy.numlen : b_copy.numlen;
    
    for (i = 1; i <= ret.numlen; i++) {
        ret.num[i] += a_copy.num[i] + b_copy.num[i];
        if (ret.num[i] >= 10) {
           ret.num[i+1]++;
           ret.num[i] -= 10;
       }
    }    
    if (ret.num[ret.numlen + 1] == 1) ret.numlen++;

    ret_copy.numlen = ret.numlen;
    for (i = 1; i <= ret.numlen; i++) {
        ret_copy.num[i] = ret.num[ret.numlen - i + 1];
    }
    
    return ret_copy;
}

//Multiply a and b 
Bignum BignumMulti(Bignum* a, Bignum* b) 
{
    Bignum ret, a_copy, b_copy, ret_copy;
    int i,j;
    
    init(&a_copy);
    init(&b_copy);
    init(&ret_copy);
    init(&ret);    
    a_copy.numlen = a->numlen;
    b_copy.numlen = b->numlen;
    
    for (i = 1; i <= a->numlen; i++) {
        a_copy.num[a->numlen - i + 1] = a->num[i];
    }
    for (i = 1; i <= b->numlen; i++) {
        b_copy.num[b->numlen - i + 1] = b->num[i];
    }
    
    ret.numlen = a_copy.numlen + b_copy.numlen - 1;
    
    for (i = 1; i <= a_copy.numlen; i++) {
        for (j = 1; j <= b_copy.numlen; j++) {
            ret.num[i + j - 1] += a_copy.num[i] * b_copy.num[j];
        }
        for (j = 1; j <= ret.numlen; j++) {
               if (ret.num[j] >= 10) {
                   ret.num[j + 1] += ret.num[j] / 10;
                   ret.num[j] %= 10;
               }
        }
    }
    for (j = 1; j <= ret.numlen; j++) {
        if (ret.num[j] >= 10) {
            ret.num[j + 1] += ret.num[j] / 10;
            ret.num[j] %= 10;
        }
    }
    while (ret.num[ret.numlen] >= 10) {
        ret.num[ret.numlen + 1] += ret.num[ret.numlen] / 10;
        ret.num[ret.numlen] %= 10;
        ret.numlen++;
    }
    if (ret.num[ret.numlen + 1] != 0) ret.numlen++;
    while (ret.num[ret.numlen] == 0 && ret.numlen > 1) ret.numlen--;
    for (i = 1; i <= ret.numlen; i++) {
        ret_copy.num[i] = ret.num[ret.numlen - i + 1];
    }
    ret_copy.numlen = ret.numlen;

    return ret_copy;
}

// Compare a with b, if a>b return 1, a<b return -1, a=b return 0
int BignumCmp(Bignum* a, Bignum* b)  
{
    int i;
    
    if (a->numlen > b->numlen) {
        return 1;
    } else {
        if (a->numlen < b->numlen) {
            return -1;
        }
    }
    
    for (i = 1; i <= a->numlen; i++) {
        if (a->num[i] > b->num[i]) return 1;
        if (a->num[i] < b->num[i]) return -1;
    }
    
    return 0;
}

// a minus b 
Bignum BignumMinus(Bignum* a, Bignum* b) 
{
    Bignum ret, a_copy, b_copy, ret_copy;
    int i;
    
    init(&ret);
    init(&a_copy);
    init(&b_copy);
    init(&ret_copy);
    a_copy.numlen = a->numlen;
    b_copy.numlen = b->numlen;
    
    for (i = 1; i <= a->numlen; i++) {
        a_copy.num[a->numlen - i + 1] = a->num[i];
    }
    for (i = 1; i <= b->numlen; i++) {
        b_copy.num[b->numlen - i + 1] = b->num[i];
    }

    ret.numlen = a->numlen > b->numlen ? a->numlen : b->numlen;
    
    for (i = 1; i <= ret.numlen; i++) {
        ret.num[i] += a_copy.num[i] - b_copy.num[i];
        if (ret.num[i] < 0) {
           ret.num[i+1]--;
           ret.num[i] += 10;
        }
    }
    while (ret.num[ret.numlen] == 0 && ret.numlen > 1) ret.numlen--;
    ret_copy.numlen = ret.numlen;
    for (i = 1; i <= ret.numlen; i++) {
        ret_copy.num[i] = ret.num[ret.numlen - i + 1];
    }
    
    return ret_copy; 
}

//return  a/b 
Bignum BignumDivide(Bignum* a, Bignum* b) 
{
    Bignum temp, ret, bitBignum, bitMulti, ret_copy;
    int i, bit, nonzero;

    nonzero = 1;
    init(&temp);
    init(&ret);
    init(&ret_copy);
    temp.numlen = 0;

    for (i = 1; i <= a->numlen; i++) {
        temp.num[++temp.numlen] = a->num[i];
        for (bit = 9; bit >= 0; bit--) {
            initWithInt(&bitBignum, bit);
            bitMulti = BignumMulti(&bitBignum, b);
            if (BignumCmp(&temp, &bitMulti) >= 0) {
                temp = BignumMinus(&temp, &bitMulti);
                if (temp.numlen == 1 && temp.num[1] == 0) temp.numlen = 0;
                ret.num[++ret.numlen] = bit;
                break;
            }
        }
    }
    
    while (ret.num[nonzero] == 0) nonzero++;
    for (i = nonzero; i <= ret.numlen; i++) {
        ret_copy.num[++ret_copy.numlen] = ret.num[i];
    }

    return ret_copy;
}

// return a % b
Bignum BignumMod(Bignum* a, Bignum* b)  
{
    Bignum ret, temp, temp1;
    
    temp = BignumDivide(a, b);
    temp1 = BignumMulti(&temp, b);
    ret = BignumMinus(a, &temp1);
    
    return ret;
}

//a^u
Bignum BignumExp(Bignum* a, Bignum* u)
{
    Bignum next, ele, ret, u_copy, a_copy;
    
    u_copy = *u;
    ret = one;
    a_copy = *a;

    while (BignumCmp(&u_copy, &zero) > 0) {
        if (u_copy.num[u_copy.numlen] % 2 == 1) ret = BignumMulti(&ret, &a_copy);
        a_copy = BignumMulti(&a_copy, &a_copy);
        u_copy = BignumDivide(&u_copy, &two);
    }
    
    return ret;
}

// a^u mod n
Bignum BignumModExp(Bignum* a, Bignum* u, Bignum* n)
{
    Bignum next, ele, ret, u_copy, a_copy;

    u_copy = *u;
    ret = one;
    a_copy = *a;

    while (BignumCmp(&u_copy, &zero) > 0) {
        if (u_copy.num[u_copy.numlen] % 2 == 1) ret = BignumMulti(&ret, &a_copy);
        ret = BignumMod(&ret, n);
        a_copy = BignumMulti(&a_copy, &a_copy);
        a_copy = BignumMod(&a_copy, n);
        u_copy = BignumDivide(&u_copy, &two);
    }
    ret = BignumMod(&ret, n);
    return ret;
}
//-------------------------------------------------