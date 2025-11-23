// Auther: Xiangcheng Tao
// Student ID: 202013365
// Filename: funcs.c
// Create date: 2025-11-19

#include <stdio.h>
#include <math.h>
#include "funcs.h"

void input_float(double *value, const char * description);

void input_float(double *value, const char * description)
{
    double buf;
    while (1) {
        printf("\nPlease enter value of %s: ", description);
        // check if input is float
        if (scanf("%lf", &buf) == 1) {
            *value = buf;
            while (getchar() != '\n'); // Clear input buffer
            break;
        }
        // check if first char of input is '?'
        char first_char = getchar();
        if (first_char == '?') {
            *value = NAN;  // Use NaN to represent unknown variable
            while (getchar() != '\n');
            break;
        } else {
            printf("Invalid input. Please enter a numeric value or '?' for unknown variable.");
            while (getchar() != '\n');
        }
    }
}

void buck_ccm_duty_cycle(void)
{
    double Vo, Vi, K;
    printf("\n>> Buck Converter - CCM Mode\n"
           "Formula Duty cycle: K = Vo / Vi\n"
           "This formula han 3 variables: Vo, Vi, and K.\n"
           "You can calculate any one of them if you provide the other two.\n"
           "Type '?' for unknown variable to calculate.\n");
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
}

void menu_item_2(void) {
    printf("\n>> Buck Converter - DCM\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 2 */
}

void menu_item_3(void) {
    printf("\n>> Boost Converter - CCM\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 3 */
}

void menu_item_4(void) {
    printf("\n>> Boost Converter - DCM\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 4 */
}