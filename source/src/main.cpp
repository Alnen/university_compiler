#include <boost/any.hpp>
#include <iostream>
#include <utility>

#include "Map.h"
#include "DFA.h"

int main (int argc, char** argv)
{
    Map<int, int> map;
    map[5] = 10;
    map[5] = 15;
    std::cout << map[5] << std::endl;
    
    //PRINT_TYPE<decltype(re)> a;
    
    return 0;
}
