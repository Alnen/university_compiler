#ifndef UTILITY_TEST_CPP
#define UTILITY_TEST_CPP

#include "gtest/gtest.h"
#include "Utility.h"

#include <vector>
#include <utility>

TEST(UtilityTest, set_union_shouldReturnUnionOfTwoVectors)
{
    std::vector<int> vec1 = {1, 2, 3, 4};
    std::vector<int> vec2 = {4, 5, 7, 3, 8};
    std::vector<int> calculated_result;
    std::vector<int> expected_result = {1, 2, 3, 4, 5, 7, 8};
    ASSERT_NO_THROW( { calculated_result = std::move(Utility::construct_set_union(vec1, vec2)); } );
    ASSERT_EQ(expected_result.size(), calculated_result.size());
    std::sort(calculated_result.begin(), calculated_result.end());
    ASSERT_EQ(expected_result, calculated_result);
}

#endif // UTILITY_TEST_CPP
