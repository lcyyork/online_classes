CXX = g++-7
CXXFLAGS = -Wall -std=c++11
LDFLAGS = -pthread

all: two_sum_thread two_sum_omp

two_sum_thread: two_sum_thread.o
	$(CXX) -o $@ $^ $(LDFLAGS)

two_sum_omp: two_sum_omp.o
	$(CXX) -o $@ $^ -fopenmp

two_sum_thread.o: two_sum_thread.cc
	$(CXX) -c $(CXXFLAGS) $<

two_sum_omp.o: two_sum_omp.cc
	$(CXX) -c -fopenmp $(CXXFLAGS) $<

.PHONY: clean

clean:
	rm -f ./*.o two_sum_thread two_sum_omp
