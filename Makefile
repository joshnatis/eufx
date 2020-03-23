CXX=g++
CXXFLAGS=-std=c++11

eufx: EUFX.cpp
	$(CXX) $(CXXFLAGS) EUFX.cpp -o eufx

careful: EUFX.cpp
	$(CXX) $(CXXFLAGS) -Wall -g EUFX.cpp -o eufx

clean:
	rm eufx

organize:
	mv *pgm images/results
	mv *txt images/results/txt
