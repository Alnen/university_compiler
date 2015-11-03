#ifndef DFA_TEST_CPP
#define DFA_TEST_CPP

#include "gtest/gtest.h"
#include "FiniteAutomata/DFA.h"

#include <memory>

class DFATest : public testing::Test
{
protected:
std::unique_ptr<DFA<int, int>> pDFA;


virtual void SetUp() {
    pDFA.reset(new DFA<int, int>());
}

virtual void TearDown()
{

}

};

TEST_F(DFATest, MappingSouldWork)
{

}

TEST_F(DFATest, SizeOfBothVectorsShouldBeEqual)\
{

}

#endif // DFA_TEST_CPP
