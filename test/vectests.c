#include "harness.h"
#include "vec.h"

Vec_S* newVec123(void)
{
    Vec_S* vec = newVecWithLen(3);

    vec->elements[0].floating = 1;
    vec->elements[1].floating = 2;
    vec->elements[2].floating = 3;

    return vec;
}

TEST(vector_sum_works)
{
    Vec_S* vec1 = newVec123();
    Vec_S* vec2 = newVec123();
    Vec_S* result = newVec123();

    elementWiseAdd(result, vec1, vec2, true);

    ASSERT_EQ(2.0, result->elements[0].floating, %lf)
    ASSERT_EQ(4.0, result->elements[1].floating, %lf)
    ASSERT_EQ(6.0, result->elements[2].floating, %lf)

    free(vec1);
    free(vec2);
    free(result);

    END_TEST
}

TEST(vec_capacity_doubles_at_correct_number_of_elements)
{
    Vec_S* myVec = newVec(0);
    ASSERT(myVec != NULL)

    // Add 9 elements, we will alloc for 16
    for (size_t i = 0; i < 9; i++)
    {
        VecElement_U myElem;
        myElem.floating = 0;

        Vec_S* tmp = pushToVec(myVec, myElem);
        ASSERT(tmp != NULL)

        myVec = tmp; 
    }

    ASSERT_EQ((size_t)16, myVec->capacity, %zu)
    ASSERT_EQ((size_t)9, myVec->len, %zu)

    END_TEST
}

int main(void)
{
    vector_sum_works();
    vec_capacity_doubles_at_correct_number_of_elements();
    return 0;
}