#pragma once
#include <vector>
#include <iostream>

template<typename T>
class ListTemplate {
    std::vector<T> items;
public:
    void add(const T& item) { items.push_back(item); }
    T& operator[](size_t idx) { return items.at(idx); }
    size_t size() const { return items.size(); }
    typename std::vector<T>::iterator begin() { return items.begin(); }
    typename std::vector<T>::iterator end() { return items.end(); }
    void clear() { items.clear(); }
}; 