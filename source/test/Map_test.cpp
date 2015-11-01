#ifndef MAP_TEST_CPP
#define MAP_TEST_CPP

#include "gtest/gtest.h"
#include "Map.h"

#include <memory>

template <class Key, class Value>
class MapTestClass : public Map<Key, Value>
{
public:
    size_t value_size() const
    {
        return this->m_values.size();
    }

    size_t key_size() const
    {
        return this->m_keys.size();
    }
};

class MapTest : public testing::Test 
{
protected:
std::unique_ptr<MapTestClass<int, int>> pMap;


virtual void SetUp() {
    pMap.reset(new MapTestClass<int, int>());
}

virtual void TearDown() 
{

}

};

/*TEST_F(MapTest, MappingSouldWork)
{
    std::vector<int> keys = {10, 20, 30};
    std::vector<int> values = {1, 2, 3};
    for (size_t i = 0; i < keys.size(); ++i)
    {
        ASSERT_NO_THROW({ (*pMap)[keys[i]] = values[i]; });
    }

    int current_value;
    for (size_t i = 0; i < keys.size(); ++i)
    {
        ASSERT_NO_THROW({ current_value = (*pMap)[keys[i]]; });
        ASSERT_EQ(values[i], current_value);
    }
}

TEST_F(MapTest, SizeOfBothVectorsShouldBeEqual)\
{
    ASSERT_EQ(0, pMap->size());
    ASSERT_EQ(0, pMap->value_size());
    ASSERT_EQ(0, pMap->key_size());
    (*pMap)[10] = 25;
    EXPECT_EQ(1, pMap->size());
    EXPECT_EQ(1, pMap->value_size());
    EXPECT_EQ(1, pMap->key_size());
}*/


#endif // MAP_TEST_CPP
