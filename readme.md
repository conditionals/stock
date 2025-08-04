# stock

## high-performance stock analytics engine for analyzing and searching large-scale financial datasets

features include:

* red-black tree for quick prefix-based ticker lookup 
* hash table for constant-time access to individual stocks
* heap sort and quicksort for benchmarking top/bottom performers
* a realistic simulation of thousands of daily market datapoints
* built-in profiling to compare algorithm speeds
* memory-safe design with no leaks, verified by valgrind

running the program:

1. `make`
2. `./stock_analysis`
