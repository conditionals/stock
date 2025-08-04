#include "../include/StockAnalysisCLI.h"
#include "../include/HeapSort.h" // this file is needed. do not remove
#include "../include/QuickSort.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <unordered_set>
#include <algorithm>

// will also need to add trees, perf profiling, etc.
StockAnalysisCLI::StockAnalysisCLI() : hash_lookup(1000), rng(std::random_device{}()) {
    rb_tree = new RedBlackTree<StockInfo*>();
}
StockAnalysisCLI::~StockAnalysisCLI() {
    for(auto* stock : stock_database) {
	delete stock;
    }
    rb_tree->clear();
    delete rb_tree;
}

void StockAnalysisCLI::display_menu() {
    std::cout << "Project 3: Stock Analysis\n";
    std::cout << "1. Generate Stock Data\n";
    std::cout << "2. Compare Algorithms (Heap Sort vs Quicksort)\n";
    std::cout << "3. Search Stock by Ticker\n";
    std::cout << "4. Display Best/Worst Performers\n";
    std::cout << "5. Search Tickers by Prefix\n";
    std::cout << "6. Display Hash Table Stats\n";
    std::cout << "7. Quit\n\n";

    std::cout << "Choice: ";
    
}

std::vector<StockInfo*> StockAnalysisCLI::heap_sort_by_avg_return() const {
    std::vector<StockInfo*> arr = stock_database;
    heapSort(arr);
    return arr;
}

// use unordered set to ensure we dont generate same ticker more than once
std::vector<std::string> StockAnalysisCLI::generate_tickers(int count) {
    std::unordered_set<std::string> unique;
    std::vector<std::string> tickers;
    std::uniform_int_distribution<> dist('A', 'Z');

    while(static_cast<int>(tickers.size()) < count) {
	std::string ticker;
	for(int i = 0; i < 4; i++) {
	    ticker += static_cast<char>(dist(rng));
	}


	// not already in the set
	if(unique.insert(ticker).second) {
	    tickers.push_back(ticker);
	}
    }

    return tickers;
}

std::string format_date(std::tm date) {
    std::ostringstream oss;
    oss << std::put_time(&date, "%Y-%m-%d");
    return oss.str();
}

void StockAnalysisCLI::create_sample_data() {
    std::vector<std::string> symbols = generate_tickers(1000);
    // possible +/- 10% + noise
    std::uniform_real_distribution<> pcnt_change_dist(-0.1,0.1);
    std::uniform_real_distribution<> noise_dist(-0.03,0.03);
    std::uniform_int_distribution<> volume_dist(10000, 1'000'000);



    for(const auto& symbol : symbols) {
	StockInfo* info = new StockInfo();
	info->symbol = symbol;
	info->company_name = "Company " + info->symbol;
	
	double prev_close = 100;

        std::tm date = {};
	date.tm_year = 25; // 2025-2000
	date.tm_mon = 0;
	date.tm_mday = 1;
	std::mktime(&date);

	for(int i = 0; i < 1500; i++) {
	    // skip weekends, no real reason to but makes it look more realistic lol
	    if(date.tm_mday == 0 || date.tm_mday == 6) {
		date.tm_mday += 1;
		std::mktime(&date);
		continue;
	    }

	    DailyStockData day;
	    day.date = format_date(date);

	    double pct_change = pcnt_change_dist(rng);
	    day.open = prev_close;
	    day.close = prev_close * (1.0 + pct_change);
	    day.high = std::max(day.open, day.close) * (1.0 + noise_dist(rng));
	    day.low = std::min(day.open, day.close) * (1.0 - noise_dist(rng));
	    day.volume = volume_dist(rng);

	    day.daily_return = (day.close - day.open) / day.open;
	    info->total_return += day.daily_return;
	    info->daily_data.push_back(day);
	    prev_close = day.close;

	    date.tm_mday += 1;
	    std::mktime(&date);

	    i++;
	}

	info->trading_days = static_cast<int>(info->daily_data.size());
	info->avg_annual_return = (info->total_return / info->trading_days) * 252; // no weekends
	
	// calc volatilitiy based upon standard dev of daily return
	double mnDailyRet = info->avg_annual_return;
	double ssd = 0.0;
	for(const auto& day : info->daily_data) {
	    double diff = day.daily_return - mnDailyRet;
	    ssd += diff * diff;
	}

	info->volatility = std::sqrt(ssd / info->trading_days);
	hash_lookup.insert(info->symbol, info);
	rb_tree->insert(info);
	stock_database.push_back(info);

    }
}

// ANSI codes
const std::string RESET = "\033[0m";
const std::string REDCOLOR = "\033[31m";

void StockAnalysisCLI::run() {
    int choice;
    std::string ticker;
    while(true) {
	display_menu();
	std::cin >> choice;
	std::cin.ignore();
	std::cout << "\n";

	switch(choice){
	    // generate stock data
	    case 1:
		create_sample_data();
		break;
	    // compare algorithms (delta t)
	    case 2: {
		if(stock_database.empty()) {
		    std::cout << REDCOLOR << "Error. You must generate data before you continue.\n" << RESET;
		    break;
		}
	    
		auto start = std::chrono::high_resolution_clock::now();
		std::vector<StockInfo*> hs = heap_sort_by_avg_return();
		auto end = std::chrono::high_resolution_clock::now();

		auto heap_time = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();

		std::vector<StockInfo*> quickData = stock_database;

		start = std::chrono::high_resolution_clock::now();
		quickSort(quickData);
		end = std::chrono::high_resolution_clock::now();


		auto quick_time = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
		

		StockInfo* top = quickData.back();
		std::cout << "Top Performer: " << top->symbol << std::endl;
		std::cout << "Avg. Annual Return: " << top->avg_annual_return << std::endl;
		std::cout << "Volatility: " << top->volatility << "\n" << std::endl;

		// lowest performer, easier to paste code and just reassign top tho 
		top = quickData.front();
		std::cout << "Bottom Performer: " << top->symbol << std::endl;
		std::cout << "Avg. Annual Return: " << top->avg_annual_return << std::endl;
		std::cout << "Volatility: " << top->volatility << "\n" << std::endl;

		std::cout << "Heap Sort time: " << heap_time << std::endl;
		std::cout << "QuickSort time: " << quick_time << std::endl;
		std::cout << std::endl;

		break;
	    }
	    // search stock ticker
	    // TODO: fix input loop
	    case 3: {
		std::cout << "Ticker Symbol (e.g. AAPL): ";
		std::cin >> ticker;
		std::cin.ignore();

		StockInfo* si = hash_lookup.search(ticker);
		if(si == nullptr) {
		    std::cout << REDCOLOR << 
			"No Stock found for ticker: " << ticker << RESET << std::endl;
		    break;
		}

		std::cout << si->company_name << std::endl;
		std::cout << "Avg. Annual Return: " << si->avg_annual_return << std::endl;
		std::cout << "Total Return: " << si->total_return << std::endl;
		std::cout << "Volatility: " << si->volatility << std::endl;

		break;
	    }
	    // display best/worst performers
	    case 4: {
		std::vector<StockInfo*> hs = heap_sort_by_avg_return();
		if(hs.empty()) {
		    std::cout << REDCOLOR << "No stock data. Please generate some first.\n" << RESET << std::endl;
		}

		StockInfo* best = hs.back();
		StockInfo* worst = hs.front();

		std::cout << "Top Performer: " << best->symbol << std::endl;
		std::cout << "Avg. Annual Return: " << best->avg_annual_return << std::endl;
		std::cout << "Volatility: " << best->volatility << "\n" << std::endl;

		std::cout << "Bottom Performer: " << worst->symbol << std::endl;
		std::cout << "Avg. Annual Return: " << worst->avg_annual_return << std::endl;
		std::cout << "Volatility: " << worst->volatility << "\n" << std::endl;

		
		break;
	    }
	    // search by prefix with RB tree
	    case 5: {
		if(rb_tree == nullptr) {
		    std::cout << "No data. Must generate some first.\n";
		    break;
		}

		std::string prefix;
		std::cout << "Enter ticker prefix (1-4 chars): ";
		std::cin >> prefix;

		std::cout << "\nFirst 10 tickers starting with: " << "\"" << prefix << "\"" <<
		    std::endl;

		rb_tree->printPrefixMatches(prefix);

		std::cout << std::endl;


		break;
	    }


	    // print hash table stats
	    case 6:
		hash_lookup.printStats();
		break;

	    case 7:
		return;
	    default:
		std::cout << REDCOLOR << "Invalid selection, please try again.\n" << RESET << std::endl;
	}
    }
}
