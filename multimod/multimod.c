#include <stdint.h>

uint64_t Add(uint64_t a, uint64_t b) //加法位运算a+b
{
    if (b)
    {
        return Add(a ^ b, (a & b) << 1);
    }
    else
    {
        return a;
    }
}
uint64_t Subt(uint64_t a, uint64_t b) //减法位运算a-b
{
    return Add(a, Add(~b, 1));
}

uint64_t Multiply(uint64_t a, uint64_t b)
{
    uint64_t sum = 0;
    while (b)
    {
        if (b & 0x1)
        {
            sum += a;
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
    for (int i = 63; i >= 0; i--)
    {
        if ((a >> i) >= b)
        {
            quotient += 1 << i;
            a -= b << i;
        }
    }

    return quotient;
}
uint64_t Remainder(uint64_t a, uint64_t b) //求余位运算a%b
{
    uint64_t c;
    c = Divide(a, b);
    return a - (Multiply(b, c));
}

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  uint64_t result = 0, tmp;
    tmp = Remainder(a, m);
    b = Remainder(b, m);
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
                result = result + tmp;
            result = Remainder(result, m);
        }
        if (tmp >> 63 == 1)
        {
            uint64_t thesub = m - tmp;
            tmp = tmp - thesub;
        }
        else
            tmp <<= 1; //计算 A*2^n的值。
        tmp = Remainder(tmp, m);
        b >>= 1;
    }
    return result;
}
