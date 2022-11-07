#include "asm.h"
#include <string.h>

int64_t asm_add(int64_t a, int64_t b)
{
    int64_t c = 0;
    asm(
        "add %2, %0"
        : "=g"(c)
        : "0"(a), "g"(b)
        : "memory");
    return c;
}

int asm_popcnt(uint64_t x)
{
    int ans;
    asm(
        "movl $0, %%eax;"       // int ans = 0
        "movl $0, %%ecx;"       // int i = 0
        "loop:cmpl $64, %%ecx;" // for (i < 64)
        "jge e;"
        "movq %%rbx, %%rdx;" //  rdx -- x
        "shrq %%ecx, %%rdx;" // rdx -- x >> i
        "and $1, %%rdx;"     // rdx -- x = x & 1
        "cmpq $1, %%rdx;"    // rdx -- x == 0 ?
        "je t1;"
        "jmp t2;"
        "t1:"
        "incl %%eax;" // ans++
        "t2:"
        "incl %%ecx;" // i++
        "jmp loop;"
        "e:"
        : "=a"(ans) // eax -- ans
        : "b"(x));  // rbx -- x
    return ans;
}

void *asm_memcpy(void *dest, const void *src, size_t n)
{
    //void *pos = dest;
    asm("movq $0,%%rsi;"
        "loop2: cmpq %%rcx, %%rsi;"
        "jae e2;"
        "mov (%%rbx,%%rsi,1),%%dl;"
        "mov %%dl, (%%rax,%%rsi,1);"
        "addq $0x1, %%rsi;"  //rsi ++
        "jmp loop2;"
        "e2:"
        : "=a"(dest)
        : "a"(dest), "b"(src), "c"(n));
    return dest;
}

int asm_setjmp(asm_jmp_buf env)
{
    return 0;
}

void asm_longjmp(asm_jmp_buf env, int val)
{
    return;
}
