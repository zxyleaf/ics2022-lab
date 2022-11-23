#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define N 10000000

static bool is_prime[N];
static int  primes[N];
/*
int *sieve(int n) {
  assert(n + 1 < N);
  for (int i = 0; i <= n; i++)
    is_prime[i] = true;

  for (int i = 2; i <= n; i++) {
    for (int j = i + i; j <= n; j += i) {
      is_prime[j] = false;
    }
  }

  int *p = primes;
  for (int i = 2; i <= n; i++)
    if (is_prime[i]) {
      *p++ = i;
    }
  *p = 0;
  return primes;
}*/

int *sieve(int n) {
 assert(n + 1 < N);
  primes[0]=2;
  int count = 1;
  for (int i = 3; i <= n; i += 2)
    is_prime[i] = true;

        for (int i = 3; i <= n; i+=2) {
            if (is_prime[i] == 1)
                primes[count++] = i;
            //循环控制表达式的意义：j小于等于素数数组的个数 或 素数数组中的每一个素数与 i 的积小于范围上限N
            for (int j = 1; (j < count) && (primes[j] * (long long)i) <= n; j++)//将i强制转换是因为vs上有warning，要求转换为宽类型防止算术溢出。数据上不产生影响
            {
                is_prime[i * primes[j]] = false;//每一个素数的 i 倍（i >= 2）都不是素数，置为false

                //这个是欧拉筛法的核心，它可以减少非素数置false的重复率
                //意义是将每一个合数（非素数）拆成 2（最小因数）与最大因数 的乘积
                if (i % primes[j] == 0)
                    break;
            }
        }
  primes[count]=0;
  return primes;
  /*
  primes[0]=2;
  int cnt=1;
  for (int i=3;i<=n;i+=2)
  {
    is_prime[i]=1;
  }

  for (int i=3;i<=n;i+=2)
  {
    if (is_prime[i])
    {
      primes[cnt++]=i;
    }


    for (int j=1;j<cnt && i*primes[j]<=n;j++)
    {
      is_prime[i*primes[j]]=0;

      if (i%primes[j]==0)
      {
        break;
      }
    }
  }

  primes[cnt]=0;
  return primes;*/
}


