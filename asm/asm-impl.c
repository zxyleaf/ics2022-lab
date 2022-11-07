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
        "movl $0, %%edi;"       // int i = 0
        "loop:cmpl $0x40, %%edi;" // for (i < 64)
        "jge e;"
        "movq %%rbx, %%rdx;" //  rdx -- x
        "and $1, %%rdx;"
        "add %%edx, %%eax;"
        "shrq $1, %%rbx;" // rdx --x =  x >> 1
        "incl %%edi;" // i++
        "jmp loop;"
        "e:"
        : "=a"(ans) // eax -- ans
        : "b"(x));  // rbx -- x
    return ans;
}

void *asm_memcpy(void *dest, const void *src, size_t n)
{
    //void *pos = dest;
    asm("movq $0,%%rdi;"
        "loop2: cmpq %%rdx, %%rdi;"
        "jae e2;"
        "movzb (%%rcx),%%rax;"
    	"mov %%rax,(%%rbx);"
    	"add $1,%%rcx;"
    	"add $1,%%rbx;"
    	"add $1,%%rdi;"
        "jmp loop2;"
        "e2:"
        : "=b"(dest)
        : "b"(dest), "c"(src), "d"(n));
    return dest;
}

int asm_setjmp(asm_jmp_buf env)
{
    asm ("mov %%rdi, %%rax\n"
       "mov %%rbx, (%%rax)\n" // * 保存rbx 到rdx的内容中
       "mov %%rsi, 8(%%rax)\n"
       "mov %%rdi, 16(%%rax)\n" 
       "mov (%%rsp), %%rcx\n" //
       "mov %%rcx, 24(%%rax)\n" // * rsp存放rbp的旧址
       
       "lea 0x10(%%rsp), %%rcx\n"
       "mov %%rcx, 32(%%rax)\n" // * rsp+10的地址是rsp的旧值
       "mov 0x8(%%rsp), %%rcx\n"   
       "mov %%rcx, 40(%%rax)\n"  // * rsp+8存放pc
       :
       :
       : );
       return 0;
}

void asm_longjmp(asm_jmp_buf env, int val)
{
     asm ("mov %%rdi, %%rdx\n"
       "mov %%esi, %%eax\n"
       "mov (%%rdx), %%rbx\n" 
       "mov 8(%%rdx), %%rsi\n"
       "mov 16(%%rdx), %%rdi\n"
       "mov 24(%%rdx), %%rbp\n"
       "mov 32(%%rdx), %%rsp\n"
       "jmpq *40(%%rdx)\n"
       :
       : 
       :);
}
