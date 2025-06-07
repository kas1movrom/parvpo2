#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct {
    double a, b, c;
    double x1, x2;
    int solutions;
} QuadraticEquation;

void solve_quadratic(QuadraticEquation *eq) {
    double a = eq->a, b = eq->b, c = eq->c;
    double discriminant = b * b - 4 * a * c;

    if (a == 0) {
        if (b != 0) {
            eq->solutions = 1;
            eq->x1 = -c / b;
        } else {
            eq->solutions = (c == 0) ? -1 : 0;
        }
        return;
    }

    if (discriminant > 0) {
        eq->solutions = 2;
        double sqrt_discriminant = sqrt(discriminant);
        eq->x1 = (-b + sqrt_discriminant) / (2 * a);
        eq->x2 = (-b - sqrt_discriminant) / (2 * a);
    } else if (discriminant == 0) {
        eq->solutions = 1;
        eq->x1 = -b / (2 * a);
    } else {
        eq->solutions = 0;
    }
}

void generate_random_equations(QuadraticEquation *equations, int count) {
    for (int i = 0; i < count; i++) {
        equations[i].a = (rand() % 2000 - 1000) / 100.0;
        equations[i].b = (rand() % 2000 - 1000) / 100.0;
        equations[i].c = (rand() % 2000 - 1000) / 100.0;
    }
}

int main() {
    const int NUM_EQUATIONS = 1000000;
    QuadraticEquation *equations = malloc(NUM_EQUATIONS * sizeof(QuadraticEquation));
    
    srand(time(NULL));
    
    generate_random_equations(equations, NUM_EQUATIONS);
    
    clock_t start = clock();
    
    for (int i = 0; i < NUM_EQUATIONS; i++) {
        solve_quadratic(&equations[i]);
    }
    
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("%d equations in %.3f seconds\n", NUM_EQUATIONS, time_spent);
    
    free(equations);
    return 0;
}