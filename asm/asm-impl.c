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
  int cnt = 0;
    asm("mov %1, -0x18(%%rbp)\n"
        "movl $0x0, -0x8(%%rbp)\n" // i
        "jmp dest1\n"
        "dest3: mov -0x8(%%rbp), %%eax\n"
        "mov -0x18(%%rbp), %%rdx\n"
        "mov %%eax, %%ecx\n"
        "shr %%cl, %%rdx\n"
        "mov %%rdx, %%rax\n"
        "and $0x1, %%eax\n"  //???
        "test %%rax,%%rax\n" // 如果rax为零，设置ZF零标志为1
        "je dest2\n"
        "addl $0x1, %0\n"
        "dest2: addl $0x1, -0x8(%%rbp)\n" // i++
        "dest1: cmpl $0x3f, -0x8(%%rbp)\n"
        "jle dest3\n"
        : "+r"(cnt) //占位符 %0
        : "r"(x)    //占位符 %1
        : "%rax", "%eax", "%ecx", "%rdx", "%cl"

    );
    return cnt;
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
