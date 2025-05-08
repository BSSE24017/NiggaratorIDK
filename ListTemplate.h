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
    typename std::vector<T>::const_iterator begin() const { return items.begin(); }
    typename std::vector<T>::const_iterator end() const { return items.end(); }
    void clear() { items.clear(); }
    void removeAt(size_t idx) {
        if (idx < items.size()) {
            items.erase(items.begin() + idx);
        }
    }
    
    const T& operator[](size_t idx) const {
        return items.at(idx);
    }
    
}; 

