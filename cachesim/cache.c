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
  uint32_t tag;
  uint8_t data[BLOCK_SIZE];
}cache_line;

static cache_line **cache;

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions

uint32_t cache_read(uintptr_t addr) {
  return 0;
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
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
	  cache[i][j].dirty_bit = 0;
	  memset(cache[i][j].data, 0, sizeof(cache[i][j].data));
	}
  }
}

void display_statistic(void) {
}
