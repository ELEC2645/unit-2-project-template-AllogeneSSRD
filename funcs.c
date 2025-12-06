// ELEC2645 Unit 2 Project
// Auther: Xiangcheng Tao
// Filename: funcs.c
// Create date: 2025-11-19

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcs.h"

#ifndef RETURN_OK
#define RETURN_OK 0   // Normal return
#define RETURN_ERROR 1   // Wrong input or error
#define RETURN_EXIT 2   // User chose to exit
#endif

#ifndef DEBUG
#define DEBUG 0  // Set to 1 to enable debug output
#endif

#define TRUE 1
#define FALSE 0

#define DATA_FILE "converter.data"

typedef struct
{
    double Vo;         // Output Voltage
    double Vi;         // Input Voltage
    double K;          // Duty Cycle
    double R;          // Load Resistance
    double L;          // Inductance
    double C;          // Capacitance
    double f_s;        // Switching Frequency
    double Io;         // Output Current
    double Delta_i;    // Current Ripple
    double Delta_v;    // Voltage Ripple
    double Delta_1;    // Close time
} Converter_Params;


void input_float(double *value, double *cache_value, const char * description);
void output_float(const char *description, double value, const char *unit, int is_limited, double min, double max);
int save_converter_params(const Converter_Params *params, const char *filename); // const for unchanged params in save
int load_converter_params(Converter_Params *params, const char *filename);
void init_converter_params(Converter_Params *params);

void input_float(double *value, double *stored_value, const char * description)
{
    double input;
    char buf[128];

    do {
        // allow user to see stored value if exists
        if (isnan(*stored_value)) printf("\nPlease enter value of %s: ", description);
        else printf("\nPlease enter value of %s\nOr use stored value (%.4f) by 'r': ", description, *stored_value);

        if (!fgets(buf, sizeof(buf), stdin)) {
            puts("\nInput error. Exiting.");
            exit(1);
        }
        buf[strcspn(buf, "\r\n")] = '\0'; // strip trailing newline

        char *startptr = buf;
        while (isspace((unsigned char)*startptr)) startptr++; // skip starting space
        // check for empty input, must have at least one digit
        if (*startptr == '\0') {
            printf("Invalid input: only whitespace detected.\n");
            continue;
        }

        // check for 'R' or 'r' to read from file
        if (!isnan(*stored_value) && (*startptr == 'R' || *startptr == 'r')) {
            startptr++; // skip 'R' or 'r'
            while (isspace((unsigned char)*startptr)) startptr++; // skip ending space
            if (*startptr != '\0') { // if extra chars after 'R' or 'r'
                if (DEBUG) printf("[Debug] Extra chars after 'R'/'r': '%s'\n", startptr);
                printf("Invalid input. Please enter a numeric value, '?' for unknown variable, or 'r' to read from file.\n");
                continue;
            }
            *value = *stored_value;
            printf("%s read from file: %.4f\n", description, *stored_value);
            break;
        }
        // reset startptr to beginning of buffer
        startptr = buf;

        // check if first char of input is '?'
        if (*startptr == '?') {
            startptr++; // skip '?'
            while (isspace((unsigned char)*startptr)) startptr++; // skip ending space
            if (*startptr != '\0') { // if extra chars after '?'
                if (DEBUG) printf("[Debug] Extra chars after '?': '%s'\n", startptr);
                if (isnan(*stored_value)) printf("Invalid input. Please enter a numeric value, '?' for unknown variable.\n");
                else printf("Invalid input. Please enter a numeric value, '?' for unknown variable, or 'r' to read from file.\n");
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
    } while (TRUE);
}

void output_float(const char *description, double value, const char *unit, int is_limited, double min, double max)
{
    if (isnan(value)) {
        printf("Error: %s cannot be calculated. Please check inputs.\n", description);
        return;
    } else if (isinf(value)) {
        printf("Warning: %s is infinite.\n", description);
        return;
    } else {
        // valid number
        if (is_limited && (value < min || value > max)) {
            printf("Warning: result is out of the expected range (%.2f to %.2f).\n", min, max);
            printf("%s = %.4f %s (Out of range: %.0f,%.0f)\n", description, value, unit, min, max);
        } else {
            printf("%s = %.4f %s\n", description, value, unit);
        }
    }
}

int save_converter_params(const Converter_Params *params, const char *filename)
{
    FILE *fptr = fopen(filename, "wb");
    if (!fptr) {
        printf("Could not open file!\n");
        return RETURN_ERROR;
    }
    // Reference: https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fwrite?view=msvc-170
    fwrite(params, sizeof(Converter_Params), 1, fptr);
    fclose(fptr);
    return RETURN_OK;
}

int load_converter_params(Converter_Params *params, const char *filename)
{
    FILE *fptr = fopen(filename, "rb");
    if (!fptr) {
        // printf("Could not open file!\n");
        if (DEBUG) printf("[Debug] Could not open %s.\n", filename);
        return RETURN_ERROR;
    }
    // Reference: https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fread?view=msvc-170
    fread(params, sizeof(Converter_Params), 1, fptr);
    fclose(fptr);
    if (DEBUG) printf("[Debug] Variables loaded from file.\n");
    return RETURN_OK;
}

void init_converter_params(Converter_Params *params)
{
    // initialize all variables to NaN
    params->Vo = NAN;
    params->Vi = NAN;
    params->K = NAN;
    params->R = NAN;
    params->L = NAN;
    params->C = NAN;
    params->f_s = NAN;
    params->Io = NAN;
    params->Delta_i = NAN;
    params->Delta_v = NAN;
    params->Delta_1 = NAN;
}

void buck_ccm_duty_cycle(void)
{
    double Vo, Vi, K;
    Converter_Params params;
    if (load_converter_params(&params, DATA_FILE) != RETURN_OK) init_converter_params(&params);
    printf("\n>> Buck Converter - CCM Mode\n"
           "Formula (Duty cycle): K = Vo / Vi\n"
           "Variables: Vo (Output Voltage), Vi (Input Voltage), K (Duty Cycle)\n"
           "Provide any 2 values to calculate the other.\n"
           "Enter '?' for the unknown variable.\n");
    // Input values
    input_float(&Vo, &params.Vo, "Output Voltage (Vo: V)");
    input_float(&Vi, &params.Vi, "Input Voltage (Vi: V)");
    input_float(&K,  &params.K, "Duty Cycle (K)");
    // Determine if more than one variable is unknown
    if ((isnan(Vo) + isnan(Vi) + isnan(K)) != -1) {
        printf("Error: Please provide exactly 2 known values and 1 unknown value.\n");
        return;
    }
    // Determine which variable to calculate
    if (isnan(Vo)) {
        Vo = K * Vi; // Vo = K * Vi
        output_float("Calculated Output Voltage (Vo = K * Vi): Vo", Vo, "V", FALSE, 0, 0);
    } else if (isnan(Vi)) {
        Vi = Vo / K; // Vi = \frac{V_o}{K}
        output_float("Calculated Input Voltage (Vi = Vo / K): Vi", Vi, "V", FALSE, 0, 0);
    } else if (isnan(K)) {
        K = Vo / Vi; // K = \frac{V_o}{V_i}
        output_float("Calculated Duty Cycle (K = Vo / Vi): K", K, "", TRUE, 0, 1);
    } else {
        printf("All variables provided. No calculation needed.\n");
    }
    // Save variables to file
    params.Vo = Vo;
    params.Vi = Vi;
    params.K = K;
    save_converter_params(&params, DATA_FILE);
}


void buck_ccm_inductor_Iripple(void)
{
    double Vo, K, L, f_s, Delta_i;
    Converter_Params params;
    if (load_converter_params(&params, DATA_FILE) != RETURN_OK) init_converter_params(&params);
    printf("\n>> Buck Converter - CCM Mode\n"
          "\nFormula: Delta i = ((1 - K) * Vo) / (f_s * L)\n"
           "Variables: Vo (Output Voltage), K (Duty Cycle), L (Inductance), f_s (Switching Frequency), Delta i (Current Ripple)\n"
           "Provide any 4 values to calculate the other.\n"
           "Enter '?' for the unknown variable.\n");
    // Input values
    input_float(&Vo, &params.Vo, "Output Voltage (Vo: V)");
    input_float(&K,  &params.K, "Duty Cycle (K)");
    input_float(&L,  &params.L, "Inductance (L: mH)");
    L /= 1e3; // convert to H
    input_float(&f_s, &params.f_s, "Switching Frequency (f_s: Hz)");
    input_float(&Delta_i, &params.Delta_i, "Current Ripple (Delta i: A)");
    // Determine if more than one variable is unknown
    if ((isnan(Vo) + isnan(K) + isnan(L) + isnan(f_s) + isnan(Delta_i)) != -1) {
        printf("Error: Please provide exactly 4 known values and 1 unknown value.\n");
        if (DEBUG) printf("[Debug] all=%d, Vo isnan=%d, K isnan=%d, L isnan=%d, f_s isnan=%d, Delta_i isnan=%d\n",
            (isnan(Vo) + isnan(K) + isnan(L) + isnan(f_s) + isnan(Delta_i)),
               isnan(Vo), isnan(K), isnan(L), isnan(f_s), isnan(Delta_i));
        return;
    }
    // Determine which variable to calculate
    if (isnan(Delta_i)) {
        Delta_i = ((1 - K) * Vo) / (f_s * L); // \Delta i = \frac{(1-K)\cdot V_o}{f_s\cdot L}
        printf("Calculated Current Ripple: Delta i = ((1 - K) * Vo) / (f_s * L): Delta i = %.4f A\n", Delta_i);
    } else if (isnan(Vo)) {
        Vo = (Delta_i * f_s * L) / (1 - K);
        printf("Calculated Output Voltage: Vo = (Delta i * f_s * L) / (1 - K): Vo = %.4f V\n", Vo);
    } else if (isnan(K)) {
        K = 1 - (Delta_i * f_s * L) / Vo;
        printf("Calculated Duty Cycle: K = 1 - (Delta i * f_s * L) / Vo: K = %.4f\n", K);
    } else if (isnan(L)) { // L = \frac{V_{o} (1 - K)}{\Delta_{i} f_{s}}
        L = ((1 - K) * Vo) / (f_s * Delta_i) * 1e3; // convert to mH
        printf("Calculated Inductance: L = ((1 - K) * Vo) / (f_s * Delta i): L = %.4f mH\n", L);
    } else if (isnan(f_s)) {
        f_s = ((1 - K) * Vo) / (L * Delta_i);
        printf("Calculated Switching Frequency: f_s = ((1 - K) * Vo) / (L * Delta i): f_s = %.4f Hz\n", f_s);
    } else {
        printf("All variables provided. No calculation needed.\n");
    }
    // Save variables to file
    params.Vo = Vo;
    params.K = K;
    params.L = L * 1e3; // convert back to mH
    params.f_s = f_s;
    params.Delta_i = Delta_i;
    save_converter_params(&params, DATA_FILE);
}

void buck_ccm_capacitor_Vripple(void)
{
    double Vo, K, C, L, f_s, Delta_v;
    Converter_Params params;
    if (load_converter_params(&params, DATA_FILE) != RETURN_OK) init_converter_params(&params);
    printf("\n>> Buck Converter - CCM Mode\n"
          "\nFormula: Delta Vout = ((1 - K) * Vo) / (8 * f_s^2 * C * L)\n"
           "Variables: Vo (Output Voltage), K (Duty Cycle), L (Inductance), f_s (Switching Frequency), Delta Vout (Voltage Ripple)\n"
           "Provide any 4 values to calculate the other.\n"
           "Enter '?' for the unknown variable.\n");
    // Input values
    input_float(&Vo, &params.Vo, "Output Voltage (Vo: V)");
    input_float(&K,  &params.K, "Duty Cycle (K)");
    input_float(&C,  &params.C, "Capacitance (C: uF)");
    C /= 1e6; // convert to F
    input_float(&L,  &params.L, "Inductance (L: mH)");
    L /= 1e3; // convert to H
    input_float(&f_s, &params.f_s, "Switching Frequency (f_s: Hz)");
    input_float(&Delta_v, &params.Delta_v, "Voltage Ripple (Delta v: V)");
    // Determine if more than one variable is unknown
    if ((isnan(Vo) + isnan(K) + isnan(C) + isnan(L) + isnan(f_s) + isnan(Delta_v)) != -1) {
        printf("Error: Please provide exactly 4 known values and 1 unknown value.\n");
        if (DEBUG) printf("[Debug] all=%d, Vo isnan=%d, K isnan=%d, C isnan=%d, L isnan=%d, f_s isnan=%d, Delta_v isnan=%d\n",
            (isnan(Vo) + isnan(K) + isnan(C) + isnan(L) + isnan(f_s) + isnan(Delta_v)),
               isnan(Vo), isnan(K), isnan(C), isnan(L), isnan(f_s), isnan(Delta_v));
        return;
    }
    // Determine which variable to calculate
    if (isnan(Delta_v)) {
        Delta_v = ((1 - K) * Vo) / (8 * f_s * f_s * C * L);
        printf("Calculated Voltage Ripple: Delta v = ((1 - K) * Vo) / (8 * f_s^2 * C * L): \n"
               "Delta v = %.4f V\n", Delta_v);
    } else if (isnan(Vo)) {
        Vo = (Delta_v * 8 * f_s * f_s * C * L) / (1 - K);
        printf("Calculated Output Voltage: Vo = (Delta v * 8 * f_s^2 * C * L) / (1 - K): \n"
               "Vo = %.4f V\n", Vo);
    } else if (isnan(K)) {
        K = 1 - (Delta_v * 8 * f_s * f_s * C * L) / Vo;
        printf("Calculated Duty Cycle: K = 1 - (Delta v * 8 * f_s^2 * C * L) / Vo: \n"
               "K = %.4f\n", K);
    } else if (isnan(C)) {
        C = ((1 - K) * Vo) / (Delta_v * 8 * f_s * f_s * L) * 1e6; // convert to uF
        printf("Calculated Capacitance: C = ((1 - K) * Vo) / (Delta v * 8 * f_s^2 * L): \n"
               "C = %.4f uF\n", C);
    }else if (isnan(L)) {
        L = ((1 - K) * Vo) / (Delta_v * 8 * f_s * f_s * C) * 1e3; // convert to mH
        printf("Calculated Inductance: L = ((1 - K) * Vo) / (Delta v * 8 * f_s^2 * C): \n"
               "L = %.4f mH\n", L);
    } else if (isnan(f_s)) {
        f_s = ((1 - K) * Vo) / (8 * C * Delta_v);
        printf("Calculated Switching Frequency: f_s = ((1 - K) * Vo) / (8 * C * Delta v): \n"
               "f_s = %.4f Hz\n", f_s);
    } else {
        printf("All variables provided. No calculation needed.\n");
    }
    // Save variables to file
    params.Vo = Vo;
    params.K = K;
    params.C = C * 1e6; // convert back to uF
    params.L = L * 1e3; // convert back to mH
    params.f_s = f_s;
    params.Delta_v = Delta_v;
    save_converter_params(&params, DATA_FILE);
}

void buck_dcm_duty_cycle(void)
{
    double Vo, Vi, K, Delta_1;
    Converter_Params params;
    if (load_converter_params(&params, DATA_FILE) != RETURN_OK) init_converter_params(&params);
    printf("\n>> Buck Converter - DCM\n"
          "\nFormula: Vo / Vi = K / (K + Delta 1)\n"
           "Variables: Vo (Output Voltage), Vi (Input Voltage), K (Duty Cycle), Delta 1 (Close time)\n"
           "Provide any 3 values to calculate the other.\n"
           "Enter '?' for the unknown variable.\n");
    // Input values
    input_float(&Vo, &params.Vo, "Output Voltage (Vo: V)");
    input_float(&Vi, &params.Vi, "Input Voltage (Vi: V)");
    input_float(&K,  &params.K, "Duty Cycle (K)");
    input_float(&Delta_1, &params.Delta_1, "Close time (Delta 1)");
    // Determine if more than one variable is unknown
    if ((isnan(Vo) + isnan(Vi) + isnan(K) + isnan(Delta_1)) != -1) {
        printf("Error: Please provide exactly 3 known values and 1 unknown value.\n");
        return;
    }
    // Determine which variable to calculate
    if (isnan(Vo)) {
        Vo = Vi * K / (K + Delta_1); // Vo = \frac{K V_{i}}{\Delta_{i} + K}
        printf("Calculated Output Voltage (Vo = Vi * K / (K + Delta 1)): %.4f V\n", Vo);
    } else if (isnan(Vi)) {
        Vi = Vo * (K + Delta_1) / K; // Vi = \frac{V_{o} (K + \Delta_{i})}{K}
        printf("Calculated Input Voltage (Vi = Vo * (K + Delta 1) / K): %.4f V\n", Vi);
    } else if (isnan(K)) {
        K = (Vo * Delta_1) / (Vi - Vo); // K = \frac{\Delta_{i} V_{o}}{V_{i} - V_{o}}
        printf("Calculated Duty Cycle (K = (Vo * Delta 1) / (Vi - Vo)): %.4f\n", K);
    } else if (isnan(Delta_1)) {
        Delta_1 = (K * (Vi - Vo)) / Vo; // Delta_i = \frac{K (V_{i} - V_{o})}{V_{o}}
        printf("Calculated Close time (Delta 1 = (K * (Vi - Vo)) / Vo): %.4f\n", Delta_1);
    } else {
        printf("All variables provided. No calculation needed.\n");
    }
    // Save variables to file
    params.Vo = Vo;
    params.Vi = Vi;
    params.K = K;
    params.Delta_1 = Delta_1;
    save_converter_params(&params, DATA_FILE);
}

void buck_dcm_boundary_constant_vi(void)
{
    double Vi, K, L, f_s, Io;
    Converter_Params params;
    if (load_converter_params(&params, DATA_FILE) != RETURN_OK) init_converter_params(&params);
    printf("\n>> Buck Converter - DCM\n"
           "\nFormula: Io = (Vi * 1/fs) / (2 * L) * K * (1 - K)\n"
           "Variables: Vi (Input Voltage), K (Duty Cycle), L (Close time), f_s (Switching Frequency), Io (Output Current)\n"
           "Provide any 4 values to calculate the other.\n"
           "Enter '?' for the unknown variable.\n");
    // Input values
    input_float(&Vi, &params.Vi, "Input Voltage (Vi: V)");
    input_float(&K,  &params.K, "Duty Cycle (K)");
    input_float(&L,  &params.L, "Inductance (L: mH)");
    L /= 1e3; // convert to H
    input_float(&f_s, &params.f_s, "Switching Frequency (f_s: Hz)");
    input_float(&Io, &params.Io, "Output Current (Io: A)");
    // Determine if more than one variable is unknown
    if ((isnan(Vi) + isnan(K) + isnan(L) + isnan(f_s) + isnan(Io)) != -1) {
        printf("Error: Please provide exactly 4 known values and 1 unknown value.\n");
        return;
    }
    // Determine which variable to calculate
    if (isnan(Vi)) {
        Vi = -2 * Io * L* f_s / (K * (K - 1)); // V_{i} = - \frac{2 I_{o} L f_{s}}{K (K - 1)}
        printf("Calculated Input Voltage (Vi = -2 * Io * L* f_s / (K * (K - 1))): \n"
               "Vi = %.4f V\n", Vi);
    } else if (isnan(K)) {
        K = (Vi - sqrt(Vi * (-8 * Io * L * f_s + Vi)))/(2 * Vi); // K = \frac{V_{i} - \sqrt{V_{i} (- 8 I_{o} L f_{s} + V_{i})}}{2 V_{i}}
        printf("Calculated Duty Cycle (K = (Vi - sqrt(Vi * (-8 * Io * L * f_s + Vi)))/(2 * Vi)): \n"
               "K = %.4f\n", K);
    } else if (isnan(L)) {
        L = (Vi * K * (1 - K)) / (2 * Io * f_s) * 1e3; // L = \frac{V_{i} K{(1 - K )}}{2 I_{o} f_{s}}
        printf("Calculated Inductance (L = (Vi * K * (1 - K)) / (2 * Io * f_s)): \n"
               "L = %.4f mH\n", L);
    } else if (isnan(f_s)) {
        f_s = K * Vi * (1 - K)/(2 * Io * L); // f_{s} = \frac{K V_{i} (1 - K)}{2 I_{o} L}
        printf("Calculated Switching Frequency (f_s = K * Vi * (1 - K)/(2 * Io * L)): \n"
               "f_s = %.4f Hz\n", f_s);
    } else if (isnan(Io)) {
        Io = Vi * K * (1 - K)/(2 * L * f_s); // I_{o} = \frac{V_{i} K{(1 - K )}}{2 L f_{s}}
        printf("Calculated Output Current (Io = Vi * K * (1 - K)/(2 * L * f_s)): \n"
               "Io = %.4f A\n", Io);
    } else {
        printf("All variables provided. No calculation needed.\n");
    }
    // Save variables to file
    params.Vi = Vi;
    params.K = K;
    params.L = L;
    params.f_s = f_s;
    params.Io = Io;
    save_converter_params(&params, DATA_FILE);
}

void buck_dcm_boundary_constant_vo(void)
{
    double Vo, K, L, f_s, Io;
    Converter_Params params;
    if (load_converter_params(&params, DATA_FILE) != RETURN_OK) init_converter_params(&params);
    printf("\n>> Buck Converter - DCM\n"
           "\nFormula: I_o = (Vo * 1/fs) / (2 * L) * (1 - K)\n"
           "Variables: Vo (Output Voltage), K (Duty Cycle), L (Close time), f_s (Switching Frequency), Io (Output Current)\n"
           "Provide any 4 values to calculate the other.\n"
           "Enter '?' for the unknown variable.\n");
    // Input values
    input_float(&Vo, &params.Vo, "Output Voltage (Vo: V)");
    input_float(&K,  &params.K, "Duty Cycle (K)");
    input_float(&L,  &params.L, "Inductance (L: mH)");
    L /= 1e3; // convert to H
    input_float(&f_s, &params.f_s, "Switching Frequency (f_s: Hz)");
    input_float(&Io, &params.Io, "Output Current (Io: A)");
    // Determine if more than one variable is unknown
    if ((isnan(Vo) + isnan(K) + isnan(L) + isnan(f_s) + isnan(Io)) != -1) {
        printf("Error: Please provide exactly 4 known values and 1 unknown value.\n");
        return;
    }
    // Determine which variable to calculate
    if (isnan(Vo)) {
        Vo = 2 * Io * L * f_s / (1 - K); // V_{o} = \frac{2 I_{o} L f_{s}}{1 - K}
        printf("Calculated Input Voltage (Vo = 2 * Io * L * f_s / (1 - K)): \n"
               "Vo = %.4f V\n", Vo);
    } else if (isnan(K)) {
        K = 1 - (2 * Io * L * f_s)/Vo; // K = \frac{- 2 I_{o} L f_{s}}{V_{o}} + 1
        printf("Calculated Duty Cycle (K = 1 - (2 * Io * L * f_s)/Vo): \n"
               "K = %.4f\n", K);
    } else if (isnan(L)) {
        L = Vo * (1 - K)/(2 * Io * f_s) * 1e3; // L = \frac{V_{o} \left(1 - K\right)}{2 I_{o} f_{s}}
        printf("Calculated Inductance (L = Vo * (1 - K)/(2 * Io * f_s)): \n"
               "L = %.4f mH\n", L);
    } else if (isnan(f_s)) {
        f_s = Vo * (1 - K)/(2 * Io * L); // f_{s} = \frac{V_{o} \left(1 - K\right)}{2 I_{o} L}
        printf("Calculated Switching Frequency (f_s = Vo * (1 - K)/(2 * Io * L)): \n"
               "f_s = %.4f Hz\n", f_s);
    } else if (isnan(Io)) {
        Io = Vo * (1 - K)/(2 * L * f_s); // I_{o} = \frac{V_{o} \left(1 - K\right)}{2 L f_{s}}
        printf("Calculated Output Current (Io = Vo * (1 - K)/(2 * L * f_s)): \n"
               "Io = %.4f A\n", Io);
    } else {
        printf("All variables provided. No calculation needed.\n");
    }
    // Save variables to file
    params.Vo = Vo;
    params.K = K;
    params.L = L;
    params.f_s = f_s;
    params.Io = Io;
    save_converter_params(&params, DATA_FILE);
}

void boost_ccm_duty_cycle(void)
{
    double Vo, Vi, K;
    Converter_Params params;
    if (load_converter_params(&params, DATA_FILE) != RETURN_OK) init_converter_params(&params);
    printf("\n>> Boost Converter - CCM\n"
           "\nFormula: Vo / Vi = 1 / (1 - K)\n"
           "Variables: Vo (Output Voltage), Vi (Input Voltage), K (Duty Cycle)\n"
           "Provide any 3 values to calculate the other.\n"
           "Enter '?' for the unknown variable.\n");
    // Input values
    input_float(&Vo, &params.Vo, "Output Voltage (Vo: V)");
    input_float(&Vi, &params.Vi, "Input Voltage (Vi: V)");
    input_float(&K,  &params.K, "Duty Cycle (K)");

    // Determine if more than one variable is unknown
    if ((isnan(Vo) + isnan(Vi) + isnan(K)) != -1) {
        printf("Error: Please provide exactly 2 known values and 1 unknown value.\n");
        return;
    }
    // Determine which variable to calculate
    if (isnan(Vo)) {
        Vo = Vi/(1 - K); // Vo = \frac{V_i}{1 - K}
        printf("Calculated Input Voltage (Vo = Vi/(1 - K)): \n"
               "Vo = %.4f V\n", Vo);
    } else if (isnan(Vi)) {
        Vi = Vo * (1 - K); // Vi = V_o \left(1 - K\right)
        printf("Calculated Input Voltage (Vi = Vo * (1 - K)): \n"
               "Vi = %.4f V\n", Vi);
    } else if (isnan(K)) {
        K = 1- Vi/Vo; // K = 1-\frac{V_i}{V_o}
        printf("Calculated Duty Cycle (K = 1 - Vi/Vo): \n"
               "K = %.4f\n", K);

    } else {
        printf("All variables provided. No calculation needed.\n");
    }
    // Save variables to file
    params.Vo = Vo;
    params.Vi = Vi;
    params.K = K;
    save_converter_params(&params, DATA_FILE);
}

void boost_ccm_inductor_Iripple(void)
{
    double Vi, K, L, f_s, Delta_i;
    Converter_Params params;
    if (load_converter_params(&params, DATA_FILE) != RETURN_OK) init_converter_params(&params);
    printf("\n>> Boost Converter - CCM\n"
           "\nFormula: L = (Vi * K) / (f_s * Delta_i)\n"
           "Variables: Vi (Input Voltage), K (Duty Cycle), L (Inductance), f_s (Switching Frequency), Delta_i (Inductor Current Ripple)\n"
           "Provide any 4 values to calculate the other.\n"
           "Enter '?' for the unknown variable.\n");
    // Input values
    input_float(&Vi, &params.Vi, "Input Voltage (Vi: V)");
    input_float(&K,  &params.K, "Duty Cycle (K)");
    input_float(&L,  &params.L, "Inductance (L: mH)");
    L /= 1e3; // convert to H
    input_float(&f_s, &params.f_s, "Switching Frequency (f_s: Hz)");
    input_float(&Delta_i, &params.Delta_i, "Inductor Current Ripple (Delta i: A)");

    // Determine if more than one variable is unknown
    if ((isnan(Vi) + isnan(K) + isnan(L) + isnan(f_s) + isnan(Delta_i)) != 1) {
        printf("Error: Please provide exactly 4 known values and 1 unknown value.\n");
        return;
    }
    // Determine which variable to calculate
    if (isnan(Vi)) {
        Vi = Delta_i * L * f_s / K; // V_{i} = \frac{\Delta_{i} L f_{s}}{K}
        printf("Calculated Input Voltage (Vi = Delta_i * L * f_s / K): \n"
               "Vi = %.4f V\n", Vi);
    } else if (isnan(K)) {
        K = Delta_i * L * f_s/Vi; // K = \frac{\Delta_{i} L f_{s}}{V_{i}}
        printf("Calculated Duty Cycle (K = Delta_i * L * f_s / Vi): \n"
               "K = %.4f\n", K);
    } else if (isnan(L)) {
        L = K * Vi / (Delta_i * f_s) * 1e-3; // L = \frac{K V_{i}}{\Delta_{i} f_{s}}
        printf("Calculated Inductance (L = K * Vi / (Delta_i * f_s)): \n"
               "L = %.4f H\n", L);
    } else if (isnan(f_s)) {
        f_s = K * Vi/(Delta_i * L); // f_{s} = \frac{K V_{i}}{\Delta_{i} L}
        printf("Calculated Switching Frequency (f_s = K * Vi / (Delta_i * L)): \n"
               "f_s = %.4f\n", f_s);
    } else if (isnan(Delta_i)) {
        Delta_i = K * Vi/(L * f_s); // \Delta_{i} = \frac{K V_{i}}{L f_{s}}
        printf("Calculated Inductor Current Ripple (Delta_i = K * Vi / (L * f_s)): \n"
               "Delta_i = %.4f\n", Delta_i);
    } else {
        printf("All variables provided. No calculation needed.\n");
    }
    // Save variables to file
    params.Vi = Vi;
    params.K = K;
    params.L = L;
    params.f_s = f_s;
    params.Delta_i = Delta_i;
    save_converter_params(&params, DATA_FILE);
}

void boost_ccm_capacitor_Vripple(void)
{
    double Vo, K, R, C, f_s, Delta_v;
    Converter_Params params;
    if (load_converter_params(&params, DATA_FILE) != RETURN_OK) init_converter_params(&params);
    printf("\n>> Boost Converter - CCM\n"
           "\nFormula: C = (Vo * K)/(Delta_v * R * f_s)\n"
           "Variables: Vo (Output Voltage), K (Duty Cycle), R (Load Resistance), C (Capacitance), f_s (Switching Frequency), Delta_v (Capacitor Voltage Ripple)\n"
           "Provide any 5 values to calculate the other.\n"
           "Enter '?' for the unknown variable.\n");
    // Input values
    input_float(&Vo, &params.Vo, "Output Voltage (Vo: V)");
    input_float(&K,  &params.K, "Duty Cycle (K)");
    input_float(&R,  &params.R, "Load Resistance (R: Ohm)");
    input_float(&C,  &params.C, "Capacitance (C: uF)");
    C /= 1e6; // convert to F
    input_float(&f_s, &params.f_s, "Switching Frequency (f_s: Hz)");
    input_float(&Delta_v, &params.Delta_v, "Capacitor Voltage Ripple (Delta v: V)");
    // Determine if more than one variable is unknown
    if ((isnan(Vo) + isnan(K) + isnan(R) + isnan(C) + isnan(f_s) + isnan(Delta_v)) != 1) {
        printf("Error: Please provide exactly 5 known values and 1 unknown value.\n");
        return;
    }
    // Determine which variable to calculate
    if (isnan(Vo)) {
        Vo = (Delta_v * R * f_s * C)/(K); // V_{o} = \frac{C \Delta_{v} R f_{s}}{K}
        printf("Calculated Output Voltage (Vo = (Delta_v * R * f_s * C)/(K)): \n"
               "Vo = %.4f V\n", Vo);
    } else if (isnan(K)) {
        K = C * Delta_v * R * f_s / Vo; // K = \frac{C \Delta_{v} R f_{s}}{V_{o}}
        printf("Calculated Duty Cycle (K = C * Delta_v * R * f_s / Vo): \n"
               "K = %.4f\n", K);
    } else if (isnan(R)) {
        R = K * Vo / (C * Delta_v * f_s); // R = \frac{K V_{o}}{C \Delta_{v} f_{s}}
        printf("Calculated Load Resistance (R = K * Vo / (C * Delta_v * f_s)): \n"
               "R = %.4f Ohm\n", R);
    } else if (isnan(C)) {
        C = K * Vo / (Delta_v * R * f_s)  *  1e6; // C = \frac{K V_{o}}{\Delta_{v} R f_{s}}
        printf("Calculated Capacitance (C = K * Vo / (Delta_v * R * f_s)): \n"
               "C = %.4f uF\n", C);
    } else if (isnan(f_s)) {
        f_s = K * Vo / (C * Delta_v * R); // f_{s} = \frac{K V_{o}}{C \Delta_{v} R}
        printf("Calculated Switching Frequency (f_s = K * Vo / (C * Delta_v * R)): \n"
               "f_s = %.4f Hz\n", f_s);
    } else if (isnan(Delta_v)) {
        Delta_v = K * Vo / (C * R * f_s); // \Delta_{v} = \frac{K V_{o}}{C R f_{s}}
        printf("Calculated Capacitor Voltage Ripple (Delta_v = K * Vo / (C * R * f_s)): \n"
               "Delta_v = %.4f V\n", Delta_v);
    } else {
        printf("All variables provided. No calculation needed.\n");
    }
    // Save variables to file
    params.Vo = Vo;
    params.K = K;
    params.R = R;
    params.C = C * 1e-6; // convert back to F
    params.f_s = f_s;
    params.Delta_v = Delta_v;
    save_converter_params(&params, DATA_FILE);
}

void boost_dcm_duty_cycle(void)
{
    double Vo, Vi, K, Delta_1;
    Converter_Params params;
    if (load_converter_params(&params, DATA_FILE) != RETURN_OK) init_converter_params(&params);
    printf("\n>> Boost Converter - DCM\n"
           "\nFormula: Vo/Vi = (K + Delta_1)/Delta_1\n" // \frac{V_{o}}{V_{i}} = \frac{\Delta_{1} + K}{\Delta_{1}}
           "Variables: Vo (Output Voltage), Vi (Input Voltage), K (Duty Cycle), Delta_1 (Close time)\n"
           "Provide any 3 values to calculate the other.\n"
           "Enter '?' for the unknown variable.\n");
    // Input values
    input_float(&Vo, &params.Vo, "Output Voltage (Vo: V)");
    input_float(&Vi, &params.Vi, "Input Voltage (Vi: V)");
    input_float(&K,  &params.K, "Duty Cycle (K)");
    input_float(&Delta_1, &params.Delta_1, "Close time (Delta 1)");
    // Determine if more than one variable is unknown
    if ((isnan(Vo) + isnan(Vi) + isnan(K) + isnan(Delta_1)) != -1) {
        printf("Error: Please provide exactly 3 known values and 1 unknown value.\n");
        return;
    }
    // Determine which variable to calculate
    if (isnan(Vo)) {
        Vo = Vi * (Delta_1 + K) / Delta_1; // V_{o} = \frac{V_{i} \left(\Delta_{1} + K\right)}{\Delta_{1}}
        printf("Calculated Output Voltage (Vo = Vi * (Delta_1 + K) / Delta_1): %.4f V\n", Vo);
    } else if (isnan(Vi)) {
        Vi = Delta_1 * Vo / (Delta_1 + K); // V_{i} = \frac{\Delta_{1} V_{o}}{\Delta_{1} + K}
        printf("Calculated Input Voltage (Vi = Delta_1 * Vo / (Delta_1 + K)): %.4f V\n", Vi);
    } else if (isnan(K)) {
        K = Delta_1 * (-Vi + Vo) / Vi; // K = \frac{\Delta_{1} \left(- V_{i} + V_{o}\right)}{V_{i}}
        printf("Calculated Duty Cycle (K = Delta_1 * (-Vi + Vo) / Vi): %.4f\n", K);
    } else if (isnan(Delta_1)) {
        Delta_1 = -K * Vi / (Vi - Vo); // \Delta_{1} = - \frac{K V_{i}}{V_{i} - V_{o}}
        printf("Calculated Close time (Delta_1 = -K * Vi / (Vi - Vo)): %.4f\n", Delta_1);
    } else {
        printf("All variables provided. No calculation needed.\n");
    }
    // Save variables to file
    params.Vo = Vo;
    params.Vi = Vi;
    params.K = K;
    params.Delta_1 = Delta_1;
    save_converter_params(&params, DATA_FILE);
}

void boost_dcm_boundary_constant_vi(void)
{
    double Vi, K, L, f_s, Io;
    Converter_Params params;
    if (load_converter_params(&params, DATA_FILE) != RETURN_OK) init_converter_params(&params);
    printf("\n>> Boost Converter - DCM\n"
           "\nFormula: Io = K * Vi * (1 - K)/(2 * L * f_s)\n" // I_{o} = \frac{K V_{i} \left(1 - K\right)}{2 L f_{s}}
           "Variables: Vi (Input Voltage), K (Duty Cycle), L (Inductance), f_s (Switching Frequency), Io (Output Current)\n"
           "Provide any 4 values to calculate the other.\n"
           "Enter '?' for the unknown variable.\n");
    // Input values
    input_float(&Vi, &params.Vi, "Input Voltage (Vi: V)");
    input_float(&K,  &params.K, "Duty Cycle (K)");
    input_float(&L,  &params.L, "Inductance (L: mH)");
    L /= 1e3; // convert to H
    input_float(&f_s, &params.f_s, "Switching Frequency (f_s: Hz)");
    input_float(&Io, &params.Io, "Output Current (Io: A)");
    // Determine if more than one variable is unknown
    if ((isnan(Vi) + isnan(K) + isnan(L) + isnan(f_s) + isnan(Io)) != -1) {
        printf("Error: Please provide exactly 4 known values and 1 unknown value.\n");
        return;
    }
    // Determine which variable to calculate
    if (isnan(Vi)) {
        Vi = -2*Io*L*f_s/(K*(K - 1)); // V_{i} = - \frac{2 I_{o} L f_{s}}{K \left(K - 1\right)}
        printf("Calculated Input Voltage (Vi = -2*Io*L*f_s/(K*(K - 1))): \n"
               "Vi = %.4f V\n", Vi);
    } else if (isnan(K)) {
        K = (Vi - sqrt(Vi*(-8*Io*L*f_s + Vi)))/(2*Vi); // K = \frac{V_{i} - \sqrt{V_{i} \left(- 8 I_{o} L f_{s} + V_{i}\right)}}{2 V_{i}}
        printf("Calculated Duty Cycle (K = (Vi - sqrt(Vi*(-8*Io*L*f_s + Vi)))/(2*Vi)): \n"
               "K = %.4f\n", K);
    } else if (isnan(L)) {
        L = K*Vi*(1 - K)/(2*Io*f_s) * 1e3; // L = \frac{K V_{i} \left(1 - K\right)}{2 I_{o} f_{s}}
        printf("Calculated Inductance (L = K*Vi*(1 - K)/(2*Io*f_s)): \n"
               "L = %.4f mH\n", L);
    } else if (isnan(f_s)) {
        f_s = K*Vi*(1 - K)/(2*Io*L); // f_{s} = \frac{K V_{i} \left(1 - K\right)}{2 I_{o} L}
        printf("Calculated Switching Frequency (f_s = K*Vi*(1 - K)/(2*Io*L)): \n"
               "f_s = %.4f Hz\n", f_s);
    } else if (isnan(Io)) {
        Io = K*Vi*(1 - K)/(2*L*f_s); // I_{o} = \frac{K V_{i} \left(1 - K\right)}{2 L f_{s}}
        printf("Calculated Output Current (Io = K*Vi*(1 - K)/(2*L*f_s)): \n"
               "Io = %.4f A\n", Io);
    } else {
        printf("All variables provided. No calculation needed.\n");
    }
    // Save variables to file
    params.Vi = Vi;
    params.K = K;
    params.L = L * 1e3; // convert back to mH
    params.f_s = f_s;
    params.Io = Io;
    save_converter_params(&params, DATA_FILE);
}

void boost_dcm_boundary_constant_vo(void)
{
    double Vo, K, L, f_s, Io;
    Converter_Params params;
    if (load_converter_params(&params, DATA_FILE) != RETURN_OK) init_converter_params(&params);
    printf("\n>> Boost Converter - DCM\n"
           "\nFormula: I_o = V_o * K * (1-K)*(1-K) / (2 * L * f_s)\n" // I_{o} = \frac{V_{o} K \left(1 - K\right)^2}{2 L f_{s}}
           "Variables: Vo (Output Voltage), K (Duty Cycle), L (Close time), f_s (Switching Frequency), Io (Output Current)\n"
           "Provide any 4 values to calculate the other.\n"
           "Enter '?' for the unknown variable.\n");
    // Input values
    input_float(&Vo, &params.Vo, "Output Voltage (Vo: V)");
    input_float(&K,  &params.K, "Duty Cycle (K)");
    input_float(&L,  &params.L, "Inductance (L: mH)");
    L /= 1e3; // convert to H
    input_float(&f_s, &params.f_s, "Switching Frequency (f_s: Hz)");
    input_float(&Io, &params.Io, "Output Current (Io: A)");
    // Determine if more than one variable is unknown
    if ((isnan(Vo) + isnan(K) + isnan(L) + isnan(f_s) + isnan(Io)) != -1) {
        printf("Error: Please provide exactly 4 known values and 1 unknown value.\n");
        return;
    }
    // Determine which variable to calculate
    if (isnan(Vo)) {
        Vo = 2*Io*L*f_s/(K*(K - 1)*(K - 1)); // V_{o} = \frac{2 I_{o} L f_{s}}{K \left(K - 1\right)^{2}}
        printf("Calculated Input Voltage (Vo = 2*Io*L*f_s/(K*(K - 1)^2)): \n"
               "Vo = %.4f V\n", Vo);
    } else if (isnan(K)) {
    /*
    Formula for finding roots of a cubic equation:
    K = \frac{
            \sqrt[3]{\frac{- 27 I_{o} L f_{s} + V_{o}
            \left(\sqrt{\frac{- V_{o}^{2} + \left(27 I_{o} L f_{s} - V_{o}\right)^{2}}{V_{o}^{2}}} + 1\right)}{V_{o}}} \left(1 + \sqrt{3} i\right)
            \left(\sqrt[3]{\frac{- 27 I_{o} L f_{s} + V_{o} \left(\sqrt{\frac{- V_{o}^{2} + \left(27 I_{o} L f_{s} - V_{o}\right)^{2}}{V_{o}^{2}}} + 1\right)}{V_{o}}} \left(1 + \sqrt{3} i\right) + 4\right) + 4
        }
        {6 \sqrt[3]{\frac{- 27 I_{o} L f_{s} + V_{o} \left(\sqrt{\frac{- V_{o}^{2} + \left(27 I_{o} L f_{s} - V_{o}\right)^{2}}{V_{o}^{2}}} + 1\right)}{V_{o}}} \left(1 + \sqrt{3} i\right)}
    */
        printf("Unable to calculate Duty Cycle (K) The formula contains imaginary numbers: \n"
               "K = %.4f\n", K);
    } else if (isnan(L)) {
        L = K*Vo*(K - 1)*(K - 1)*2/(2*Io*f_s) * 1e3; // L = \frac{K V_{o} \left(K - 1\right)^{2}}{2 I_{o} f_{s}}
        printf("Calculated Inductance (L = K*Vo*(K - 1)*(K - 1)*2/(2*Io*f_s)): \n"
               "L = %.4f mH\n", L);
    } else if (isnan(f_s)) {
        f_s = K*Vo*(K - 1)*(K - 1)*2/(2*Io*L); // f_{s} = \frac{K V_{o} \left(K - 1\right)^{2}}{2 I_{o} L}
        printf("Calculated Switching Frequency (f_s = K*Vo*(K - 1)*(K - 1)*2/(2*Io*L)): \n"
               "f_s = %.4f Hz\n", f_s);
    } else if (isnan(Io)) {
        Io = K*Vo*(K - 1)*(K - 1)*2/(2*L*f_s); // I_{o} = \frac{K V_{o} \left(K - 1\right)^{2}}{2 L f_{s}}
        printf("Calculated Output Current (Io = K*Vo*(K - 1)*(K - 1)*2/(2*L*f_s)): \n"
               "Io = %.4f A\n", Io);
    } else {
        printf("All variables provided. No calculation needed.\n");
    }
    // Save variables to file
    params.Vo = Vo;
    params.K = K;
    params.L = L * 1e3; // convert back to mH
    params.f_s = f_s;
    params.Io = Io;
    save_converter_params(&params, DATA_FILE);
}
