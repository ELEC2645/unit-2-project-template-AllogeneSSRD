// Auther: Xiangcheng Tao
// Student ID: 202013365
// Filename: funcs.c
// Create date: 2025-11-19

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcs.h"

#ifndef DEBUG
#define DEBUG 1  // Set to 1 to enable debug output
#endif

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

void buck_ccm_duty_cycle(void)
{
    double Vo, Vi, K;
    printf("\n>> Buck Converter - CCM Mode\n"
           "Formula (Duty cycle): K = Vo / Vi\n"
           "Variables: Vo (Output Voltage), Vi (Input Voltage), K (Duty Cycle)\n"
           "Provide any two values to calculate the other.\n"
           "Enter '?' for the unknown variable.\n");
    // Input values
    input_float(&Vo, "Output Voltage (Vo: V)");
    input_float(&Vi, "Input Voltage (Vi: V)");
    input_float(&K,  "Duty Cycle (K)");
    // Determine if more than one variable is unknown
    if ((isnan(Vo)+ isnan(Vi)+ isnan(K)) != -1) {
        printf("Error: Please provide exactly two known values and one unknown value.\n");
        return;
    }
    // Determine which variable to calculate
    if (isnan(Vo)) {
        Vo = K * Vi;
        printf("Calculated Output Voltage (Vo = K * Vi): %.4f V\n", Vo);
    } else if (isnan(Vi)) {
        Vi = Vo / K;
        printf("Calculated Input Voltage (Vi = Vo / K): %.4f V\n", Vi);
    } else if (isnan(K)) {
        K = Vo / Vi;
        printf("Calculated Duty Cycle (K = Vo / Vi): %.4f\n", K);
    } else {
        printf("All variables provided. No calculation needed.\n");
    }
}

void buck_ccm_inductor_Iripple(void)
{
    double Vo = NAN, K = NAN, L = NAN, f_s = NAN, delta_i = NAN;
    printf("\n>> Buck Converter - CCM Mode\n"
          "\nFormula: Delta i = ((1 - K) * Vo) / (f_s * L)\n"
           "Variables: Vo (Output Voltage), K (Duty Cycle), L (Inductance), f_s (Switching Frequency), Delta i (Current Ripple)\n"
           "Provide any 4 values to calculate the other.\n"
           "Enter '?' for the unknown variable.\n");
    // Input values
    input_float(&Vo, "Output Voltage (Vo: V)");
    input_float(&K,  "Duty Cycle (K)");
    input_float(&L,  "Inductance (L: H)");
    input_float(&f_s, "Switching Frequency (f_s: Hz)");
    input_float(&delta_i, "Current Ripple (Delta i: A)");
    // Determine if more than one variable is unknown
    if ((isnan(Vo)+ isnan(K)+ isnan(L)+ isnan(f_s)+ isnan(delta_i)) != -1) {
        printf("Error: Please provide exactly four known values and one unknown value.\n");
        if (DEBUG) printf("[Debug] all=%d, Vo isnan=%d, K isnan=%d, L isnan=%d, f_s isnan=%d, delta_i isnan=%d\n",
            (isnan(Vo)+ isnan(K)+ isnan(L)+ isnan(f_s)+ isnan(delta_i)),
               isnan(Vo), isnan(K), isnan(L), isnan(f_s), isnan(delta_i));
        // return;
    }
    // Determine which variable to calculate
    if (isnan(delta_i)) {
        delta_i = ((1 - K) * Vo) / (f_s * L);
        printf("Calculated Current Ripple: Delta i = ((1 - K) * Vo) / (f_s * L): %.4f A\n", delta_i);
    } else if (isnan(Vo)) {
        Vo = (delta_i * f_s * L) / (1 - K);
        printf("Calculated Output Voltage: Vo = (Delta i * f_s * L) / (1 - K): %.4f V\n", Vo);
    } else if (isnan(K)) {
        K = 1 - (delta_i * f_s * L) / Vo;
        printf("Calculated Duty Cycle: K = 1 - (Delta i * f_s * L) / Vo: %.4f\n", K);
    } else if (isnan(L)) {
        L = ((1 - K) * Vo) / (f_s * delta_i);
        printf("Calculated Inductance: L = ((1 - K) * Vo) / (f_s * Delta i): %.4f H\n", L);
    } else if (isnan(f_s)) {
        f_s = ((1 - K) * Vo) / (L * delta_i);
        printf("Calculated Switching Frequency: f_s = ((1 - K) * Vo) / (L * Delta i): %.4f Hz\n", f_s);
    } else {
        printf("All variables provided. No calculation needed.\n");
    }
}

void buck_ccm_capacitor_Vripple(void)
{
    double Vo = NAN, K = NAN, C = NAN, L = NAN, f_s = NAN, delta_v = NAN;
    printf("\n>> Buck Converter - CCM Mode\n"
          "\nFormula: Delta v = ((1 - K) * Vo) / (8 * f_s^2 * C * L)\n"
           "Variables: Vo (Output Voltage), K (Duty Cycle), L (Inductance), f_s (Switching Frequency), Delta i (Current Ripple)\n"
           "Provide any 4 values to calculate the other.\n"
           "Enter '?' for the unknown variable.\n");
    // Input values
    input_float(&Vo, "Output Voltage (Vo: V)");
    input_float(&K,  "Duty Cycle (K)");
    input_float(&C,  "Capacitance (C: F)");
    input_float(&L,  "Inductance (L: H)");
    input_float(&f_s, "Switching Frequency (f_s: Hz)");
    input_float(&delta_v, "Voltage Ripple (Delta v: V)");
    // Determine if more than one variable is unknown
    if ((isnan(Vo)+ isnan(K)+ isnan(C)+ isnan(L)+ isnan(f_s)+ isnan(delta_v)) != -1) {
        printf("Error: Please provide exactly four known values and one unknown value.\n");
        if (DEBUG) printf("[Debug] all=%d, Vo isnan=%d, K isnan=%d, C isnan=%d, L isnan=%d, f_s isnan=%d, delta_v isnan=%d\n",
            (isnan(Vo)+ isnan(K)+ isnan(C)+ isnan(L)+ isnan(f_s)+ isnan(delta_v)),
               isnan(Vo), isnan(K), isnan(C), isnan(L), isnan(f_s), isnan(delta_v));
        // return;
    }
    // Determine which variable to calculate
    if (isnan(delta_v)) {
        delta_v = ((1 - K) * Vo) / (8 * f_s * f_s * C * L);
        printf("Calculated Voltage Ripple: Delta v = ((1 - K) * Vo) / (8 * f_s^2 * C * L): %.4f V\n", delta_v);
    } else if (isnan(Vo)) {
        Vo = (delta_v * 8 * f_s * f_s * C * L) / (1 - K);
        printf("Calculated Output Voltage: Vo = (Delta v * 8 * f_s^2 * C * L) / (1 - K): %.4f V\n", Vo);
    } else if (isnan(K)) {
        K = 1 - (delta_v * 8 * f_s * f_s * C * L) / Vo;
        printf("Calculated Duty Cycle: K = 1 - (Delta v * 8 * f_s^2 * C * L) / Vo: %.4f\n", K);
    } else if (isnan(C)) {
        C = ((1 - K) * Vo) / (delta_v * 8 * f_s * f_s * L);
        printf("Calculated Capacitance: C = ((1 - K) * Vo) / (Delta v * 8 * f_s^2 * L): %.4f F\n", C);
    }else if (isnan(L)) {
        L = ((1 - K) * Vo) / (delta_v * 8 * f_s * f_s * C);
        printf("Calculated Inductance: L = ((1 - K) * Vo) / (Delta v * 8 * f_s^2 * C): %.4f H\n", L);
    } else if (isnan(f_s)) {
        f_s = ((1 - K) * Vo) / (8 * C * delta_v);
        printf("Calculated Switching Frequency: f_s = ((1 - K) * Vo) / (8 * C * Delta v): %.4f Hz\n", f_s);
    } else {
        printf("All variables provided. No calculation needed.\n");
    }
}

void buck_dcm_duty_cycle(void) {
    printf("\n>> Boost Converter - DCM\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 4 */
}

void buck_dcm_boundary_constant_vi(void) {
    printf("\n>> Boost Converter - DCM\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 4 */
}

void buck_dcm_boundary_constant_vo(void) {
    printf("\n>> Boost Converter - DCM\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 4 */
}
