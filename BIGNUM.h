#include "BIGNUM.c"

int  getBignum(Bignum*);
void putBignum(Bignum*);
void fputBignum(FILE*, Bignum*);

void init(Bignum*);
void initWithInt(Bignum*, int);
int  initWithDigitString(Bignum*, char*);

Bignum BignumAdd   (Bignum*, Bignum*);
Bignum BignumMinus (Bignum*, Bignum*);
Bignum BignumMulti (Bignum*, Bignum*);
Bignum BignumDivide(Bignum*, Bignum*);
Bignum BignumMod   (Bignum*, Bignum*);
Bignum BignumExp   (Bignum*, Bignum*);
Bignum BignumModExp(Bignum*, Bignum*, Bignum*);