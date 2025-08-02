#pragma once

#include "StockData.h"

// https://www.geeksforgeeks.org/dsa/load-factor-and-rehashing/
//
// fast lookup of stock by symbol
class StockHashTable {
    private:
	struct HashNode {
	    std::string symbol;
	    StockInfo* stock_data;
	    bool is_occupied;
	    bool is_deleted;
	    
	    HashNode() : stock_data(nullptr), is_occupied(false), is_deleted(false) {}
	};

	std::vector<HashNode> table;
	size_t table_size;
	size_t num_elements;
	
	static constexpr double LOAD_FACTOR = 0.75;

	size_t hash_function(const std::string& symbol);
	void resize_table();
	size_t find_slot(const std::string& symbol);
    public:
	StockHashTable(size_t init_size = 1000);
	bool insert(const std::string& symbol, StockInfo* stock);
	StockInfo* search(const std::string& symbol);
	double get_load_factor() const;
	void printStats();

	std::vector<StockInfo *> get_all() const;
};
