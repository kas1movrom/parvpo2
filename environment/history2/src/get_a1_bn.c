#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>

#define NUM_ITERATIONS 100000
#define NUM_CLOCKS 11

typedef struct {
    clockid_t id;
    const char *name;
    double init_time;    
    double return_time;  
    double init_error;
    double return_error;
} ClockMeasurement;

void measure_clocks(ClockMeasurement *base, ClockMeasurement *target) {
    struct timespec t1, t2, t3, t4;
    double aa[NUM_ITERATIONS], ab[NUM_ITERATIONS], ba[NUM_ITERATIONS], bb[NUM_ITERATIONS];
    
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        clock_gettime(base->id, &t1);
        clock_gettime(base->id, &t2);
        aa[i] = (t2.tv_sec - t1.tv_sec) * 1e9 + (t2.tv_nsec - t1.tv_nsec);
        
        clock_gettime(base->id, &t1);
        clock_gettime(target->id, &t2);
        ab[i] = (t2.tv_sec - t1.tv_sec) * 1e9 + (t2.tv_nsec - t1.tv_nsec);
        
        clock_gettime(target->id, &t1);
        clock_gettime(base->id, &t2);
        ba[i] = (t2.tv_sec - t1.tv_sec) * 1e9 + (t2.tv_nsec - t1.tv_nsec);
        
        clock_gettime(target->id, &t1);
        clock_gettime(target->id, &t2);
        bb[i] = (t2.tv_sec - t1.tv_sec) * 1e9 + (t2.tv_nsec - t1.tv_nsec);
    }
    
    double sum_aa = 0, sum_ab = 0, sum_ba = 0, sum_bb = 0;
    double sum_aa_sq = 0, sum_ab_sq = 0, sum_ba_sq = 0, sum_bb_sq = 0;
    
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        sum_aa += aa[i]; sum_ab += ab[i]; sum_ba += ba[i]; sum_bb += bb[i];
        sum_aa_sq += aa[i]*aa[i]; sum_ab_sq += ab[i]*ab[i];
        sum_ba_sq += ba[i]*ba[i]; sum_bb_sq += bb[i]*bb[i];
    }
    
    double avg_aa = sum_aa / NUM_ITERATIONS;
    double avg_ab = sum_ab / NUM_ITERATIONS;
    double avg_ba = sum_ba / NUM_ITERATIONS;
    double avg_bb = sum_bb / NUM_ITERATIONS;
    
    base->init_time = (avg_aa + avg_ab - avg_ba) / 2;
    base->return_time = (avg_aa - avg_ab + avg_ba) / 2;
    target->init_time = (-avg_aa + avg_ab + avg_ba) / 2;
    target->return_time = (avg_aa + avg_ab + avg_ba - 2*avg_bb) / 2;
    
    base->init_error = sqrt((sum_aa_sq - sum_aa*sum_aa/NUM_ITERATIONS) / (NUM_ITERATIONS*(NUM_ITERATIONS-1)));
    target->init_error = sqrt((sum_bb_sq - sum_bb*sum_bb/NUM_ITERATIONS) / (NUM_ITERATIONS*(NUM_ITERATIONS-1)));
    base->return_error = base->init_error;
    target->return_error = target->init_error;
}

int main() {
    ClockMeasurement clocks[NUM_CLOCKS] = {
        {CLOCK_REALTIME, "CLOCK_REALTIME"},
        {CLOCK_MONOTONIC, "CLOCK_MONOTONIC"},
        {CLOCK_MONOTONIC_RAW, "CLOCK_MONOTONIC_RAW"},
        {CLOCK_BOOTTIME, "CLOCK_BOOTTIME"},
        {CLOCK_PROCESS_CPUTIME_ID, "CLOCK_PROCESS_CPUTIME_ID"},
        {CLOCK_THREAD_CPUTIME_ID, "CLOCK_THREAD_CPUTIME_ID"},
        {CLOCK_REALTIME_COARSE, "CLOCK_REALTIME_COARSE"},
        {CLOCK_MONOTONIC_COARSE, "CLOCK_MONOTONIC_COARSE"},
        {CLOCK_REALTIME_ALARM, "CLOCK_REALTIME_ALARM"},
        {CLOCK_TAI, "CLOCK_TAI"},
        {CLOCK_BOOTTIME_ALARM, "CLOCK_BOOTTIME_ALARM"}
    };
    
    printf("Измерение времени инициализации и возврата системных вызовов:\n");
    printf("------------------------------------------------------------\n");
    printf("%-25s %-15s %-15s\n", "Тип часов", "Время иниц. (нс)", "Время возвр. (нс)");
    printf("------------------------------------------------------------\n");
    
    for (int i = 1; i < NUM_CLOCKS; i++) {
        measure_clocks(&clocks[0], &clocks[i]);
        
        printf("%-25s %6.1f ± %-5.1f %6.1f ± %-5.1f\n", 
               clocks[0].name, clocks[0].init_time, clocks[0].init_error,
               clocks[0].return_time, clocks[0].return_error);
        
        printf("%-25s %6.1f ± %-5.1f %6.1f ± %-5.1f\n", 
               clocks[i].name, clocks[i].init_time, clocks[i].init_error,
               clocks[i].return_time, clocks[i].return_error);
        
        printf("------------------------------------------------------------\n");
    }
    
    return 0;
}