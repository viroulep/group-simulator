CFLAGS=-g -Wall

all: simu

# Make sure you installed libyaml-cpp-dev!

simu: types.o simu.o costs.o minisim.cpp
	$(CXX) $(CFLAGS) -o simu types.o simu.o costs.o minisim.cpp -lyaml-cpp

types.o: types.hpp types.cpp
	$(CXX) $(CFLAGS) -c -o types.o types.cpp

simu.o: simu.hpp types.hpp simu.cpp
	$(CXX) $(CFLAGS) -c -o simu.o simu.cpp

costs.o: costs.hpp costs.cpp
	$(CXX) $(CFLAGS) -c -o costs.o costs.cpp

clean:
	rm -rf simu *.o
