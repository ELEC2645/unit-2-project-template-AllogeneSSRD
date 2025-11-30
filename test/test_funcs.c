// Unit tests for funcs.c
// Auther: Xiangcheng Tao
// Student ID: 202013365
// Filename: main.c
// Create date: 2025-11-30

#include <stdio.h>
#include <math.h>
#include <string.h>

#include "cutest/CuTest.h"
#include "../funcs.h"

void test_hello(CuTest *cuTest)
{
    CuAssert(cuTest, "\nFail: input_float.\t",1 + 1 == 2);
}

void test_input_positive(CuTest *tc)
{
    double value;
    // Reference: https://www.tutorialspoint.com/c_standard_library/c_function_freopen.htm
    FILE *tmp = fopen("input.txt", "w");
    fprintf(tmp, "12345.6789\n");
    fclose(tmp);
    freopen("input.txt", "r", stdin);
    input_float(&value, "test positive");
    CuAssertDblEquals(tc, 12345.6789, value, 0.0001);
}

void test_input_negative(CuTest *tc)
{
    double value;
    FILE *tmp = fopen("input.txt", "w");
    fprintf(tmp, "-98765.4321\n");
    fclose(tmp);
    freopen("input.txt", "r", stdin);
    input_float(&value, "test negative");
    CuAssertDblEquals(tc, -98765.4321, value, 0.0001);
}

void test_buck_ccm_duty_cycle_K(CuTest *tc) {
    // create input file, provide Vo=10, Vi=25, K=?
    FILE *input = fopen("input.txt", "w");
    fprintf(input, "10\n25\n?\n");
    fclose(input);
    // redirect stdin to file
    freopen("input.txt", "r", stdin);
    // redirect stdout to file
    FILE *output = fopen("output.txt", "w");
    freopen("output.txt", "w", stdout);

    buck_ccm_duty_cycle();

    // Reference: https://stackoverflow.com/a/7665043/31971778
    // fflush(stdout);
    freopen("CON", "w", stdout); // redirect stdout to console on Windows
    fclose(output);

    // verify output contains correct result
    char buffer[1024];
    FILE *result = fopen("output.txt", "r");
    int n = fread(buffer, sizeof(char), sizeof(buffer) - 1, result);
    buffer[n] = '\0';  // ensure string ending with '\0'
    fclose(result);

    CuAssertTrue(tc, strstr(buffer, "K = 0.4000") != NULL); // find string "0.4000" in output
}

void test_buck_ccm_duty_cycle_Vo(CuTest *tc) {
    FILE *input = fopen("input.txt", "w");
    fprintf(input, "?\n100\n0.2\n");
    fclose(input);
    freopen("input.txt", "r", stdin);
    FILE *output = fopen("output.txt", "w");
    freopen("output.txt", "w", stdout);

    buck_ccm_duty_cycle();
    freopen("CON", "w", stdout); // redirect stdout to console on Windows
    fclose(output);

    // verify output contains correct result
    char buffer[1024];
    FILE *result = fopen("output.txt", "r");
    int n = fread(buffer, sizeof(char), sizeof(buffer) - 1, result);
    buffer[n] = '\0';  // ensure string ending with '\0'
    fclose(result);

    CuAssertTrue(tc, strstr(buffer, "Vo = 20.0000") != NULL);
}

void test_buck_ccm_duty_cycle_Vi(CuTest *tc) {
    FILE *input = fopen("input.txt", "w");
    fprintf(input, "80\n?\n0.5\n");
    fclose(input);
    freopen("input.txt", "r", stdin);
    FILE *output = fopen("output.txt", "w");
    freopen("output.txt", "w", stdout);

    buck_ccm_duty_cycle();
    freopen("CON", "w", stdout);
    fclose(output);

    char buffer[1024];
    FILE *result = fopen("output.txt", "r");
    int n = fread(buffer, sizeof(char), sizeof(buffer) - 1, result);
    buffer[n] = '\0';
    fclose(result);

    CuAssertTrue(tc, strstr(buffer, "Vi = 160.0000") != NULL);
}

CuSuite* CuGetSuite(void)
{
    CuSuite* suite = CuSuiteNew();

    // test input_float
    SUITE_ADD_TEST(suite, test_hello);
    SUITE_ADD_TEST(suite, test_input_positive);
    SUITE_ADD_TEST(suite, test_input_negative);
    // Test buck_ccm_duty_cycle
    SUITE_ADD_TEST(suite, test_buck_ccm_duty_cycle_K);
    SUITE_ADD_TEST(suite, test_buck_ccm_duty_cycle_Vo);
    SUITE_ADD_TEST(suite, test_buck_ccm_duty_cycle_Vi);
    return suite;
}


int main()
{
    printf("####################################\n"
           "        Runing test_funcs...\n"
           "####################################\n"
    );
    CuString* output = CuStringNew();
    CuSuite* suite = CuGetSuite();
    CuSuiteRun(suite);
    // CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
    return 0;
}
