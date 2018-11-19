#include <benchmark/benchmark.h>
#include <kangaru/kangaru.hpp>
#include <chrono>

template<std::size_t nth, std::size_t size>
struct Service1 {
	std::array<char, size> data;
};

template<std::size_t nth, std::size_t size>
struct Definition1 : kgr::single_service<Service1<nth, size>> {};

template<std::size_t size, std::size_t... S>
static void service_bench(kgr::detail::seq<S...>, benchmark::State& state) {
	using unpack = int[];
	for (auto _ : state) {
		kgr::container container;
		(void) unpack{(
			container.service<Definition1<S, size>>()
		, 0)...};
	}
}

template<std::size_t amount, std::size_t size>
static void service_bench(benchmark::State& state) {
	service_bench<size>(typename kgr::detail::seq_gen<amount>::type{}, state);
}

BENCHMARK_TEMPLATE(service_bench, 8, 1);
BENCHMARK_TEMPLATE(service_bench, 16, 1);
BENCHMARK_TEMPLATE(service_bench, 32, 1);
BENCHMARK_TEMPLATE(service_bench, 64, 1);
BENCHMARK_TEMPLATE(service_bench, 128, 1);

BENCHMARK_TEMPLATE(service_bench, 8, 8);
BENCHMARK_TEMPLATE(service_bench, 16, 8);
BENCHMARK_TEMPLATE(service_bench, 32, 8);
BENCHMARK_TEMPLATE(service_bench, 64, 8);
BENCHMARK_TEMPLATE(service_bench, 128, 8);

BENCHMARK_TEMPLATE(service_bench, 8, 16);
BENCHMARK_TEMPLATE(service_bench, 16, 16);
BENCHMARK_TEMPLATE(service_bench, 32, 16);
BENCHMARK_TEMPLATE(service_bench, 64, 16);
BENCHMARK_TEMPLATE(service_bench, 128, 16);

template<std::size_t size, std::size_t... S>
static void emplace_bench(kgr::detail::seq<S...>, benchmark::State& state) {
	using unpack = int[];
	for (auto _ : state) {
		kgr::container container;
		(void) unpack{(
			container.emplace<Definition1<S, size>>()
		, 0)...};
	}
}

template<std::size_t amount, std::size_t size>
static void emplace_bench(benchmark::State& state) {
	emplace_bench<size>(typename kgr::detail::seq_gen<amount>::type{}, state);
}

BENCHMARK_TEMPLATE(emplace_bench, 8, 1);
BENCHMARK_TEMPLATE(emplace_bench, 16, 1);
BENCHMARK_TEMPLATE(emplace_bench, 32, 1);
BENCHMARK_TEMPLATE(emplace_bench, 64, 1);
BENCHMARK_TEMPLATE(emplace_bench, 128, 1);

BENCHMARK_TEMPLATE(emplace_bench, 8, 8);
BENCHMARK_TEMPLATE(emplace_bench, 16, 8);
BENCHMARK_TEMPLATE(emplace_bench, 32, 8);
BENCHMARK_TEMPLATE(emplace_bench, 64, 8);
BENCHMARK_TEMPLATE(emplace_bench, 128, 8);

BENCHMARK_TEMPLATE(emplace_bench, 8, 16);
BENCHMARK_TEMPLATE(emplace_bench, 16, 16);
BENCHMARK_TEMPLATE(emplace_bench, 32, 16);
BENCHMARK_TEMPLATE(emplace_bench, 64, 16);
BENCHMARK_TEMPLATE(emplace_bench, 128, 16);

template<std::size_t size, std::size_t... S>
static void service_half_insert_bench(kgr::detail::seq<S...>, benchmark::State& state) {
	using unpack = int[];
	kgr::container container;
	
	(void) unpack{(
		S % 2 ? void(container.service<Definition1<S, size>>()) : void()
	, 0)...};
	
	for (auto _ : state) {
		// Simulate some useful workload with a sleep
		kgr::container fork = container.fork();
		
		auto start = std::chrono::high_resolution_clock::now();
		(void) unpack{(
			container.service<Definition1<S, size>>()
		, 0)...};
		auto end = std::chrono::high_resolution_clock::now();
		
		auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
		state.SetIterationTime(elapsed_seconds.count());
	}
}

template<std::size_t amount, std::size_t size>
static void service_half_insert_bench(benchmark::State& state) {
	service_half_insert_bench<size>(typename kgr::detail::seq_gen<amount>::type{}, state);
}

BENCHMARK_TEMPLATE(service_half_insert_bench, 8, 1);
BENCHMARK_TEMPLATE(service_half_insert_bench, 16, 1);
BENCHMARK_TEMPLATE(service_half_insert_bench, 32, 1);
BENCHMARK_TEMPLATE(service_half_insert_bench, 64, 1);
BENCHMARK_TEMPLATE(service_half_insert_bench, 128, 1);

BENCHMARK_TEMPLATE(service_half_insert_bench, 8, 8);
BENCHMARK_TEMPLATE(service_half_insert_bench, 16, 8);
BENCHMARK_TEMPLATE(service_half_insert_bench, 32, 8);
BENCHMARK_TEMPLATE(service_half_insert_bench, 64, 8);
BENCHMARK_TEMPLATE(service_half_insert_bench, 128, 8);

BENCHMARK_TEMPLATE(service_half_insert_bench, 8, 16);
BENCHMARK_TEMPLATE(service_half_insert_bench, 16, 16);
BENCHMARK_TEMPLATE(service_half_insert_bench, 32, 16);
BENCHMARK_TEMPLATE(service_half_insert_bench, 64, 16);
BENCHMARK_TEMPLATE(service_half_insert_bench, 128, 16);

constexpr auto distribute(std::size_t nth, std::size_t iteration_count, std::size_t total_size) -> std::size_t {
	return iteration_count == total_size ? nth : ((nth) * (total_size / iteration_count) + (total_size / (iteration_count * 2)));
}

template<std::size_t size, std::size_t... S, std::size_t... S2>
static void service_inserted_bench(kgr::detail::seq<S...>, kgr::detail::seq<S2...>, benchmark::State& state) {
	using unpack = int[];
	kgr::container container;
	
	(void) unpack{(
		container.service<Definition1<S, size>>()
	, 0)...};
	
	for (auto _ : state) {
		(void) unpack{(
			container.service<Definition1<distribute(S2, sizeof...(S2), sizeof...(S)), size>>()
		, 0)...};
	}
}

template<std::size_t amount, std::size_t size, std::size_t nb = amount>
static void service_inserted_bench(benchmark::State& state) {
	service_inserted_bench<size>(typename kgr::detail::seq_gen<amount>::type{}, typename kgr::detail::seq_gen<nb>::type{}, state);
}

BENCHMARK_TEMPLATE(service_inserted_bench, 8, 1);
BENCHMARK_TEMPLATE(service_inserted_bench, 16, 1);
BENCHMARK_TEMPLATE(service_inserted_bench, 32, 1);
BENCHMARK_TEMPLATE(service_inserted_bench, 64, 1);
BENCHMARK_TEMPLATE(service_inserted_bench, 128, 1);

BENCHMARK_TEMPLATE(service_inserted_bench, 8, 8);
BENCHMARK_TEMPLATE(service_inserted_bench, 16, 8);
BENCHMARK_TEMPLATE(service_inserted_bench, 32, 8);
BENCHMARK_TEMPLATE(service_inserted_bench, 64, 8);
BENCHMARK_TEMPLATE(service_inserted_bench, 128, 8);

BENCHMARK_TEMPLATE(service_inserted_bench, 8, 16);
BENCHMARK_TEMPLATE(service_inserted_bench, 16, 16);
BENCHMARK_TEMPLATE(service_inserted_bench, 32, 16);
BENCHMARK_TEMPLATE(service_inserted_bench, 64, 16);
BENCHMARK_TEMPLATE(service_inserted_bench, 128, 16);

BENCHMARK_TEMPLATE(service_inserted_bench, 8, 1, 1);
BENCHMARK_TEMPLATE(service_inserted_bench, 16, 1, 1);
BENCHMARK_TEMPLATE(service_inserted_bench, 32, 1, 1);
BENCHMARK_TEMPLATE(service_inserted_bench, 64, 1, 1);
BENCHMARK_TEMPLATE(service_inserted_bench, 128, 1, 1);

BENCHMARK_TEMPLATE(service_inserted_bench, 8, 8, 1);
BENCHMARK_TEMPLATE(service_inserted_bench, 16, 8, 1);
BENCHMARK_TEMPLATE(service_inserted_bench, 32, 8, 1);
BENCHMARK_TEMPLATE(service_inserted_bench, 64, 8, 1);
BENCHMARK_TEMPLATE(service_inserted_bench, 128, 8, 1);

BENCHMARK_TEMPLATE(service_inserted_bench, 8, 16, 1);
BENCHMARK_TEMPLATE(service_inserted_bench, 16, 16, 1);
BENCHMARK_TEMPLATE(service_inserted_bench, 32, 16, 1);
BENCHMARK_TEMPLATE(service_inserted_bench, 64, 16, 1);
BENCHMARK_TEMPLATE(service_inserted_bench, 128, 16, 1);
BENCHMARK_TEMPLATE(service_inserted_bench, 256, 16, 1);

BENCHMARK_TEMPLATE(service_inserted_bench, 8, 1, 8);
BENCHMARK_TEMPLATE(service_inserted_bench, 16, 1, 8);
BENCHMARK_TEMPLATE(service_inserted_bench, 32, 1, 8);
BENCHMARK_TEMPLATE(service_inserted_bench, 64, 1, 8);
BENCHMARK_TEMPLATE(service_inserted_bench, 128, 1, 8);
BENCHMARK_TEMPLATE(service_inserted_bench, 256, 1, 8);

BENCHMARK_TEMPLATE(service_inserted_bench, 8, 8, 8);
BENCHMARK_TEMPLATE(service_inserted_bench, 16, 8, 8);
BENCHMARK_TEMPLATE(service_inserted_bench, 32, 8, 8);
BENCHMARK_TEMPLATE(service_inserted_bench, 64, 8, 8);
BENCHMARK_TEMPLATE(service_inserted_bench, 128, 8, 8);
BENCHMARK_TEMPLATE(service_inserted_bench, 256, 8, 8);

BENCHMARK_TEMPLATE(service_inserted_bench, 8, 16, 8);
BENCHMARK_TEMPLATE(service_inserted_bench, 16, 16, 8);
BENCHMARK_TEMPLATE(service_inserted_bench, 32, 16, 8);
BENCHMARK_TEMPLATE(service_inserted_bench, 64, 16, 8);
BENCHMARK_TEMPLATE(service_inserted_bench, 128, 16, 8);
BENCHMARK_TEMPLATE(service_inserted_bench, 256, 16, 8);

BENCHMARK_TEMPLATE(service_inserted_bench, 16, 16, 16);
BENCHMARK_TEMPLATE(service_inserted_bench, 32, 16, 16);
BENCHMARK_TEMPLATE(service_inserted_bench, 64, 16, 16);
BENCHMARK_TEMPLATE(service_inserted_bench, 128, 16, 16);
BENCHMARK_TEMPLATE(service_inserted_bench, 256, 16, 16);

BENCHMARK_MAIN();
