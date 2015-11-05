#ifndef SET_H
#define SET_H

#include <vector>
#include <initializer_list>

namespace Parser
{

template<class T>
class Set
{
public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    Set() {};
    Set(std::initializer_list<T> init_list);
    Set(const Set<T>& other);

    int size() const;
    int elementNumber(const T& el) const;
    bool contain(const T& el) const;
    Set<T> union_set(const Set<T>& other) const;
    Set<T> intersection(const Set<T>& other) const; // need
    T& operator [](int index);
    const T& operator [](int index) const;
    Set<T> copy() const; // union

    void add(T el);
    bool remove(const T& el);// *
    bool insert(int index, T el); // *
    bool erase(int index); // *
    void clear(); // *
    void update(const Set<T>& other);

    iterator begin() { return m_set.begin(); }
    iterator end() { return m_set.end(); }
    const_iterator begin() const { return m_set.begin(); }
    const_iterator end() const { return m_set.end(); }

private:
    std::vector<T> m_set;
};

template<class T>
Set<T>::Set(std::initializer_list<T> init_list)
{
    for (T el : init_list) {
        add(el);
    }
}

template<class T>
Set<T>::Set(const Set<T>& other)
{
    for (int i = 0; i < other.size(); ++i) {
		m_set.push_back(other[i]);
	}
}

template<class T>
int Set<T>::size() const
{
	return m_set.size();
}

template<class T>
int Set<T>::elementNumber(const T& el) const
{
    for (int i = 0; i < m_set.size(); ++i) {
		if (m_set[i] == el) return i;
	}
	return -1;
}

template<class T>
bool Set<T>::contain(const T& el) const
{
    for (int i = 0; i < m_set.size(); ++i) {
		if (m_set[i] == el) return true;
	}
	return false;
}

template<class T>
Set<T> Set<T>::union_set(const Set<T>& other) const
{
	Set<T> result = copy();
    for (int i = 0; i < other.size(); ++i) {
		result.add(other[i]);
	}
	return result;
}

template<class T>
Set<T> Set<T>::intersection(const Set<T>& other) const
{
	Set<T> result;
    for (int i = 0; i < m_set.size(); ++i) {
        for (int j = 0; j < other.size(); ++j) {
			if (m_set[i] == other[j]) {
				result.add(other[j]);
				break;
			}
		}
	}
	return result;
}

template<class T>
T& Set<T>::operator [](int index)
{ 
	return m_set[index];
}

template<class T>
const T& Set<T>::operator [](int index) const
{
	return m_set[index];
}

template<class T>
Set<T> Set<T>::copy() const
{
	return Set<T>(*this);
}

template<class T>
void Set<T>::add(T el)
{
    for (int i = 0; i < m_set.size(); ++i) {
		if (m_set[i] == el) return;
	}
	m_set.push_back(el);
}

template<class T>
bool Set<T>::remove(const T& el)
{
    int i = 0;
	while (i < m_set.size()) {
		if (m_set[i] == el) {
			erase(i);
			return true;
		}
		++i;
	}
	return false;
}

template<class T>
bool Set<T>::insert(int index, T el)
{
	if ((index >= 0) && (index < m_set.size()) && (not_in(el))) {
		m_set.push_back(m_set[m_set.size() - 1]);
        for (int i = m_set.size() - 1; i > index; --i) {
			m_set[i] = m_set[i - 1];
		}
		return true;
	}
	return false;
}

template<class T>
bool Set<T>::erase(int index)
{
	if ((index >= 0) && (index < m_set.size())) {
        for (int i = index; i < m_set.size()-1; ++i) {
			m_set[i] = m_set[i + 1];
		}
		m_set.pop_back();
		return true;
	}
	return false;
}

template<class T>
void Set<T>::clear()
{
	m_set.clear();
}

template<class T>
void Set<T>::update(const Set<T>& other)
{
    for (int i = 0; i < other.size(); ++i) {
		add(other[i]);
	}
}

} // namespace Grammar

#endif // SET_H
