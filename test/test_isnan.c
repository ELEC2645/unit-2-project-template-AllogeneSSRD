// Filename: test\test_isnan.c
#include <stdio.h>
#include <math.h>

int main() {
    double nan_value = NAN;
    double normal_value = 1.0;

    int count = 0;
    count = isnan(nan_value);      // should be 1
    printf("isnan test count: %d\n", count); // Expected output: 1
    // isnan test count: -1

    count = isnan(normal_value);   // should be 0
    printf("isnan test count: %d\n", count); // Expected output: 0
    // isnan test count: 0

    printf("sizeof(isnan(nan_value)): %d\n", sizeof(isnan(nan_value)));
    // sizeof(isnan(nan_value)): 4

    return 0;
}
