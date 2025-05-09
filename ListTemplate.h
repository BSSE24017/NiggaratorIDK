#pragma once
#include <vector>
#include <iostream>
using namespace std;

template<typename T>
class ListTemplate {
    vector<T> items;
public:
    void add(const T& item) { items.push_back(item); }
    T& operator[](size_t a) { return items.at(a); }
    size_t size() const { return items.size(); }
    typename vector<T>::const_iterator begin() const { return items.begin(); }
    typename vector<T>::const_iterator end() const { return items.end(); }
    void clear() { items.clear(); }
    
    void removeAt(size_t a) {
        if (a < items.size()) {
            items.erase(items.begin() + a);
        }
    }
    
    const T& operator[](size_t a) const {
        return items.at(a);
    }
    
}; 

