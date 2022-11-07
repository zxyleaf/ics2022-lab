#include "asm.h"
#include <string.h>

int64_t asm_add(int64_t a, int64_t b) {
  int64_t c = 0;
  asm(
        "add %2, %0"      
        : "=g"(c)          
        : "0"(a), "g"(b)    
        : "memory");        
  return c;
}

int asm_popcnt(uint64_t x) {
  int ans = 0;
    asm(
        "movq %1, -0x18(%%rbp)\n"
        "movl $0x0, -0x8(%%rbp)\n" // i
        "themain:\n"
        "cmpl $0x3f, -0x8(%%rbp)\n"
        "jge e0\n" // 63小则跳转
        "mov -0x8(%%rbp), %%eax\n"
        "movq -0x18(%%rbp), %%rdx\n"
        "mov %%eax, %%ecx\n"
        "shr %%cl, %%rdx\n"
        "movq %%rdx, %%rax\n"
        "and $0x1, %%eax\n"
        "cmp $0x0,%%rax\n" // 如果rax为零，设置ZF零标志为1   "cmp %%eax $0\n\t"
        "je e1\n"
        "addl $0x1, %0\n"
        "e1:\n"
        "addl $0x1, -0x8(%%rbp)\n"
        "jmp themain\n"
        "e0:\n"
        : "+r"(ans)
        : "r"(x)
        : "%rax", "%eax", "%ecx", "%rdx", "%cl");
    return ans;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  void *pos = dest;
    asm("mov %0, -0x18(%%rbp)\n" // dest
        "mov %1, -0x20(%%rbp)\n" // src
        "mov %2, -0x28(%%rbp)\n" // n
        "mov -0x18(%%rbp), %%rax\n"
        "mov %%rax, %3\n"          //传递pos参数
        "movl $0x0, -0xc(%%rbp)\n" // loop variable
        "jmp start\n"
        "loop:\n"
        "mov -0x20(%%rbp),%%rax\n"
        "movzbl (%%rax),%%edx\n"
        "mov -0x18(%%rbp),%%rax\n"
        "mov %%dl, (%%rax)\n"
        "addq $0x1, -0x18(%%rbp)\n"
        "addq $0x1, -0x20(%%rbp)\n"
        "addq $0x1, -0xc(%%rbp)\n"
        "start:\n"
        "mov -0xc(%%rbp),%%rax\n"
        "cltq\n"
        "cmp %%rax, -0x28(%%rbp)\n"
        "ja loop\n"
        :
        : "r"(dest), "r"(src), "r"(n), "r"(pos)
        : "%rax", "%edx", "%rdx", "%dl");
    // printf("%s\n",(char*)pos);
    return pos;
}

int asm_setjmp(asm_jmp_buf env) {
  return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  return;
}
