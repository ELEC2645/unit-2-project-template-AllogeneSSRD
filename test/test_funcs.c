// Unit tests for funcs.c
// Auther: Xiangcheng Tao
// Filename: test_funcs.c
// Create date: 2025-11-30

#include <stdio.h>
#include <math.h>
#include <string.h>

#include "cutest/CuTest.h"
#include "../funcs.h"

#define DEBUG 1  // Set to 1 to enable debug output

void test_hello(CuTest *cuTest)
{
    CuAssert(cuTest, "\nFail: input_float.\t",1 + 1 == 2);
}

void test_input_positive(CuTest *tc)
{
    if (DEBUG) printf("[Debug] Running test_input_positive...");
    double value, params = NAN;
    // Reference: https://www.tutorialspoint.com/c_standard_library/c_function_freopen.htm
    FILE *tmp = fopen("input.txt", "w");
    fprintf(tmp, "12345.6789\n");
    fclose(tmp);
    freopen("input.txt", "r", stdin);
    input_float(&value, &params, "test positive");
    CuAssertDblEquals(tc, 12345.6789, value, 0.0001);
}

void test_input_negative(CuTest *tc)
{
    if (DEBUG) printf("\n[Debug] Running test_input_negative...");
    double value, params = NAN;
    FILE *tmp = fopen("input.txt", "w");
    fprintf(tmp, "-98765.4321\n");
    fclose(tmp);
    freopen("input.txt", "r", stdin);
    input_float(&value, &params, "test negative");
    CuAssertDblEquals(tc, -98765.4321, value, 0.0001);
}

void test_buck_ccm_duty_cycle_K(CuTest *tc) {
    if (DEBUG) printf("\n[Debug] Running test_buck_ccm_duty_cycle_K...\n");
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

    if (DEBUG) printf("[Debug] Output buffer:\n%s\n", buffer);
    CuAssertTrue(tc, strstr(buffer, "K = 0.4000") != NULL); // find string "0.4000" in output
}

void test_buck_ccm_duty_cycle_Vo(CuTest *tc) {
    if (DEBUG) printf("[Debug] Running test_buck_ccm_duty_cycle_Vo...\n");
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

    if (DEBUG) printf("[Debug] Output buffer:\n%s\n", buffer);
    CuAssertTrue(tc, strstr(buffer, "Vo = 20.0000") != NULL);
}

void test_buck_ccm_duty_cycle_Vi(CuTest *tc) {
    if (DEBUG) printf("[Debug] Running test_buck_ccm_duty_cycle_Vi...\n");
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

    if (DEBUG) printf("[Debug] Output buffer:\n%s\n", buffer);
    CuAssertTrue(tc, strstr(buffer, "Vi = 160.0000") != NULL);
}

void test_buck_ccm_inductor_Iripple_Delta_i(CuTest *tc) {
    if (DEBUG) printf("[Debug] Running test_buck_ccm_inductor_Iripple_Delta_i...\n");
    FILE *input = fopen("input.txt", "w");
    fprintf(input, "50\n0.8\n15\n45000\n?\n");
    fclose(input);
    freopen("input.txt", "r", stdin);
    FILE *output = fopen("output.txt", "w");
    freopen("output.txt", "w", stdout);

    buck_ccm_inductor_Iripple();
    freopen("CON", "w", stdout);
    fclose(output);

    char buffer[1024];
    FILE *result = fopen("output.txt", "r");
    int n = fread(buffer, sizeof(char), sizeof(buffer) - 1, result);
    buffer[n] = '\0';
    fclose(result);

    if (DEBUG) printf("[Debug] Output buffer:\n%s\n", buffer);
    CuAssertTrue(tc, strstr(buffer, "Delta i = 0.0148") != NULL);
}

void test_buck_ccm_inductor_Iripple_Delta_Vo(CuTest *tc) {
    if (DEBUG) printf("[Debug] Running test_buck_ccm_inductor_Iripple_Vo...\n");
    FILE *input = fopen("input.txt", "w");
    fprintf(input, "?\n0.8\n15\n45000\n0.0148148\n");
    fclose(input);
    freopen("input.txt", "r", stdin);
    FILE *output = fopen("output.txt", "w");
    freopen("output.txt", "w", stdout);

    buck_ccm_inductor_Iripple();
    freopen("CON", "w", stdout);
    fclose(output);

    char buffer[1024];
    FILE *result = fopen("output.txt", "r");
    int n = fread(buffer, sizeof(char), sizeof(buffer) - 1, result);
    buffer[n] = '\0';
    fclose(result);

    if (DEBUG) printf("[Debug] Output buffer:\n%s\n", buffer);
    CuAssertTrue(tc, strstr(buffer, "Vo = 50.00") != NULL);
}

void test_buck_ccm_inductor_Iripple_Delta_K(CuTest *tc) {
    if (DEBUG) printf("[Debug] Running test_buck_ccm_inductor_Iripple_K...\n");
    FILE *input = fopen("input.txt", "w");
    fprintf(input, "50\n?\n15\n45000\n0.0148148\n");
    fclose(input);
    freopen("input.txt", "r", stdin);
    FILE *output = fopen("output.txt", "w");
    freopen("output.txt", "w", stdout);

    buck_ccm_inductor_Iripple();
    freopen("CON", "w", stdout);
    fclose(output);

    char buffer[1024];
    FILE *result = fopen("output.txt", "r");
    int n = fread(buffer, sizeof(char), sizeof(buffer) - 1, result);
    buffer[n] = '\0';
    fclose(result);

    if (DEBUG) printf("[Debug] Output buffer:\n%s\n", buffer);
    CuAssertTrue(tc, strstr(buffer, "K = 0.80") != NULL);
}

void test_buck_ccm_inductor_Iripple_Delta_L(CuTest *tc) {
    if (DEBUG) printf("[Debug] Running test_buck_ccm_inductor_Iripple_L...\n");
    FILE *input = fopen("input.txt", "w");
    fprintf(input, "50\n0.8\n?\n45000\n0.0148148\n");
    fclose(input);
    freopen("input.txt", "r", stdin);
    FILE *output = fopen("output.txt", "w");
    freopen("output.txt", "w", stdout);

    buck_ccm_inductor_Iripple();
    freopen("CON", "w", stdout);
    fclose(output);

    char buffer[1024];
    FILE *result = fopen("output.txt", "r");
    int n = fread(buffer, sizeof(char), sizeof(buffer) - 1, result);
    buffer[n] = '\0';
    fclose(result);

    if (DEBUG) printf("[Debug] Output buffer:\n%s\n", buffer);
    CuAssertTrue(tc, strstr(buffer, "L = 15.00") != NULL);
}

void test_buck_ccm_inductor_Iripple_Delta_f_s(CuTest *tc) {
    if (DEBUG) printf("[Debug] Running test_buck_ccm_inductor_Iripple_f_s...\n");
    FILE *input = fopen("input.txt", "w");
    fprintf(input, "50\n0.8\n15\n?\n0.0148148148\n");
    fclose(input);
    freopen("input.txt", "r", stdin);
    FILE *output = fopen("output.txt", "w");
    freopen("output.txt", "w", stdout);

    buck_ccm_inductor_Iripple();
    freopen("CON", "w", stdout);
    fclose(output);

    char buffer[1024];
    FILE *result = fopen("output.txt", "r");
    int n = fread(buffer, sizeof(char), sizeof(buffer) - 1, result);
    buffer[n] = '\0';
    fclose(result);

    if (DEBUG) printf("[Debug] Output buffer:\n%s\n", buffer);
    CuAssertTrue(tc, strstr(buffer, "f_s = 45000.00") != NULL);
}

void test_buck_ccm_inductor_Iripple_Vripple_Delta_v(CuTest *tc) {
    if (DEBUG) printf("[Debug] Running test_buck_ccm_inductor_Vripple_Delta_v...\n");
    FILE *input = fopen("input.txt", "w");
    fprintf(input, "50\n0.8\n0.22\n15\n45000\n?\n");
    fclose(input);
    freopen("input.txt", "r", stdin);
    FILE *output = fopen("output.txt", "w");
    freopen("output.txt", "w", stdout);

    buck_ccm_capacitor_Vripple();
    freopen("CON", "w", stdout);
    fclose(output);

    char buffer[1024];
    FILE *result = fopen("output.txt", "r");
    int n = fread(buffer, sizeof(char), sizeof(buffer) - 1, result);
    buffer[n] = '\0';
    fclose(result);

    if (DEBUG) printf("[Debug] Output buffer:\n%s\n", buffer);
    CuAssertTrue(tc, strstr(buffer, "Delta v = 0.187") != NULL);
}

void test_buck_ccm_inductor_Iripple_Vripple_Vo(CuTest *tc) {
    if (DEBUG) printf("[Debug] Running test_buck_ccm_inductor_Vripple_Vo...\n");
    FILE *input = fopen("input.txt", "w");
    fprintf(input, "?\n0.8\n0.22\n15\n45000\n0.1870557\n");
    fclose(input);
    freopen("input.txt", "r", stdin);
    FILE *output = fopen("output.txt", "w");
    freopen("output.txt", "w", stdout);

    buck_ccm_capacitor_Vripple();
    freopen("CON", "w", stdout);
    fclose(output);

    char buffer[1024];
    FILE *result = fopen("output.txt", "r");
    int n = fread(buffer, sizeof(char), sizeof(buffer) - 1, result);
    buffer[n] = '\0';
    fclose(result);

    if (DEBUG) printf("[Debug] Output buffer:\n%s\n", buffer);
    CuAssertTrue(tc, strstr(buffer, "Vo = 50.0") != NULL);
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
    // Test buck_ccm_inductor_Iripple
    SUITE_ADD_TEST(suite, test_buck_ccm_inductor_Iripple_Delta_i);
    SUITE_ADD_TEST(suite, test_buck_ccm_inductor_Iripple_Delta_Vo);
    SUITE_ADD_TEST(suite, test_buck_ccm_inductor_Iripple_Delta_K);
    SUITE_ADD_TEST(suite, test_buck_ccm_inductor_Iripple_Delta_L);
    SUITE_ADD_TEST(suite, test_buck_ccm_inductor_Iripple_Delta_f_s);
    // Test buck_ccm_inductor_Vripple
    SUITE_ADD_TEST(suite, test_buck_ccm_inductor_Iripple_Vripple_Delta_v);
    SUITE_ADD_TEST(suite, test_buck_ccm_inductor_Iripple_Vripple_Vo);
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
    printf("\n\n----- Test Details -----\n\n");
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
    return 0;
}
