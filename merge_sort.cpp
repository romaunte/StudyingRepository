#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <algorithm>
using namespace std;

mt19937 mt(time(nullptr));

vector<long> get_random_vector(long n) {
    vector<long> a(n);
    for (int i = 0; i < n; i++) {
        a[i] = abs((long)mt());
    }
    return a;
}

void merge(vector<long>& A, int l, int q, int r) {
    vector<long> result_vector(r-l+1);
    int ptr_l = l;
    int ptr_r = q+1;
    while (ptr_l <= q and ptr_r <= r) {
        cout << "Res:: ";
        for (int x: result_vector) { cout << x << " "; }
        cout << endl;
        cout << "ptr_r:  " << ptr_r;
        cout << endl;
        cout << "ptr_l:  " << ptr_l;
        cout << endl;
        if (A[ptr_l] == A[ptr_r]) {
            result_vector.insert(result_vector.end(), A[ptr_l], 2);
            ptr_r++;
            ptr_l++;
        } 
        else {
            result_vector.push_back(min(A[ptr_l], A[ptr_r]));
            if (A[ptr_l] > A[ptr_r]) {
                ptr_r++;
            } 
            else {
                ptr_l++;
            }
        }
    }
    if (ptr_l > q) {
        result_vector.insert(result_vector.end(), A.begin() + ptr_r, A.begin() + r);
    } else if (ptr_r > r) {
        result_vector.insert(result_vector.end(), A.begin() + ptr_l, A.begin() + q);
    }
    A.erase(A.begin() + l, A.begin() + r);
    A.insert(A.begin() + l - 1, result_vector.begin(), result_vector.end());
}

void merge_sort(vector<long>& A, int l, int r) {
    int q;
    cout << l << " " << r << endl;
    for (int x: A) {
        cout << x << " ";
    }
    cout << endl;
    if (l < r) {
        q = (l+r) / 2;
        merge_sort(A, l, q);
        merge_sort(A, q+1, r);
        merge(A, l, q, r);
    }
    
}

int main()
{
    vector<long> A = {10, 2, 5, 6, 8, 3, 5, 7, 1, 9, 4};
    cout << "Before sort" << endl;
    for (int x: A) {
        cout << x << " ";
    }
    cout << endl;
    merge_sort(A, 0, size(A));
    cout << "After sort" << endl;
    for (int x: A) {
        cout << x << " ";
    }
    cout << endl;
    return 0;
}