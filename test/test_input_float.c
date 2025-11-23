#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

#ifndef RETURN_OK
#define RETURN_OK 0   // Normal return
#define RETURN_ERROR 1   // Wrong input or error
#define RETURN_EXIT 2   // User chose to exit
#endif

void input_float(double *value, const char * description);

void input_float(double *value, const char * description)
{
    double input;
    char buf[128];
    int success;
    do {
        printf("\nPlease enter value of %s: ", description);
        if (!fgets(buf, sizeof(buf), stdin))
            printf("Input error. fgets()\n"),
            success = RETURN_ERROR;
        // check if first char of input is '?'
        if (buf[0] == '?') {
            *value = NAN;  // Use NaN to represent unknown variable
            success = RETURN_OK;
            break;
        }

        // check if input is float
        char *endptr;
        errno = 0;
        input = strtod(buf, &endptr);
        if (errno == ERANGE || endptr == buf || (*endptr && *endptr != '\n')) {
            printf("Invalid input. Please enter a numeric value or '?' for unknown variable.");
            success = RETURN_ERROR;
        } else {
            *value = input;
            success = RETURN_OK;
        }
    } while (success != RETURN_OK);
}

int main() {
    double Vo, Vi, K;

    printf("Buck Converter - CCM Duty Cycle Calculation\n");
    printf("Formula: K = Vo / Vi\n"
           "(type '?' for unknown variable to calculate)\n");

    // Input values
    input_float(&Vo, "Output Voltage (Vo)");
    input_float(&Vi, "Input Voltage (Vi)");
    input_float(&K,  "Duty Cycle (K)");

    // Determine which variable to calculate
    if (isnan(Vo)) {
        Vo = K * Vi;
        printf("Calculated Output Voltage (Vo = K * Vi): %.4f\n", Vo);
    } else if (isnan(Vi)) {
        Vi = Vo / K;
        printf("Calculated Input Voltage (Vi = Vo / K): %.4f\n", Vi);
    } else if (isnan(K)) {
        K = Vo / Vi;
        printf("Calculated Duty Cycle (K = Vo / Vi): %.4f\n", K);
    } else {
        printf("All variables provided. No calculation needed.\n");
    }

    return 0;
}
