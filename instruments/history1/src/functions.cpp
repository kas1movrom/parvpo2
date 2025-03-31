#include "../headers/functions.h"

double get_discriminant(double a, double b, double c) {
    return (b * b - 4 * a * c);
}

double get_first_solution(double a, double b, double dis) {
    return (-b + sqrt(dis)) / (2 * a);
}

double get_second_solution(double a, double b, double dis) {
    return (-b - sqrt(dis)) / (2 * a);
}