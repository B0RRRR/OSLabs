#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/contract.h"


void process_command(int func_id, char* args[]) {
    // Вызов первой функции: Pi(K)
    if (func_id == 1) {
        if (args[0] == NULL) {
            printf("Ошибка: Функция 1 требует 1 аргумент (K).\n");
            return;
        }
        int K = atoi(args[0]);
        if (K <= 0) {
            printf("Ошибка: K должно быть положительным.\n");
            return;
        }
        float result = Pi(K); // Вызов функции из библиотеки
        printf("Результат Pi(%d) = %.8f\n", K, result);
    } 
    // Вызов второй функции: Square(A, B)
    else if (func_id == 2) {
        if (args[0] == NULL || args[1] == NULL) {
            printf("Ошибка: Функция 2 требует 2 аргумента (A, B).\n");
            return;
        }
        float A = atof(args[0]);
        float B = atof(args[1]);
        
        float result = Square(A, B); // Вызов функции из библиотеки
        printf("Результат Square(%.2f, %.2f) = %.2f\n", A, B, result);
    } else {
        printf("Неизвестная команда.\n");
    }
}

int main() {
    char line[256];
    printf("Используется Реализация 1 (Ряд Лейбница / Прямоугольник)\n");
    printf("Формат: <ID функции> <arg1> <arg2>...\n");
    printf("Введите 'exit' для выхода.\n\n");

    while (1) {
        printf(">>> ");
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        // Разделение строки на токены
        char *token;
        char *tokens[3] = {NULL, NULL, NULL}; // Макс. 3 токена (ID + 2 аргумента)
        int token_count = 0;

        token = strtok(line, " \n");
        while (token != NULL && token_count < 3) {
            tokens[token_count++] = token;
            token = strtok(NULL, " \n");
        }
        
        if (token_count == 0 || strcmp(tokens[0], "exit") == 0) break;

        if (token_count >= 1) {
            int func_id = atoi(tokens[0]);
            if (func_id != 0) {
                process_command(func_id, &tokens[1]);
            } else {
                printf("Команда '0' не поддерживается\n");
            }
        }
    }

    return 0;
}