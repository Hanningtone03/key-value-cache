![CI](https://github.com/Hanningtone03/key-value-cache/actions/workflows/ci.yml/badge.svg)

# Key-Value Cache

An LRU cache built from scratch in C++; O(1) get and put using a hash map paired with a doubly linked list, with TTL support and a benchmark suite.

## How it works

A hash map gives O(1) lookup from key to node, and a doubly linked list tracks recency order; ![CI](https://github.com/Hanningtone03/key-value-cache/actions/workflows/ci.yml/badge.svg)

# Key-Value Cache

An LRU cache built from scratch in C++ — O(1) get and put using a hash map paired with a doubly linked list, with TTL support and a benchmark suite.

## How it works

A hash map gives O(1) lookup from key to node, and a doubly linked list tracks recency order — the front is most recently used, the back is least recently used. Every get moves its node to the front; every put either updates an existing node or inserts a new one, evicting the tail if the cache is full. TTL entries carry an expiry timestamp checked lazily on read.

## Project structure

```
include/
└── lru_cache.hpp
src/
└── main.cpp
test/
└── test_cache.cpp
benchmark/
└── bench.cpp
```

## Building and running

```bash
g++ -std=c++17 -O2 -o cache_demo src/main.cpp
./cache_demo

g++ -std=c++17 -O2 -o cache_test test/test_cache.cpp
./cache_test

g++ -std=c++17 -O2 -o cache_bench benchmark/bench.cpp
./cache_bench
```

## Test results

22/22 tests passing, covering eviction order, recency tracking, TTL expiry, capacity edge cases, and stats tracking.

## Benchmark results

```
PUT: ~7.3M ops/sec
GET: ~18.6M ops/sec
Mixed workload: ~9.0M ops/sec
```

## Tech

- C++17
- Template-based, works with any key/value types
- `std::unordered_map` + manually implemented doubly linked list
- No external dependenciesthe front is most recently used, the back is least recently used. Every get moves its node to the front; every put either updates an existing node or inserts a new one, evicting the tail if the cache is full. TTL entries carry an expiry timestamp checked lazily on read.

## Project structure

```
include/
└── lru_cache.hpp
src/
└── main.cpp
test/
└── test_cache.cpp
benchmark/
└── bench.cpp
```

## Building and running

```bash
g++ -std=c++17 -O2 -o cache_demo src/main.cpp
./cache_demo

g++ -std=c++17 -O2 -o cache_test test/test_cache.cpp
./cache_test

g++ -std=c++17 -O2 -o cache_bench benchmark/bench.cpp
./cache_bench
```

## Test results

22/22 tests passing, covering eviction order, recency tracking, TTL expiry, capacity edge cases, and stats tracking.

## Benchmark results

```
PUT: ~7.3M ops/sec
GET: ~18.6M ops/sec
Mixed workload: ~9.0M ops/sec
```

## Tech

- C++17
- Template-based, works with any key/value types
- `std::unordered_map` + manually implemented doubly linked list
- No external dependencies
