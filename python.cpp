#include <iostream>
#include <string>
#include <chrono>

#include <benchmark/benchmark.h>

#include <pybind11/embed.h>

namespace py = pybind11;
using namespace py::literals;

struct data final {
    uint32_t id;
    std::string name;
    std::string last_name;
    std::string hostname;
    uint32_t id2;
    std::string hostname2;
};

PYBIND11_EMBEDDED_MODULE(fast_calc, m) {
    m.def("print_data", [](data &self) -> void {
        std::cout << "data { id: " << self.id << " name " << self.name << " }"
                  <<
                  std::endl;
    });

    m.def("rbp", [](int a,int b) -> int {
        return a +b;
    });

    py::class_<data>(m,"Data")
            .def_readwrite("id", &data::id)
            .def_readwrite("name", &data::name)
            .def_readwrite("last_name", &data::last_name)
            .def_readwrite("hostname", &data::hostname)
            .def_readwrite("id2", &data::id2)
            .def_readwrite("hostname2", &data::hostname2);

}


static void bm_python_invoke_with_binded(benchmark::State &state) {
    py::scoped_interpreter guard{};
    auto script = py::module_::import("script");
    for (auto _ : state) {
        auto run = script.attr("run");
        int abc = run(1, 2).cast<int>();;
    }
}

static void bm_python_invoke_with_usertype(benchmark::State &state) {
    py::scoped_interpreter guard{};
    auto script = py::module_::import("script");
/*
    auto bark = lua["bark"].get_or_create<sol::table>();
    bark.new_usertype<data>("data", "id", &data::id, "name", &data::name,
                            "last_name", &data::last_name, "hosname",
                            &data::hostname, "id2", &data::id2, "hosname2",
                            &data::hostname2);  // the usual

    bark.set_function("print_data", [](data &self) {
        std::cout << "data { id: " << self.id << " name " << self.name << " }"
                  << std::endl;
    });
    */
    for (auto _ : state) {
        auto fcn2 = script.attr("fcn2");
        data abc = fcn2(data{1, "mnm"}).cast<data>();
    }
}

// Register the function as a benchmark
BENCHMARK(bm_python_invoke_with_binded);
BENCHMARK(bm_python_invoke_with_usertype);

// Run the benchmark
BENCHMARK_MAIN();
