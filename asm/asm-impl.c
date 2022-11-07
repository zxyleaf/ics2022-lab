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
        "movzbl (%%rcx),%%rax;"
    	"mov %%rax,(%%rbx);"
    	"add $1,%%rcx;"
    	"add $1,%%rbx;"
    	"add $1,%%rsi;"
        "jmp loop2;"
        "e2:"
        : "=b"(dest)
        : "b"(dest), "c"(src), "d"(n));
    return dest;
}

int asm_setjmp(asm_jmp_buf env)
{
    asm ("mov %[env], %%rdx\n"
       "mov %%rbx, (%%rdx)\n" // * 保存rbx 到rdx的内容中
       "mov (%%rsp), %%rax\n" //
       "mov %%rax, 0x8(%%rdx)\n" // * rsp存放rbp的旧址
       "mov %%r12, 0x10(%%rdx)\n"
       "mov %%r13, 0x18(%%rdx)\n"
       "mov %%r14, 0x20(%%rdx)\n"
       "mov %%r15, 0x28(%%rdx)\n" //被调用者保存寄存器
       "lea 0x10(%%rsp), %%rax\n"
       "mov %%rax, 0x30(%%rdx)\n" // * rsp+10的地址是rsp的旧值
       "mov 0x8(%%rsp), %%rax\n"   
       "mov %%rax, 0x38(%%rdx)\n"  // * rsp+8存放pc
       :
       : [env] "m"(env)
       : "%rax", "cc", "memory");
       return 0;
}

void asm_longjmp(asm_jmp_buf env, int val)
{
     asm ("mov %[env], %%rdx\n"
       "mov (%%rdx), %%rbx\n" 
       "mov 0x10(%%rdx), %%r12\n"
       "mov 0x18(%%rdx), %%r13\n"
       "mov 0x20(%%rdx), %%r14\n"
       "mov 0x28(%%rdx), %%r15\n"
       "mov %[val], %%rax\n"
       "mov 0x30(%%rdx), %%rsp\n"
       "mov 0x8(%%rdx), %%rbp\n"
       "mov 0x38(%%rdx), %%rdx\n"
       "jmpq *%%rdx\n"
       :
       : [env] "m"(env), [val] "m"(val)
       : "cc", "memory");
}
