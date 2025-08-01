#include "../include/StockHashTable.h"
#include <iomanip>
#include <iostream>

StockHashTable::StockHashTable(size_t init_size): table_size(init_size), num_elements(0) {
    table.resize(table_size);
}

// https://www.geeksforgeeks.org/dsa/string-hashing-using-polynomial-rolling-hash-function/#
// https://stackoverflow.com/a/299748
size_t StockHashTable::hash_function(const std::string& symbol) {
    size_t hash = 0;
    for(char c : symbol) {
	hash = hash * 31 + c;
    }

    return hash % table_size;
}

void StockHashTable::resize_table() {
    std::vector<HashNode> old_table = table;
    table_size *= 2;

    table.clear();
    table.resize(table_size);

    num_elements = 0;

    for(const auto& node : old_table) {
	if(!node.is_deleted && node.is_occupied) {
	    insert(node.symbol, node.stock_data);
	}
    }
}



// https://www.geeksforgeeks.org/dsa/implementing-hash-table-open-addressing-linear-probing-cpp/
size_t StockHashTable::find_slot(const std::string& symbol) {
    size_t idx = hash_function(symbol);
    size_t original_idx = idx;

    while(table[idx].is_occupied) {
	if(!table[idx].is_deleted && table[idx].symbol == symbol) {
	    return idx;
	}
	idx += 1;
	idx %= table_size;

	if(idx == original_idx) {
	    break;
	}
    }

    return idx;
}





bool StockHashTable::insert(const std::string& symbol, StockInfo* stock) {
    if(get_load_factor() > LOAD_FACTOR) {
	resize_table();
    } 

    size_t idx = find_slot(symbol);

    if(!table[idx].is_occupied || table[idx].is_deleted) {
	table[idx].symbol = symbol;
	table[idx].stock_data = stock;
	table[idx].is_deleted = false;
	table[idx].is_occupied = true;
	num_elements++;
	return true;
    }

    return false;
}


StockInfo* StockHashTable::search(const std::string& symbol) {
    size_t idx = find_slot(symbol);

    if(table[idx].is_occupied && !table[idx].is_deleted && table[idx].symbol == symbol) {
	return table[idx].stock_data;
    }

    return nullptr;
}

double StockHashTable::get_load_factor() const {
    return static_cast<double>(num_elements) / table_size;
}

void StockHashTable::printStats() {
    std::cout << "Hash Table Stats:" << std::endl;
    std::cout << "Size: " << table_size << std::endl;
    std::cout << "Elements: " << num_elements << std::endl;
    std::cout << "Load Factor: " << std::fixed << std::setprecision(2) << get_load_factor() << std::endl;
}








