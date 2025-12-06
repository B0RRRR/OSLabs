#include <stdio.h>

#include "../inc/contract.h"

// Формула Валлиса для Pi
float Pi(int K) {
    if (K <= 0) return 0.0f;
    float pi_half_approx = 1.0f;
    for (int n = 1; n <= K; n++) {
        pi_half_approx *= (2.0f * n) / (2.0f * n - 1.0f);
        pi_half_approx *= (2.0f * n) / (2.0f * n + 1.0f);
    }
    return 2.0f * pi_half_approx;
}

// Площадь прямоугольного треугольника
float Square(float A, float B) {
    return 0.5f * A * B;
}