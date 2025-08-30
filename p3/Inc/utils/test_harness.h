#ifndef __TEST_HARNESS_H
#define __TEST_HARNESS_H

enum TestResult_t
{
    UNKNOWN = -1,
    PASS,
    SKIP,
    FAIL,
};
typedef enum TestResult_t TestResult;
typedef TestResult (*TestCase)(void);
void Harness_Loop(TestCase *test_cases, int num_cases);

#endif