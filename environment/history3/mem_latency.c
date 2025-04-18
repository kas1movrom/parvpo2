#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

#define ITERATIONS (100000000UL) 

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file> <k>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    uint64_t k = strtoull(argv[2], NULL, 10);

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1 || file_size < 8) {
        perror("lseek");
        close(fd);
        return 1;
    }

    if (k > (uint64_t)file_size - 8) {
        fprintf(stderr, "k must be <= file_size - 8\n");
        close(fd);
        return 1;
    }

    uint8_t *mem = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (mem == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    uint64_t state = 123456789;
    const uint64_t multiplier = 6364136223846793005ULL;
    const uint64_t increment = 1ULL;

    volatile uint64_t sum = 0; 

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (uint64_t i = 0; i < ITERATIONS; ++i) {
        state = state * multiplier + increment;
        uint64_t offset = state % k;
        sum += *(uint64_t*)(mem + offset);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    uint64_t delta_ns = (end.tv_sec - start.tv_sec) * 1000000000ULL + 
                       (end.tv_nsec - start.tv_nsec);
    printf("k=%lu, time=%.2f ns/iter, sum=%lu\n", 
           k, (double)delta_ns / ITERATIONS, (uint64_t)sum);

    munmap(mem, file_size);
    return 0;
}