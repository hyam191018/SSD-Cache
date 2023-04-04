#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "cache_api.h"
#include "spdk.h"
#include "stdinc.h"

/**
 *  Author: Hyam
 *  Date: 2023/03/28
 *  Description: 壓力測試 admin submit pio (read only)
 */

/**
 * 實驗記錄 跑1000萬次
 * hit time = 4989637, miss time = 5010363, hit ratio = 49.90%
 * 292.30user 49.55system 23:11.36elapsed 24%CPU (0avgtext+0avgdata 12008maxresident)k
 * 17998592inputs+20038176outputs (1major+2320minor)pagefaults 0swaps
 */

#define ROUND 10000000  // submit次數
#define EXCEPT 50       // 期望的 hit ratio
#define FILE_NAME "testfile"
const unsigned long long MAX_PAGE_INDEX =
    (CACHE_BLOCK_NUMBER * CACHE_BLOCK_SIZE * 100ull) / (1024 * EXCEPT * 4ull);

// 隨機4K讀寫
static void send_pio(void) {
    unsigned page_index = rand() % MAX_PAGE_INDEX;
    operate operation = rand() % 2 ? READ : WRITE;
    void* buffer = alloc_dma_buffer(PAGE_SIZE);
    unsigned pio_cnt = 1;
    struct pio* head = create_pio(FILE_NAME, 0, page_index, operation, buffer, pio_cnt);
    submit_pio(head);
    free_pio(head);
    free_dma_buffer(buffer);
}

int main(int argc, char* argv[]) {
    // 開啟 udm-cache
    printf("%d init rc = %d\n", getpid(), init_udm_cache());

    for (int i = 0; i < ROUND; i++)
        send_pio();

    // 關閉 udm-cache
    info_udm_cache();
    printf("%d exit rc = %d\n", getpid(), exit_udm_cache());
    return 0;
}