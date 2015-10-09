#include "gtest/gtest.h"
#include "Map.h"

#include <memory>

class MapTest : public testing::Test 
{
protected:
std::unique_ptr<Map<int, int>> pMap;


virtual void SetUp() {
    pMap.reset(new Map<int, int>());
}

virtual void TearDown() 
{

}

};

TEST_F(MapTest, MappingSouldWork)
{
    (*pMap)[10] = 25;
    ASSERT_EQ(25, (*pMap)[10]);
}
