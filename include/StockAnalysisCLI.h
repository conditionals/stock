#pragma once
#include "StockData.h"
#include "StockHashTable.h"
#include "RedBlackTree.h"
#include <random>


class StockAnalysisCLI {
    private:
	std::vector<StockInfo*> stock_database;
	StockHashTable hash_lookup;
	RedBlackTree<StockInfo*> *rb_tree;

	void display_menu();
	void create_sample_data();

	std::vector<StockInfo*> heap_sort_by_avg_return() const;

	void analyze_performace();
	void search_stock();
	void display_performance();

    public:
	StockAnalysisCLI();
	~StockAnalysisCLI();

	void run();

	std::mt19937 rng;
	std::vector<std::string> generate_tickers(int count);
};
