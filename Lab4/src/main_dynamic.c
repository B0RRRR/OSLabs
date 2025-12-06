#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>


typedef float (*Pi_func)(int);
typedef float (*Square_func)(float, float);

void *lib_handle = NULL;
Pi_func pi_func_ptr = NULL;
Square_func square_func_ptr = NULL;
int current_lib = 0; // 0 - не загружена, 1 - lib1, 2 - lib2

const char *LIB_PATH_1 = "./liblib1.so";
const char *LIB_PATH_2 = "./liblib2.so";


// Загружает нужную библиотеку и получает указатели на функции
int load_library(int lib_id) {
    const char *lib_path = (lib_id == 1) ? LIB_PATH_1 : LIB_PATH_2;

    // 1. Закрыть текущую библиотеку, если она открыта
    if (lib_handle != NULL) {
        dlclose(lib_handle);
        lib_handle = NULL;
        pi_func_ptr = NULL;
        square_func_ptr = NULL;
    }

    // 2. Открыть новую библиотеку
    // RTLD_LAZY: разрешать неопределенные символы при первом вызове
    lib_handle = dlopen(lib_path, RTLD_LAZY);
    if (!lib_handle) {
        fprintf(stderr, "Ошибка при загрузке библиотеки %s: %s\n", lib_path, dlerror());
        current_lib = 0;
        return 0; // Ошибка
    }

    // 3. Получить адреса функций
    pi_func_ptr = (Pi_func)dlsym(lib_handle, "Pi");
    if (dlerror() != NULL) { // dlsym возвращает NULL при ошибке или если символ - NULL
        fprintf(stderr, "Ошибка при поиске Pi в %s: %s\n", lib_path, dlerror());
        dlclose(lib_handle);
        lib_handle = NULL;
        current_lib = 0;
        return 0;
    }

    square_func_ptr = (Square_func)dlsym(lib_handle, "Square");
    if (dlerror() != NULL) {
        fprintf(stderr, "Ошибка при поиске Square в %s: %s\n", lib_path, dlerror());
        dlclose(lib_handle);
        lib_handle = NULL;
        current_lib = 0;
        return 0;
    }

    current_lib = lib_id;
    printf("Успешно загружена Реализация %d.\n", current_lib);
    return 1; // Успех
}

void process_command(int func_id, char* args[]) {
    if (lib_handle == NULL) {
        printf("Библиотека не загружена.\n");
        return;
    }

    // Вызов первой функции: Pi(K)
    if (func_id == 1) {
        if (args[0] == NULL) {
            printf("Функция 1 требует 1 аргумент (K).\n");
            return;
        }
        int K = atoi(args[0]);
        if (K <= 0) {
            printf("K должно быть положительным.\n");
            return;
        }

        // Вызов функции через указатель
        float result = pi_func_ptr(K); 
        const char *impl_name = (current_lib == 1) ? "Ряд Лейбница" : "Формула Валлиса";
        printf("Pi(%d) [%s] = %.8f\n", K, impl_name, result);
    } 
    // Вызов второй функции: Square(A, B)
    else if (func_id == 2) {
        if (args[0] == NULL || args[1] == NULL) {
            printf("Функция 2 требует 2 аргумента.\n");
            return;
        }
        float A = atof(args[0]);
        float B = atof(args[1]);
        
        // Вызов функции через указатель
        float result = square_func_ptr(A, B);
        const char *impl_name = (current_lib == 1) ? "Прямоугольник" : "Прямоугольный треугольник";
        printf("Square(%.2f, %.2f) [%s] = %.2f\n", A, B, impl_name, result);
    } else {
        printf("Неизвестная команда.\n");
    }
}

int main() {
    char line[256];
    printf("Команды:\n");
    printf("0 : Переключение реализации (1 <-> 2).\n");
    printf("1 arg1...: Вызов Pi(K).\n");
    printf("2 arg1 arg2...: Вызов Square(A, B).\n");
    
    // Начальная загрузка Реализации 1
    if (!load_library(1)) {
        fprintf(stderr, "Не удалось загрузить начальную библиотеку.\n");
        return 1;
    }

    while (1) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        // Разделение строки на токены
        char *token;
        char *tokens[3] = {NULL, NULL, NULL};
        int token_count = 0;

        token = strtok(line, " \n");
        while (token != NULL && token_count < 3) {
            tokens[token_count++] = token;
            token = strtok(NULL, " \n");
        }
        
        if (token_count == 0 || strcmp(tokens[0], "exit") == 0) break;

        if (token_count >= 1) {
            int command = atoi(tokens[0]);
            
            // Переключение реализации
            if (command == 0) {
                int next_lib = (current_lib == 1) ? 2 : 1;
                load_library(next_lib);
            } else {
                process_command(command, &tokens[1]);
            }
        }
    }

    if (lib_handle != NULL) {
        dlclose(lib_handle);
    }
    return 0;
}