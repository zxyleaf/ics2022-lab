#include "common.h"
#include <inttypes.h>
#include <string.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static int CACHE_LINE_NUM;
static int CACHE_GROUP_NUM;
static int CACHE_ALL_LINE_NUM;
static int CACHE_GROUP_WIDTH;
typedef struct{
  bool dirty_bit : 1;
  bool valid_bit : 1;
  uint32_t tag : 13;
  uint8_t data[BLOCK_SIZE];
}cache_line;

static cache_line **cache;

static uint64_t hit_num = 0;
static uint64_t miss_num = 0;
static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions

uint32_t cache_read(uintptr_t addr) {
  addr &= ~0x3;
  uint32_t block_addr = addr & (BLOCK_SIZE - 1);
  uint32_t group_id = (addr >> BLOCK_WIDTH) & (CACHE_GROUP_NUM - 1);
  uint32_t tag = addr >> (BLOCK_WIDTH + CACHE_GROUP_WIDTH);
  uint32_t block_num  = addr >> (BLOCK_WIDTH);
  uint32_t *ret;

  for (int i = 0; i < CACHE_LINE_NUM; i++)
  {
    if (cache[group_id][i].valid_bit && cache[group_id][i].tag == tag)
    {
      hit_num++;
      ret = (void *)(cache[group_id][i].data + (block_addr));
      return *ret;
    }
  }

  miss_num++;
  for (int i = 0; i < CACHE_LINE_NUM; i++)
  {
    if (cache[group_id][i].valid_bit == false)
    {
      mem_read(block_num, cache[group_id][i].data);
      cache[group_id][i].valid_bit = true;
      cache[group_id][i].tag = tag;
      ret = (void *)(cache[group_id][i].data + (block_addr));
      return *ret;
    }
  }

  int line = rand() % CACHE_LINE_NUM;
  mem_read(block_num, cache[group_id][line].data);
  cache[group_id][line].valid_bit = true;
  cache[group_id][line].tag = tag;
  ret = (void *)cache[group_id][line].data + (block_addr);
  return *ret;
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  addr &= ~0x3;
  uint32_t block_addr = addr & (BLOCK_SIZE - 1);
  uint32_t group_id = (addr >> BLOCK_WIDTH) & (CACHE_GROUP_NUM - 1);
  uint32_t tag = addr >> (BLOCK_WIDTH + CACHE_GROUP_WIDTH);
  uint32_t block_num  = addr >> (BLOCK_WIDTH);
  for (int i = 0; i < CACHE_LINE_NUM; i++)
  {
    if (cache[group_id][i].valid_bit && cache[group_id][i].tag == tag)
    {
      uint32_t *tmp =(void *)(cache[group_id][i].data + (addr % BLOCK_SIZE));
      *tmp=(*tmp & ~wmask) | (data & wmask);
      mem_write(block_num, cache[group_id][i].data);
      return;
    }
  }
   miss_num++;
  for (int i = 0; i < CACHE_LINE_NUM; i++)
  {
    if (cache[group_id][i].valid_bit == false)
    {
      mem_read(block_num, cache[group_id][i].data);

      cache[group_id][i].tag = tag;
      cache[group_id][i].valid_bit = true;
      uint32_t *tmp =(void *)(cache[group_id][i].data + (addr % BLOCK_SIZE));
      *tmp = (*tmp & ~wmask) | (data & wmask);
      mem_write(block_num, cache[group_id][i].data);
      return ;
    }
  }

  int line = rand() % CACHE_LINE_NUM;
  mem_read(block_num, cache[group_id][line].data);
  cache[group_id][line].tag = tag;
  uint32_t *tmp = (void *)(cache[group_id][line].data + (addr % BLOCK_SIZE));
  *tmp = (*tmp & ~wmask) | (data & wmask);
  mem_write(block_num,cache[group_id][line].data);
  return ;

}

void init_cache(int total_size_width, int associativity_width) {
  CACHE_LINE_NUM = exp2(associativity_width);
  CACHE_GROUP_NUM = (exp2(total_size_width) / BLOCK_SIZE) / CACHE_LINE_NUM;
  CACHE_ALL_LINE_NUM = exp2(total_size_width - BLOCK_WIDTH);
  CACHE_GROUP_WIDTH = total_size_width - BLOCK_WIDTH - associativity_width;
  cache = (cache_line**)malloc(sizeof(cache_line*)*CACHE_GROUP_NUM);
  for (int i = 0; i < CACHE_GROUP_NUM; i++){
    cache[i] = (cache_line*)malloc(sizeof(cache_line)*CACHE_LINE_NUM);
  }
  for (int i = 0; i < CACHE_GROUP_NUM; i++){
    for (int j = 0; j < CACHE_LINE_NUM; j++){
      cache[i][j].valid_bit = 0;
	  memset(cache[i][j].data, 0, sizeof(cache[i][j].data));
	}
  }
}

void display_statistic(void) {
}
