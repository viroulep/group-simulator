CFLAGS=-g -Wall

all: simu

# Make sure you installed libyaml-cpp-dev!

simu: types.o simu.o costs.o judge_runs_system.o minisim.cpp
	$(CXX) $(CFLAGS) -o simu types.o simu.o costs.o judge_runs_system.o minisim.cpp -lyaml-cpp

types.o: types.hpp types.cpp
	$(CXX) $(CFLAGS) -c -o types.o types.cpp

simu.o: simu.hpp types.hpp simu.cpp
	$(CXX) $(CFLAGS) -c -o simu.o simu.cpp

judge_runs_system.o: simu.hpp types.hpp judge_runs_system.cpp
	$(CXX) $(CFLAGS) -c -o judge_runs_system.o judge_runs_system.cpp

costs.o: costs.hpp costs.cpp models.def
	$(CXX) $(CFLAGS) -c -o costs.o costs.cpp

clean:
	rm -rf simu *.o
