#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <algorithm>

using namespace std;
using namespace chrono;

mt19937 mt(time(nullptr));

vector<long> get_smooth_numbers(long n) {
    vector<long> smoothn2 = {2};
    vector<long> smoothn3 = {2};
    vector<long> smoothn = {2};
    long next_n = 3;
    while (n / next_n > 1) {
        smoothn.push_back(next_n);
        smoothn2.push_back(next_n);
        smoothn3.push_back(next_n);

        if (smoothn2[0]*2 > smoothn3[0]*3) {
            next_n = smoothn3[0]*3;
            smoothn3.erase(smoothn3.begin());
        } else if (smoothn2[0]*2 < smoothn3[0]*3) {
            next_n = smoothn2[0]*2;
            smoothn2.erase(smoothn2.begin());
        } else {
            next_n = smoothn2[0]*2;
            smoothn2.erase(smoothn2.begin());
            smoothn3.erase(smoothn3.begin());
        }
    }
    return smoothn;
}

void shell_sort(vector<long>& a, long n) {
    vector<long> smooth_numbers = get_smooth_numbers(n);

    for (int i = 0; i < size(smooth_numbers); i++) {
        long step = n / smooth_numbers[i];
        for (int j = 0; j < (n - step); j++) {
            if (a[j] > a[j+step]) {
                swap(a[j], a[j+step]);
            }
        }
    }
}

vector<long> get_random_vector(long n) {
    vector<long> a(n);
    for (int i = 0; i < n; i++) {
        a[i] = abs((long)mt());
    }
    return a;
}

int main() {
    vector<long> a, b;
    long n;
    for (int i = 3; i < 7; i++) {
        n = pow(10, i);
        a = get_random_vector(n);
        b = a;
        cout << "N = " << n << endl;
        auto start = steady_clock::now();
        shell_sort(a, n);
        auto end = steady_clock::now();
        duration<double> elapsed_seconds = end - start;
        cout << "Shell sort:    " << elapsed_seconds.count() << endl;
        start = steady_clock::now();
        sort(b.begin(), b.end());
        end = steady_clock::now();
        elapsed_seconds = end - start;
        cout << "Sort:    " << elapsed_seconds.count() << endl << endl;
    }
    return 0;
}
