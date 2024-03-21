#include "harness.h"
#include "vec.h"

TEST(vec_capacity_doubles_at_correct_number_of_elements)
{
    Vec_T* myVec = newVec(0);
    ASSERT(myVec != NULL)

    // Add 9 elements, we will alloc for 16
    for (size_t i = 0; i < 9; i++)
    {
        VecElement_T myElem;
        myElem.floating = 0;

        Vec_T* tmp = pushToVec(myVec, myElem);
        ASSERT(tmp != NULL)

        myVec = tmp; 
    }

    ASSERT_EQ((size_t)16, myVec->capacity, %zu)
    ASSERT_EQ((size_t)9, myVec->len, %zu)

    END_TEST
}

int main(void)
{
    vec_capacity_doubles_at_correct_number_of_elements();
    return 0;
}