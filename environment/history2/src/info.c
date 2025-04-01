#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <math.h>
#include <errno.h>

#define SAMPLE_SIZE 1000 

typedef struct {
    clockid_t clk_id;
    const char *name;
} ClockInfo;

int is_clock_supported(clockid_t clk_id) {
    struct timespec res;
    return (clock_getres(clk_id, &res) == 0);
}

void measure_clock(clockid_t clk_id, const char *name) {
    if (!is_clock_supported(clk_id)) {
        printf("[✗] %s не поддерживается\n", name);
        return;
    }

    struct timespec prev_ts, curr_ts;
    uint64_t prev_ns, curr_ns;
    uint64_t diffs[SAMPLE_SIZE];
    int count = 0;

    clock_gettime(clk_id, &prev_ts);
    prev_ns = prev_ts.tv_sec * 1000000000ULL + prev_ts.tv_nsec;

    while (count < SAMPLE_SIZE) {
        clock_gettime(clk_id, &curr_ts);
        curr_ns = curr_ts.tv_sec * 1000000000ULL + curr_ts.tv_nsec;

        if (curr_ns != prev_ns) {
            diffs[count] = curr_ns - prev_ns;
            prev_ns = curr_ns;
            count++;
        }
    }

    uint64_t sum = 0;
    for (int i = 0; i < SAMPLE_SIZE; i++) sum += diffs[i];
    double avg_ns = (double)sum / SAMPLE_SIZE;

    double variance = 0;
    for (int i = 0; i < SAMPLE_SIZE; i++) {
        variance += pow(diffs[i] - avg_ns, 2);
    }
    double stddev_ns = sqrt(variance / SAMPLE_SIZE);

    printf("[✓] %-25s | Разрешение: %8.1f нс | Погрешность: ±%.1f нс\n",
           name, avg_ns, stddev_ns);
}

int main() {
    ClockInfo clocks[] = {
        {CLOCK_REALTIME, "CLOCK_REALTIME"},
        {CLOCK_MONOTONIC, "CLOCK_MONOTONIC"},
        {CLOCK_MONOTONIC_RAW, "CLOCK_MONOTONIC_RAW"},
        {CLOCK_BOOTTIME, "CLOCK_BOOTTIME"},
        {CLOCK_PROCESS_CPUTIME_ID, "CLOCK_PROCESS_CPUTIME_ID"},
        {CLOCK_THREAD_CPUTIME_ID, "CLOCK_THREAD_CPUTIME_ID"},
        {CLOCK_REALTIME_COARSE, "CLOCK_REALTIME_COARSE"},
        {CLOCK_MONOTONIC_COARSE, "CLOCK_MONOTONIC_COARSE"},
	    {CLOCK_REALTIME_ALARM, "CLOCL_REALTIME_ALARM"},
	    {CLOCK_TAI, "CLOCK_TAI"},
	    {CLOCK_BOOTTIME_ALARM, "CLOCK_BOOTTIME_ALARM"}
    };

    for (size_t i = 0; i < sizeof(clocks) / sizeof(clocks[0]); i++) {
        measure_clock(clocks[i].clk_id, clocks[i].name);
    }

    return 0;
}
