#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <math.h>

#define SAMPLE_SIZE 1000

int main() {
    struct timespec prev_ts, curr_ts;
    uint64_t prev_ns, curr_ns;
    uint64_t diffs[SAMPLE_SIZE];
    int count = 0;

    clock_gettime(CLOCK_REALTIME, &prev_ts);
    prev_ns = prev_ts.tv_sec * 1000000000ULL + prev_ts.tv_nsec;

    while (count < SAMPLE_SIZE) {
        clock_gettime(CLOCK_REALTIME, &curr_ts);
        curr_ns = curr_ts.tv_sec * 1000000000ULL + curr_ts.tv_nsec;

        if (curr_ns != prev_ns) {
            diffs[count] = curr_ns - prev_ns;
            prev_ns = curr_ns;
            count++;
        }
    }

    uint64_t sum = 0;
    for (int i = 0; i < SAMPLE_SIZE; i++) {
        sum += diffs[i];
    }
    double avg_resolution_ns = (double)sum / SAMPLE_SIZE;

    double variance = 0;
    for (int i = 0; i < SAMPLE_SIZE; i++) {
        variance += pow(diffs[i] - avg_resolution_ns, 2);
    }
    double stddev_ns = sqrt(variance / SAMPLE_SIZE);

    printf("Среднее разрешение: %.1f нс\n", avg_resolution_ns);
    printf("Погрешность (σ): ±%.1f нс\n", stddev_ns);

    return 0;
}
