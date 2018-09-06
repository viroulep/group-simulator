CFLAGS=-g -Wall

all: simu

# Make sure you installed libyaml-cpp-dev!

simu: types.o simu.o costs.o judge_runs_system.o runner_system.o generator.o minisim.cpp
	$(CXX) $(CFLAGS) -o simu types.o simu.o costs.o judge_runs_system.o runner_system.o generator.o minisim.cpp -lyaml-cpp

types.o: types.hpp types.cpp
	$(CXX) $(CFLAGS) -c -o types.o types.cpp

simu.o: simu.hpp types.hpp simu.cpp
	$(CXX) $(CFLAGS) -c -o simu.o simu.cpp

generator.o: generator.hpp types.hpp generator.cpp
	$(CXX) $(CFLAGS) -c -o generator.o generator.cpp

judge_runs_system.o: simu.hpp types.hpp judge_runs_system.cpp
	$(CXX) $(CFLAGS) -c -o judge_runs_system.o judge_runs_system.cpp

runner_system.o: simu.hpp types.hpp runner_system.cpp
	$(CXX) $(CFLAGS) -c -o runner_system.o runner_system.cpp

costs.o: costs.hpp costs.cpp models.def
	$(CXX) $(CFLAGS) -c -o costs.o costs.cpp

clean:
	rm -rf simu *.o
