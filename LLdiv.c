#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long ULL;
typedef long long LL;

void ULLdivULL(ULL a, ULL b, ULL *qp, ULL *rp)
{
    ULL r = 0, q = 0;
    int i;
    for (i = 0; i < 64; i++) {
        r = (r << 1) + (a >> 63);
        a <<= 1;
        q <<= 1;
        if (r >= b) {
            r -= b;
            q += 1;
        }
    }
    if (qp) *qp = q;
    if (rp) *rp = r;
}

void LLdivLL(LL a, LL b, LL *qp, LL *rp)
{
    int qf = 0, rf = 0;
    if (a < 0) { qf = rf = 1; a = -a; }
    if (b < 0) { qf ^= 1; b = -b; }

    ULLdivULL(a, b, (ULL *) qp, (ULL *) rp);

    if (qp && qf) *qp = -*qp;
    if (rp && rf) *rp = -*rp;
}



/* test data */
LL data[] = {
    0, 1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29
    -1, -2, -3, -5, -7, -11, -13, -17, -19, -23, -29,

    20, 30, 50,
    -20, -30, -50,

    0x7ffffffffffffffa, 0x7ffffffffffffffb,
    0x7ffffffffffffffc, 0x7ffffffffffffffd,
    0x7ffffffffffffffe, 0x7fffffffffffffff,
    0x8000000000000000, 0x8000000000000001,
    0x8000000000000002, 0x8000000000000003,
    0x8000000000000004, 0x8000000000000005,

    0x6631a3c5992d428b, 0x3a6ac5ed24cb0563,
    0x2fa099392715ddf4, 0x434c1488ec806d82,
    0xf8a9e478877a4107, 0x17f95918e3c2dad4,
    0x9dc1dadd05fe03c5, 0x4660525725384aa2,
    0x91a4716950c8f9b7, 0x7d85a9cbf5371f84,
    0x0f49b3ddf4f1e168, 0xbae686f31cfb42e0,
    0xe3665b91bc16d67e, 0xd808a3ba85fdacbd,
    0x2865e0d75a01f456, 0x292d75ea6fb0e5e8,
    0x36fd4f379934ed15, 0xff8f2d6ea26d7db4,
    0xf9035c37627d8250, 0x87d18d6b2d3cf3ca,
};

int data_size = sizeof(data) / sizeof(data[0]);



/* test function */
#define MAKE_TEST_FUNC(TYPE, FMT) \
void test_ ## TYPE() \
{ \
    TYPE a, b; \
    TYPE q, r; \
    TYPE qans, rans; \
    int i, j; \
    for (i = 0; i < data_size; i++) \
        for (j = 0; j < data_size; j++) { \
            a = data[i]; \
            b = data[j]; \
            if (b == 0) continue; \
            qans = a / b; \
            rans = a % b; \
            TYPE ## div ## TYPE (a, b, &q, &r); \
            if (q != qans || r != rans) { \
                puts(#TYPE " ERROR"); \
                printf(FMT " div " FMT "\n", a, b); \
                printf(FMT " " FMT "\n", q, r); \
                printf(FMT " " FMT "\n", qans, rans); \
                exit(1); \
            } \
        } \
}

MAKE_TEST_FUNC(LL, "%lld")
MAKE_TEST_FUNC(ULL, "%llu")

int main()
{
    test_LL();
    test_ULL();
    puts("TEST OK");
    exit(0);
}
