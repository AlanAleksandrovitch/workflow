#include "HashTable.h"
#include <cstdint>

HashTable::HashTable(size_t size) noexcept
	: _capacity(static_cast<int32_t>(size)), _filled(0), table(size) {}

HashTable::~HashTable() {}

size_t HashTable::hash_function(const KeyType &key) const{
	std::size_t hash = 0;
	for (char c : key)
		hash = hash * 31 + static_cast<unsigned char>(c);
	return hash % _capacity;
}

void HashTable::insert(const KeyType &key, const ValueType &value){
	size_t idx = hash_function(key);
	for (auto &elem : table[idx]){
		if (elem.first == key){
			elem.second = value;
			return;
		}
	}
	if (table[idx].empty()){
		++_filled;
	}
	table[idx].push_back({key, value});
	if (getLoadFactor() > 0.75){
		int32_t oldCapacity = _capacity;
		_capacity *= 2;
		std::vector<std::list<std::pair<KeyType, ValueType>>> oldTable = table;
		table.clear();
		table.resize(_capacity);
		_filled = 0;
		for (const auto &bucket : oldTable){
			for (const auto &elem : bucket){
				size_t newIdx = hash_function(elem.first);
				if (table[newIdx].empty()){
					++_filled;
				}
				table[newIdx].push_back(elem);
			}
		}
	}
}

bool HashTable::find(const KeyType &key, ValueType &value) const{
	size_t idx = hash_function(key);
	for (const auto &elem : table[idx]){
		if (elem.first == key){
			value = elem.second;
			return true;
		}
	}
	return false;
}

void HashTable::remove(const KeyType &key){
	size_t idx = hash_function(key);
	for (auto it = table[idx].begin(); it != table[idx].end(); ++it){
		if (it->first == key){
			table[idx].erase(it);
			if (table[idx].empty()){
				--_filled;
			}
			return;
		}
	}
}

ValueType& HashTable::operator[](const KeyType &key){
	size_t idx = hash_function(key);
	for (auto &elem : table[idx]){
		if (elem.first == key){
			return elem.second;
		}
	}
	if (table[idx].empty()){
		++_filled;
	}
	table[idx].push_back({key, ValueType{}});
	return table[idx].back().second;
}

double HashTable::getLoadFactor(){
    return static_cast<double>(_filled)/_capacity;
}
