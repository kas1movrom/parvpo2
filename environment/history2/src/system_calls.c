#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <string.h>

void print_clock_res(clockid_t clk_id, const char *name) {
    struct timespec res;
    if (clock_getres(clk_id, &res)) {
        printf("[✗] %s: ошибка (%s)\n", name, strerror(errno));
    } else {
        printf("[✓] %-20s: разрешение = %ld нс\n", name, res.tv_nsec);
    }
}

int main() {
    print_clock_res(CLOCK_REALTIME, "CLOCK_REALTIME");
    print_clock_res(CLOCK_MONOTONIC, "CLOCK_MONOTONIC");
    print_clock_res(CLOCK_MONOTONIC_RAW, "CLOCK_MONOTONIC_RAW");
    print_clock_res(CLOCK_BOOTTIME, "CLOCK_BOOTTIME");
    print_clock_res(CLOCK_PROCESS_CPUTIME_ID, "CLOCK_PROCESS_CPUTIME_ID");
    print_clock_res(CLOCK_THREAD_CPUTIME_ID, "CLOCK_THREAD_CPUTIME_ID");
    print_clock_res(CLOCK_REALTIME_COARSE, "CLOCK_REALTIME_COARSE");
    print_clock_res(CLOCK_MONOTONIC_COARSE, "CLOCK_MONOTONIC_COARSE");

    print_clock_res(CLOCK_REALTIME_ALARM, "CLOCL_REALTIME_ALARM");
    print_clock_res(CLOCK_TAI, "CLOCK_TAI");
	print_clock_res(CLOCK_BOOTTIME_ALARM, "CLOCK_BOOTTIME_ALARM");

    return 0;
}