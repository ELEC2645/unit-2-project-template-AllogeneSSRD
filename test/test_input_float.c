#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 1  // Set to 1 to enable debug output

void input_float(double *value, const char * description);

void input_float(double *value, const char * description)
{
    double input;
    char buf[128];

    do {
        printf("\nPlease enter value of %s: ", description);
        if (!fgets(buf, sizeof(buf), stdin)) {
            puts("\nInput error. Exiting.");
            exit(1);
        }
        buf[strcspn(buf, "\r\n")] = '\0'; // strip trailing newline

        // skip starting space
        char *startptr = buf;
        while (isspace((unsigned char)*startptr)) startptr++;
        // check for empty input, must have at least one digit
        if (*startptr == '\0') {
            printf("Invalid input: only whitespace detected.\n");
            continue;
        }

        // check if first char of input is '?'
        if (*startptr == '?') {
            startptr++; // skip '?'
            while (isspace((unsigned char)*startptr)) startptr++; // skip ending space
            if (*startptr != '\0') { // if extra chars after '?'
                if (DEBUG) printf("[Debug] Extra chars after '?': '%s'\n", startptr);
                printf("Invalid input. Please enter a numeric value or '?' for unknown variable.\n");
                continue;
            }
            *value = NAN;  // Use NaN to represent unknown variable
            break;
        }

        // check if input is float
        // Reference: https://eee-elec2645.github.io/docs/menus/user_input.html
        char *endptr;
        errno = 0;
        input = strtod(startptr, &endptr);
        // skip ending space
        while(*endptr != '\0' && isspace((unsigned char)*endptr)) endptr++;
        if (DEBUG) printf("[Debug] input=%f, buf='%s', startptr='%s', endptr='%s', errno=%d\n",
                            input, buf, startptr, endptr, errno);
        if (errno == ERANGE || endptr == startptr || *endptr != '\0') {
            printf("Invalid input. Please enter a numeric value or '?' for unknown variable.\n");
        } else {
            *value = input;
            break;
        }
    } while (1);
}

int main() {
    if (DEBUG) printf("[Debug] Enable debug output.\n");
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
