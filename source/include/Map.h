//
//  Map.h
//  LexerUni
//
//  Created by Alnen on 08.10.15.
//  Copyright © 2015 Alnen. All rights reserved.
//

#ifndef Map_h
#define Map_h

#include <algorithm>
#include <boost/iterator/zip_iterator.hpp>
#include <iterator>
#include <vector>

template <class Key, class Value, class Compare = std::equal_to<Key>>
class Map
{
public:
    using iterator_pair  =  boost::tuple<
                                typename std::vector<Key>::iterator,
                                typename std::vector<Value>::iterator
                            >;
    using iterator       =  boost::zip_iterator<iterator_pair>;
    using const_iterator =  const iterator;
    using value_iterator =  typename std::vector<Value>::iterator;
    using key_iterator   =  typename std::vector<Key>::iterator;
    using value_type     =  std::pair<Key, Value>;

    Map() = default;
    Map(const Map&) = delete;
    Map& operator=(const Map&) = delete;
    Map(Map&& rhs): m_keys(std::move(rhs.m_keys)), m_values(std::move(rhs.m_values)), m_compare(std::move(rhs.m_compare)) {}
    Map& operator=(Map&& rhs)
    {
        m_keys = std::move(rhs.m_keys);
        m_values = std::move(rhs.m_values);
        m_compare = std::move(rhs.m_compare);
        return *this;
    }

    Value& operator[](Key&& key);
    Value& operator[](const Key&  key);

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    key_iterator key_begin();
    key_iterator key_end();

    value_iterator value_begin();
    value_iterator value_end();

    size_t size() const;
    void clear();
    
protected:
    std::vector<Key>   m_keys;
    std::vector<Value> m_values;
    Compare m_compare;
};

template <class Key, class Value, class Compare>
Value& Map<Key, Value, Compare>::operator[](Key&& key)
{
    auto insert = std::find_if(m_keys.begin(), m_keys.end(), [&key, this](auto& stored_key){ return m_compare(stored_key, key); });
    if (insert == m_keys.end())
    {
        //printf("new\n");
        m_keys.emplace_back(std::move(key));
        m_values.push_back(Value());
        return m_values.back();
    }
    else
    {
        //printf("old\n");
        return m_values[std::distance(m_keys.begin(), insert)];
    }
}

template <class Key, class Value, class Compare>
Value& Map<Key, Value, Compare>::operator[](const Key& key)
{
    auto insert = std::find_if(m_keys.begin(), m_keys.end(), [&key, this](auto& stored_key){ return m_compare(stored_key, key); });
    if (insert == m_keys.end())
    {
        m_keys.emplace_back(key);
        m_values.push_back(Value());
        return m_values.back();
        //throw std::exception();
    }
    else
    {
        //printf("old\n");
        return m_values[std::distance(m_keys.begin(), insert)];
    }
}

template <class Key, class Value, class Compare>
void Map<Key, Value, Compare>::clear()
{
    m_keys.clear();
    m_values.clear();
}

template <class Key, class Value, class Compare>
typename Map<Key, Value, Compare>::iterator Map<Key, Value, Compare>::begin()
{
    return boost::make_zip_iterator(
        boost::make_tuple(
            m_keys.begin(),
            m_values.begin()
        )
    );
}

template <class Key, class Value, class Compare>
typename Map<Key, Value, Compare>::iterator Map<Key, Value, Compare>::end()
{
    return boost::make_zip_iterator(
        boost::make_tuple(
            m_keys.end(),
            m_values.end()
        )
    );
}

template <class Key, class Value, class Compare>
const typename Map<Key, Value, Compare>::iterator Map<Key, Value, Compare>::begin() const
{
    return boost::make_zip_iterator(
        boost::make_tuple(
            m_keys.begin(),
            m_values.begin()
        )
    );
}

template <class Key, class Value, class Compare>
const typename Map<Key, Value, Compare>::iterator Map<Key, Value, Compare>::end() const
{
    return boost::make_zip_iterator(
        boost::make_tuple(
            m_keys.end(),
            m_values.end()
        )
    );
}

template <class Key, class Value, class Compare>
typename Map<Key, Value, Compare>::key_iterator Map<Key, Value, Compare>::key_begin()
{
    return m_keys.begin();
}

template <class Key, class Value, class Compare>
typename Map<Key, Value, Compare>::key_iterator Map<Key, Value, Compare>::key_end()
{
    return m_keys.end();
}

template <class Key, class Value, class Compare>
typename Map<Key, Value, Compare>::value_iterator Map<Key, Value, Compare>::value_begin()
{
    return m_values.begin();
}

template <class Key, class Value, class Compare>
typename Map<Key, Value, Compare>::value_iterator Map<Key, Value, Compare>::value_end()
{
    return m_values.end();
}

template <class Key, class Value, class Compare>
size_t Map<Key, Value, Compare>::size() const
{
    return m_keys.size();
}

#endif /* Map_h */
