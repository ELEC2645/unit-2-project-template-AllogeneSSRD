// ELEC2645 Unit 2 Project
// Auther: Xiangcheng Tao
// Student ID: 202013365
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

/* Prototypes mirroring the C++ version */
// main menu
static void main_menu(void);            // runs in the main loop
static void print_main_menu(void);      // output the main menu description
static void select_menu_item(int input); // run code based on user's choice
// sub menu
static void sub_menu_buck_ccm(void);
static void print_sub_menu_buck_ccm(void);
static int select_sub_menu_buck_ccm(int input);
static int go_back_to_sub_menu(void);
// input handling
static int  get_user_input(void);       /* get a valid integer menu choice */
static int  is_integer(const char *s);  /* validate integer string */

int main(void)
{
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
        int input = get_user_input();
        select_menu_item(input);
    }
}

static int get_user_input(void)
{
    enum { MENU_ITEMS = 5 };   /* 1..4 = items, 5 = Exit */
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
            menu_item_2();
            break;
        case 3:
            menu_item_3();
            break;
        case 4:
            menu_item_4();
            break;
        default:
            printf("Bye!\n");
            exit(0);
    }
}

static void print_main_menu(void)
{
    printf("\n----------- Main menu -----------\n");
    printf("\n"
           "\t\t\t\t\t\t\n"
           "\t1. Buck Converter - CCM\t\t\n"
           "\t2. Buck Converter - Constant input voltage\t\t\n"
           "\t3. Buck Converter - Constant output voltage\t\t\n"
           "\t4. Boost Converter - CCM\t\t\n"
           "\t5. Boost Converter - Constant input voltage\t\t\n"
           "\t6. Boost Converter - Constant output voltage\t\t\n"
           "\t7. Exit\t\t\t\t\n"
           "\t\t\t\t\t\t\n");
    printf("-----------------------------------\n");
}


static void sub_menu_buck_ccm(void)
{
    for(;;) {
        print_sub_menu_buck_ccm();
        {
            int input = get_user_input();
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
           "\t1. Duty cycle: K = Vo / Vi\t\t\n"
           "\t2. Sub Item 2\t\t\n"
           "\t3. Back to Main Menu\t\t\n"
           "\t\t\t\t\t\t\n");
    printf("--------------------------------------------\n");
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
            menu_item_2();
            return go_back_to_sub_menu();

        default:
            return RETURN_EXIT;
            // return 2; back to main menu
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