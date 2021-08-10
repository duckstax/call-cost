#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <benchmark/benchmark.h>
#include <iostream>
#include <string>
#include <chrono>

struct data final {
  uint32_t id;
  std::string name;
  std::string last_name;
  std::string hostname;
  uint32_t id2;
  std::string hostname2;
};

static void BM_LuaInvokeWithBinded(benchmark::State& state) {
  sol::state lua;
  lua.open_libraries(sol::lib::base);
  lua.script_file("script.lua");
  lua["rbp"] = [](int a, int b) { return a + b; };
  for (auto _ : state) {
    sol::function f = lua["run"];
    int abc = f(1, 2);
  }
}

static void BM_LuaInvokeWithUsertype(benchmark::State& state) {
  sol::state lua;
  lua.open_libraries(sol::lib::base);
  lua.script_file("script.lua");

  auto bark = lua["bark"].get_or_create<sol::table>();
  bark.new_usertype<data>("data", "id", &data::id, "name", &data::name,
                          "last_name", &data::last_name, "hosname",
                          &data::hostname, "id2", &data::id2, "hosname2",
                          &data::hostname2);  // the usual

  bark.set_function("print_data", [](data& self) {
    std::cout << "data { id: " << self.id << " name " << self.name << " }"
              << std::endl;
  });

  for (auto _ : state) {
    sol::function f = lua["fcn2"];
    data abc = f(data{1, "mnm"});
  }
}
// Register the function as a benchmark
BENCHMARK(BM_LuaInvokeWithBinded);
BENCHMARK(BM_LuaInvokeWithUsertype);
// Run the benchmark
BENCHMARK_MAIN();