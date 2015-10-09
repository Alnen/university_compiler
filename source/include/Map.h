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

template <class Key, class Value>
class Map
{
public:
    using iterator_pair  = std::pair<typename std::vector<Key>::iterator, typename std::vector<Value>::iterator>;
    using iterator       = boost::zip_iterator<iterator_pair>;
    using value_iterator = typename std::vector<Value>::iterator;
    using key_iterator   = typename std::vector<Key>::iterator;
    using value_type     = std::pair<Key, Value>;
    
    Value& operator[](Key key)
    {
        auto insert = std::find(m_keys.begin(), m_keys.end(), key);
        if (insert == m_keys.end())
        {
            m_keys.push_back(key);
            m_values.push_back(Value());
            return m_values.back();
        }
        else
        {
            return m_values[std::distance(m_keys.begin(), insert)];
        }
    }
    
    iterator begin() const
    {
        return boost::make_zip_iterator(
            std::make_pair(
                m_keys.begin(),
                m_values.begin()
            )
        );
    }
    
    iterator end() const
    {
        return boost::make_zip_iterator(
            std::make_pair(
                m_keys.end(),
                m_values.end()
            )
        );
    }



    
    size_t size() const
    {
        return m_keys.size();
    }
    
protected:
    std::vector<Key>   m_keys;
    std::vector<Value> m_values;
};

#endif /* Map_h */
