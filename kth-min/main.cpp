#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <boost/timer.hpp>
#include <cassert>

// O(NlogN)
int kthMin1(std::vector<int>& data, int k) {
    std::sort(data.begin(), data.end());

    return data[k - 1];
}

// O(NlogN)
int kthMin2(const std::vector<int>& data, int k) {
    std::priority_queue<int> pq;
    for (const auto& num : data) {
        pq.push(num);
        if (pq.size() > k) {
            pq.pop();
        }
    }

    return pq.top();
}

// O(N + klogN)
int kthMin3(std::vector<int>& data, int k) {
    std::make_heap(data.begin(), data.end(), std::greater<>());

    for (int i = k - 1; i > 0; i--) {
        std::pop_heap(data.begin(), data.end(), std::greater<>());
        data.pop_back();
    }

    return data.front();
}

int partition(std::vector<int>& data, int low, int high) {
    int i = low;
    int j = high + 1;
    int pivot = data[low];
    while (true) {
        while (data[++i] < pivot) {
            if (i == high) break;
        }

        while (data[--j] > pivot) {
            if (j == low) break;
        }

        if (i >= j) break;

        std::swap(data[i], data[j]);
    }

    std::swap(data[low], data[j]);

    return j;
}

// O(N) average case
int kthMin4(std::vector<int>& data, int k) {
    k--;
    int low = 0;
    int high = data.size() - 1;
    while (high > low) {
        int middle = partition(data, low, high);
        if (middle == k) return data[k];
        else if (middle > k) high = middle - 1;
        else low = middle + 1;
    }

    return data[low];
}

std::vector<std::vector<int>> createVectors(int row, int col) {
    static std::uniform_int_distribution<int> distribution(
            std::numeric_limits<int>::min(),
            std::numeric_limits<int>::max()
    );
    static std::default_random_engine generator;

    std::vector<int> data(col);
    std::generate(data.begin(), data.end(),
                  []() { return distribution(generator); });

    return {row, data};
}


int main() {
    const int NUMS_LENGTH = 100;
    auto test_cases = createVectors(4, NUMS_LENGTH);

    std::random_device device;
    std::default_random_engine engine(device());
    std::uniform_int_distribution<int> dist(1, 100);

    const int K = dist(engine);

    boost::timer t1;
    int k1 = kthMin1(test_cases[0], K);
    std::cout << t1.elapsed() * 1000 << std::endl;

    boost::timer t2;
    int k2 = kthMin2(test_cases[1], K);
    std::cout << t2.elapsed() * 1000 << std::endl;

    boost::timer t3;
    int k3 = kthMin3(test_cases[2], K);
    std::cout << t3.elapsed() * 1000 << std::endl;

    boost::timer t4;
    int k4 = kthMin4(test_cases[3], K);
    std::cout << t4.elapsed() * 1000 << std::endl;

    assert(k1 == k2 && k2 == k3 && k3 == k4);

    return 0;
}
