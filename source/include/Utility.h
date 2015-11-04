#ifndef UTILITY_H
#define UTILITY_H

#include <algorithm>
#include <vector>
#include <memory>

struct PointerEquality
{
    template <class T, class V>
    bool operator ()(const T& lhs, const V& rhs) const
    {
        bool answer = (*lhs) == (*rhs);
        return answer;
    }
};

struct PointerLess
{
    template <class T, class V>
    bool operator ()(const T& lhs, const V& rhs) const
    {
        bool answer = (*lhs) < (*rhs);
        std::cout << lhs << rhs << " " << answer << std::endl;
        return answer;
    }
};

template <class T>
std::ostream& operator << (std::ostream& out, const std::shared_ptr<T>& pointer)
{
    out << pointer.get();
    return out;
}

template <class T, class V>
std::ostream& operator << (std::ostream& out, const std::pair<T, V>& cont)
{
    out << "[";
    out << cont.first;
    out << "-";
    out << cont.second;
    out << "]";
    return out;
}

template <class T>
std::ostream& operator << (std::ostream& out, const std::vector<T>& cont)
{
    out << "[";
    if (cont.size() > 0)
    {
        out << cont[0];
    }

    for (size_t i = 1; i < cont.size(); ++i)
    {
        out << ", " << cont[i];
    }
    out << "]";
    return out;
}

namespace Utility {

template <class T>
std::vector<T> construct_set_union(const std::vector<T>& vec1, const std::vector<T>& vec2)
{
    std::vector<T> answer = vec1;
    std::copy_if(vec2.begin(), vec2.end(), std::back_inserter(answer),
                [&vec1](T value)
                {
                    return std::find(vec1.begin(), vec1.end(), value) == vec1.end();
                }
    );
    return answer;
}

template <class T>
void set_union(std::vector<T>& vec1, const std::vector<T>& vec2)
{
    std::copy_if(vec2.begin(), vec2.end(), std::back_inserter(vec1),
                [&vec1](T value)
                {
                    return std::find(vec1.begin(), vec1.end(), value) == vec1.end();
                }
    );
}

template <class T>
void set_union(std::vector<T>& vec1, const T& value)
{
    if (std::find(vec1.begin(), vec1.end(), value) == vec1.end())
    {
        vec1.push_back(value);
    }
}

template <class T>
bool are_intersects(const std::vector<T>& vec1, const std::vector<T>& vec2)
{
    for (auto val1: vec1)
    {
        for (auto val2: vec2)
        {
            if (val1 == val2) return true;
        }
    }
    return false;
}

}

#endif // UTILITY_H
