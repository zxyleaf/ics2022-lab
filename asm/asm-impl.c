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
  int x=0;
  asm ("xor %%rax, %%rax\n"
       "movl $0x0, -0x4(%%rbp)\n"
       "dest2:mov %[in], -0x18(%%rbp)\n"
       "and $0x1, %[in]\n"
       "test %[in], %[in]\n"
       "je dest1\n"
       "inc %%rax\n"
       "dest1:mov -0x18(%%rbp),%[in]\n"
       "shr $0x1, %[in] \n"
       "incl -0x4(%%rbp)\n"
       "cmp $0x3f,-0x4(%%rbp)\n"
       "jle dest2\n"
       "mov %%rax, -0x18(%%rbp)\n"
       "mov -0x18(%%rbp), %[out]\n"
       : [out] "+g"(x)
       : [in] "r" (n)
       : "%rax","cc","memory");
  
  return x;
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
