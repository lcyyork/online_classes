#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <chrono>
#include <ratio>
#include <omp.h>
#include <ctime>

// this is probably not a good idea
bool find_2sum_task(const std::unordered_set<long int>& H, const long int& target){
    volatile bool flag = false;
#pragma omp parallel shared(H)
    {
#pragma omp single nowait
        for (auto it = H.begin(); it != H.end(); ++it) {
            if (flag) continue;

#pragma omp task firstprivate(it)
            {
                long int lookup = target - *it;
                if (H.find(lookup) != H.end() && *it != lookup) {
                    flag = true;
                }
            }
        }
    }
    return flag;
}

bool find_2sum(const std::unordered_set<long int>& H, const long int& target) {
    for (const auto& value: H) {
        long int lookup = target - value;
        if (lookup == value) {continue;}
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

int main () {
    using namespace std::chrono;
    std::time_t tt = system_clock::to_time_t(system_clock::now());
    std::cout << "2sum started at " << std::ctime(&tt) << std::endl;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    std::unordered_set<long int> H;
    H.max_load_factor(0.1);
    readfile("2sum.txt", H);

    omp_set_num_threads(4);
    std::cout << "Number of threads: " << omp_get_max_threads() << std::endl;

    int count = 0;
#pragma omp parallel for reduction(+ : count) shared(H)
    for (int t = -10000; t < 10001; ++t) {
        if (find_2sum(H, t)) {
            std::cout << "t = " << t << " is possible!" << std::endl;
            count += 1;
        }
    }

    std::cout << "Total possible t: " << count << std::endl;

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    tt = system_clock::to_time_t(system_clock::now());
    std::cout << "2sum ended at " << std::ctime(&tt) << std::endl;

    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "2sum took " << time_span.count() << " seconds." << std::endl;

    return 0;
}
