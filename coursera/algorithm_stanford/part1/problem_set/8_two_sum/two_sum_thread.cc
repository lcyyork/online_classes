#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <thread>
#include <atomic>
#include <chrono>
#include <ratio>

bool find_2sum(const std::unordered_set<long int>& H, const long int& target){
    for (const long int& value: H) {
        long int lookup = target - value;
        if (value == lookup) {continue;}
        if (H.find(lookup) != H.end()) {
            return true;
        }
    }
    return false;
}

void readfile(const std::string& filename, std::unordered_set<long int>& H) {
    std::ifstream ifs (filename);
    while (ifs.good()) {
        long int v;
        ifs >> v;
        H.insert(v);
    }
}

void find_2sum_in_range(const int& lb, const int& ub,
                        const std::unordered_set<long int>& H,
                        std::atomic<int>& count) {
    int partial_count = 0;
    for (int t = lb; t < ub; ++t) {
        if (find_2sum(H, t)) {
            std::cout << "t = " << t << " is possible!" << std::endl;
            partial_count += 1;
        }
    }
    count += partial_count;
}

std::vector<int> bounds(const int& l, const int& r, const int& parts) {
    std::vector<int> out;

    int total = r - l + 1;
    int delta = total / parts;
    int remainder = total % parts;

    int n1 = l, n2 = 0;
    out.emplace_back(n1);
    for (int i = 0; i < parts; ++i) {
        if (remainder-- > 0) {
            n2 = n1 + delta + 1;
        } else {
            n2 = n1 + delta;
        }
        out.emplace_back(n2);
        n1 = n2;
    }

    return out;
}

int main () {
    using namespace std::chrono;
    std::time_t tt = system_clock::to_time_t(system_clock::now());
    std::cout << "2sum started at " << std::ctime(&tt) << std::endl;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    std::unordered_set<long int> H;
    H.max_load_factor(0.1);
    readfile("2sum.txt", H);

    int nthreads = 3;
    std::vector<std::thread> threads;
    std::cout << "Number of threads: " << nthreads << std::endl;

    std::atomic<int> count (0);

    std::vector<int> limits = bounds(-10000, 10000, nthreads);
    for (int i = 0; i < nthreads; ++i) {
        threads.push_back(std::thread(find_2sum_in_range,
                                      limits[i], limits[i+1],
                                      std::ref(H), std::ref(count)));
    }

    for(auto &t : threads){
        t.join();
    }

    std::cout << "Total possible t: " << count << std::endl;

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    tt = system_clock::to_time_t(system_clock::now());
    std::cout << "2sum ended at " << std::ctime(&tt) << std::endl;
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "2sum took " << time_span.count() << " seconds." << std::endl;

    return 0;
}
