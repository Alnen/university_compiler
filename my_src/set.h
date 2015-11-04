#pragma once
#include <vector>
#include <initializer_list>

namespace Grammar
{

template<class T>
class Set
{
public:
	Set() {};
	Set(std::initializer_list<T> init_list);
	Set(const Set<T>& other);

	size_t size() const;
	int elementNumber(const T& el) const;
	bool contain(const T& el) const;
	bool isdisjoint(const Set<T>& other) const;
	Set<T> union_set(const Set<T>& other) const;
	Set<T> intersection(const Set<T>& other) const;
	T& operator [](size_t index);
	const T& operator [](size_t index) const;
	Set<T> copy() const;

	void add(T el);
	bool remove(const T& el);
	bool insert(size_t index, T el);
	bool erase(size_t index);
	void clear();
	void update(const Set<T>& other);
	void intersection_update(const Set<T>& other);

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
	for (size_t i = 0; i < other.size(); ++i) {
		m_set.push_back(other[i]);
	}
}

template<class T>
size_t Set<T>::size() const
{
	return m_set.size();
}

template<class T>
int Set<T>::elementNumber(const T& el) const
{
	for (size_t i = 0; i < m_set.size(); ++i) {
		if (m_set[i] == el) return i;
	}
	return -1;
}

template<class T>
bool Set<T>::contain(const T& el) const
{
	for (size_t i = 0; i < m_set.size(); ++i) {
		if (m_set[i] == el) return true;
	}
	return false;
}

template<class T>
bool Set<T>::isdisjoint(const Set<T>& other) const
{
	for (size_t i = 0; i < m_set.size(); ++i) {
		if (m_set[i] == el) return false;
	}
	return true;
}

template<class T>
Set<T> Set<T>::union_set(const Set<T>& other) const
{
	Set<T> result = copy();
	for (size_t i = 0; i < other.size(); ++i) {
		result.add(other[i]);
	}
	return result;
}

template<class T>
Set<T> Set<T>::intersection(const Set<T>& other) const
{
	Set<T> result;
	for (size_t i = 0; i < m_set.size(); ++i) {
		for (size_t j = 0; j < other.size(); ++j) {
			if (m_set[i] == other[j]) {
				result.add(other[j]);
				break;
			}
		}
	}
	return result;
}

template<class T>
T& Set<T>::operator [](size_t index)
{ 
	return m_set[index];
}

template<class T>
const T& Set<T>::operator [](size_t index) const
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
	for (size_t i = 0; i < m_set.size(); ++i) {
		if (m_set[i] == el) return;
	}
	m_set.push_back(el);
}

template<class T>
bool Set<T>::remove(const T& el)
{
	size_t i = 0;
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
bool Set<T>::insert(size_t index, T el)
{
	if ((index >= 0) && (index < m_set.size()) && (not_in(el))) {
		m_set.push_back(m_set[m_set.size() - 1]);
		for (size_t i = m_set.size() - 1; i > index; --i) {
			m_set[i] = m_set[i - 1];
		}
		return true;
	}
	return false;
}

template<class T>
bool Set<T>::erase(size_t index)
{
	if ((index >= 0) && (index < m_set.size())) {
		for (size_t i = index; i < m_set.size()-1; ++i) {
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
	for (size_t i = 0; i < other.size(); ++i) {
		add(other[i]);
	}
}

template<class T>
void Set<T>::intersection_update(const Set<T>& other)
{
	Set<T> temp;
	for (size_t i = 0; i < m_set.size(); ++i) {
		for (size_t j = 0; j < other.size(); ++j) {
			if (m_set[i] == other[j]) {
				result.add(other[j]);
				break;
			}
		}
	}
	m_set.clear();
	for (size_t i = 0; i < temp; ++i) {
		m_set.push_back(temp[i]);
	}
}

} // namespace Grammar