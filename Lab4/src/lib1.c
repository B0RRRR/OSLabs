#include <math.h>

#include "../inc/contract.h"

// Ряд Лейбница для Pi
float Pi(int K) {
    float pi_approx = 0.0f;
    for (int n = 0; n < K; n++) {
        if (n % 2 == 0) {
            pi_approx += 1.0f / (2.0f * n + 1.0f);
        } else {
            pi_approx -= 1.0f / (2.0f * n + 1.0f);
        }
    }
    return 4.0f * pi_approx;
}

// Площадь прямоугольника (A * B)
float Square(float A, float B) {
    return A * B;
}