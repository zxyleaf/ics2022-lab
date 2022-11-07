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
  int ans;
  asm(
  "movl $0, %%eax;"//ans
  "movl $0, %%ecx;"//i
  "cycle_popcnt:cmpl $64, %%ecx;"
  "jge label;"
  "movq %%rbx, %%rdx;"
  "shrq %%ecx, %%rdx;"
  "and $1, %%rdx;"
  "cmpq $1, %%rdx;"
  "je t1;"
  "jmp t2;"
  "t1:"
  "incl %%eax;"
  "t2:"
  "incl %%ecx;"
  "jmp cycle_popcnt;"
  "label:"
  :"=a"(ans)
  :"b"(x));
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
  return pos;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  return;
}
