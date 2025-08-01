#pragma once
#include "StockData.h"
#include "StockHashTable.h"
#include <random>


class StockAnalysisCLI {
    private:
	std::vector<StockInfo> stock_database;
	StockHashTable hash_lookup;

	void display_menu();
	void create_sample_data();
	void analyze_performace();
	void search_stock();
	void display_performance();

    public:
	StockAnalysisCLI();
	void run();

	std::mt19937 rng;
	std::vector<std::string> generate_tickers(int count);
};
