#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

int sum_floats_in_str(const char *s, double *out_sum) {
    if (!s || !out_sum) return -1;
    *out_sum = 0.0;
    const char *p = s;
    char *end;
    int found = 0;

    while (*p != '\0') {
        while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;
        if (*p == '\0') break;

        double val = strtod(p, &end);
        if (end == p) { p++; continue; }
        *out_sum += val;
        found = 1;
        p = end;
    }
    return found ? 0 : 1;
}
