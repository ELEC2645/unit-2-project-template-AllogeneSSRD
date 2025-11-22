// Auther: Xiangcheng Tao
// Student ID: 202013365
// Filename: funcs.c
// Create date: 2025-11-19

#include <stdio.h>
#include <math.h>
#include "funcs.h"

void input_float(double *value, char variable_name[20]);

void input_float(double *value, char variable_name[20])
{
    float input;
    printf("Please enter value of %s (type '?' for unknown variable to calculate): ", variable_name);
    while (scanf("%f", &input) != 1) {
        char ch = getchar();
        if (ch == '?') {
            *value = NAN;  // Use NaN to represent unknown variable
            while (getchar() != '\n'); // Clear input buffer
        } else {
            printf("Invalid input. Please enter a numeric value or '?' for unknown variable: ");
            while (getchar() != '\n'); // Clear input buffer
        }
    }
    *value = input;
}

void buck_ccm_duty_cycle(void)
{
    printf("\n>> Buck Converter - CCM\n"
           "Duty cycle: K = Vo / Vi\n");
    printf("This formula han 3 variables: Vo, Vi, and K\n"
           "You can calculate any one of them if you provide the other two.\n");
    /* you can call a function from here that handles menu 1 */
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