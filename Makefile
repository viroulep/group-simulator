all: simu

simu: types.o simu.o minisim.cpp
	g++ -o simu types.o simu.o minisim.cpp

types.o: types.hpp types.cpp
	g++ -c -o types.o types.cpp

simu.o: simu.hpp types.hpp simu.cpp
	g++ -c -o simu.o simu.cpp

clean:
	rm -rf simu *.o
