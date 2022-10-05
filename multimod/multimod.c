#include <stdint.h>

uint64_t Multiply(uint64_t a, uint64_t b)
{
    uint64_t sum = 0;
    while (b)
    {
        if (b & 0x1)
        {
            sum = sum + a;
        }
        a = a << 1;
        b = b >> 1;
    }
    return sum;
}
uint64_t Divide(uint64_t a, uint64_t b)
{
    uint64_t quotient = 0;
    if (!b)
    {
        return 1;
    }
    for (int i = 63; i--; i > 0)
    {
        if ((a >> i) >= b)
        {
            quotient = quotient + (1 << i);
            a = a - (b << i);
        }
    }

    return quotient;
}
uint64_t Remainder(uint64_t a, uint64_t b) //求余位运算a%b
{
    uint64_t c, f;
    c = Divide(a, b);
    f = a - (Multiply(b, c));
    return f;
}

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  uint64_t result = 0, tmp;
    tmp = Remainder(a, m);

    while (b)
    {
        if (b & 1) //每次求b的最低位 如果是1则进
        {
            uint64_t ifcf = result + tmp;
            if ((result >> 63 == 1 && tmp >> 63 == 1) || (result >> 63 == 1 && ifcf >> 63 == 0 && tmp >> 63 == 0) || (result >> 63 == 0 && ifcf >> 63 == 0 && tmp >> 63 == 1))
            {
                uint64_t thesub = m - tmp;
                result = result - thesub;
            }
            else
                result += tmp;
            while (result >= m)
                result -= m;
        }
        if (tmp >> 63 == 1)
        {
            uint64_t thesub = m - tmp;
            tmp = tmp - thesub;
        }
        else
            tmp <<= 1; //计算 A*2^n的值。
        while (tmp >= m)
            tmp -= m;
        b >>= 1;
    }
    return result;
}
