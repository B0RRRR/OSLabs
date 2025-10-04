#include <stdio.h>
#include <stdlib.h>

int main(void) {
    float x, sum = 0.0f;
    int read_count = 0;   // счетчик количества чисел в строке
    int c;

    while (1) {
        int r = scanf("%f", &x);

        if (r == 1) {
            sum += x;
            read_count++;
        } else if (r == EOF) {
            /* конец файла */
            if (read_count > 0) {
                printf("Sum: %.2f\n", sum);
            }
            break;
        } else {
            /* scanf не смог прочитать число */
            printf("Not number in file!\n");
            return -1;
        }

        // Проверяем, что дальше либо пробел, либо таб, либо перевод строки, либо EOF
        c = getchar();
        if (c == '\n' || c == EOF) {
            if (read_count > 0) {
                printf("Sum: %.2f\n", sum);
                sum = 0.0f;
                read_count = 0;
            }
            if (c == EOF) break;
        } else if (c == ' ' || c == '\t' || c == '\r') {
            // допустимые разделители
        } else {
            // если нашли некорректный символ
            printf("Error: Incorrect Symbol '%c'\n", c);
            return -1;
        }
    }

    return 0;
}