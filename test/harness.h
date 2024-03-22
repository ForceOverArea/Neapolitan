#ifndef HARNESS_H_
#define HARNESS_H_

#include <stdio.h>
#include <stdlib.h>

#define TEST(testName)\
void _ ## testName();\
void testName()\
{\
    printf("%s..........", __func__);\
    _ ## testName();\
}\
void _ ## testName()

#define END_TEST printf("PASS\n");

#define ASSERT(boolExpr)\
if (!(boolExpr))\
{\
    printf("FAILED (assertion on line %d)\n", __LINE__);\
    return;\
}\

#define ASSERT_EQ(expected, actual, fmt)\
if ((expected) != (actual))\
{\
    printf("FAILED (equal assertion on line %d. ", __LINE__);\
    printf("Expected: "); printf(#fmt, (expected));\
    printf(", Actual: "); printf(#fmt, (actual));\
    printf(")\n");\
    return;\
}

#define ASSERT_OK(npStatusFn)\
if (0 != (size_t)(npStatusFn))\
{\
    printf("FAILED (ok status assertion on line %d. Status: %zu)\n", __LINE__, (size_t)(npStatusFn));\
    return;\
}\

#endif