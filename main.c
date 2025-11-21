// ELEC2645 Unit 2 Project Template
// Command Line Application Menu Handling Code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "funcs.h"

/* Prototypes mirroring the C++ version */
// main menu
static void main_menu(void);            /* runs in the main loop */
static void print_main_menu(void);      /* output the main menu description */
static void select_menu_item(int input);/* run code based on user's choice */
static void go_back_to_main(void);      /* wait for 'b'/'B' to continue */
// sub menu
static void sub_menu_1(void);
static void print_sub_menu(void);
static int select_sub_menu_item(int input);
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
            /* EOF or error; bail out gracefully */
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
            sub_menu_1();
            go_back_to_main();
            break;
        case 2:
            menu_item_2();
            go_back_to_main();
            break;
        case 3:
            menu_item_3();
            go_back_to_main();
            break;
        case 4:
            menu_item_4();
            go_back_to_main();
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
           "\t2. Buck Converter - DCM\t\t\n"
           "\t3. Boost Converter - CCM\t\t\n"
           "\t4. Boost Converter - DCM\t\t\n"
           "\t5. Exit\t\t\t\t\n"
           "\t\t\t\t\t\t\n");
    printf("-----------------------------------\n");
}

static void go_back_to_main(void)
{
    char buf[64];
    do {
        printf("\nEnter 'b' or 'B' to go back to main menu: ");
        if (!fgets(buf, sizeof(buf), stdin)) {
            puts("\nInput error. Exiting.");
            exit(1);
        }
        buf[strcspn(buf, "\r\n")] = '\0'; /* strip newline */
    } while (!(buf[0] == 'b' || buf[0] == 'B') || buf[1] != '\0');
}


static void sub_menu_1(void)
{
    for(;;) {
        print_sub_menu();
        {
            int input = get_user_input();
                select_sub_menu_item(input);
                printf("\nReturning to Main Menu...\n");
                // Exit sub menu, return to main menu
                break;
            }
            printf("\nReturn Sub Menu \n");
    }
}

static void print_sub_menu(void)
{
    printf("\n--------- Sub Menu ---------\n");
    printf("\n"
           "\t\t\t\t\t\t\n"
           "\t1. Duty cycle K\t\t\n"
           "\t2. Sub Item 2\t\t\n"
           "\t3. Back to Main Menu\t\t\n"
           "\t\t\t\t\t\t\n");
    printf("------------------------------\n");
}

static int select_sub_menu_item(int input)
{
    switch (input) {
        case 1:
            menu_item_1();
            return go_back_to_sub_menu();
            // break;
        case 2:
            menu_item_2();
            return go_back_to_sub_menu();
            // break;
        default:
            return 2;
            /* return 2; back to main menu */
    }
}

static int go_back_to_sub_menu(void)
{
    char buf[64];
    do {
        printf("\nEnter 'b' or 'B' to go back to sub menu: ");
        if (!fgets(buf, sizeof(buf), stdin)) {
            puts("\nInput error. Exiting.");
            return 1; // Error, stay in sub menu
        }
        buf[strcspn(buf, "\r\n")] = '\0'; /* strip newline */
    } while (!(buf[0] == 'b' || buf[0] == 'B') || buf[1] != '\0');

    return 0; // Go back to sub menu
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