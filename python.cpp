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
                  << std::endl;
    });

    m.def("rbp", [](int a, int b) -> int { return a + b; });

    py::class_<data>(m, "Data")
            .def_readwrite("id", &data::id)
            .def_readwrite("name", &data::name)
            .def_readwrite("last_name", &data::last_name)
            .def_readwrite("hostname", &data::hostname)
            .def_readwrite("id2", &data::id2)
            .def_readwrite("hostname2", &data::hostname2);

}



constexpr static char script[] = R"__(
        #import sys, os
        #from importlib import import_module
        #sys.modules['fast_calc'] = fast_calc

        from fast_calc import  rbp

        def run(a,b):
            return rbp(a, b)

        def fcn2( a  ):
            a.id  = a.id2
            return a
)__";


static void bm_python_invoke_with_binded(benchmark::State &state) {
    py::scoped_interpreter guard{};

    ///py::eval_file("script.py", py::globals(), py::dict());

    auto script = py::module_::import("script.py");
   // auto run = script.attr("run");

    //for (auto _ : state) {

//        int abc = run(1, 2).cast<int>();;
  //  }
}
/*
static void bm_python_invoke_with_usertype(benchmark::State& state) {
    py::scoped_interpreter guard{};
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
*/
// Register the function as a benchmark
BENCHMARK(bm_python_invoke_with_binded);
/*
BENCHMARK(bm_python_invoke_with_usertype);
*/
// Run the benchmark
BENCHMARK_MAIN();
