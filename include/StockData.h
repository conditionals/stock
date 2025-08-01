#pragma once

#include <string>
#include <vector>

struct DailyStockData {
    std::string date;
    double high, low, open, close;
    long volume;
    double daily_return;

    DailyStockData() : high(0), low(0), open(0), close(0), volume(0), daily_return(0) {}
};

struct StockInfo {
    std::string symbol;
    std::string company_name;
    std::vector<DailyStockData> daily_data;
    double avg_annual_return;
    double volatility;
    double total_return;
    int trading_days;

    StockInfo() : avg_annual_return(0), volatility(0), total_return(0), trading_days(0) {}
    
    bool operator<(const StockInfo& other) {
	return avg_annual_return < other.avg_annual_return;
    }

    bool operator>(const StockInfo& other) {
	return avg_annual_return > other.avg_annual_return;
    }
};
