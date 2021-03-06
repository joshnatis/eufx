CXX=g++
CXXFLAGS=-std=c++11

eufx: EUFX.cpp FILTERS.hpp
	$(CXX) $(CXXFLAGS) EUFX.cpp -o eufx

clean:
	rm eufx FILTERS.hpp.gch

organize:
	mv *pgm images/results
	mv *txt images/results/txt
