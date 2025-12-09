// ELEC2645 Unit 2 Project
// Auther: Xiangcheng Tao
// Filename: main.c
// Create date: 2025-11-19

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "funcs.h"

#ifndef RETURN_OK
#define RETURN_OK 0   // Normal return
#define RETURN_ERROR 1   // Wrong input or error
#define RETURN_EXIT 2   // User chose to exit
#endif

#ifndef DEBUG
#define DEBUG 0  // Set to 1 to enable debug output
#endif

/* Prototypes mirroring the C++ version */
// main menu
static void main_menu(void);            // runs in the main loop
static void print_main_menu(void);      // output the main menu description
static void select_menu_item(int input); // run code based on user's choice
// sub menu
// Buck Converter - CCM
static void sub_menu_buck_ccm(void);
static void print_sub_menu_buck_ccm(void);
static int select_sub_menu_buck_ccm(int input);
// Buck Converter - DCM
static void sub_menu_buck_dcm(void);
static void print_sub_menu_buck_dcm(void);
static int select_sub_menu_buck_dcm(int input);
// Boost Converter - CCM
static void sub_menu_boost_ccm(void);
static void print_sub_menu_boost_ccm(void);
static int select_sub_menu_boost_ccm(int input);
// Boost Converter - DCM
static void sub_menu_boost_dcm(void);
static void print_sub_menu_boost_dcm(void);
static int select_sub_menu_boost_dcm(int input);

// go back to sub menu
static int go_back_to_sub_menu(void);
// input handling
static int  get_user_input(int items);       /* get a valid integer menu choice */
static int  is_integer(const char *s);  /* validate integer string */

int main(void)
{
    printf("DC-DC Converter Smart Variable Solver\n"
           "Auther: Xiangcheng Tao\n"
           "\n"
    );
    /* this will run forever until we call exit(0) in select_menu_item() */
    for(;;) {
        main_menu();
    }
    /* not reached */
    return 0;
}

static void main_menu(void)
{
    print_main_menu();
    {
        int input = get_user_input(7); // number of menu items
        select_menu_item(input);
    }
}

static int get_user_input(int items)
{
    const int MENU_ITEMS = items;
    // enum { MENU_ITEMS = it};   /* 1..6 = items, 7 = Exit */
    char buf[128];
    int valid_input = 0;
    int value = 0;

    do {
        printf("\nSelect item: ");
        if (!fgets(buf, sizeof(buf), stdin)) {
            // EOF or error; bail out gracefully
            puts("\nInput error. Exiting.");
            exit(1);
        }

        // strip trailing newline
        buf[strcspn(buf, "\r\n")] = '\0';

        if (!is_integer(buf)) {
            printf("Enter an integer!\n");
            valid_input = 0;
        } else {
            value = (int)strtol(buf, NULL, 10);
            if (value >= 1 && value <= MENU_ITEMS) {
                valid_input = 1;
            } else {
                printf("Invalid menu item!\n");
                valid_input = 0;
            }
        }
    } while (!valid_input);

    return value;
}

static void select_menu_item(int input)
{
    switch (input) {
        case 1:
            sub_menu_buck_ccm();
            break;
        case 2:
            sub_menu_buck_dcm();
            break;
        case 3:
            sub_menu_boost_ccm();
            break;
        case 4:
            sub_menu_boost_dcm();
            break;
        default:
            printf("Bye!\n");
            exit(0);
    }
}

static void print_main_menu(void)
{
    printf("\n------------ Main menu ------------\n");
    printf("\n"
           "\t\t\t\t\t\t\n"
           "\t1. Buck Converter - CCM\t\t\n"
           "\t2. Buck Converter - DCM\t\t\n"
           "\t3. Boost Converter - CCM\t\t\n"
           "\t4. Boost Converter - DCM\t\t\n"
           "\t5. Exit\t\t\t\t\n"
           "\t\t\t\t\t\t\n");
    printf("-----------------------------------\n");
}


static void sub_menu_buck_ccm(void)
{
    for(;;) {
        print_sub_menu_buck_ccm();
        {
            int input = get_user_input(5);
            if (select_sub_menu_buck_ccm(input) == RETURN_EXIT) {
                printf("\nReturning to Main Menu...\n");
                // Exit sub menu, return to main menu
                break;
            }
            printf("\nReturn Sub Menu \n");
        }
    }
}

static void print_sub_menu_buck_ccm(void)
{
    printf("\n---------- Buck Converter - CCM ----------\n");
    printf("\n"
           "\tChoice a formula to calculate.\n"
           "\t1. Duty cycle K\t\t\n"
           "\t2. Inductor & Current Ripple\t\t\n"
           "\t3. Capacitor & Voltage Ripple\t\t\n"
           "\t4. Back to Main Menu\t\t\n"
           "\t5. Exit\t\t\t\t\n"
           "\t\t\t\t\t\t\n");
    printf("------------------------------------------\n");
}

static int select_sub_menu_buck_ccm(int input)
{
    switch (input) {
        case 1:
            buck_ccm_duty_cycle();
            return go_back_to_sub_menu();
            // return 0 for error
            // return 1 for success
        case 2:
            buck_ccm_inductor_Iripple();
            return go_back_to_sub_menu();
        case 3:
            buck_ccm_capacitor_Vripple();
            return go_back_to_sub_menu();
        case 5:
            printf("Bye!\n");
            exit(0);
        default:
            return RETURN_EXIT;
            // return 2; back to main menu
    }
}

static void sub_menu_buck_dcm(void)
{
    for(;;) {
        print_sub_menu_buck_dcm();
        {
            int input = get_user_input(5);
            if (select_sub_menu_buck_dcm(input) == RETURN_EXIT) {
                printf("\nReturning to Main Menu...\n");
                // Exit sub menu, return to main menu
                break;
            }
            printf("\nReturn Sub Menu \n");
        }
    }
}

static void print_sub_menu_buck_dcm(void)
{
    printf("\n---------- Buck Converter - DCM ----------\n");
    printf("\n"
           "\tChoice a formula to calculate.\n"
           "\t1. Duty cycle K\t\t\n"
           "\t2. Boundary when constant input voltage\t\t\n"
           "\t3. Boundary when constant output voltage\t\t\n"
           "\t4. Back to Main Menu\t\t\n"
           "\t5. Exit\t\t\t\t\n"
           "\t\t\t\t\t\t\n");
    printf("------------------------------------------\n");
}

static int select_sub_menu_buck_dcm(int input)
{
    switch (input) {
        case 1:
            buck_dcm_duty_cycle();
            return go_back_to_sub_menu();
        case 2:
            buck_dcm_boundary_constant_vi();
            return go_back_to_sub_menu();
        case 3:
            buck_dcm_boundary_constant_vo();
            return go_back_to_sub_menu();
        case 5:
            printf("Bye!\n");
            exit(0);
        default:
            return RETURN_EXIT;
    }
}

// Boost Converter - CCM

static void sub_menu_boost_ccm(void)
{
    for(;;) {
        print_sub_menu_boost_ccm();
        {
            int input = get_user_input(5);
            if (select_sub_menu_boost_ccm(input) == RETURN_EXIT) {
                printf("\nReturning to Main Menu...\n");
                // Exit sub menu, return to main menu
                break;
            }
            printf("\nReturn Sub Menu \n");
        }
    }
}

static void print_sub_menu_boost_ccm(void)
{
    printf("\n---------- Boost Converter - CCM ----------\n");
    printf("\n"
           "\tChoice a formula to calculate.\n"
           "\t1. Duty cycle K\t\t\n"
           "\t2. Inductor & Current Ripple\t\t\n"
           "\t3. Capacitor & Voltage Ripple\t\t\n"
           "\t4. Back to Main Menu\t\t\n"
           "\t5. Exit\t\t\t\t\n"
           "\t\t\t\t\t\t\n");
    printf("-------------------------------------------\n");
}

static int select_sub_menu_boost_ccm(int input)
{
    switch (input) {
        case 1:
            boost_ccm_duty_cycle();
            return go_back_to_sub_menu();
        case 2:
            boost_ccm_inductor_Iripple();
            return go_back_to_sub_menu();
        case 3:
            boost_ccm_capacitor_Vripple();
            return go_back_to_sub_menu();
        case 5:
            printf("Bye!\n");
            exit(0);
        default:
            return RETURN_EXIT;
    }
}

// Boost Converter - DCM

static void sub_menu_boost_dcm(void)
{
    for(;;) {
        print_sub_menu_boost_dcm();
        {
            int input = get_user_input(5);
            if (select_sub_menu_boost_dcm(input) == RETURN_EXIT) {
                printf("\nReturning to Main Menu...\n");
                break;
            }
            printf("\nReturn Sub Menu \n");
        }
    }
}

static void print_sub_menu_boost_dcm(void)
{
    printf("\n---------- Boost Converter - DCM ----------\n");
    printf("\n"
           "\tChoice a formula to calculate.\n"
           "\t1. Duty cycle K\t\t\n"
           "\t2. Boundary when constant input voltage\t\t\n"
           "\t3. Boundary when constant output voltage\t\t\n"
           "\t4. Back to Main Menu\t\t\n"
           "\t5. Exit\t\t\t\t\n"
           "\t\t\t\t\t\t\n");
    printf("-------------------------------------------\n");
}

static int select_sub_menu_boost_dcm(int input)
{
    switch (input) {
        case 1:
            boost_dcm_duty_cycle();
            return go_back_to_sub_menu();
        case 2:
            boost_dcm_boundary_constant_vi();
            return go_back_to_sub_menu();
        case 3:
            boost_dcm_boundary_constant_vo();
            return go_back_to_sub_menu();
        case 5:
            printf("Bye!\n");
            exit(0);
        default:
            return RETURN_EXIT;
    }
}

static int go_back_to_sub_menu(void)
{
    char buf[64];
    do {
        printf("\nEnter 'b' or 'B' to go back to sub menu: ");
        if (!fgets(buf, sizeof(buf), stdin)) {
            puts("\nInput error. Exiting.");
            return RETURN_ERROR; // Error, stay in sub menu
        }
        buf[strcspn(buf, "\r\n")] = '\0'; /* strip newline */
    } while (!(buf[0] == 'b' || buf[0] == 'B') || buf[1] != '\0');

    return RETURN_OK; // Go back to sub menu
}

/* Return 1 if s is an optional [+/-] followed by one-or-more digits, else 0. */
static int is_integer(const char *s)
{
    if (!s || !*s) return 0;

    /* optional sign */
    if (*s == '+' || *s == '-') s++;

    /* must have at least one digit */
    if (!isdigit((unsigned char)*s)) return 0;

    while (*s) {
        if (!isdigit((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}
