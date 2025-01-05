#pragma once

#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>


template<typename K, typename V>
using Pair = std::pair<K, V>;

template<typename T>
using List = std::vector<T>;

template<typename Key, typename Value, typename Compare = std::less<Key>>
using Map = std::map<Key, Value, Compare>;

template<typename Key, typename Value, typename Hasher = std::hash<Key>>
using Dictionary = std::unordered_map<Key, Value, Hasher>;

template<typename T>
using Set = std::unordered_set<T>;
