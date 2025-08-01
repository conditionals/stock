# Makefile: builds our tooll

CXX = g++
CXXFLAGS = -std=c++14 -Wall -Werror -Iinclude
SRCS = src/main.cpp src/StockHashTable.cpp src/StockAnalysisCLI.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = stock_analysis

all: $(EXEC)
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@rm -f $(OBJS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(EXEC)

.PHONY: all clean
