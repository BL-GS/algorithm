/*
 * @author: BL-GS 
 * @date:   2023/5/21
 */

#include <cstddef>
#include <vector>
#include <array>

#include <benchmark/benchmark.h>

#include <structure/array/small_vector.h>


template<class Container>
void construct(benchmark::State &state) {
	for (auto _: state) {
		for (size_t i = state.range(0); i != 0; --i) {
			Container vector;
			benchmark::DoNotOptimize(vector);
		}
	}
}

template<class Container>
void push_back(benchmark::State &state) {
	for (auto _: state) {
		Container vector;
		for (size_t i = state.range(0); i != 0; --i) {
			vector.push_back(i);
			benchmark::DoNotOptimize(vector);
		}
	}
}

BENCHMARK_TEMPLATE(construct, algorithm::structure::SmallVector<size_t, 100, algorithm::allocator::BaseAllocator>)
        ->RangeMultiplier(10)->Range(10, 10000)
        ->Iterations(10);
BENCHMARK_TEMPLATE(construct, std::vector<size_t>)
        ->RangeMultiplier(10)->Range(10, 10000)
        ->Iterations(10);
BENCHMARK_TEMPLATE(construct, std::array<size_t, 10000>)
        ->RangeMultiplier(10)->Range(10, 10000)
        ->Iterations(10);

BENCHMARK_TEMPLATE(push_back, algorithm::structure::SmallVector<size_t, 100, algorithm::allocator::BaseAllocator>)
        ->RangeMultiplier(10)->Range(10, 1000)
        ->Iterations(10);
BENCHMARK_TEMPLATE(push_back, std::vector<size_t>)
        ->RangeMultiplier(10)->Range(10, 1000)
        ->Iterations(10);

BENCHMARK_MAIN();