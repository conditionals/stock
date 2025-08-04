#include "../include/StockAnalysisCLI.h"
#include "../include/HeapSort.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <unordered_set>
#include <algorithm>

// will also need to add trees, perf profiling, etc.
StockAnalysisCLI::StockAnalysisCLI() : hash_lookup(1000), rng(std::random_device{}()) {}

void StockAnalysisCLI::display_menu() {
    std::cout << "temp menu. replace\n\n1. load_stock_data\n2.analyze pf_prof\n7. quit\netc.\n\n";
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
    std::vector<std::string> symbols = generate_tickers(200);
    // possible +/- 10% + noise
    std::uniform_real_distribution<> pcnt_change_dist(-0.1,0.1);
    std::uniform_real_distribution<> noise_dist(-0.03,0.03);
    std::uniform_int_distribution<> volume_dist(10000, 1'000'000);



    for(const auto& symbol : symbols) {
	StockInfo info;
	info.symbol = symbol;
	info.company_name = "Company " + info.symbol;
	
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
	    info.total_return += day.daily_return;
	    info.daily_data.push_back(day);
	    prev_close = day.close;

	    date.tm_mday += 1;
	    std::mktime(&date);

	    i++;
	}

	info.trading_days = static_cast<int>(info.daily_data.size());
	info.avg_annual_return = (info.total_return / info.trading_days) * 252; // no weekends
	
	// calc volatilitiy based upon standard dev of daily return
	double mnDailyRet = info.avg_annual_return;
	double ssd = 0.0;
	for(const auto& day : info.daily_data) {
	    double diff = day.daily_return - mnDailyRet;
	    ssd += diff * diff;
	}

	info.volatility = std::sqrt(ssd / info.trading_days);
	hash_lookup.insert(info.symbol, &info);

    }
}

void StockAnalysisCLI::run() {
    int choice;
    while(true) {
	display_menu();
	std::cin >> choice;
	std::cin.ignore();

	switch(choice){
	    case 7:
	    	break;
		// case 6:
		// 	heap_sort_by_avg_return();  //calls the heap sort in ascending order
		return;
	    default:
		std::cout << "Invalid chioce, please try again\n";
	}
    }
}
